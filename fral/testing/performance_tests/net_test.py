# Run from project directory on VM
import tempfile
import os
import pandas as pd

from fral.testing.performance_tests import RELEASE_PATH, BLOB_SIZES, ENTRIES, TEST_PATH
import subprocess


def spawn_client(bin_name: str, entries: int, size: int):
    sp = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, "client"),
            "--size",
            str(size),
            "--bin_name",
            bin_name,
            "--entries",
            str(entries),
        ]
    )

    sp.wait()


def spawn_server(bin_name: str, entries: int, size: int, csv_name: str):
    sp = subprocess.Popen(
        [
            os.path.join(RELEASE_PATH, "server"),
            "--size",
            str(size),
            "--bin_name",
            bin_name,
            "--csv_name",
            csv_name,
            "--entries",
            str(entries),
        ]
    )

    sp.wait()


def main():
    output_df = None

    for blob_size in BLOB_SIZES:
        for entries in ENTRIES:
            with tempfile.TemporaryDirectory() as tmp:
                server_bin_name = os.path.join(tmp, "net-test-server.bin")
                client_bin_name = os.path.join(tmp, "net-test-server.bin")
                csv_path = os.path.join(tmp, f"net_test.csv")

                spawn_server(server_bin_name, entries, blob_size, csv_path)
                spawn_client(client_bin_name, entries, blob_size)

                sub_df = pd.read_csv(csv_path)
                output_df = pd.concat(
                    [output_df, sub_df]
                    if isinstance(output_df, pd.DataFrame)
                    else sub_df
                )

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "net_test.csv"), index=False)


if __name__ == "__main__":
    main()
