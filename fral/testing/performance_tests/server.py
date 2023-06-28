# Run from project directory on VM
import tempfile
import os
import argparse
import subprocess

from fral.testing.performance_tests import RELEASE_PATH, TEST_PATH


def spawn_server(bin_name: str, entries: int, size: int, csv_name: str):
    sp = subprocess.Popen([
        os.path.join(RELEASE_PATH, "server"),
        "--size",
        str(size),
        "--bin_name",
        bin_name,
        "--csv_name",
        csv_name,
        "--entries",
        str(entries)
    ])
    sp.wait()


def main():
    args = get_args()

    with tempfile.TemporaryDirectory() as tmp:

        bin_name = os.path.join(tmp, "net-test-server.bin")
        csv_name = os.path.join(TEST_PATH, "test-results", "local", f"net_test_{args.entries}_{args.blob_size}.csv")
        spawn_server(bin_name, args.entries, args.blob_size, csv_name)


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

