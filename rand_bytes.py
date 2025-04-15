import sys
import os
import argparse

def rand_hex(n_bytes):
    rand_bytes = os.urandom(n_bytes)
    hex_str = rand_bytes.hex()
    return ' '.join(hex_str[i:i+2] for i in range(0, len(hex_str), 2))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate random bytes as hexadecimal.")
    parser.add_argument("n", type=int, help="Number of random bytes to generate.")

    try:
        args = parser.parse_args()
        num = args.n

        if num < 0:
            print("Error: Number of bytes must be non-negative.", file=sys.stderr)
            sys.exit(1)

        hex_seq = rand_hex(num)
        print(hex_seq)

    except SystemExit:
        pass
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
        import sys
