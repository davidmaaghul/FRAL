# Run from project directory
import subprocess
import os
import tempfile
import pandas as pd
from fral.testing.performance import (
    TEST_PATH,
    RELEASE_PATH,
    BLOB_SIZES,
    WRITERS,
)


def spawn_test(gib: float, writers: int, blob_size: int, csv_name: str, bin_name: str, sqlite: bool = False):

    sp = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, f"pc_test{'_sqlite' if sqlite else ''}"),
            "--gib",
            str(gib),
            "--size",
            str(blob_size),
            "--bin_name",
            bin_name,
            "--csv_name",
            csv_name,
            "--writers",
            str(writers),
        ]
    )
    sp.wait()


def main(sqlite: bool = False, gib: float = 1, subdir: str = None):
    if not os.path.exists(results := os.path.join(TEST_PATH, "test-results")):
        os.mkdir(results)

    output_df = pd.DataFrame()
    print(f"Starting producer-consumer test...")
    for writers in WRITERS:
        for blob_size in BLOB_SIZES:
            with tempfile.TemporaryDirectory() as tmp:
                print(
                    f"Starting {'SQLite ' if sqlite else ''}"
                    f"producer-consumer test for blob_size={blob_size}, writers={writers}"
                )
                bin_name = os.path.join(tmp, f"pc-test{'-sqlite.db' if sqlite else '.bin'}")
                csv_name = os.path.join(tmp, f"pc-test{'-sqlite' if 'sqlite' else ''}.csv")
                spawn_test(gib, writers, blob_size, csv_name, bin_name, sqlite)
                sub_df = pd.read_csv(csv_name)
                output_df = pd.concat([output_df, sub_df])

    file_name = f"pc_test{'_sqlite' if sqlite else ''}.csv"

    if subdir:
        file_path = os.path.join(subdir, file_name)
    else:
        file_path = file_name

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", file_path), index=False)
    print("Producer-consumer test complete")


if __name__ == "__main__":
    main()
