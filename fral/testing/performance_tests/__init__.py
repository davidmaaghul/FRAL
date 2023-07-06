import os

TEST_PATH = "fral/testing/performance_tests"

RELEASE_PATH = os.path.join("cmake-build-release", TEST_PATH)

BLOB_SIZES = [100, 500, 1000, 5000, 10000, 50000]

WRITERS = [
    1,
    2,
    4,
    8,
]

ENTRIES = [100, 500, 1000, 5000, 10000]

GB_TO_B = 10**9
