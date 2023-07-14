/*
    nanobind/stl/unique_ptr.h: Type caster for std::unique_ptr<T>

    Copyright (c) 2022 Wenzel Jakob

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#pragma once

#include <nanobind/nanobind.h>
#include <memory>

NAMESPACE_BEGIN(NB_NAMESPACE)

// Deleter for std::unique_ptr<T> (handles ownership by both C++ and Python)
template <typename T> struct deleter {
    /// Instance should be cleared using a delete expression
    deleter()  = default;

    /// Instance owned by Python, reduce reference count upon deletion
    deleter(handle h) : o(h.ptr()) { }

    /// Does Python own storage of the underlying object
    bool owned_by_python() const { return o != nullptr; }

    /// Does C++ own storage of the underlying object
    bool owned_by_cpp() const { return o == nullptr; }

    /// Perform the requested deletion operation
    void operator()(void *p) noexcept {
        if (o) {
            gil_scoped_acquire guard;
            Py_DECREF(o);
        } else {
            delete (T *) p;
        }
    }

    PyObject *o{nullptr};
};

NAMESPACE_BEGIN(detail)

template <typename T, typename Deleter>
struct type_caster<std::unique_ptr<T, Deleter>> {
    using Value = std::unique_ptr<T, Deleter>;
    using Caster = make_caster<T>;

    static constexpr bool IsDefaultDeleter =
        std::is_same_v<Deleter, std::default_delete<T>>;
    static constexpr bool IsNanobindDeleter =
        std::is_same_v<Deleter, deleter<T>>;

    static_assert(Caster::IsClass,
                  "Binding 'std::unique_ptr<T>' requires that 'T' can also be "
                  "bound by nanobind. It appears that you specified a type which "
                  "would undergo conversion/copying, which is not allowed.");

    static_assert(IsDefaultDeleter || IsNanobindDeleter,
                  "Binding std::unique_ptr<T, Deleter> requires that 'Deleter' is either "
                  "'std::default_delete<T>' or 'nanobind::deleter<T>'");

    static constexpr auto Name = Caster::Name;
    static constexpr bool IsClass = true;
    template <typename T_> using Cast = Value;

    Caster caster;
    handle src;

    bool from_python(handle src_, uint8_t, cleanup_list *) noexcept {
        // Stash source python object
        src = src_;

        /* Try casting to a pointer of the underlying type. We pass flags=0 and
           cleanup=nullptr to prevent implicit type conversions (they are
           problematic since the instance then wouldn't be owned by 'src') */
        return caster.from_python(src_, 0, nullptr);
    }

    template <typename T2>
    static handle from_cpp(T2 *value, rv_policy policy,
                           cleanup_list *cleanup) noexcept {
        if (!value)
            return handle();

        return from_cpp(*value, policy, cleanup);
    }

    template <typename T2>
    static handle from_cpp(T2 &&value,
                           rv_policy, cleanup_list *cleanup) noexcept {

        bool cpp_delete = true;
        if constexpr (IsNanobindDeleter)
            cpp_delete = value.get_deleter().owned_by_cpp();

        T *ptr = value.get();
        const std::type_info *type = &typeid(T);

        constexpr bool has_type_hook =
            !std::is_base_of_v<std::false_type, type_hook<T>>;
        if constexpr (has_type_hook)
            type = type_hook<T>::get(ptr);

        handle result;
        if constexpr (!std::is_polymorphic_v<T>) {
            result = nb_type_put_unique(type, ptr, cleanup, cpp_delete);
        } else {
            const std::type_info *type_p =
                (!has_type_hook && ptr) ? &typeid(*ptr) : nullptr;

            result = nb_type_put_unique_p(type, type_p, ptr, cleanup, cpp_delete);
        }

        if (result.is_valid()) {
            if (cpp_delete)
                value.release();
            else
                value.reset();
        }

        return result;
    }

    explicit operator Value() {
        nb_type_relinquish_ownership(src.ptr(), IsDefaultDeleter);

        T *value = caster.operator T *();
        if constexpr (IsNanobindDeleter)
            return Value(value, deleter<T>(src.inc_ref()));
        else
            return Value(value);
    }
};

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)
