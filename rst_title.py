#!/usr/bin/env python3

import argparse


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Print text wrapped with reStructuredText title markers."
    )
    parser.add_argument("text", nargs="+", help="Title text to print")
    args = parser.parse_args()

    title = " ".join(args.text)
    marker = "=" * len(title)
    print(marker)
    print(title)
    print(marker)


if __name__ == "__main__":
    main()
