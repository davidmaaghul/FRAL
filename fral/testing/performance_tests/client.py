# Run from project directory on VM
import tempfile
import os
import argparse
from fral.testing.performance_tests import RELEASE_PATH
import subprocess


def spawn_client(bin_name: str, size: int, entries: int):
    sp = subprocess.Popen([
        os.path.join(RELEASE_PATH, "client"),
        "--size",
        str(size),
        "--bin_name",
        bin_name,
        "--entries",
        str(entries)
    ])
    sp.wait()


def main():
    args = get_args()
    with tempfile.TemporaryDirectory() as tmp:
        bin_name = os.path.join(tmp, "net-test-client.bin")
        spawn_client(bin_name, args.blob_size, args.entries)


def get_args():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument(
        "-b", "--blob_size", dest="blob_size", nargs='?', default=100, type=int
    )
    arg_parser.add_argument(
        "-e", "--entries", dest="entries", nargs='?', default=10000, type=int
    )
    return arg_parser.parse_args()


if __name__ == "__main__":
    main()
