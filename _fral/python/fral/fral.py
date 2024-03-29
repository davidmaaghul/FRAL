import os
import fral.fral_ext as fral_cpp
from typing import Optional


class FRAL(object):
    def __init__(self, file_name: str, sz: Optional[int] = None, max_entries: Optional[int] = None):
        self._file_name = file_name
        self._ral = None
        if os.path.exists(os.path.join(os.getcwd(), file_name)):
            self._connect()
        else:
            if any([not sz, not max_entries]):
                raise Exception("Can't create new queue without sz and max_entries args")
            self._create(sz, max_entries)

    def _connect(self):
        self._ral = fral_cpp.FRAL(self._file_name)

    def _create(self, sz: int, max_entries: int) -> "fral_cpp.FRAL":
        self._ral = fral_cpp.FRAL(self._file_name, sz, max_entries)

    def prime_cache(self):
        self._ral.prime_cache()

    def allocate(self, sz: int) -> Optional[memoryview]:
        blob = self._ral.allocate(sz)
        if not blob:
            return None
        b = fral_cpp.Bytes(blob)
        return b.read()

    def size(self):
        return self._ral.size()

    def max_size(self):
        return self._ral.max_size()

    def max_memory(self):
        return self._ral.max_memory()

    def get_blob_size(self, arr: memoryview) -> int:
        capsule = fral_cpp.memoryview_to_pointer(arr)
        return self._ral.get_blob_size(capsule)

    def append(self, arr: memoryview) -> int:
        return self._ral.append(fral_cpp.memoryview_to_pointer(arr))

    def read(self, idx: int) -> Optional[memoryview]:
        blob = self._ral.load(idx)
        if not blob:
            return None
        b = fral_cpp.Bytes(blob)
        return b.read()

    def __getitem__(self, idx: int) -> Optional[memoryview]:

        return self.read(idx)
