from fral.python.fral.fral import FRAL


def main():

    # Process 1
    ral_A = FRAL("test.bin", 1000, 100)
    test_bytes = "TEST".encode()

    test_blob = ral_A.allocate(len(test_bytes))
    test_blob[:len(test_bytes)] = test_bytes
    print(bytes(test_blob).decode())
    ral_A.append(test_blob)

    # Process 2
    ral_B = FRAL("test.bin")

    test_blob2 = ral_B.read(0)
    print(bytes(test_blob2).decode())
    test_blob2[0:1] = 'D'.encode()

    test_blob3 = ral_B.read(0)
    print(bytes(test_blob3).decode())


if __name__ == '__main__':
    main()




