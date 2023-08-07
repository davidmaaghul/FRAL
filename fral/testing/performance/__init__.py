import os

TEST_PATH = "fral/testing/performance"

RELEASE_PATH = os.path.join("cmake-build-release", TEST_PATH)

BLOB_SIZES = [100, 500, 1000, 5000, 10000, 50000]

WRITERS = [
    1,
    2,
    4,
    8,
]

GB_TO_B = 10**9

WRITERS_2 = [
    1,
    5,
    10,
    25,
    50,
    100,
    250,
    500,
    1000,
]
