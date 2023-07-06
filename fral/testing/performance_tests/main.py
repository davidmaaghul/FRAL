import argparse
import os

from fral.testing.performance_tests import (
    TEST_PATH,
    net_test,
    write_test,
    pc_test,
    py_test,
)


def main():

    if not os.path.exists(
        results_path := os.path.join(
            TEST_PATH, "test-results"
        )
    ):
        os.makedirs(results_path)

    args = get_args()

    if args.net_test:
        net_test.main()

    if args.write_test:
        write_test.main()

    if args.pc_test:
        pc_test.main()

    if args.py_test:
        py_test.main()


def get_args():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument(
        "-w", "--write_test", dest="write_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-p", "--pc_test", dest="pc_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-t", "--py_test", dest="py_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-n", "--net_test", dest="net_test", action="store_true", default=False
    )

    return arg_parser.parse_args()


if __name__ == "__main__":
    main()
