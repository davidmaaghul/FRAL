# Run from project directory
import os
import subprocess
import tempfile

import pandas as pd
from fral.testing.performance import TEST_PATH, RELEASE_PATH, BLOB_SIZES


def spawn_test(gib: float, blob_size: int, csv_name: str, bin_name: str, sqlite: bool = False):
    test = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, f"write_test{'_sqlite' if sqlite else ''}"),
            "--gib",
            str(gib),
            "--size",
            str(blob_size),
            "--csv_name",
            csv_name,
            "--bin_name",
            bin_name,
        ],
    )
    test.wait()


def main(sqlite: bool = False, gib: float = 1, subdir: str = None):
    print("Starting write test...")
    output_df = pd.DataFrame()
    for blob_size in BLOB_SIZES:
        print(f"Starting {'SQLite ' if sqlite else ''}write test for blob_size={blob_size}")
        with tempfile.TemporaryDirectory() as tmp:
            bin_name = os.path.join(tmp, f"write-test{'-sqlite.db' if sqlite else '.bin'}")
            csv_name = os.path.join(tmp, f"write-test{'-sqlite' if sqlite else ''}.csv")
            spawn_test(gib, blob_size, csv_name, bin_name, sqlite)
            sub_df = pd.read_csv(csv_name)
            output_df = pd.concat([output_df, sub_df])

    file_name = f"write_test{'_sqlite' if sqlite else ''}.csv"

    if subdir:
        file_path = os.path.join(subdir, file_name)
    else:
        file_path = file_name

    output_df.to_csv(os.path.join(TEST_PATH,
                                  "test-results",
                                  file_path
                                  ),
                     index=False)
    print("Write test complete")


if __name__ == "__main__":
    main()
