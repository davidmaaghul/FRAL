import struct
import subprocess
import os

from fral.python.fral.fral import FRAL

BIN_NAME = "demo.bin"

RELEASE_PATH = os.path.join("cmake-build-release", "fral", "demo")

ENTRIES = 10


def main():

    val = 1
    val_bytes = struct.pack('i', val)
    int_size = len(val_bytes)

    ral = FRAL(BIN_NAME, ENTRIES*int_size, ENTRIES)

    subprocess.Popen([f"./{RELEASE_PATH}/cpp_runner"])

    blob = ral.allocate(int_size)
    blob[:int_size] = val_bytes
    ral.append(blob)
    idx = 1

    while True:

        read_blob = ral[idx]

        if read_blob:
            val = struct.unpack('i', read_blob[:int_size])[0]
            print(f"Python received {val} from C++!")

            if val == ENTRIES:
                break

            write_blob = ral.allocate(int_size)
            write_blob[:int_size] = struct.pack('i', val + 1)
            ral.append(write_blob)

            idx += 2

    os.remove(BIN_NAME)


if __name__ == '__main__':
    main()
