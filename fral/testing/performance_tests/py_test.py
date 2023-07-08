import multiprocessing
import tempfile
import os
import time
import pandas as pd
from multiprocessing import Pool
from fral.python.fral.fral import FRAL
from fral.testing.performance_tests import BLOB_SIZES, TEST_PATH, WRITERS, GB_TO_B

PC_TEST_CSV = "py_pc_test.csv"


def create(size: int, name: str, gib: int):
    total_size = gib * GB_TO_B
    assert total_size % size == 0
    entries = total_size // size
    FRAL(name, total_size, entries)
    return entries


def spawn_writer(name: str, entries: int, blob_size: int):
    ral = FRAL(name)
    ral.prime_cache()
    time.sleep(3)  # enough time for all processes to be created
    t = time.time_ns()
    stream = ("a" * blob_size).encode()
    for i in range(entries):
        blob = ral.allocate(blob_size)
        blob[:blob_size] = stream
        ral.append(blob)
    return t


def run_one_pc(blob_size: int, writers: int, tmp_dir: str):
    print(f"starting py pc test for blob_size={blob_size}, writers={writers}")
    bin_name = os.path.join(tmp_dir, "py_pc_test.bin")
    entries = create(blob_size, bin_name, 1)

    writer_entries = entries // writers

    with Pool(processes=writers) as pool:
        star_args = [(bin_name, writer_entries, blob_size) for i in range(writers)]
        pool_maps = [pool.apply_async(spawn_writer, star_arg) for star_arg in star_args]
        ral = FRAL(bin_name)
        ral.prime_cache()
        buffer = memoryview(bytearray(("c" * blob_size).encode()))
        i = 0
        while i < entries:
            blob = ral.read(i, blob_size)
            if blob:
                buffer[:blob_size] = blob
                i += 1
        stop = time.time_ns()

        start = stop
        for start_time in [result.get() for result in pool_maps]:
            if start_time < start:
                start = start_time

        out_df = pd.DataFrame(
            {
                "Time": [
                    stop - start,
                ],
                "Entries": [
                    entries,
                ],
                "Writers": [
                    writers,
                ],
                "Size": [
                    blob_size,
                ],
            }
        )
        out_df.to_csv(os.path.join(tmp_dir, PC_TEST_CSV))


def py_pc_test():
    output_df = None
    for blob_size in BLOB_SIZES:
        for writers in WRITERS:
            with tempfile.TemporaryDirectory() as tmp:
                p = multiprocessing.Process(
                    target=run_one_pc, args=(blob_size, writers, tmp)
                )
                p.start()
                p.join()
                sub_df = pd.read_csv(os.path.join(tmp, PC_TEST_CSV))
                output_df = (
                    pd.concat([output_df, sub_df])
                    if isinstance(output_df, pd.DataFrame)
                    else sub_df
                )

    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "py_pc_test.csv"), index=False)


def py_write_test():
    output_data = []
    for blob_size in BLOB_SIZES:
        print("Starting py write test for blob_size =", blob_size)
        with tempfile.TemporaryDirectory() as tmp:
            bin_name = os.path.join(tmp, "py_write_test.bin")
            entries = create(blob_size, bin_name, 1)

            ral = FRAL(bin_name)
            rand_str = ("a" * blob_size).encode()
            start = time.time_ns()

            while True:
                blob = ral.allocate(blob_size)

                if not blob:
                    break

                blob[:blob_size] = rand_str

                ral.append(blob)

            stop = time.time_ns()
            print((stop - start) / entries)
            output_data.append(
                {"Time": stop - start, "Entries": entries, "Size": blob_size}
            )

    output_df = pd.DataFrame(output_data)
    output_df.to_csv(
        os.path.join(TEST_PATH, "test-results", "py_write_test.csv"), index=False
    )


def main():
    py_pc_test()
    py_write_test()
    print("Python tests complete!")


if __name__ == "__main__":
    main()

