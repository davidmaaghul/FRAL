#include <nanobind/nanobind.h>
#include "../engine/engine.h"
#include "Python.h"

namespace nb = nanobind;

class memoryview : public nb::object {
    NB_OBJECT_DEFAULT(memoryview, nb::object, "memoryview", PyMemoryView_Check);
    memoryview(char *str, size_t n)
            : nb::object(PyMemoryView_FromMemory(str, (Py_ssize_t) n, PyBUF_WRITE),
                         nb::detail::borrow_t{}){}
    void *c_str() {return PyMemoryView_GET_BUFFER(m_ptr)->buf; }
};

class Bytes {
public:
    Bytes(void * blob) : blob(blob), sz(fral::FRAL::getBlobSize(blob)) {}
    [[nodiscard]] memoryview read_bytes() const {return memoryview((char *) blob, sz);}
    void * blob;
    size_t sz;
};

void * memoryview_to_pointer(memoryview * bytearr){
    return bytearr->c_str();
}


NB_MODULE(fral_ext, m) {

nb::class_<fral::FRAL>(m, "FRAL")
.def(nb::init<const char *, size_t, size_t>(), nb::rv_policy::reference)
.def(nb::init<const char *>(), nb::rv_policy::reference)
.def("allocate", &fral::FRAL::allocate)
.def("append", &fral::FRAL::append)
.def("load", &fral::FRAL::load);

nb::class_<Bytes>(m, "Bytes")
.def(nb::init<void *>())
.def("read", &Bytes::read_bytes, nb::rv_policy::reference);
m.def("memoryview_to_pointer", &memoryview_to_pointer, nb::rv_policy::reference);

}
