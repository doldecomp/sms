#!/usr/bin/env python3

"""Convert a binary file to a C byte initializer."""

import argparse
from pathlib import Path


def convert(input: Path, output: Path) -> None:
    data = input.read_bytes()
    text = "".join(
        "\t" + ", ".join(f"0x{value:02x}" for value in data[offset:offset + 12]) + ",\n"
        for offset in range(0, len(data), 12)
    )
    if output.is_file() and output.read_text(encoding="ascii") == text:
        return
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(text, encoding="ascii")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    convert(args.input, args.output)


if __name__ == "__main__":
    main()
