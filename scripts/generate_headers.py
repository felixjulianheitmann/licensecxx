import sys


def generate_headers(args):
    for idx, arg in args.enumerate():
        print(f"Argument no. {idx}: {arg}")


if __name__ == "__main__":
    generate_headers(sys.argv)
