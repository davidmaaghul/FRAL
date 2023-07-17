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


def spawn_test(writers: int, blob_size: int, csv_name: str, bin_name: str):
    sp = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, "pc_test"),
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


def main():
    if not os.path.exists(results := os.path.join(TEST_PATH, "test-results")):
        os.mkdir(results)

    output_df = pd.DataFrame()
    print("Starting producer-consumer test...")
    for writers in WRITERS:
        for blob_size in BLOB_SIZES:
            with tempfile.TemporaryDirectory() as tmp:
                print(
                    f"Starting producer-consumer test for blob_size={blob_size}, writers={writers}"
                )
                bin_name = os.path.join(tmp, "pc-test.bin")
                csv_name = os.path.join(tmp, "pc-test.csv")
                spawn_test(writers, blob_size, csv_name, bin_name)
                sub_df = pd.read_csv(csv_name)
                output_df = pd.concat([output_df, sub_df])

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "pc_test.csv"), index=False)
    print("Producer-consumer test complete")


if __name__ == "__main__":
    main()
