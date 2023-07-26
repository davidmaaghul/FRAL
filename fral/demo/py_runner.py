import struct
import subprocess
import os

from fral.python.fral.fral import FRAL

BIN_NAME = "demo.bin"

RELEASE_PATH = os.path.join("cmake-build-release", "fral", "demo")

ENTRIES = 6


def main():

    val = 1
    val_bytes = struct.pack('i', val)
    int_size = len(val_bytes)

    ral = FRAL(BIN_NAME, ENTRIES*int_size, ENTRIES)

    subprocess.Popen([f"./{RELEASE_PATH}/cpp_runner"])

    print(f"Python sending {val} to C++!")
    blob = ral.allocate(int_size)
    blob[:int_size] = val_bytes
    idx = ral.append(blob) + 1

    while True:

        read_blob = ral[idx]

        if read_blob:
            val = struct.unpack('i', read_blob[:int_size])[0]
            print(f"Python received {val} from C++, ", end="")

            if idx == ENTRIES - 1:
                print("done!")
                break

            write_blob = ral.allocate(int_size)
            send_val = val + 1
            write_blob[:int_size] = struct.pack('i', send_val)
            print(f"sending {send_val}!")
            idx = ral.append(write_blob) + 1

            if idx == ENTRIES:
                break

    os.remove(BIN_NAME)


if __name__ == '__main__':
    main()
