import os
import fral.python.fral.fral_ext as fral_cpp
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

    def allocate(self, sz: int) -> Optional[memoryview]:
        blob = self._ral.allocate(sz)
        if not blob:
            return None
        b = fral_cpp.Bytes(blob)
        return b.read()

    def append(self, arr: memoryview) -> int:
        return self._ral.append(fral_cpp.memoryview_to_pointer(arr))

    def read(self, idx: int) -> Optional[memoryview]:
        blob = self._ral.load(idx)
        if not blob:
            return None
        b = fral_cpp.Bytes(blob)
        return b.read()
