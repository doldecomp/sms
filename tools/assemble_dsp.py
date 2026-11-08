#!/usr/bin/env python3

"""Assemble DSP microcode into a C byte initializer."""

import argparse
import subprocess
from pathlib import Path

from bin2c import convert


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("assembler", type=Path)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    args.binary.parent.mkdir(parents=True, exist_ok=True)
    args.binary.unlink(missing_ok=True)
    subprocess.run(
        [str(args.assembler.resolve()), "-o", str(args.binary.resolve()),
         str(args.input.resolve())],
        check=True,
    )
    convert(args.binary, args.output)


if __name__ == "__main__":
    main()
