# Run from project directory on VM
import tempfile
import time
import os
import pandas as pd
import multiprocessing as mp
from fral.testing.performance_tests import RELEASE_PATH, BLOB_SIZES, TEST_PATH
import subprocess


ENTRIES = 10000


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
        print(f"Starting net test for blob_size={blob_size}, entries={ENTRIES}")
        with tempfile.TemporaryDirectory() as tmp:

            server_bin_name = os.path.join(tmp, "net-test-server.bin")
            client_bin_name = os.path.join(tmp, "net-test-client.bin")
            csv_path = os.path.join(tmp, f"net_test.csv")

            server = mp.Process(target=spawn_server, args=(server_bin_name, ENTRIES, blob_size, csv_path))
            client = mp.Process(target=spawn_client, args=(client_bin_name, ENTRIES, blob_size))

            server.start()
            time.sleep(3)
            client.start()

            server.join()
            client.join()

            sub_df = pd.read_csv(csv_path)
            output_df = pd.concat([output_df, sub_df]) if isinstance(output_df, pd.DataFrame) else sub_df

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "net_test.csv"), index=False)


if __name__ == "__main__":
    main()
