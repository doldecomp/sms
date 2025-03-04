#!/bin/bash

# Script to extract and demangle symbols from MAP files

set +e


if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "Usage: $0 <cpp_filename> [output_file]"
    echo "Example: $0 MarioMain.cpp demangled_symbols.txt"
    echo "If output_file is not provided, results will only be displayed on screen."
    exit 1
fi

CPP_FILE=$1
MAP_FILE="mario.MAP"
OUTPUT_FILE=""

if [ $# -eq 2 ]; then
    OUTPUT_FILE="$2"
    touch "$OUTPUT_FILE" 2>/dev/null
    if [ $? -ne 0 ]; then
        echo "Error: Cannot write to output file $OUTPUT_FILE"
        exit 1
    fi
    echo "Results will be saved to $OUTPUT_FILE"
    > "$OUTPUT_FILE"
fi

if [ ! -f "$MAP_FILE" ]; then
    echo "Error: $MAP_FILE not found in the current directory."
    exit 1
fi

if [ ! -f "./build/tools/dtk.exe" ]; then
    echo "Error: ./build/tools/dtk.exe not found."
    exit 1
fi
echo "Starting symbol extraction for $CPP_FILE..."

mapfile -t MATCHING_LINES < <(grep -i "$CPP_FILE" "$MAP_FILE")

echo "Found ${#MATCHING_LINES[@]} matching lines in $MAP_FILE"

for ((i=0; i<${#MATCHING_LINES[@]}; i++)); do
    line="${MATCHING_LINES[$i]}"
    echo "Processing line $((i+1))/${#MATCHING_LINES[@]}: $line"
    
    for word in $line; do
        if [ ${#word} -lt 5 ] || [[ "$word" =~ ^[0-9]+$ ]] || 
           [[ "$word" == *".cpp"* ]] || [[ "$word" == *".o"* ]] || 
           [[ "$word" == *".h"* ]] || [[ "$word" == "0x"* ]]; then
            continue
        fi
        
        if [[ ! "$word" =~ _[0-9]* ]]; then
            continue
        fi
        
        demangled=$(bash -c "./build/tools/dtk.exe demangle \"$word\"" 2>/dev/null || echo "FAILED")
        
        if [ -n "$demangled" ] && [ "$demangled" != "FAILED" ] && 
           [ "$demangled" != "$word" ] && 
           [[ ! "$demangled" =~ "error" ]] && [[ ! "$demangled" =~ "failed" ]]; then
            
            if [ -n "$OUTPUT_FILE" ]; then
                echo "$demangled" >> "$OUTPUT_FILE"
            else
            echo "SUCCESS - Original: $word"
            echo "Demangled: $demangled"
            fi
        fi
    done
done

if [ -n "$OUTPUT_FILE" ]; then
    echo "All results have been saved to $OUTPUT_FILE"
fi

echo "All done! Processed ${#MATCHING_LINES[@]} lines."