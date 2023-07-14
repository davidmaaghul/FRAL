/*
    nanobind/nb_misc.h: Miscellaneous bits (GIL, etc.)

    Copyright (c) 2022 Wenzel Jakob

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

NAMESPACE_BEGIN(NB_NAMESPACE)

struct gil_scoped_acquire {
public:
    gil_scoped_acquire() noexcept : state(PyGILState_Ensure()) { }
    ~gil_scoped_acquire() { PyGILState_Release(state); }
    gil_scoped_acquire(const gil_scoped_acquire &) = delete;
    gil_scoped_acquire& operator=(const gil_scoped_acquire &) = delete;

private:
    const PyGILState_STATE state;
};

class gil_scoped_release {
public:
    gil_scoped_release() noexcept : state(PyEval_SaveThread()) { }
    ~gil_scoped_release() { PyEval_RestoreThread(state); }
    gil_scoped_release(const gil_scoped_release &) = delete;
    gil_scoped_release& operator=(const gil_scoped_release &) = delete;

private:
    PyThreadState *state;
};

inline void set_leak_warnings(bool value) noexcept {
    detail::set_leak_warnings(value);
}

inline void set_implicit_cast_warnings(bool value) noexcept {
    detail::set_implicit_cast_warnings(value);
}

NAMESPACE_END(NB_NAMESPACE)
