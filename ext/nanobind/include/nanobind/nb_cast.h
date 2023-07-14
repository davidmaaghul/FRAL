/*
    nanobind/nb_cast.h: Type caster interface and essential type casters

    Copyright (c) 2022 Wenzel Jakob

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#define NB_TYPE_CASTER(Value_, descr)                                          \
    using Value = Value_;                                                      \
    static constexpr bool IsClass = false;                                     \
    static constexpr auto Name = descr;                                        \
    template <typename T_> using Cast = movable_cast_t<T_>;                    \
    static handle from_cpp(Value *p, rv_policy policy, cleanup_list *list) {   \
        if (!p)                                                                \
            return none().release();                                           \
        return from_cpp(*p, policy, list);                                     \
    }                                                                          \
    explicit operator Value *() { return &value; }                             \
    explicit operator Value &() { return value; }                              \
    explicit operator Value &&() && { return (Value &&) value; }               \
    Value value;

#define NB_MAKE_OPAQUE(...)                                                    \
    namespace nanobind::detail {                                               \
    template <> class type_caster<__VA_ARGS__>                                 \
        : public type_caster_base<__VA_ARGS__> { }; }

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

enum cast_flags : uint8_t {
    // Enable implicit conversions (impl. assumes this is 1, don't reorder..)
    convert = (1 << 0),

    // Passed to the 'self' argument in a constructor call (__init__)
    construct = (1 << 1)
};

template <typename T> using cast_t = typename make_caster<T>::template Cast<T>;

template <typename T>
using simple_cast_t =
    std::conditional_t<is_pointer_v<T>, intrinsic_t<T> *, intrinsic_t<T> &>;

template <typename T>
using movable_cast_t =
    std::conditional_t<is_pointer_v<T>, intrinsic_t<T> *,
                       std::conditional_t<std::is_rvalue_reference_v<T>,
                                          intrinsic_t<T> &&, intrinsic_t<T> &>>;

template <typename T>
struct type_caster<T, enable_if_t<std::is_arithmetic_v<T> && !is_std_char_v<T>>> {
public:
    NB_INLINE bool from_python(handle src, uint8_t flags, cleanup_list *) noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            if constexpr (sizeof(T) == 8)
                return detail::load_f64(src.ptr(), flags, &value);
            else
                return detail::load_f32(src.ptr(), flags, &value);
        } else {
            if constexpr (std::is_signed_v<T>) {
                if constexpr (sizeof(T) == 8)
                    return detail::load_i64(src.ptr(), flags, (int64_t *) &value);
                else if constexpr (sizeof(T) == 4)
                    return detail::load_i32(src.ptr(), flags, (int32_t *) &value);
                else if constexpr (sizeof(T) == 2)
                    return detail::load_i16(src.ptr(), flags, (int16_t *) &value);
                else
                    return detail::load_i8(src.ptr(), flags, (int8_t *) &value);
            } else {
                if constexpr (sizeof(T) == 8)
                    return detail::load_u64(src.ptr(), flags, (uint64_t *) &value);
                else if constexpr (sizeof(T) == 4)
                    return detail::load_u32(src.ptr(), flags, (uint32_t *) &value);
                else if constexpr (sizeof(T) == 2)
                    return detail::load_u16(src.ptr(), flags, (uint16_t *) &value);
                else
                    return detail::load_u8(src.ptr(), flags, (uint8_t *) &value);
            }
        }
    }

    NB_INLINE static handle from_cpp(T src, rv_policy, cleanup_list *) noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return PyFloat_FromDouble((double) src);
        } else {
            if constexpr (std::is_signed_v<T>) {
                if constexpr (sizeof(T) <= sizeof(long))
                    return PyLong_FromLong((long) src);
                else
                    return PyLong_FromLongLong((long long) src);
            } else {
                if constexpr (sizeof(T) <= sizeof(unsigned long))
                    return PyLong_FromUnsignedLong((unsigned long) src);
                else
                    return PyLong_FromUnsignedLongLong((unsigned long long) src);
            }
        }
    }

    NB_TYPE_CASTER(T, const_name<std::is_integral_v<T>>("int", "float"));
};

template <> struct type_caster<void_type> {
    static constexpr auto Name = const_name("None");
    static constexpr bool IsClass = false;
};

template <> struct type_caster<void> {
    template <typename T_> using Cast = void *;
    using Value = void*;
    static constexpr bool IsClass = false;
    static constexpr auto Name = const_name("capsule");
    explicit operator void *() { return value; }
    Value value;

    bool from_python(handle src, uint8_t, cleanup_list *) noexcept {
        if (src.is_none()) {
            value = nullptr;
            return true;
        } else {
            value = PyCapsule_GetPointer(src.ptr(), "nb_handle");
            if (!value) {
                PyErr_Clear();
                return false;
            }
            return true;
        }
    }

    static handle from_cpp(void *ptr, rv_policy, cleanup_list *) noexcept {
        if (ptr)
            return PyCapsule_New(ptr, "nb_handle", nullptr);
        else
            return none().release();
    }
};

template <> struct type_caster<std::nullptr_t> {
    bool from_python(handle src, uint8_t, cleanup_list *) noexcept {
        if (src.is_none())
            return true;
        return false;
    }

    static handle from_cpp(std::nullptr_t, rv_policy, cleanup_list *) noexcept {
        return none().release();
    }

    NB_TYPE_CASTER(std::nullptr_t, const_name("None"));
};

template <> struct type_caster<bool> {
    bool from_python(handle src, uint8_t, cleanup_list *) noexcept {
        if (src.ptr() == Py_True) {
            value = true;
            return true;
        } else if (src.ptr() == Py_False) {
            value = false;
            return true;
        } else {
            return false;
        }
    }

    static handle from_cpp(bool src, rv_policy, cleanup_list *) noexcept {
        return handle(src ? Py_True : Py_False).inc_ref();
    }

    NB_TYPE_CASTER(bool, const_name("bool"));
};

template <> struct type_caster<char> {
    using Value = const char *;
    Value value;
    static constexpr bool IsClass = false;
    static constexpr auto Name = const_name("str");
    template <typename T_>
    using Cast = std::conditional_t<is_pointer_v<T_>, const char *, char>;

    bool from_python(handle src, uint8_t, cleanup_list *) noexcept {
        value = PyUnicode_AsUTF8AndSize(src.ptr(), nullptr);
        if (!value) {
            PyErr_Clear();
            return false;
        }
        return true;
    }

    static handle from_cpp(const char *value, rv_policy,
                           cleanup_list *) noexcept {
        return PyUnicode_FromString(value);
    }

    static handle from_cpp(char value, rv_policy, cleanup_list *) noexcept {
        return PyUnicode_FromStringAndSize(&value, 1);
    }

    explicit operator const char *() { return value; }

    explicit operator char() {
        if (value && value[0] && value[1] == '\0')
            return value[0];
        else
            throw next_overload();
    }
};

template <typename T> struct type_caster<pointer_and_handle<T>> {
    using Caster = detail::make_caster<T>;
    using T2 = pointer_and_handle<T>;
    NB_TYPE_CASTER(T2, Caster::Name)

    bool from_python(handle src, uint8_t flags, cleanup_list *cleanup) noexcept {
        Caster c;
        if (!c.from_python(src, flags, cleanup))
            return false;
        value.h = src;
        value.p = c.operator T*();
        return true;
    }
};

template <typename T, typename X> struct type_caster<typed<T, X>> {
    using Caster = detail::make_caster<T>;
    using T2 = typed<T, X>;
    NB_TYPE_CASTER(T2, X::Name)

    bool from_python(handle src, uint8_t flags, cleanup_list *cleanup) noexcept {
        Caster c;
        if (!c.from_python(src, flags, cleanup))
            return false;
        value = T2{ (T &&) c.value };
        return true;
    }

    static handle from_cpp(const T2 &src, rv_policy policy,
                           cleanup_list *cleanup) noexcept {
        return Caster::from_cpp(src.value, policy, cleanup);
    }
};

template <typename T>
struct type_caster<T, enable_if_t<std::is_base_of_v<detail::api_tag, T>>> {
public:
    NB_TYPE_CASTER(T, T::Name)

    bool from_python(handle src, uint8_t, cleanup_list *) noexcept {
        if (!isinstance<T>(src))
            return false;

        if constexpr (std::is_base_of_v<object, T>)
            value = borrow<T>(src);
        else
            value = src;

        return true;
    }

    static handle from_cpp(const handle &src, rv_policy,
                           cleanup_list *) noexcept {
        return src.inc_ref();
    }
};

template <typename T> NB_INLINE rv_policy infer_policy(rv_policy policy) {
    if constexpr (is_pointer_v<T>) {
        if (policy == rv_policy::automatic)
            policy = rv_policy::take_ownership;
        else if (policy == rv_policy::automatic_reference)
            policy = rv_policy::reference;
    } else if constexpr (std::is_lvalue_reference_v<T>) {
        if (policy == rv_policy::automatic ||
            policy == rv_policy::automatic_reference)
            policy = rv_policy::copy;
    } else {
        if (policy == rv_policy::automatic ||
            policy == rv_policy::automatic_reference ||
            policy == rv_policy::reference ||
            policy == rv_policy::reference_internal)
            policy = rv_policy::move;
    }
    return policy;
}

template <typename T, typename SFINAE = int> struct type_hook : std::false_type { };

template <typename Type_> struct type_caster_base {
    using Type = Type_;
    static constexpr auto Name = const_name<Type>();
    static constexpr bool IsClass = true;

    template <typename T> using Cast = movable_cast_t<T>;

    NB_INLINE bool from_python(handle src, uint8_t flags,
                               cleanup_list *cleanup) noexcept {
        return nb_type_get(&typeid(Type), src.ptr(), flags, cleanup,
                           (void **) &value);
    }

    template <typename T>
    NB_INLINE static handle from_cpp(T &&value, rv_policy policy,
                                     cleanup_list *cleanup) noexcept {
        Type *ptr;
        if constexpr (is_pointer_v<T>)
            ptr = (Type *) value;
        else
            ptr = (Type *) &value;

        policy = infer_policy<T>(policy);
        const std::type_info *type = &typeid(Type);

        constexpr bool has_type_hook =
            !std::is_base_of_v<std::false_type, type_hook<Type>>;
        if constexpr (has_type_hook)
            type = type_hook<Type>::get(ptr);

        if constexpr (!std::is_polymorphic_v<Type>) {
            return nb_type_put(type, ptr, policy, cleanup);
        } else {
            const std::type_info *type_p =
                (!has_type_hook && ptr) ? &typeid(*ptr) : nullptr;
            return nb_type_put_p(type, type_p, ptr, policy, cleanup);
        }
    }

    operator Type*() { return value; }

    operator Type&() {
        raise_next_overload_if_null(value);
        return *value;
    }

    operator Type&&() && {
        raise_next_overload_if_null(value);
        return (Type &&) *value;
    }

private:
    Type *value;
};

template <typename Type, typename SFINAE>
struct type_caster : type_caster_base<Type> { };

NAMESPACE_END(detail)

template <typename T, typename Derived>
bool try_cast(const detail::api<Derived> &value, T &out, bool convert = true) noexcept {
    using Caster = detail::make_caster<T>;
    using Output = typename Caster::template Cast<T>;

    static_assert(!std::is_same_v<const char *, T>,
                  "nanobind::try_cast(): cannot return a reference to a temporary.");

    Caster caster;
    if (caster.from_python(value.derived().ptr(),
                           convert ? (uint8_t) detail::cast_flags::convert
                                   : (uint8_t) 0, nullptr)) {
        if constexpr (Caster::IsClass)
            out = caster.operator Output();
        else
            out = std::move(caster.operator Output&&());

        return true;
    }

    return false;
}

template <typename T, typename Derived>
T cast(const detail::api<Derived> &value, bool convert = true) {
    if constexpr (std::is_same_v<T, void>) {
        return;
    } else {
        using Caster = detail::make_caster<T>;
        using Output = typename Caster::template Cast<T>;

        static_assert(
            !(std::is_reference_v<T> || std::is_pointer_v<T>) || Caster::IsClass ||
            std::is_same_v<const char *, T>,
            "nanobind::cast(): cannot return a reference to a temporary.");

        Caster caster;
        if (!caster.from_python(value.derived().ptr(),
                                convert ? (uint8_t) detail::cast_flags::convert
                                        : (uint8_t) 0, nullptr))
            detail::raise_cast_error();

        // GCC misses that from_python will return or ensure orderly initialization
        #if defined(__GNUC__) && !defined(__clang__)
          #pragma GCC diagnostic push
          #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        #endif

        return caster.operator Output();

        #if defined(__GNUC__) && !defined(__clang__)
          #pragma GCC diagnostic pop
        #endif
    }
}

template <typename T>
object cast(T &&value, rv_policy policy = rv_policy::automatic_reference) {
    handle h = detail::make_caster<T>::from_cpp((detail::forward_t<T>) value,
                                                policy, nullptr);
    if (!h.is_valid())
        detail::raise_cast_error();
    return steal(h);
}

template <typename T> object find(const T &value) noexcept {
    return steal(detail::make_caster<T>::from_cpp(value, rv_policy::none, nullptr));
}

template <rv_policy policy = rv_policy::automatic, typename... Args>
tuple make_tuple(Args &&...args) {
    tuple result = steal<tuple>(PyTuple_New((Py_ssize_t) sizeof...(Args)));

    size_t nargs = 0;
    PyObject *o = result.ptr();

    (NB_TUPLE_SET_ITEM(o, nargs++,
                       detail::make_caster<Args>::from_cpp(
                           (detail::forward_t<Args>) args, policy, nullptr)
                           .ptr()),
     ...);

    detail::tuple_check(o, sizeof...(Args));

    return result;
}

template <typename T> arg_v arg::operator=(T &&value) const {
    return arg_v(*this, cast((detail::forward_t<T>) value));
}

template <typename Impl> template <typename T>
detail::accessor<Impl>& detail::accessor<Impl>::operator=(T &&value) {
    object result = cast((detail::forward_t<T>) value);
    Impl::set(m_base, m_key, result.ptr());
    return *this;
}

template <typename T> void list::append(T &&value) {
    object o = nanobind::cast((detail::forward_t<T>) value);
    if (PyList_Append(m_ptr, o.ptr()))
        detail::raise_python_error();
}

NAMESPACE_END(NB_NAMESPACE)
