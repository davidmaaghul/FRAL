# Run from project directory
# Long Run Append Test
import subprocess
import os
import tempfile
import pandas as pd
from fral.testing.performance import (
    TEST_PATH,
    RELEASE_PATH,
    WRITERS_2,
)


def spawn_test(writers: int, csv_name: str, bin_name: str, fral2: bool = False):

    cmd = [
        os.path.join(RELEASE_PATH, "pc_test"),
        "--size",
        "100",
        "--bin_name",
        bin_name,
        "--csv_name",
        csv_name,
        "--writers",
        str(writers),
    ]

    if fral2:
        cmd.append("--fral2")

    sp = subprocess.Popen(
        cmd
    )
    sp.wait()


def main():
    if not os.path.exists(results := os.path.join(TEST_PATH, "test-results")):
        os.mkdir(results)

    output_df = pd.DataFrame()
    print(f"Starting append test...")
    for writers in WRITERS_2:
        for opt in [True, False]:
            with tempfile.TemporaryDirectory() as tmp:
                print(
                    f"Starting append test for writers={writers}; {'fral2' if opt else 'fral'}"
                )
                bin_name = os.path.join(tmp, "append-test.bin")
                csv_name = os.path.join(tmp, "append-test.csv")
                spawn_test(writers, csv_name, bin_name, opt)
                sub_df = pd.read_csv(csv_name)
                sub_df["append2"] = opt
                output_df = pd.concat([output_df, sub_df])

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "append_test.csv"), index=False)
    print("Append test complete")


if __name__ == "__main__":
    main()
