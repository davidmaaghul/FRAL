import os
import tempfile
import pandas as pd

from unittest import TestCase
from unittest.mock import patch
from _fral.python.fral.fral import FRAL
import pickle


class Test(TestCase):
    def test_init(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            test = FRAL(fname, 1000, 100)
            self.assertEqual(0, test.size())
            self.assertEqual(100, test.max_size())
            self.assertEqual(1000, test.max_memory())

    def test_connect(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            FRAL(fname, 1000, 100)
            test = FRAL(fname)
            self.assertEqual(0, test.size())
            self.assertEqual(100, test.max_size())
            self.assertEqual(1000, test.max_memory())

    def test_prime_cache(self):
        with tempfile.TemporaryDirectory() as tmp:
            with patch("_fral.python.fral.fral.fral_cpp") as test_fral_cpp:
                fname = os.path.join(tmp, "test.bin")
                test = FRAL(fname, 1000, 100)
                test.prime_cache()
                test_fral_cpp.FRAL.assert_called()
                test_fral_cpp.FRAL.return_value.prime_cache.assert_called()

    def test_allocate_fail(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            test = FRAL(fname, 1000, 100)
            blob = test.allocate(1801)
            self.assertEqual(None, blob)

    def test_append(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            test = FRAL(fname, 1000, 100)
            blob = test.allocate(10)
            test.append(blob)
            self.assertEqual(1, test.size())

    def test_append_fail(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            test = FRAL(fname, 1000, 1)
            blob = test.allocate(10)
            idx = test.append(blob)
            idx2 = test.append(blob)
            self.assertEqual(0, idx)
            self.assertEqual(-1, idx2)

    def test_get_blob_size(self):
        with tempfile.TemporaryDirectory() as tmp:
            fname = os.path.join(tmp, "test.bin")
            test = FRAL(fname, 1000, 100)
            blob = test.allocate(10)
            self.assertEqual(10, test.get_blob_size(blob))

    def test_allocate_append_read(self):
        with tempfile.TemporaryDirectory() as tmp:

            fname = os.path.join(tmp, "test.bin")

            test = FRAL(fname, 1000, 100)
            blob = test.allocate(100)

            blob[0:1] = b'5'
            blob[50:51] = b'6'

            idx = test.append(blob)

            blob2 = test[idx]

            self.assertEqual(b'5', blob2[0:1])
            self.assertEqual(b'6', blob2[50:51])

    def test_read_brackets(self):
        with tempfile.TemporaryDirectory() as tmp:

            fname = os.path.join(tmp, "test.bin")

            test = FRAL(fname, 1000, 100)
            blob = test.allocate(100)

            blob[0:1] = b'5'
            blob[50:51] = b'6'

            idx = test.append(blob)

            self.assertEqual(test[idx], test.read(idx))

    def test_allocate_append_read_dataframe(self):
        with tempfile.TemporaryDirectory() as tmp:

            fname = os.path.join(tmp, "test.bin")

            test = FRAL(fname, 100000, 100)

            test_df = pd.DataFrame({"A": [1, 2, 3, 4], "B": [1,  2, 3, 4]})

            all_dfs = [test_df, ]

            for k in range(1, 100):
                sub_df = all_dfs[k - 1] + 1
                all_dfs.append(sub_df)

            byte_count = 0
            i = 0
            for df in all_dfs:
                pkl_df = pickle.dumps(df)
                blob = test.allocate(len(pkl_df))
                blob[:len(pkl_df)] = pkl_df
                byte_count += test.get_blob_size(blob)
                idx = test.append(blob)
                self.assertEqual(i, idx)
                i += 1

            self.assertEqual(100, test.size())

            for i in range(100):
                blob = test[i]
                pd.testing.assert_frame_equal(all_dfs[i], pickle.loads(blob))















