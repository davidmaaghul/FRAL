C++ API Reference (Extras)
==========================

.. cpp:namespace:: nanobind

Operator overloading
--------------------

The following optional include directive imports the special value :cpp:var:`self`.

.. code-block:: cpp

   #include <nanobind/operators.h>

The underlying type exposes various C++ operators that enable a shorthand
notation to bind operators to python. See the :ref:`operator overloading
<operator_overloading>` example in the main documentation for details.


.. cpp:class:: detail::self_t

   This is an internal class that should be accessed through the singleton
   :cpp:var:`self` value.

   It supports the overloaded operators listed below. Depending on whether or
   not :cpp:var:`self` is the left or right argument of a binary operation, the
   binding will map to different Python methods as shown below.

   .. list-table::
      :header-rows: 1
      :widths: 50 50

      * - C++ operator
        - Python method (left or right)
      * - ``operator-``
        - ``__sub__``, ``__rsub__``
      * - ``operator+``
        - ``__add__``, ``__radd__``
      * - ``operator*``
        - ``__mul__``, ``__rmul__``
      * - ``operator/``
        - ``__truediv__``, ``__rtruediv__``
      * - ``operator%``
        - ``__mod__``, ``__rmod__``
      * - ``operator<<``
        - ``__lshift__``, ``__rlshift__``
      * - ``operator>>``
        - ``__rshift__``, ``__rrshift__``
      * - ``operator&``
        - ``__and__``, ``__rand__``
      * - ``operator^``
        - ``__xor__``, ``__rxor__``
      * - ``operator|``
        - ``__or__``, ``__ror__``
      * - ``operator>``
        - ``__gt__``, ``__lt__``
      * - ``operator>=``
        - ``__ge__``, ``__le__``
      * - ``operator<``
        - ``__lt__``, ``__gt__``
      * - ``operator<=``
        - ``__le__``, ``__ge__``
      * - ``operator==``
        - ``__eq__``
      * - ``operator!=``
        - ``__ne__``
      * - ``operator+=``
        - ``__iadd__``
      * - ``operator-=``
        - ``__isub__``
      * - ``operator*=``
        - ``__mul__``
      * - ``operator/=``
        - ``__itruediv__``
      * - ``operator%=``
        - ``__imod__``
      * - ``operator<<=``
        - ``__ilrshift__``
      * - ``operator>>=``
        - ``__ilrshift__``
      * - ``operator&=``
        - ``__iand__``
      * - ``operator^=``
        - ``__ixor__``
      * - ``operator|=``
        - ``__ior__``
      * - ``operator-`` (unary)
        - ``__neg__``
      * - ``operator+`` (unary)
        - ``__pos__``
      * - ``operator~``  (unary)
        - ``__invert__``
      * - ``operator!``  (unary)
        - ``__bool__`` (with extra negation)
      * - ``nb::abs(..)``
        - ``__abs__``
      * - ``nb::hash(..)``
        - ``__hash__``

.. cpp:var:: detail::self_t self

Trampolines
-----------

The following macros to implement trampolines that forward virtual function
calls to Python require an additional include directive:

.. code-block:: cpp

   #include <nanobind/trampoline.h>

See the section on :ref:`trampolines <trampolines>` for further detail.

.. c:macro:: NB_TRAMPOLINE(base, size)

   Install a trampoline in an alias class to enable dispatching C++ virtual
   function calls to a Python implementation. Refer to the documentation on
   :ref:`trampolines <trampolines>` to see how this macro can be used.

.. c:macro:: NB_OVERRIDE(func, ...)

   Dispatch the call to a Python method named ``"func"`` if it is overloaded on
   the Python side, and forward the function arguments specified in the
   variable length argument ``...``. Otherwise, call the C++ implementation
   `func` in the base class.

   Refer to the documentation on :ref:`trampolines <trampolines>` to see how
   this macro can be used.

.. c:macro:: NB_OVERRIDE_PURE(func, ...)

   Dispatch the call to a Python method named ``"func"`` if it is overloaded on
   the Python side, and forward the function arguments specified in the
   variable length argument ``...``. Otherwise, raise an exception. This macro
   should be used when the C++ function is pure virtual.

   Refer to the documentation on :ref:`trampolines <trampolines>` to see how
   this macro can be used.

.. c:macro:: NB_OVERRIDE_NAME(name, func, ...)

   Dispatch the call to a Python method named ``name`` if it is overloaded on
   the Python side, and forward the function arguments specified in the
   variable length argument ``...``. Otherwise, call the C++ function `func` in
   the base class.

   This function differs from :c:macro:`NB_OVERRIDE() <NB_OVERRIDE>` in that
   C++ and Python functions can be named differently (e.g., ``operator+`` and
   ``__add__``). Refer to the documentation on :ref:`trampolines <trampolines>`
   to see how this macro can be used.

.. c:macro:: NB_OVERRIDE_PURE_NAME(name, func, ...)

   Dispatch the call to a Python method named ``name`` if it is overloaded on
   the Python side, and forward the function arguments specified in the
   variable length argument ``...``. Otherwise, raise an exception. This macro
   should be used when the C++ function is pure virtual.

   This function differs from :c:macro:`NB_OVERRIDE_PURE() <NB_OVERRIDE_PURE>`
   in that C++ and Python functions can be named differently (e.g.,
   ``operator+`` and ``__add__``). Although the C++ base implementation cannot
   be called, its name is still important since nanobind uses it to infer the
   return value type. Refer to the documentation on :ref:`trampolines
   <trampolines>` to see how this macro can be used.

.. _vector_bindings:

STL vector bindings
-------------------

The following function can be used to expose ``std::vector<...>`` variants
in Python. It is not part of the core nanobind API and require an additional
include directive:

.. code-block:: cpp

   #include <nanobind/stl/bind_vector.h>

.. cpp:function:: template <typename Vector, typename... Args> class_<Vector> bind_vector(handle scope, const char * name, Args &&...args)

   Bind the STL vector-derived type `Vector` to the identifier `name` and
   place it in `scope` (e.g., a :cpp:class:`module_`). The variable argument
   list can be used to pass a docstring and other :ref:`class binding
   annotations <class_binding_annotations>`.

   The type includes the following methods resembling ``list``:

   .. list-table::
      :header-rows: 1
      :widths: 50 50

      * - Signature
        - Documentation
      * - ``__init__(self)``
        - Default constructor
      * - ``__init__(self, arg: Vector)``
        - Copy constructor
      * - ``__init__(self, arg: typing.Sequence)``
        - Construct from another sequence type
      * - ``__len__(self) -> int``
        - Return the number of elements
      * - ``__repr__(self) -> str``
        - Generate a string representation
      * - ``__contains__(self, arg: Value)``
        - Check if the vector contains ``arg``
      * - ``__eq__(self, arg: Vector)``
        - Check if the vector is equal to ``arg``
      * - ``__ne__(self, arg: Vector)``
        - Check if the vector is not equal to ``arg``
      * - ``__bool__(self) -> bool``
        - Check whether the vector is empty
      * - ``__iter__(self) -> iterator``
        - Instantiate an iterator to traverse the elements
      * - ``__getitem__(self, arg: int) -> Value``
        - Return an element from the list (supports negative indexing)
      * - ``__setitem__(self, arg0: int, arg1: Value)``
        - Assign an element in the list (supports negative indexing)
      * - ``__delitem__(self, arg: int)``
        - Delete an item from the list (supports negative indexing)
      * - ``__setitem__(self, arg: slice) -> Vector``
        - Slice-based getter
      * - ``__setitem__(self, arg0: slice, arg1: Value)``
        - Slice-based assignment
      * - ``__delitem__(self, arg: slice)``
        - Slice-based deletion
      * - ``clear(self)``
        - Remove all items from the list
      * - ``append(self, arg: Value)``
        - Append a list item
      * - ``insert(self, arg0: int, arg1: Value)``
        - Insert a list item (supports negative indexing)
      * - ``pop(self, index: int = -1)``
        - Pop an element at position ``index`` (the end by default)
      * - ``extend(self, arg: Vector)``
        - Extend ``self`` by appending elements from ``arg``.
      * - ``count(self, arg: Value)``
        - Count the number of times that ``arg`` is contained in the vector
      * - ``remove(self, arg: Value)``
        - Remove all occurrences of ``arg``.

   In contrast to ``std::vector<...>``, all bound functions perform range
   checks to avoid undefined behavior. When the type underlying the vector is
   not comparable or copy-assignable, some of these functions will not be
   generated.

   The binding operation is a no-op if the vector type has already been
   registered with nanobind.

.. _map_bindings:

STL map bindings
----------------

The following function can be used to expose ``std::map<...>`` or
``std::unordered_map<...>`` variants in Python. It is not part of the core
nanobind API and require an additional include directive:

.. code-block:: cpp

   #include <nanobind/stl/bind_map.h>

.. cpp:function:: template <typename Map, typename... Args> class_<Map> bind_map(handle scope, const char * name, Args &&...args)

   Bind the STL map-derived type `Map` (ordered or unordered) to the identifier
   `name` and place it in `scope` (e.g., a :cpp:class:`module_`). The variable
   argument list can be used to pass a docstring and other :ref:`class binding
   annotations <class_binding_annotations>`.

   The type includes the following methods resembling ``dict``:

   .. list-table::
      :header-rows: 1
      :widths: 50 50

      * - Signature
        - Documentation
      * - ``__init__(self)``
        - Default constructor
      * - ``__init__(self, arg: Map)``
        - Copy constructor
      * - ``__init__(self, arg: dict)``
        - Construct from a Python dictionary
      * - ``__len__(self) -> int``
        - Return the number of elements
      * - ``__repr__(self) -> str``
        - Generate a string representation
      * - ``__contains__(self, arg: Key)``
        - Check if the map contains ``arg``
      * - ``__eq__(self, arg: Map)``
        - Check if the map is equal to ``arg``
      * - ``__ne__(self, arg: Map)``
        - Check if the map is not equal to ``arg``
      * - ``__bool__(self) -> bool``
        - Check whether the map is empty
      * - ``__iter__(self) -> iterator``
        - Instantiate an iterator to traverse the set of map keys
      * - ``__getitem__(self, arg: Key) -> Value``
        - Return an element from the map
      * - ``__setitem__(self, arg0: Key, arg1: Value)``
        - Assign an element in the map
      * - ``__delitem__(self, arg: Key)``
        - Delete an item from the map
      * - ``clear(self)``
        - Remove all items from the list
      * - ``update(self, arg: Map)``
        - Update the map with elements from ``arg``.
      * - ``keys(self, arg: Map) -> Map.KeyView``
        - Returns an iterable view of the map's keys
      * - ``values(self, arg: Map) -> Map.ValueView``
        - Returns an iterable view of the map's values
      * - ``items(self, arg: Map) -> Map.ItemView``
        - Returns an iterable view of the map's items

   The binding operation is a no-op if the map type has already been
   registered with nanobind.

Unique pointer deleter
----------------------

The following *deleter* should be used to gain maximal flexibility in combination with
``std::unique_ptr<..>``. It requires the following additional include directive:

.. code-block:: cpp

   #include <nanobind/stl/unique_ptr.h>

See the two documentation sections on unique pointers for further detail
(:ref:`#1 <unique_ptr>`, :ref:`#2 <unique_ptr_adv>`).

.. cpp:struct:: template <typename T> deleter

   .. cpp:function:: deleter() = default

      Create a deleter that destroys the object using a ``delete`` expression.

   .. cpp:function:: deleter(handle h)

      Create a deleter that destroys the object by reducing the Python reference count.

   .. cpp:function:: bool owned_by_python() const

      Check if the object is owned by Python.

   .. cpp:function:: bool owned_by_cpp() const

      Check if the object is owned by C++.

   .. cpp:function:: void operator()(void * p) noexcept

      Destroy the object at address `p`.

.. _iterator_bindings:

Iterator bindings
-----------------

The following functions can be used to expose existing C++ iterators in
Python. They are not part of the core nanobind API and require an additional
include directive:

.. code-block:: cpp

   #include <nanobind/make_iterator.h>

.. cpp:function:: template <rv_policy Policy = rv_policy::reference_internal, typename Iterator, typename... Extra> iterator make_iterator(handle scope, const char * name, Iterator &&first, Iterator &&last, Extra &&...extra)

   Create a Python iterator wrapping the C++ iterator represented by the range
   ``[first, last)``. The `Extra` parameter can be used to pass additional
   function binding annotations.

   This function lazily creates a new Python iterator type identified by
   `name`, which is stored in the given `scope`. Usually, some kind of
   :cpp:class:`keep_alive` annotation is needed to tie the lifetime of the
   parent container to that of the iterator.

   Here is an example of what this might look like for a STL vector:

   .. code-block:: cpp

      using IntVec = std::vector<int>;

      nb::class_<IntVec>(m, "IntVec")
         .def("__iter__",
              [](const IntVec &v) {
                  return nb::make_iterator(nb::type<IntVec>(), "iterator",
                                           v.begin(), v.end());
              }, nb::keep_alive<0, 1>());


.. cpp:function:: template <rv_policy Policy = rv_policy::reference_internal, typename Type, typename... Extra> iterator make_iterator(handle scope, const char * name, Type &value, Extra &&...extra)

   This convenience wrapper calls the above `make_iterator` variant with
   ``first`` and ``last`` set to ``std::begin(value)`` and ``std::end(value)``,
   respectively.

.. cpp:function:: template <rv_policy Policy = rv_policy::reference_internal, typename Iterator, typename... Extra> iterator make_key_iterator(handle scope, const char * name, Iterator &&first, Iterator &&last, Extra &&...extra)

   :cpp:func:`make_iterator` specialization for C++ iterators that return
   key-value pairs. `make_key_iterator` returns the first pair element to
   iterate over keys.


.. cpp:function:: template <rv_policy Policy = rv_policy::reference_internal, typename Iterator, typename... Extra> iterator make_value_iterator(handle scope, const char * name, Iterator &&first, Iterator &&last, Extra &&...extra)

   :cpp:func:`make_iterator` specialization for C++ iterators that return
   key-value pairs. `make_value_iterator` returns the second pair element to
   iterate over values.

N-dimensional array type
------------------------

The following type can be used to exchange n-dimension arrays with frameworks
like NumPy, PyTorch, Tensorflow, JAX, and others. It requires an additional
include directive:

.. code-block:: cpp

   #include <nanobind/ndarray.h>

Detailed documentation including example code is provided in a :ref:`separate
section <ndarrays>`.

.. cpp:function:: bool ndarray_check(handle h) noexcept

   Test whether the Python object represents an ndarray. Currently, the
   function considers NumPy, PyTorch, TensorFlow, and XLA arrays.

.. cpp:class:: template <typename... Args> ndarray

   .. cpp:function:: ndarray() = default

      Create an invalid array.

   .. cpp:function:: ndarray(const ndarray &)

      Copy constructor. Increases the reference count of the referenced array.

   .. cpp:function:: ndarray(ndarray &&)

      Move constructor. Steals the referenced array without changing reference counts.

   .. cpp:function:: ~ndarray()

      Decreases the reference count of the referenced array and potentially destroy it.

   .. cpp:function:: ndarray& operator=(const ndarray &)

      Copy assignment operator. Increases the reference count of the referenced array.
      Decreases the reference count of the previously referenced array and potentially destroy it.

   .. cpp:function:: ndarray& operator=(ndarray &&)

      Move assignment operator. Steals the referenced array without changing reference counts.
      Decreases the reference count of the previously referenced array and potentially destroy it.

   .. cpp:function:: ndarray(void * value, size_t ndim, const size_t * shape, handle owner = nanobind::handle(), const int64_t * strides = nullptr, dlpack::dtype dtype = nanobind::dtype<Scalar>(), int32_t device_type = device::cpu::value, int32_t device_id = 0)

      Create an array wrapping an existing memory allocation. The following
      parameters can be specified:

      - `value`: pointer address of the memory region. When the ndarray is
        parameterized by a constant scalar type to indicate read-only access, a
        const pointer must be passed instead.

      - `ndim`: the number of dimensions.

      - `shape`: specifies the size along each axis. The referenced array must
        must have `ndim` entries.

      - `owner`: if provided, the array will hold a reference to this object
        until it is destructed.

      - `strides` is optional; a value of ``nullptr`` implies C-style strides.

      - `dtype` describes the data type (floating point, signed/unsigned
        integer) and bit depth.

      - The `device_type` and `device_id` indicate the device and address
        space associated with the pointer `value`.

   .. cpp:function:: ndarray(void * value, const std::initializer_list<size_t> shape, handle owner = nanobind::handle(), std::initializer_list<int64_t> strides = { }, dlpack::dtype dtype = nanobind::dtype<Scalar>(), int32_t device_type = device::cpu::value, int32_t device_id = 0)

      Alternative form of the above constructor, which accepts the ``shape``
      and ``strides`` arguments using a ``std::initializer_list``. It
      automatically infers the value of ``ndim`` based on the size of
      ``shape``.

   .. cpp:function:: dlpack::dtype dtype() const

      Return the data type underlying the array

   .. cpp:function:: size_t ndim() const

      Return the number of dimensions.

   .. cpp:function:: size_t size() const

      Return the size of the array (i.e. the product of all dimensions).

   .. cpp:function:: size_t itemsize() const

      Return the size of a single array element in bytes. The returned value
      is rounded to the next full byte in case of bit-level representations
      (query :cpp:member:`dtype::bits` for bit-level granularity).

   .. cpp:function:: size_t nbytes() const

      Return the size of the entire array bytes. The returned value is rounded
      to the next full byte in case of bit-level representations.

   .. cpp:function:: size_t shape(size_t i) const

      Return the size of dimension `i`.

   .. cpp:function:: int64_t stride(size_t i) const

      Return the stride of dimension `i`.

   .. cpp:function:: const int64_t* shape_ptr() const

      Return a pointer to the shape array. Note that the return type is
      ``const int64_t*``, which may be unexpected as the scalar version
      :cpp:func:`shape()` casts its result to a ``size_t``.

      This is a consequence of the DLPack tensor representation that uses
      signed 64-bit integers for all of these fields.

   .. cpp:function:: const int64_t* stride_ptr() const

      Return pointer to the stride array.

   .. cpp:function:: bool is_valid() const

      Check whether the array is in a valid state.

   .. cpp:function:: int32_t device_type() const

      ID denoting the type of device hosting the array. This will match the
      ``value`` field of a device class, such as :cpp:class:`device::cpu::value
      <device::cpu>` or :cpp:class:`device::cuda::value <device::cuda>`.

   .. cpp:function:: int32_t device_id() const

      In a multi-device/GPU setup, this function returns the ID of the device
      storing the array.

   .. cpp:function:: const Scalar * data() const

      Return a const pointer to the array data.

   .. cpp:function:: Scalar * data()

      Return a mutable pointer to the array data. Only enabled when `Scalar` is
      not itself ``const``.

   .. cpp:function:: template <typename... Ts> auto& operator()(Ts... indices)

      Return a mutable reference to the element at stored at the provided
      index/indices. ``sizeof(Ts)`` must match :cpp:func:`ndim()`.

Data types
^^^^^^^^^^

Nanobind uses the `DLPack <https://github.com/dmlc/dlpack>`_ ABI to represent
metadata describing n-dimensional arrays (even when they are exchanged using
the buffer protocol). Consequently, the set of possible dtypes is :ref:`more
restricted <dtype_restrictions>` than that of other nd-array libraries (e.g.,
NumPy). Relevant data structures are located in the ``nanobind::dlpack``
sub-namespace.


.. cpp:enum-class:: dlpack::dtype_code : uint8_t

   This enumeration characterizes the elementary array data type regardless of
   bit depth.

   .. cpp:enumerator:: Int = 0

      Signed integer format

   .. cpp:enumerator:: UInt = 1

      Unsigned integer format

   .. cpp:enumerator:: Float = 2

      IEEE-754 floating point format

   .. cpp:enumerator:: Bfloat = 4

      "Brain" floating point format

   .. cpp:enumerator:: Complex = 5

      Complex numbers parameterized by real and imaginary component

.. cpp:struct:: dlpack::dtype

   Represents the data type underlying an n-dimensional array. Use the
   :cpp:func:`dtype\<T\>() <::nanobind::dtype>` function to return a populated
   instance of this data structure given a scalar C++ arithmetic type.

   .. cpp:member:: uint8_t code = 0;

      This field must contain the value of one of the
      :cpp:enum:`dlpack::dtype_code` enumerants.

   .. cpp:member:: uint8_t bits = 0;

      Number of bits per entry (e.g., 32 for a C++ single precision ``float``)

   .. cpp:member:: uint16_t lanes = 0;

      Number of SIMD lanes (typically ``1``)

.. cpp:function:: template <typename T> dlpack::dtype dtype()

   Returns a populated instance of the :cpp:class:`dlpack::dtype` structure
   given a scalar C++ arithmetic type.

Array annotations
^^^^^^^^^^^^^^^^^

The :cpp:class:`ndarray\<..\> <ndarray>` class admits optional template
parameters. They constrain the type of array arguments that may be passed to a
function.

The following are supported:

Data type
+++++++++

The data type of the underlying scalar element. The following are supported.

- ``[u]int8_t`` up to ``[u]int64_t`` and other variations (``unsigned long long``, etc.)
- ``float``, ``double``
- ``bool``

Annotate the data type with ``const`` to indicate a read-only array. Note that
only the buffer protocol/NumPy interface considers ``const``-ness at the
moment; data exchange with other array libraries will ignore this annotation.

When the is unspecified (e.g., to accept arbitrary input arrays), the
:cpp:class:`ro` annotation can instead be used to denote read-only access:

.. cpp:class:: ro

   Indicate read-only access (use only when no data type is specified.)


nanobind does not support non-standard types as documented in the section on
:ref:`dtype limitations <dtype_restrictions>`.

Shape
+++++

.. cpp:class:: template<size_t... Is> shape

   Require the array to have ``sizeof...(Is)`` dimensions. Each entry of `Is`
   specifies a fixed size constraint for that specific dimension. An entry
   equal to :cpp:var:`any` indicates that any size should be accepted for this
   dimension.

.. cpp:var:: constexpr size_t any = (size_t) -1

Contiguity
++++++++++

.. cpp:class:: c_contig

   Request that the array storage uses a C-contiguous representation.

.. cpp:class:: f_contig

   Request that the array storage uses a F (Fortran)-contiguous representation.

.. cpp:class:: any_contig

   Don't place any demands on array contiguity (the default).

Device type
+++++++++++

.. cpp:class:: device

   The following helper classes can be used to constrain the device and
   address space of an array. Each class has a ``static constexpr int32_t
   value`` field that will then match up with
   :cpp:func:`ndarray::device_id()`.

   .. cpp:class:: cpu

      CPU heap memory

   .. cpp:class:: cuda

      NVIDIA CUDA device memory

   .. cpp:class:: cuda_host

      NVIDIA CUDA host-pinned memory

   .. cpp:class:: cuda_managed

      NVIDIA CUDA managed memory

   .. cpp:class:: vulkan

      Vulkan device memory

   .. cpp:class:: metal

      Apple Metal device memory

   .. cpp:class:: rocm

      AMD ROCm device memory

   .. cpp:class:: rocm_host

      AMD ROCm host memory

   .. cpp:class:: oneapi

      Intel OneAPI device memory

Framework
+++++++++

Framework annotations cause :cpp:class:`nb::ndarray <ndarray>` objects to
convert into an equivalent representation in one of the following frameworks:

.. cpp:class:: numpy

.. cpp:class:: tensorflow

.. cpp:class:: pytorch

.. cpp:class:: jax

Eigen convenience type aliases
------------------------------

The following helper type aliases require an additional include directive:

.. code-block:: cpp

   #include <nanobind/eigen/dense.h>

.. cpp:type:: DStride = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>

   This type alias refers to an Eigen stride object that is sufficiently flexible
   so that can be easily called with NumPy arrays and array slices.

.. cpp:type:: template <typename T> DRef = Eigen::Ref<T, 0, DStride>

   This templated type alias creates an ``Eigen::Ref<..>`` with flexible strides for
   zero-copy data exchange between Eigen and NumPy.

.. cpp:type:: template <typename T> DMap = Eigen::Map<T, 0, DStride>

   This templated type alias creates an ``Eigen::Map<..>`` with flexible strides for
   zero-copy data exchange between Eigen and NumPy.

.. _chrono_conversions:

Timestamp and duration conversions
----------------------------------

nanobind supports bidirectional conversions of timestamps and
durations between their standard representations in Python
(:py:class:`datetime.datetime`, :py:class:`datetime.timedelta`) and in C++
(``std::chrono::time_point``, ``std::chrono::duration``).
A few unidirectional conversions from other Python types to these
C++ types are also provided and explained below.

These type casters require an additional include directive:

.. code-block:: cpp

   #include <nanobind/stl/chrono.h>

.. The rest of this section is adapted from pybind11/docs/advanced/cast/chrono.rst

An overview of clocks in C++11
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The C++11 standard defines three different clocks, and users can
define their own. Each ``std::chrono::time_point`` is defined relative
to a particular clock. When using the ``chrono`` type caster, you must be
aware that only ``std::chrono::system_clock`` is guaranteed to convert
to a Python :py:class:`~datetime.datetime` object; other clocks may convert to
:py:class:`~datetime.timedelta` if they don't represent calendar time.

The first clock defined by the standard is ``std::chrono::system_clock``.
This clock measures the current date and time, much like the Python
:py:func:`time.time` function. It can change abruptly due to
administrative actions, daylight savings time transitions, or
synchronization with an external time server. That makes this clock a
poor choice for timing purposes, but a good choice for wall-clock time.

The second clock defined by the standard is ``std::chrono::steady_clock``.
This clock ticks at a steady rate and is never adjusted, like
:py:func:`time.monotonic` in Python. That makes it excellent for timing
purposes, but the value in this clock does not correspond to the
current date and time. Often this clock will measure the amount of
time your system has been powered on. This clock will never be
the same clock as the system clock, because the system clock can
change but steady clocks cannot.

The third clock defined in the standard is ``std::chrono::high_resolution_clock``.
This clock is the clock that has the highest resolution out of all the
clocks in the system. It is normally an alias for either ``system_clock``
or ``steady_clock``, but can be its own independent clock. Due
to this uncertainty, conversions of time measured on the
``high_resolution_clock`` to Python produce platform-dependent types:
you'll get a :py:class:`~datetime.datetime` if ``high_resolution_clock`` is
an alias for ``system_clock`` on your system, or a :py:class:`~datetime.timedelta`
value otherwise.

Provided conversions
^^^^^^^^^^^^^^^^^^^^

The C++ types described in this section may be instantiated with any
precision. Conversions to a less-precise type will round towards zero.
Since Python's built-in date and time objects support only microsecond
precision, any precision beyond that on the C++ side will be lost when
converting to Python.

.. rubric:: C++ to Python

- ``std::chrono::system_clock::time_point`` → :py:class:`datetime.datetime`
    A system clock time will be converted to a Python
    :py:class:`~datetime.datetime` instance.  The result describes a time in the
    local timezone, but does not have any timezone information
    attached to it (it is a naive datetime object).

- ``std::chrono::duration`` → :py:class:`datetime.timedelta`
    A duration will be converted to a Python :py:class:`~datetime.timedelta`.
    Any precision beyond microseconds is lost by rounding towards zero.

- ``std::chrono::[other_clock]::time_point`` → :py:class:`datetime.timedelta`
    A time on any clock except the system clock will be converted to a Python
    :py:class:`~datetime.timedelta`, which measures the number of seconds between
    the clock's epoch and the time point of interest.

.. rubric:: Python to C++

- :py:class:`datetime.datetime` or :py:class:`datetime.date` or :py:class:`datetime.time` → ``std::chrono::system_clock::time_point``
    A Python date, time, or datetime object can be converted into a
    system clock timepoint.  A :py:class:`~datetime.time` with no date
    information is treated as that time on January 1, 1970. A
    :py:class:`~datetime.date` with no time information is treated as midnight
    on that date. **Any timezone information is ignored.**

- :py:class:`datetime.timedelta` → ``std::chrono::duration``
    A Python time delta object can be converted into a duration
    that describes the same number of seconds (modulo precision limitations).

- :py:class:`datetime.timedelta` → ``std::chrono::[other_clock]::time_point``
    A Python time delta object can be converted into a timepoint on a
    clock other than the system clock. The resulting timepoint will be
    that many seconds after the target clock's epoch time.

- ``float`` → ``std::chrono::duration``
    A floating-point value can be converted into a duration. The input is
    treated as a number of seconds, and fractional seconds are supported
    to the extent representable.

- ``float`` → ``std::chrono::[other_clock]::time_point``
    A floating-point value can be converted into a timepoint on a
    clock other than the system clock. The input is treated as a
    number of seconds, and fractional seconds are supported to the
    extent representable. The resulting timepoint will be that many
    seconds after the target clock's epoch time.
