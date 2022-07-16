#!/usr/bin/env python3
#
# Usage: dump_common_data.py file.s
# Dumps all incbin data and prints the revised file to stdout.

import os
import re
import sys

# Reads a bytearray from baserom.dol
def read_baserom(start, size):
    with open('baserom.dol', 'rb') as f:
        f.seek(start, os.SEEK_SET)
        return bytearray(f.read(size))

if len(sys.argv) != 2:
    print('Usage: %s ASM_FILE' % sys.argv[0])
    exit()

# reads a 32-bit big endian value starting at pos
def read_u32(data, pos):
    return (data[pos]<<24) | (data[pos+1]<<16) | (data[pos+2]<<8) | (data[pos+3])

def is_ascii(code):
    if code >= 0x20 and code <= 0x7E:  # normal characters
        return True
    if code in [0x09, 0x0A]:  # tab, newline
        return True
    return False

# reads a string starting at pos
def read_string(data, pos):
    text = ''
    while pos < len(data) and is_ascii(data[pos]):
        text += chr(data[pos])
        pos += 1
    if pos < len(data) and data[pos] == 0:
        return text
    return ''

# escapes special characters in the string for use in a C string literal
def escape_string(text):
    return text.replace('\\','\\\\').replace('"','\\"').replace('\n','\\n').replace('\t','\\t')

# returns True if value is 4-byte aligned
def is_aligned(num):
    return num % 4 == 0

# returns True if value is a possible pointer
def is_pointer(num):
    return num >= 0x80003100 and num <= 0x802F6C80

# returns True if all elements are zero
def is_all_zero(arr):
    for val in arr:
        if val != 0:
            return False
    return True

# returns string of comma-separated hex bytes
def hex_bytes(data):
    return ', '.join('0x%02X' % n for n in data)

def convert_data(data, offset):
    text = ''
    size = len(data)
    pos = 0
    while pos < size:
        # pad unaligned
        pad = []
        while not is_aligned(offset + pos) and pos < size:
            pad.append(data[pos])
            pos += 1
        if len(pad) > 0:
            if is_all_zero(pad):
                text += '\t.balign 4\n'
            else:
                text += '\t.byte %s\n' % hex_bytes(pad)
        
        # string?
        string = read_string(data, pos)
        if len(string) > 3:
            text += '\t.asciz "%s"\n' % escape_string(string) 
            pos += len(string) + 1
            continue

        assert(is_aligned(offset + pos))

        if pos + 4 <= size:
            val = read_u32(data, pos)
            if is_pointer(val):
                text += '\t.4byte 0x%08X  ;# ptr\n' % val
            elif val == 0:
                text += '\t.4byte 0\n'
            else:
                text += '\t.4byte 0x%08X\n' % val
            pos += 4
    return text

currSection = ''

with open(sys.argv[1], 'rt') as f:
    for line in f.readlines():
        line = line.rstrip()
        # Section directive
        m = re.match(r'\s*\.section\s+([\._A-Za-z0-9]+)', line)
        if m:
            currSection = m.groups()[0]
        elif currSection in ['.rodata', '.data', '.sdata', '.sdata2', '.ctors', '.dtors', 'extab_', 'extabindex_']:
            # Incbin directive
            m = re.match(r'\s*\.incbin\s+"baserom.dol"\s*,\s*([^,]+),\s*([^,]+)', line)
            if m:
                g = m.groups()
                start = int(g[0], 0)
                size = int(g[1], 0)
                data = read_baserom(start, size)
                print('\t# ROM: 0x%X' % start)
                print(convert_data(data, start))
                continue
        print(line)
