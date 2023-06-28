import tempfile
import os
import time
import pandas as pd

from multiprocessing import Process, Queue
from fral.python.fral.fral import FRAL
from fral.testing.performance_tests import BLOB_SIZES, TEST_PATH, WRITERS, GB_TO_B


def create(size: int, name: str, gib: int):

    total_size = gib*GB_TO_B
    assert(total_size % size == 0)
    entries = total_size // size
    FRAL(name, total_size, entries)
    return entries


def spawn_writer(name: str, entries: int, mp_queue: Queue, blob_size: int):
    ral = FRAL(name)
    time.sleep(3)  # enough time for all processes to be created
    t = time.time_ns()
    stream = ("a"*blob_size).encode()
    for i in range(entries):
        blob = ral.allocate(blob_size)
        blob[:blob_size] = stream
        ral.append(blob)
    mp_queue.put(t)


def py_pc_test():

    output_data = []
    for blob_size in BLOB_SIZES:
        for writers in WRITERS:
            with tempfile.TemporaryDirectory() as tmp:
                print(f"starting py pc test for blob_size={blob_size}, writers={writers}")
                bin_name = os.path.join(tmp, "py_pc_test.bin")
                entries = create(blob_size, bin_name, 1)
                start_queue = Queue()
                writer_entries = entries // writers
                all_writers = []
                for writer in range(writers):
                    w = Process(target=spawn_writer, args=(bin_name, writer_entries, start_queue, blob_size))
                    w.start()
                    all_writers.append(w)

                ral = FRAL(bin_name)
                buffer = memoryview(bytearray(('c'*blob_size).encode()))
                i = 0
                while i < entries:
                    blob = ral.read(i)
                    if blob:
                        buffer[:blob_size] = blob
                        i += 1
                stop = time.time_ns()

                start = stop
                for w in all_writers:
                    w.join()
                    start_time = start_queue.get()
                    if start_time < start:
                        start = start_time
                print((stop - start)/entries)
                output_data.append({"Time": stop - start,
                                    "Entries": entries, "Writers": writers, "Size": blob_size})

    output_df = pd.DataFrame(output_data)
    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "local", "py_pc_test.csv"), index=False)


def py_write_test():

    output_data = []
    for blob_size in BLOB_SIZES:
        print("Starting py write test for blob_size =", blob_size)
        with tempfile.TemporaryDirectory() as tmp:
            bin_name = os.path.join(tmp, "py_write_test.bin")
            entries = create(blob_size, bin_name, 1)

            ral = FRAL(bin_name)
            rand_str = ("a"*blob_size).encode()
            start = time.time_ns()

            while True:
                blob = ral.allocate(blob_size)

                if not blob:
                    break

                blob[:blob_size] = rand_str

                ral.append(blob)

            stop = time.time_ns()
            print((stop - start) / entries)
            output_data.append({"Time": stop - start, "Entries": entries, "Size": blob_size})

    output_df = pd.DataFrame(output_data)
    output_df.to_csv(os.path.join(TEST_PATH, "test-results", "local", "py_write_test.csv"), index=False)


if __name__ == '__main__':
    py_write_test()
    py_pc_test()
