# Run from project directory
import os
import subprocess
import tempfile

import pandas as pd
from fral.testing.performance import TEST_PATH, RELEASE_PATH, BLOB_SIZES


def spawn_test(blob_size: int, csv_name: str, bin_name: str):
    test = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, "write_test"),
            "--size",
            str(blob_size),
            "--csv_name",
            csv_name,
            "--bin_name",
            bin_name,
        ],
    )
    test.wait()


def main():
    print("Starting write test...")
    output_df = pd.DataFrame()
    for blob_size in BLOB_SIZES:
        print(f"Starting write test for blob_size={blob_size}")
        with tempfile.TemporaryDirectory() as tmp:
            bin_name = os.path.join(tmp, "write-test.bin")
            csv_name = os.path.join(tmp, "write-test.csv")
            spawn_test(blob_size, csv_name, bin_name)
            sub_df = pd.read_csv(csv_name)
            output_df = pd.concat([output_df, sub_df])

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "write_test.csv"), index=False)
    print("Write test complete")


if __name__ == "__main__":
    main()
