.. _api:

.. cpp:namespace:: nanobind

C++ API Reference (Core)
========================

Macros
------

.. c:macro:: NB_MODULE(name, variable)

   This macro creates the entry point that will be invoked when the Python
   interpreter imports an extension module. The module name is given as the
   fist argument and it should not be in quotes. It **must** match the module
   name given to the :cmake:command:`nanobind_add_module()` function in the
   CMake build system.

   The second macro argument defines a variable of type :cpp:class:`module_`.
   The body of the declaration typically contains a sequence of operations
   that populate the module variable with contents.

   .. code-block:: cpp

       NB_MODULE(example, m) {
           m.doc() = "Example module";

           // Add bindings here
           m.def("add", []() {
               return "Hello, World!";
           });
       }

.. c:macro:: NB_MAKE_OPAQUE(T)

   The macro registers a partial template specialization pattern for the type
   `T` that marks it as *opaque*, mening that nanobind won't try to run its
   type casting template machinery on it.

   This is useful when trying to register a binding for `T` that is simultaneously
   also covered by an existing type caster.

   This macro should be used at the top level (outside of namespaces and
   program code).

Python object API
-----------------

Nanobind ships with a wide range of Python wrapper classes like
:cpp:class:`object`, :cpp:class:`list`, etc. Besides class-specific operations
(e.g., :cpp:func:`list::append`), these classes also implement core operations
that can be performed on *any* Python object. Since it would be tedious to
implement this functionality over and over again, it is realized by the
following mixin class that lives in the ``nanobind::detail`` namespace.

.. cpp:namespace:: nanobind::detail

.. cpp:class:: template <typename Derived> api

   This mixin class adds common functionality to various nanobind types using
   the `curiously recurring template pattern
   <https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern>`_
   (CRTP). The only requirement for the `Derived` template parameter is that it
   implements the member function ``PyObject *ptr() const`` that gives access
   to the underlying Python object pointer.

   .. cpp:function:: Derived &derived()

      Obtain a mutable reference to the derived class.

   .. cpp:function:: const Derived &derived() const

      Obtain a const reference to the derived class.

   .. cpp:function:: handle inc_ref() const noexcept

      Increases the reference count and returns a reference to the Python object.

   .. cpp:function:: handle ref_ref() const noexcept

      Decreases the reference count and returns a reference to the Python object.

   .. cpp:function:: iterator begin() const

      Return a forward iterator analogous to ``iter()`` in Python. The object
      must be a collection that supports the iteration protocol. This interface
      provides a generic iterator that works any type of Python object. The
      :cpp:class:`tuple`, :cpp:class:`list`, and :cpp:class:`dict` wrappers
      provide more efficient specialized alternatives.

   .. cpp:function:: iterator end() const

      Return a sentinel that ends the iteration.

   .. cpp:function:: handle type() const

      Return a :cpp:class:`handle` to the underlying Python type object.

   .. cpp:function:: operator handle() const

      Return a :cpp:class:`handle` wrapping the underlying ``PyObject*`` pointer.

   .. cpp:function:: detail::accessor<obj_attr> attr(handle key) const

      Analogous to ``self.key`` in Python, where ``key`` is a Python object.
      The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so
      that it can be read and written.

   .. cpp:function:: detail::accessor<str_attr> attr(const char * key) const

      Analogous to ``self.key`` in Python, where ``key`` is a C-style string.
      The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so
      that it can be read and written.

   .. cpp:function:: detail::accessor<str_attr> doc() const

       Analogous to ``self.__doc__``. The result is wrapped in an
       :cpp:class:`accessor <detail::accessor>` so that it can be read and
       written.

   .. cpp:function:: detail::accessor<obj_item> operator[](handle key) const

      Analogous to ``self[key]`` in Python, where ``key`` is a Python object.
      The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so that it can be read and
      written.

   .. cpp:function:: detail::accessor<str_item> operator[](const char * key) const

      Analogous to ``self[key]`` in Python, where ``key`` is a C-style string.
      The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so that it can be read and
      written.

   .. cpp:function:: template <typename T, enable_if_t<std::is_arithmetic_v<T>> = 1> detail::accessor<num_item> operator[](T key) const

      Analogous to ``self[key]`` in Python, where ``key`` is an arithmetic
      type (e.g., an integer). The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so
      that it can be read and written.

   .. cpp:function:: template <rv_policy policy = rv_policy::automatic_reference, typename... Args> object operator()(Args &&...args) const

      Assuming the Python object is a function or implements the ``__call__``
      protocol, `operator()` invokes the underlying function, passing an
      arbitrary set of parameters, while expanding any detected variable length
      argument and keyword argument packs. The result is returned as an
      :cpp:class:`object` and may need to be converted back into a Python
      object using :cpp:func:`cast()`.

      Type conversion is performed using the return value policy `policy`

      When type conversion of arguments or return value fails, the function
      raises a :cpp:type:`cast_error`. When the Python function call fails, it
      instead raises a :cpp:class:`python_error`.

   .. cpp:function:: args_proxy operator*() const

      Given a a tuple or list, this helper function performs variable argument
      list unpacking in function calls resembling the ``*`` operator in Python.
      Applying `operator*()` twice yields ``**`` keyword argument
      unpacking for dictionaries.

   .. cpp:function:: bool is(handle value) const

      Analogous to ``self is value`` in Python.

   .. cpp:function:: bool is_none() const

      Analogous to ``self is None`` in Python.

   .. cpp:function:: bool is_type() const

      Analogous to ``isinstance(self, type)`` in Python.

   .. cpp:function:: bool is_valid() const

      Checks if this wrapper contains a valid Python object (in the sense that
      the ``PyObject *`` pointer is non-null).

   .. cpp:function:: template <typename T> bool equal(const api<T> &other)

      Equivalent to ``self == other`` in Python.

   .. cpp:function:: template <typename T> bool not_equal(const api<T> &other)

      Equivalent to ``self != other`` in Python.

   .. cpp:function:: template <typename T> bool operator<(const api<T> &other)

      Equivalent to ``self < other`` in Python.

   .. cpp:function:: template <typename T> bool operator<=(const api<T> &other)

      Equivalent to ``self <= other`` in Python.

   .. cpp:function:: template <typename T> bool operator>(const api<T> &other)

      Equivalent to ``self > other`` in Python.

   .. cpp:function:: template <typename T> bool operator>=(const api<T> &other)

      Equivalent to ``self >= other`` in Python.

   .. cpp:function:: object operator-()

      Equivalent to ``-self`` in Python.

   .. cpp:function:: object operator~()

      Equivalent to ``~self`` in Python.

   .. cpp:function:: template <typename T> object operator+(const api<T> &other)

      Equivalent to ``self + other`` in Python.

   .. cpp:function:: template <typename T> object operator-(const api<T> &other)

      Equivalent to ``self - other`` in Python.

   .. cpp:function:: template <typename T> object operator*(const api<T> &other)

      Equivalent to ``self * other`` in Python.

   .. cpp:function:: template <typename T> object operator/(const api<T> &other)

      Equivalent to ``self / other`` in Python.

   .. cpp:function:: template <typename T> object floor_div(const api<T> &other)

      Equivalent to ``self // other`` in Python.

   .. cpp:function:: template <typename T> object operator|(const api<T> &other)

      Equivalent to ``self | other`` in Python.

   .. cpp:function:: template <typename T> object operator&(const api<T> &other)

      Equivalent to ``self & other`` in Python.

   .. cpp:function:: template <typename T> object operator^(const api<T> &other)

      Equivalent to ``self ^ other`` in Python.

   .. cpp:function:: template <typename T> object operator<<(const api<T> &other)

      Equivalent to ``self << other`` in Python.

   .. cpp:function:: template <typename T> object operator>>(const api<T> &other)

      Equivalent to ``self >> other`` in Python.

   .. cpp:function:: template <typename T> object operator+=(const api<T> &other)

      Equivalent to ``self += other`` in Python. Note that the `api<T>` version
      of the in-place operator does not update the ``self`` reference, which
      may lead to unexpected results when working with immutable types that
      return their result instead of updating ``self``.

      The :cpp:class:`object` class and subclasses override the in-place
      operators to achieve more intuitive behavior.

   .. cpp:function:: template <typename T> object operator-=(const api<T> &other)

       Equivalent to ``self -= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator*=(const api<T> &other)

       Equivalent to ``self *= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator/=(const api<T> &other)

       Equivalent to ``self /= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator|=(const api<T> &other)

       Equivalent to ``self |= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator&=(const api<T> &other)

       Equivalent to ``self &= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator^=(const api<T> &other)

       Equivalent to ``self ^= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator<<=(const api<T> &other)

       Equivalent to ``self <<= other`` in Python. See :cpp:func:`operator+=` for limitations.

   .. cpp:function:: template <typename T> object operator>>=(const api<T> &other)

       Equivalent to ``self >>= other`` in Python. See :cpp:func:`operator+=` for limitations.

.. cpp:class:: template <typename Impl> accessor

   This helper class facilitates attribute and item access. Casting an
   :cpp:class:`accessor` to a :cpp:class:`handle` or :cpp:class:`object`
   subclass causes a corresponding call to ``__getitem__`` or ``__getattr__``
   depending on the template argument `Impl`. Assigning a
   :cpp:class:`handle` or :cpp:class:`object` subclass causes a call to
   ``__setitem__`` or ``__setattr__``.

.. cpp:namespace:: nanobind

Handles and objects
-------------------

nanobind provides two styles of Python object wrappers: classes without
reference counting deriving from :cpp:class:`handle`, and reference-counted
wrappers deriving from :cpp:class:`object`. Reference counting bugs can be
really tricky to track down, hence it is recommended that you always prefer
:cpp:class:`object`-style wrappers unless there are specific reasons that
warrant the use of raw handles.

Without reference counting
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:class:: handle: public detail::api<handle>

   This class provides a thin wrapper around a raw ``PyObject *`` pointer. Its
   main purpose is to intercept various C++ operations and convert them into
   Python C API calls. It does *not* do any reference counting and can be
   somewhat unsafe to use.

   .. cpp:function:: handle() = default

      Default constructor. Creates an invalid handle wrapping a null pointer.
      (:cpp:func:`detail::api::is_valid()` is ``false``)

   .. cpp:function:: handle(const handle &) = default

      Default copy constructor.

   .. cpp:function:: handle(handle &&) = default

      Default move constructor.

   .. cpp:function:: handle(const PyObject * o)

      Initialize a handle from a Python object pointer. Does not change the reference count of `o`.

   .. cpp:function:: handle(const PyTypeObject * o)

      Initialize a handle from a Python type object pointer. Does not change the reference count of `o`.

   .. cpp:function:: handle &operator=(const handle &) = default

      Default copy assignment operator.

   .. cpp:function:: handle &operator=(handle &&) = default

      Default move assignment operator.

   .. cpp:function:: explicit operator bool() const

      Check if the handle refers to a valid Python object. Equivalent to
      :cpp:func:`detail::api::is_valid()`

   .. cpp:function:: PyObject * ptr() const

      Return the underlying ``PyObject*`` pointer.

With reference counting
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:class:: object: public handle

   This class provides a convenient `RAII
   <https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization>`_
   wrapper around a ``PyObject*`` pointer. Like :cpp:class:`handle`, it
   intercepts various C++ operations and converts them into Python C API calls.

   The main difference to :cpp:class:`handle` is that it uses reference
   counting to keep the underlying Python object alive.

   Use the :cpp:func:`borrow()` and :cpp:func:`steal()` functions to create an
   :cpp:class:`object` from a :cpp:class:`handle` or ``PyObject*`` pointer.

   .. cpp:function:: object() = default

      Default constructor. Creates an invalid object wrapping a null pointer.
      (:cpp:func:`detail::api::is_valid()` is ``false``)

   .. cpp:function:: object(object &&o)

      Move constructor. Steals the object from `o` without
      changing its reference count.

   .. cpp:function:: object(const object &o)

      Copy constructor. Acquires a new reference to `o` (if valid).

   .. cpp:function:: ~object()

      Decrease the reference count of the referenced Python object (if valid).

   .. cpp:function:: object& operator=(object &&o)

      Move assignment operator. Decreases the reference count of the currently
      held object (if valid) and steals the object from `o` without
      changing its reference count.

   .. cpp:function:: object& operator=(const object &o)

      Copy assignment operator. Decreases the reference count of the currently
      held object (if valid) and acquires a new reference to the object
      `o` (if valid).

   .. cpp:function:: void reset()

      Decreases the reference count of the currently held object (if valid) and
      resets the internal pointer to ``nullptr``.

   .. cpp:function:: handle release()

      Resets the internal pointer to ``nullptr`` and returns its previous
      contents as a :cpp:class:`handle`. This operation does not change
      the object's reference count and should be used carefully.

   .. cpp:function:: template <typename T> object& operator+=(const api<T> &other)

      Equivalent to ``self += other`` in Python.

   .. cpp:function:: template <typename T> object& operator-=(const api<T> &other)

       Equivalent to ``self -= other`` in Python.

   .. cpp:function:: template <typename T> object& operator*=(const api<T> &other)

       Equivalent to ``self *= other`` in Python.

   .. cpp:function:: template <typename T> object& operator/=(const api<T> &other)

       Equivalent to ``self /= other`` in Python.

   .. cpp:function:: template <typename T> object& operator|=(const api<T> &other)

       Equivalent to ``self |= other`` in Python.

   .. cpp:function:: template <typename T> object& operator&=(const api<T> &other)

       Equivalent to ``self &= other`` in Python.

   .. cpp:function:: template <typename T> object& operator^=(const api<T> &other)

       Equivalent to ``self ^= other`` in Python.

   .. cpp:function:: template <typename T> object& operator<<=(const api<T> &other)

       Equivalent to ``self <<= other`` in Python.

   .. cpp:function:: template <typename T> object& operator>>=(const api<T> &other)

       Equivalent to ``self >>= other`` in Python.


.. cpp:function:: template <typename T = object> T borrow(handle h)

   Create a reference-counted Python object wrapper of type `T` from a raw
   handle or ``PyObject *`` pointer. The target type `T` must be
   :cpp:class:`object` (the default) or one of its derived classes. The
   function does not perform any conversions or checks---it is up to the user
   to make sure that the target type is correct.

   The function *borrows* a reference, which means that it will increase the
   reference count while constructing ``T``.

   For example, consider the Python C API function `PyList_GetItem()
   <https://docs.python.org/3/c-api/list.html#c.PyList_GetItem>`_, whose
   documentation states that it returns a borrowed reference. An interface
   between this API and nanobind could look as follows:

   .. code-block:: cpp


       PyObject* list = ...;
       Py_ssize_t index = ...;
       nb::object o = nb::borrow(PyList_GetItem(obj, index));

   Using :cpp:func:`steal()` in this setting is incorrect and would lead to a
   reference underflow.

.. cpp:function:: template <typename T = object> T steal(handle h)

   Create a reference-counted Python object wrapper of type `T` from a raw
   handle or ``PyObject *`` pointer. The target type `T` must be
   :cpp:class:`object` (the default) or one of its derived classes. The
   function does not perform any conversions or checks---it is up to the user
   to make sure that the target type is correct.

   The function *steals* a reference, which means that constructing ``T``
   leaves the object's reference count unchanged.

   For example, consider the Python C API function `PyObject_Str()
   <https://docs.python.org/3/c-api/object.html#c.PyObject_Str>`_, whose
   documentation states that it returns a *new reference*. An interface
   between this API and nanobind could look as follows:

   .. code-block:: cpp

       PyObject* value = ...;
       nb::object o = nb::steal(PyObject_Str(value));

   Using :cpp:func:`borrow()` in this setting is incorrect and would lead to a
   reference leak.


Attribute access
----------------

.. cpp:function:: bool hasattr(handle h, const char * key) noexcept

   Check if the given object has an attribute string ``key``. The function never
   raises an exception and returns ``false`` in case of an internal error.

   Equivalent to ``hasattr(h, key)`` in Python.

.. cpp:function:: bool hasattr(handle h, handle key) noexcept

   Check if the given object has a attribute represented by the Python object
   ``key``. The function never raises an exception and returns ``false`` in
   case of an internal error.

   Equivalent to ``hasattr(h, key)`` in Python.

.. cpp:function:: object getattr(handle h, const char * key)

   Equivalent to ``h.key`` and ``getattr(h, key)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

.. cpp:function:: object getattr(handle h, handle key)

   Equivalent to ``h.key`` and ``getattr(h, key)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

.. cpp:function:: object getattr(handle h, const char * key, handle def) noexcept

   Equivalent to ``getattr(h, key, def)`` in Python. Never raises an
   exception and returns ``def`` when the operation fails, or when the desired
   attribute could not be found.

.. cpp:function:: object getattr(handle h, handle key, handle def) noexcept

   Equivalent to ``getattr(h, key, def)`` in Python. Never raises an
   exception and returns ``def`` when the operation fails, or when the desired
   attribute could not be found.

.. cpp:function:: void setattr(handle h, const char * key, handle value)

   Equivalent to ``h.key = value`` and ``setattr(h, key, value)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

.. cpp:function:: void setattr(handle h, handle key, handle value)

   Equivalent to ``h.key = value`` and ``setattr(h, key, value)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

.. cpp:function:: void delattr(handle h, const char * key)

   Equivalent to ``del h.key`` and ``delattr(h, key)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

.. cpp:function:: void delattr(handle h, handle key)

   Equivalent to ``del h.key`` and ``delattr(h, key)`` in Python.
   Raises :cpp:class:`python_error` if the operation fails.

Size queries
------------

.. cpp:function:: size_t len(handle h)

   Equivalent to ``len(h)`` in Python. Raises :cpp:class:`python_error` if the
   operation fails.

.. cpp:function:: size_t len(const tuple &t)

   Equivalent to ``len(t)`` in Python. Optimized variant for tuples.

.. cpp:function:: size_t len(const list &l)

   Equivalent to ``len(l)`` in Python. Optimized variant for lists.

.. cpp:function:: size_t len(const dict &d)

   Equivalent to ``len(d)`` in Python. Optimized variant for dictionaries.

.. cpp:function:: size_t len_hint(handle h)

   Equivalent to ``operator.length_hint(h)`` in Python. Raises
   :cpp:class:`python_error` if the operation fails.

Type queries
------------

.. cpp:function:: template <typename T> isinstance(handle h)

   Checks if the Python object `h` represents a valid instance of the C++ type
   `T`. This works for bound C++ classes, basic types (``int``, ``bool``,
   etc.), and Python type wrappers ( :cpp:class:`list`, :cpp:class:`dict`,
   :cpp:class:`module_`, etc.).

   *Note*: the check even works when `T` involves a type caster (e.g., an STL
   types like ``std::vector<float>``). However, this involve a wasteful attempt
   to convert the object to C++. It may be more efficient to just perform the
   conversion using :cpp:func:`cast` and catch potential raised exceptions.

.. cpp:function:: template <typename T> handle type() noexcept

   Returns the Python type object associated with the C++ type `T`. When the
   type not been bound via nanobind, the function returns an invalid handle
   (:cpp:func:`detail::api::is_valid()` is ``false``).

   *Note*: in contrast to the :cpp:func:`isinstance()` function above, builtin
   types, type wrappers, and types handled using type casters, are *not*
   supported.

Wrapper classes
---------------

.. cpp:class:: tuple: public object

   Wrapper class representing Python ``tuple`` instances.

   .. cpp:function:: size_t size() const

      Return the number of tuple elements.

   .. cpp:function:: detail::fast_iterator begin() const

      Return a forward iterator analogous to ``iter()`` in Python. The function
      overrides a generic version in :cpp:class:`detail::api` and is more
      efficient for tuples.

   .. cpp:function:: detail::fast_iterator end() const

      Return a sentinel that ends the iteration.

   .. cpp:function:: template <typename T, enable_if_t<std::is_arithmetic_v<T>> = 1> detail::accessor<num_item_tuple> operator[](T key) const

      Analogous to ``self[key]`` in Python, where ``key`` is an arithmetic
      type (e.g., an integer). The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so
      that it can be read and written.

      The function overrides the generic version in :cpp:class:`detail::api`
      and is more efficient for tuples.


.. cpp:class:: list : public object

   Wrapper class representing Python ``list`` instances.

   .. cpp:function:: size_t size() const

      Return the number of list elements.

   .. cpp:function:: template <typename T> void append(T &&value)

      Append an element to the list. When `T` does not already represent a
      wrapped Python object, the function performs a cast.

   .. cpp:function:: template <typename T, enable_if_t<std::is_arithmetic_v<T>> = 1> detail::accessor<num_item_list> operator[](T key) const

      Analogous to ``self[key]`` in Python, where ``key`` is an arithmetic
      type (e.g., an integer). The result is wrapped in an :cpp:class:`accessor <detail::accessor>` so
      that it can be read and written.

      The function overrides the generic version in :cpp:class:`detail::api`
      and is more efficient for lists.

   .. cpp:function:: detail::fast_iterator begin() const

      Return a forward iterator analogous to ``iter()`` in Python. The operator
      provided here overrides the generic version in :cpp:class:`detail::api`
      and is more efficient for lists.

   .. cpp:function:: detail::fast_iterator end() const

      Return a sentinel that ends the iteration.


.. cpp:class:: dict: public object

   Wrapper class representing Python ``dict`` instances.

   .. cpp:function:: dict()

      Create an empty dictionary

   .. cpp:function:: size_t size() const

      Return the number of dictionary elements.

   .. cpp:function:: detail::dict_iterator begin() const

      Return an item iterator that returns ``std::pair<handle, handle>``
      key-value pairs analogous to ``iter(dict.items())`` in Python.

   .. cpp:function:: detail::dict_iterator end() const

      Return a sentinel that ends the iteration.

   .. cpp:function:: list keys() const

      Return a list containing all dictionary keys.

   .. cpp:function:: list values() const

      Return a list containing all dictionary values.

   .. cpp:function:: list items() const

      Return a list containing all dictionary items as ``(key, value)`` pairs.


.. cpp:class:: module_: public object

   Wrapper class representing Python ``module`` instances. The underscore at
   the end disambiguates the class name from the C++20 ``module`` declaration.

   .. cpp:function:: template <typename Func, typename... Extra> module_ &def(const char * name, Func &&f, const Extra &...extra)

      Bind the function `f` to the identifier `name` within the module. Returns
      a reference to ``*this`` so that longer sequences of binding declarations
      can be chained, as in ``m.def(...).def(...);``. The variable length
      `extra` parameter can be used to pass docstrings and other :ref:`function
      binding annotations <function_binding_annotations>`.

      Example syntax:

      .. code-block:: cpp

         void test() { printf("Hello world!"); }

         NB_MODULE(example, m) {
             // here, "m" is variable of type 'module_'.
             m.def("test", &test, "A test function")
              .def(...); // more binding declarations
         }


   .. cpp:function:: module_ import_(const char * name)

      Import the Python module with the specified name and return a reference
      to it. The underscore at the end disambiguates the function name from the
      C++20 ``import`` statement.

      Example usage:

      .. code-block:: cpp

         nb::module_ np = nb::module_::import_("numpy");
         nb::object np_array = np.attr("array");

   .. cpp:function:: module_ def_submodule(const char * name, const char * doc = nullptr)

      Create a Python submodule within an existing module and return a
      reference to it. Can be chained recursively.

      Example usage:

      .. code-block:: cpp

         NB_MODULE(example, m) {
             nb::module_ m2 = m.def_submodule("sub", "A submodule of 'example'");
             nb::module_ m3 = m2.def_submodule("subsub", "A submodule of 'example.sub'");
         }

.. cpp:class:: capsule: public object

   Capsules are small opaque Python objects that wrap a C or C++ pointer and a cleanup routine.

   .. cpp:function:: capsule(const void * ptr, void (* cleanup)(void*) noexcept = nullptr)

      Construct an *unnamed* capsule wrapping the pointer `p`. When the
      capsule is garbage collected, Python will call the destructor `cleanup`
      (if provided) with the value of `p`.

   .. cpp:function:: capsule(const void * ptr, const char * name, void (* cleanup)(void*) noexcept = nullptr)

      Construct a *named* capsule with name `name` wrapping the pointer `p`.
      When the capsule is garbage collected, Python will call the destructor
      `cleanup` (if provided) with the value of `p`.

   .. cpp:function:: const char * name() const

      Return the capsule name (or ``nullptr`` when the capsule is unnamed)

   .. cpp:function:: void * data() const

      Return the pointer wrapped by the capsule.


.. cpp:class:: int_: public object

   This wrapper class represents Python ``int`` instances. It can handle large
   numbers requiring more than 64 bits of storage.

   .. cpp:function:: int_(handle h)

      Performs an integer cast within Python. This is equivalent to the Python
      expression ``int(h)``.

   .. cpp:function:: template <typename T, detail::enable_if_t<std::is_arithmetic_v<T>> = 0> explicit int_(T value)

      Convert an C++ arithmetic type into a Python integer.

   .. cpp:function:: template <typename T, detail::enable_if_t<std::is_arithmetic_v<T>> = 0> explicit operator T() const

      Convert a Python integer into a C++ arithmetic type.


.. cpp:class:: float_: public object

   This wrapper class represents Python ``float`` instances.

   .. cpp:function:: float_(handle h)

      Performs an floating point cast within Python. This is equivalent to the
      Python expression ``float(h)``.

   .. cpp:function:: explicit float_(double value)

      Convert an C++ double value into a Python float objecct

   .. cpp:function:: explicit operator double() const

      Convert a Python float object into a C++ double value


.. cpp:class:: str: public object

   This wrapper class represents Python unicode ``str`` instances.

   .. cpp:function:: str(handle h)

      Performs a string cast within Python. This is equivalent equivalent to
      the Python expression ``str(h)``.

   .. cpp:function:: str(const char * s)

      Convert a null-terminated C-style string in UTF-8 encoding into a Python string.

   .. cpp:function:: str(const char * s, size_t n)

      Convert a C-style string in UTF-8 encoding of length ``n`` bytes into a Python string.

   .. cpp:function:: const char * c_str()

      Convert a Python string into a null-terminated C-style string with UTF-8
      encoding.

      *Note*: The C string will be deleted when the `str` instance is garbage
      collected.

   .. cpp:function:: template <typename... Args> str format(Args&&... args)

      C++ analog of the Python routine ``str.format``. Can be called with
      positional and keyword arguments.


.. cpp:class:: bytes: public object

   This wrapper class represents Python unicode ``bytes`` instances.

   .. cpp:function:: bytes(handle h)

      Performs a cast within Python. This is equivalent equivalent to
      the Python expression ``bytes(h)``.

   .. cpp:function:: bytes(const char * s)

      Convert a null-terminated C-style string encoding into a Python ``bytes`` object.

   .. cpp:function:: bytes(const char * s, size_t n)

      Convert a null-terminated C-style string encoding of length ``n`` bytes into a Python ``bytes`` object.

   .. cpp:function:: const char * c_str()

      Convert a Python bytes object into a null-terminated C-style string.

   .. cpp:function:: size_t size() const

      Return the size in bytes.


.. cpp:class:: type_object: public object

   Wrapper class representing Python ``type`` instances.

.. cpp:class:: sequence: public object

   Wrapper class representing arbitrary Python sequence types.

.. cpp:class:: mapping : public object

   Wrapper class representing arbitrary Python mapping types.

   .. cpp:function:: list keys() const

      Return a list containing all of the map's keys.

   .. cpp:function:: list values() const

      Return a list containing all of the map's values.

   .. cpp:function:: list items() const

      Return a list containing all of the map's items as ``(key, value)`` pairs.

.. cpp:class:: iterator : public object

   Wrapper class representing a Python iterator.

   .. cpp:function:: iterator& operator++()

      Advance to the next element (pre-increment form).

   .. cpp:function:: iterator& operator++(int)

      Advance to the next element (post-increment form).

   .. cpp:function:: handle operator*() const

      Return the item at the current position.

   .. cpp:function:: handle operator->() const

      Convenience routine for pointer-style access.

   .. static iterator sentinel();

      Return a sentinel that ends the iteration.

   .. cpp:function:: friend bool operator==(const iterator &a, const iterator &b);

      Iterator equality comparison operator.

   .. cpp:function:: friend bool operator!=(const iterator &a, const iterator &b);

      Iterator inequality comparison operator.

.. cpp:class:: iterable : public object

   Wrapper class representing an object that can be iterated upon (in the sense
   that calling :cpp:func:`iter()` is valid).

.. cpp:class:: slice : public object

   Wrapper class representing a Python slice object.

   .. cpp:function:: slice(handle start, handle stop, handle step)

      Create the slice object given by ``slice(start, stop, step)`` in Python.

   .. cpp:function:: template <typename T, detail::enable_if_t<std::is_arithmetic_v<T>> = 0> slice(T stop)

      Create the slice object ``slice(stop)``, where `stop` is represented by a
      C++ integer type.

   .. cpp:function:: template <typename T, detail::enable_if_t<std::is_arithmetic_v<T>> = 0> slice(T start, T stop)

      Create the slice object ``slice(start, stop)``, where `start` and `stop`
      are represented by a C++ integer type.

   .. cpp:function:: template <typename T, detail::enable_if_t<std::is_arithmetic_v<T>> = 0> slice(T start, T stop, T step)

      Create the slice object ``slice(start, stop, step)``, where `start`,
      `stop`, and `step` are represented by a C++ integer type.

   .. cpp:function:: detail::tuple<Py_ssize_t, Py_ssize_t, Py_ssize_t, size_t> compute(size_t size) const

      Adjust the slice to the `size` value of a given container. Returns a tuple containing
      ``(start, stop, step, slice_length)``.

.. cpp:class:: ellipsis: public object

   Wrapper class representing a Python ellipsis (``...``) object.

   .. cpp:function:: ellipsis()

      Create a wrapper referencing the unique Python ``Ellipsis`` object.

.. cpp:class:: not_implemented: public object

   Wrapper class representing a Python ``NotImplemented`` object.

   .. cpp:function:: not_implemented()

      Create a wrapper referencing the unique Python ``NotImplemented`` object.

.. cpp:class:: callable: public object

   Wrapper class representing a callable Python object.

.. cpp:class:: args : public tuple

   Variable argument keyword list for use in function argument declarations.

.. cpp:class:: kwargs : public dict

   Variable keyword argument keyword list for use in function argument declarations.

Parameterized wrapper classes
-----------------------------

.. cpp:class:: template <typename T> handle_t : public handle

   Wrapper class representing a handle to a subclass of the C++ type `T`. It
   can be used to bind functions that take the associated Python object in its
   wrapped form, while rejecting objects with a different type (i.e., it is
   more discerning than :cpp:class:`handle`, which accepts *any* Python object).

   .. code-block:: cpp

      // Bind the class A
      class A { int value; };
      nb::class_<A>(m, "A");

      // Bind a function that takes a Python object representing a 'A' instance
      m.def("process_a", [](nb::handle_t<A> h) {
         PyObject * a_py = h.ptr();   // PyObject* pointer to wrapper
         A &a_cpp = nb::cast<A &>(h); // Reference to C++ instance
      });

.. cpp:class:: template <typename T> type_object_t : public type_object

   Wrapper class representing a Python type object that is a subtype of the C++
   type `T`. It can be used to bind functions that only accept type objects
   satisfying this criterion (i.e., it is more discerning than
   :cpp:class:`type_object`, which accepts *any* Python type object).

Error management
----------------

nanobind provides a range of functionality to convert C++ exceptions into
equivalent Python exceptions and raise captured Python error state in C++. The
:cpp:class:`exception` class is also relevant in this context, but is listed in
the reference section on :ref:`class binding <class_binding>`.

.. cpp:struct:: error_scope

   RAII helper class that temporarily stashes any existing Python error status.
   This is important when running Python code in the context of an existing
   failure that must be processed (e.g., to generate an error message).

   .. cpp:function:: error_scope()

      Stash the current error status (if any)

   .. cpp:function:: ~error_scope()

      Restore the stashed error status (if any)

.. cpp:struct:: python_error : public std::exception

   Exception that represents a detected Python error status.

   .. cpp:function:: python_error()

      This constructor may only be called when a Python error has occurred
      (``PyErr_Occurred()`` must be ``true``). It creates a C++ exception
      object that represents this error and clears the Python error status.

   .. cpp:function:: python_error(const python_error &)

      Copy constructor

   .. cpp:function:: python_error(python_error &&) noexcept

      Move constructor

   .. cpp:function:: const char * what() noexcept

      Return a stringified version of the exception. nanobind internally
      normalizes the exception and generates a traceback that is included
      as part of this string. This can be a relatively costly operation
      and should only be used if all of this detail is actually needed.

   .. cpp:function:: bool matches(handle exc) noexcept

      Checks whether the exception has the same type as `exc`.

      The argument to this function is usually one of the `Standard Exceptions
      <https://docs.python.org/3/c-api/exceptions.html#standard-exceptions>`_.

   .. cpp:function:: void restore() noexcept

      Restore the error status in Python and clear the `python_error`
      contents. This may only be called once, and you should not
      reraise the `python_error` in C++ afterward.

   .. cpp:function:: void discard_as_unraisable(handle context) noexcept

      Pass the error to Python's :py:func:`sys.unraisablehook`, which
      prints a traceback to :py:data:`sys.stderr` by default but may
      be overridden.  Like :cpp:func:`restore`, this consumes the
      error and you should not reraise the exception in C++ afterward.

      The *context* argument should be some object whose ``repr()``
      helps identify the location of the error. The default
      :py:func:`sys.unraisablehook` prints a traceback that begins
      with the text ``Exception ignored in:`` followed by
      the result of ``repr(context)``.

      Example use case: handling a Python error that occurs in a C++
      destructor where you cannot raise a C++ exception.

   .. cpp:function:: void discard_as_unraisable(const char * context) noexcept

      Convenience wrapper around the above function, which takes a C-style
      string for the ``context`` argument.

   .. cpp:function:: handle type() const

      Returns a handle to the exception type

   .. cpp:function:: handle value() const

      Returns a handle to the exception value

   .. cpp:function:: object traceback() const

      Returns a handle to the exception's traceback object

.. cpp:class:: cast_error

   The function :cpp:func:`cast` raises this exception to indicate that a cast
   was unsuccessful.

   .. cpp:function:: cast_error()

      Constructor

.. cpp:class:: next_overload

   Raising this special exception from a bound function informs nanobind that
   the function overload detected incompatible inputs. nanobind will then try
   other overloads before reporting a ``TypeError``.

   This feature is useful when a multiple overloads of a function accept
   overlapping or identical input types (e.g. :cpp:class:`object`) and must run
   code at runtime to select the right overload.

   You should probably write a thorough docstring explicing the expected inputs
   in this case, since the behavior won't be obvious from the auto-generated
   function signature list in the docstring. It can be frustrating when a
   function call fails with an error message stating that the provided
   arguments aren't compatible with any overload, when the associated error
   message suggests otherwise.

   .. cpp:function:: next_overload()

      Constructor

.. cpp:class:: builtin_exception : public std::runtime_error

   General-purpose class to propagate builtin Python exceptions from C++. A
   number of convenience functions (see below) instantiate it.

.. cpp:function:: builtin_exception stop_iteration(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``StopIteration`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception index_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``IndexError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception key_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``KeyError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception value_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``ValueError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception type_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``TypeError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception buffer_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``BufferError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception import_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``ImportError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: builtin_exception attribute_error(const char * what = nullptr)

   Convenience wrapper to create a :cpp:class:`builtin_exception` C++ exception
   instance that nanobind will re-raise as a Python ``AttributeError`` exception
   boundary. when it crosses the C++ ↔ Python interface.

.. cpp:function:: void register_exception_translator(void (* exception_translator)(const std::exception_ptr &, void*), void * payload = nullptr)

   Install an exception translator callback that will be invoked whenever
   nanobind's function call dispatcher catches a previously unknown C++
   exception. This exception translator should follow a standard structure of
   re-throwing an exception, catching a specific type, and converting this into
   a Python error status upon "success".

   Here is an example for a hypothetical ``ZeroDivisionException``.

   .. code-block:: cpp

      register_exception_translator(
          [](const std::exception_ptr &p, void * /*payload*/) {
              try {
                  std::rethrow_exception(p);
              } catch (const ZeroDivisionException &e) {
                  PyErr_SetString(PyExc_ZeroDivisionError, e.what());
              }
          }, nullptr /*payload*/);

   Generally, you will want to use the more convenient exception binding
   interface provided by :cpp:class:`exception` class. This function provides
   an escape hatch for more specialized use cases.

.. cpp:function:: void chain_error(handle type, const char * fmt, ...) noexcept

   Raise a Python error of type ``type`` using the format string ``fmt``
   interpreted by ``PyErr_FormatV``.

   If a Python error state was already set prior to calling this method, then
   the new error is *chained* on top of the existing one. Otherwise, the
   function creates a new error without initializing its ``__cause__`` field.

.. cpp:function:: void raise_from(python_error &e, handle type, const char * fmt, ...)

   Convenience wrapper around :cpp:func:`chain_error <chain_error>`. It takes
   an existing Python error (e.g. caught in a ``catch`` block) and creates an
   additional Python exception with the current error as cause. It then
   re-raises :cpp:class:`python_error`. The argument ``fmt`` is a
   ``printf``-style format string interpreted by ``PyErr_FormatV``.

   Usage of this function is explained in the documentation section on
   :ref:`exception chaining <exception_chaining>`.

Casting
-------

.. cpp:function:: template <typename T, typename Derived> T cast(const detail::api<Derived> &value, bool convert = true)

   Convert the Python object `value` (typically a :cpp:class:`handle` or a
   :cpp:class:`object` subclass) into a C++ object of type `T`.

   When the `convert` argument is set to ``true`` (the default), the
   implementation may also attempt *implicit conversions* to perform the cast.

   The function raises a :cpp:type:`cast_error` when the conversion fails.
   See :cpp:func:`try_cast()` for an alternative that never raises.

.. cpp:function:: template <typename T, typename Derived> bool try_cast(const detail::api<Derived> &value, T &out, bool convert = true) noexcept

   Convert the Python object `value` (typically a :cpp:class:`handle` or a
   :cpp:class:`object` subclass) into a C++ object of type `T`, and store it
   in the output parameter `out`.

   When the `convert` argument is set to ``true`` (the default), the
   implementation may also attempt *implicit conversions* to perform the cast.

   The function returns `false` when the conversion fails. In this case, the
   `out` parameter is left untouched. See :cpp:func:`cast()` for an alternative
   that instead raises an exception in this case.

.. cpp:function:: template <typename T> object cast(T &&value, rv_policy policy = rv_policy::automatic_reference)

   Convert the C++ object ``value`` into a Python object. The return value
   policy `policy` is used to handle ownership-related questions when a new
   Python object must be created.

   The function raises a :cpp:type:`cast_error` when the conversion fails.

.. cpp:function:: template <typename T> object find(const T &value) noexcept

   Return the Python object associated with the C++ instance `value`. When no
   such object can be found, the function it returns an invalid object
   (:cpp:func:`detail::api::is_valid()` is ``false``).

.. cpp:function:: template <rv_policy policy = rv_policy::automatic, typename... Args> tuple make_tuple(Args&&... args)

   Create a Python tuple from a sequence of C++ objects ``args...``. The return
   value policy `policy` is used to handle ownership-related questions when a
   new Python objects must be created.

   The function raises a :cpp:type:`cast_error` when the conversion fails.

Common binding annotations
--------------------------

The following annotations can be specified in both function and class bindings.

.. cpp:struct:: scope

   .. cpp:function:: scope(handle value)

      Captures the Python scope (e.g., a :cpp:class:`module_` or
      :cpp:class:`type_object`) in which the function or class should be
      registered.

.. _function_binding_annotations:

Function binding annotations
----------------------------

The following annotations can be specified using the variable-length ``Extra``
parameter of :cpp:func:`module_::def`, :cpp:func:`class_::def`,
:cpp:func:`cpp_function`, etc.

.. cpp:struct:: name

   .. cpp:function:: name(const char * value)

      Captures the name of the function.

.. cpp:struct:: arg

   Function argument annotation to enable keyword-based calling, default
   arguments, passing ``None``, and implicit conversion hints. Note that when a
   function argument should be annotated, you *must* specify annotations for
   arguments of that function.

   Example use:

   .. code-block:: cpp

       m.def("add", [](int a, int b) { return a + b; }, nb::arg("a"), nb::arg("b"));

   It is usually convenient to add the following ``using`` declaration to your binding code.

   .. code-block:: cpp

       using namespace nb::literals;

   In this case, the argument annotations can be shortened:

   .. code-block:: cpp

       m.def("add", [](int a, int b) { return a + b; }, "a"_a, "b"_a);

   .. cpp:function:: explicit arg(const char * name = nullptr)

      Create a function argument annotation. The name is optional.

   .. cpp:function:: template <typename T> arg_v operator=(T &&value) const

      Assign a default value to the argument.

   .. cpp:function:: arg &none(bool value = true)

      Set a flag noting that the function argument accepts ``None``. Can only
      be used for python wrapper types (e.g. :cpp:class:`handle`,
      :cpp:class:`int_`) and types that have been bound using
      :cpp:class:`class_`. You cannot use this to implement functions that
      accept null pointers to builtin C++ types like ``int *i = nullptr``.

   .. cpp:function:: arg &noconvert(bool value = true)

      Set a flag noting that implicit conversion should never be performed for
      this function argument.

.. cpp:struct:: is_method

   Indicate that the bound function is a method.

.. cpp:struct:: is_operator

   Indicate that the bound operator represents a special double underscore
   method (``__add__``, ``__radd__``, etc.) that implements an arithmetic
   operation.

   When a bound functions with this annotation is called with incompatible
   arguments, it will return ``NotImplemented`` rather than raising a
   ``TypeError``.

.. cpp:struct:: is_implicit

   Indicate that the bound constructor can be used to perform implicit conversions.

.. cpp:struct:: template <typename... Ts> call_guard

   Invoke the call guard(s) `Ts` when the bound function executes. The RAII
   helper :cpp:struct:`gil_scoped_release` is often combined with this feature.

.. cpp:struct:: template <size_t Nurse, size_t Patient> keep_alive

   Following the function evaluation, keep object ``Patient`` alive while the
   object ``Nurse`` exists. The function uses the following indexing
   convention:

   - Index ``0`` refers to the return value of methods. (It should not be used in
     constructors)

   - Index ``1`` refers to the first argument. In methods and constructors, index ``1``
     refers to the implicit ``this`` pointer, while regular arguments begin at index ``2``.

    When the nurse or patient equal ``None``, the annotation does nothing.

    nanobind will raise an exception when the nurse object is neither a
    nanobind-registered type nor weak-referenceable.

.. cpp:struct:: raw_doc

   .. cpp:function:: raw_doc(const char * value)

      Take complete control over the docstring of a bound function and replace it with `value`.

.. cpp:enum-class:: rv_policy

   A return value policy determines the question of *ownership* when a bound
   function returns a previously unknown C++ instance that must now be
   converted into a Python object.

   Return value policies apply to functions that return values handled using
   :ref:`class bindings <bindings>`, which means that their Python equivalent
   was registered using :cpp:class:`class_\<...\> <class_>`. They are ignored
   in most other cases. One exception are STL types handled using :ref:`type
   casters <type_casters>` (e.g. ``std::vector<T>``), which contain a nested
   type ``T`` handled using class bindings. In this case, the return value
   policy also applies recursively.

   A return value policy is unnecessary when the type itself clarifies
   ownership (e.g., ``std::unique_ptr<T>``, ``std::shared_ptr<T>``, a type with
   :ref:`intrusive reference counting <intrusive>`).

   The following policies are available (where `automatic` is the default).
   Please refer to the :ref:`return value policy section <rvp>` of the main
   documentation, which clarifies the list below using concrete examples.

   .. cpp:enumerator:: take_ownership

      Create a Python object that wraps the existing C++ instance and takes
      full ownership of it. No copies are made. Python will call the C++
      destructor and ``delete`` operator when the Python wrapper is garbage
      collected at some later point. The C++ side *must* relinquish ownership
      and is not allowed to destruct the instance, or undefined behavior will
      ensue.

   .. cpp:enumerator:: copy

      Copy-construct a new Python object from the C++ instance. The new copy
      will be owned by Python, while C++ retains ownership of the original.

   .. cpp:enumerator:: move

      Move-construct a new Python object from the C++ instance. The new object
      will be owned by Python, while C++ retains ownership of the original
      (whose contents were likely invalidated by the move operation).

   .. cpp:enumerator:: reference

      Create a Python object that wraps the existing C++ instance *without
      taking ownership* of it. No copies are made. Python will never call the
      destructor or ``delete`` operator, even when the Python wrapper is
      garbage collected.

   .. cpp:enumerator:: reference_internal

      A safe extension of the `reference` policy for methods that implement
      some form of attribute access. It creates a Python object that wraps the
      existing C++ instance *without taking ownership* of it. Additionally, it
      adjusts reference counts to keeps the method's implicit ``self`` argument
      alive until the newly created object has been garbage collected.

   .. cpp:enumerator:: none

      This is the most conservative policy: it simply refuses the cast unless
      the C++ instance already has a corresponding Python object, in which case
      the question of ownership becomes moot.

   .. cpp:enumerator:: automatic

      This is the default return value policy, which falls back to
      `take_ownership` when the return value is a pointer, `move`  when it is a
      rvalue reference, and `copy` when it is a lvalue reference.

   .. cpp:enumerator:: automatic_reference

      This policy matches `automatic` but falls back to `reference` when the
      return value is a pointer.


.. _class_binding_annotations:

Class binding annotations
-------------------------

The following annotations can be specified using the variable-length ``Extra``
parameter of the constructor :cpp:func:`class_::class_`.

.. cpp:struct:: is_final

   Indicate that a type cannot be subclassed.

.. cpp:struct:: dynamic_attr

   Indicate that instances of a type require a Python dictionary to support the dynamic addition of attributes.

.. cpp:struct:: template <typename T> supplement

   Indicate that ``sizeof(T)`` bytes of memory should be set aside to
   store supplemental data in the type object. See :ref:`Supplemental
   type data <supplement>` for more information.

.. cpp:struct:: type_slots

   .. cpp:function:: type_slots(PyType_Slot * value)

   nanobind uses the ``PyType_FromSpec`` Python C API interface to construct
   types. In certain advanced use cases, it may be helpful to append additional
   type slots during type construction. This class binding annotation can be
   used to accomplish this. The provided list should be followed by a
   zero-initialized ``PyType_Slot`` element. See :ref:`Customizing type creation
   <typeslots>` for more information about this feature.

.. cpp:struct:: type_slots_callback

   .. cpp:function:: type_slots_callback(void (*callback)(detail::type_init_data *, PyType_Slot *&slots, size_t max_slots) noexcept)

   This is an alternative to `type_slots` that provides a callback
   which will be invoked during type creation to populate the type's
   list of slots. It is used by `enum_`. It can be used in addition to
   the `type_slots` annotation; if both are provided,
   `type_slots_callback` runs first (so `type_slots` can override its choices).

   The callback should execute ``*slots++ = {Py_tp_foo, (void *) handle_foo};``
   at most *max_slots* times.

   Information about the type under construction is available via the first
   parameter received by the callback, but be aware that this is an internal
   type which is not subject to nanobind's usual semantic versioning guarantees.
   See ``include/nanobind/nb_class.h`` for more details.

.. cpp:struct:: template <typename T> intrusive_ptr

   nanobind provides a custom interface for intrusive reference-counted C++
   types that nicely integrate with Python reference counting. See the
   :ref:`separate section <intrusive>` on this topic. This annotation
   marks a type as compatible with this interface.

   .. cpp:function:: intrusive_ptr(void (* set_self_py)(T*, PyObject*) noexcept)

      Declares a callback that will be invoked when a C++ instance is first
      cast into a Python object.


.. _enum_binding_annotations:

Enum binding annotations
------------------------

The following annotations can be specified using the variable-length
``Extra`` parameter of the constructor :cpp:func:`enum_::enum_`.
Enums also support the :cpp:struct:`dynamic_attr` and
:cpp:struct:`type_slots` annotations
documented for :ref:`classes <class_binding_annotations>`.

.. cpp:struct:: is_arithmetic

   Indicate that the enumeration may be used with arithmetic
   operations.  This enables the binary operators ``+ - * // & | ^ <<
   >>`` and unary ``- ~ abs()``, with operands of either enumeration
   or numeric type; the result will be as if the enumeration operands
   were first converted to integers. (So ``Shape(2) + Shape(1) == 3`` and
   ``Shape(2) * 1.5 == 3.0``.) It is unspecified whether operations on
   mixed enum types (such as ``Shape.Circle + Color.Red``) are
   permissible.

Function binding
----------------

.. cpp:function:: object cpp_function(Func &&f, const Extra&... extra)

   Convert the function `f` into a Python callable. This function has
   a few overloads (not shown here) to separately deal with function/method
   pointers and lambda functions.

   The variable length `extra` parameter can be used to pass a docstring and
   other :ref:`function binding annotations <function_binding_annotations>`.

.. _class_binding:

Class binding
-------------

.. cpp:class:: template <typename T, typename... Ts> class_ : public object

   Binding helper class to expose a custom C++ type `T` (declared using either
   the ``class`` or ``struct`` keyword) in Python.

   The variable length parameter `Ts` is optional and  can be used to specify
   the base class of `T` and/or an alias needed to realize :ref:`trampoline
   classes <trampolines>`.

   When the type ``T`` was previously already registered (either within the
   same extension or another extension), the the ``class_<..>`` declaration is
   redundant. nanobind will print a warning message in this case:

   .. code-block:: text

      RuntimeWarning: nanobind: type 'MyType' was already registered!

   The ``class_<..>`` instance will subsequently wrap the original type object
   instead of creating a new one.

   .. cpp:function:: template <typename... Extra> class_(handle scope, const char * name, const Extra &... extra)

      Bind the type `T` to the identifier `name` within the scope `scope`. The
      variable length `extra` parameter can be used to pass a docstring and
      other :ref:`class binding annotations <class_binding_annotations>`.

   .. cpp:function:: template <typename Func, typename... Extra> class_ &def(const char * name, Func &&f, const Extra &... extra)

      Bind the function `f` and assign it to the class member `name`.
      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      This function has two overloads (listed just below) to handle constructor
      binding declarations.

      **Example**:

      .. code-block:: cpp

         struct A {
             void f() { /*...*/ }
         };

         nb::class_<A>(m, "A")
             .def(nb::init<>()) // Bind the default constructor
             .def("f", &A::f);  // Bind the method A::f

   .. cpp:function:: template <typename... Args, typename... Extra> class_ &def(init<Args...> init, const Extra &... extra)

      Bind a constructor. The variable length `extra` parameter can be used to
      pass a docstring and other :ref:`function binding annotations
      <function_binding_annotations>`.

   .. cpp:function:: template <typename Arg, typename... Extra> class_ &def(init_implicit<Arg> init, const Extra &... extra)

      Bind a constructor that may be used for implicit type conversions. The
      constructor must take a single argument of an unspecified type `Arg`.

      When nanobind later tries to dispatch a function call requiring an
      argument of type `T` while `Arg` was actually provided, it will run this
      constructor to perform the necessary conversion.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      This constructor generates more compact code than a separate call to
      :cpp:func:`implicitly_convertible`, but is otherwise equivalent.

   .. cpp:function:: template <typename C, typename D, typename... Extra> class_ &def_rw(const char * name, D C::* p, const Extra &...extra)

      Bind the field `p` and assign it to the class member `name`. nanobind
      constructs a ``property`` object with *read-write* access (hence the
      ``rw`` suffix) to do so.

      Every access from Python will read from or write to the C++ field while
      performing a suitable conversion (using :ref:`type casters
      <type_casters>`, :ref:`bindings <bindings>`, or :ref:`wrappers
      <wrappers>`) as determined by its type.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`
      that are forwarded to the anonymous functions used to construct the
      property

      **Example**:

      .. code-block:: cpp

         struct A { int value; };

         nb::class_<A>(m, "A")
             .def_rw("value", &A::value); // Enable mutable access to the field A::value

   .. cpp:function:: template <typename C, typename D, typename... Extra> class_ &def_ro(const char * name, D C::* p, const Extra &...extra)

      Bind the field `p` and assign it to the class member `name`. nanobind
      constructs a ``property`` object with *read only* access (hence the
      ``ro`` suffix) to do so.

      Every access from Python will read the C++ field while performing a
      suitable conversion (using :ref:`type casters <type_casters>`,
      :ref:`bindings <bindings>`, or :ref:`wrappers <wrappers>`) as determined
      by its type.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`
      that are forwarded to the anonymous functions used to construct the
      property.

      **Example**:

      .. code-block:: cpp

         struct A { int value; };

         nb::class_<A>(m, "A")
             .def_ro("value", &A::value);  // Enable read-only access to the field A::value

   .. cpp:function:: template <typename Getter, typename Setter, typename... Extra> class_ &def_prop_rw(const char * name, Getter &&getter, Setter &&setter, const Extra &...extra)

      Construct a *mutable* (hence the ``rw`` suffix) Python ``property`` and
      assign it to the class member `name`. Every read access will call the
      function ``getter``  with the `T` instance, and every write access will
      call the ``setter`` with the `T` instance and value to be assigned.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      Note that this function implicitly assigns the
      :cpp:enumerator:`rv_policy::reference_internal` return value policy to
      `getter` (as opposed to the usual
      :cpp:enumerator:`rv_policy::automatic`). Provide an explicit return value
      policy as part of the `extra` argument to override this.

      **Example**: the example below uses `def_prop_rw` to expose a C++
      setter/getter pair as a more "Pythonic" property:

      .. code-block:: cpp

          class A {
          public:
              A(int value) : m_value(value) { }
              void set_value(int value) { m_value = value; }
              int value() const { return m_value; }
          private:
              int m_value;
          };

          nb::class_<A>(m, "A")
              .def(nb::init<int>())
              .def_prop_rw("value",
                  [](A &t) { return t.value() ; },
                  [](A &t, int value) { t.set_value(value); });

   .. cpp:function:: template <typename Getter, typename... Extra> class_ &def_prop_ro(const char * name, Getter &&getter, const Extra &...extra)

      Construct a *read-only* (hence the ``ro`` suffix) Python ``property`` and
      assign it to the class member `name`. Every read access will call the
      function ``getter``  with the `T` instance.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      Note that this function implicitly assigns the
      :cpp:enumerator:`rv_policy::reference_internal` return value policy to
      `getter` (as opposed to the usual
      :cpp:enumerator:`rv_policy::automatic`). Provide an explicit return value
      policy as part of the `extra` argument to override this.

      **Example**: the example below uses `def_prop_ro` to expose a C++ getter
      as a more "Pythonic" property:

      .. code-block:: cpp

          class A {
          public:
              A(int value) : m_value(value) { }
              int value() const { return m_value; }
          private:
              int m_value;
          };

          nb::class_<A>(m, "A")
              .def(nb::init<int>())
              .def_prop_ro("value",
                  [](A &t) { return t.value() ; });

   .. cpp:function:: template <typename Func, typename... Extra> class_ &def_static(const char * name, Func &&f, const Extra &... extra)

      Bind the *static* function `f` and assign it to the class member `name`.
      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      **Example**:

      .. code-block:: cpp

         struct A {
             static void f() { /*...*/ }
         };

         nb::class_<A>(m, "A")
             .def_static("f", &A::f);  // Bind the static method A::f

   .. cpp:function:: template <typename D, typename... Extra> class_ &def_rw_static(const char * name, D* p, const Extra &...extra)

      Bind the *static* field `p` and assign it to the class member `name`. nanobind
      constructs a class ``property`` object with *read-write* access (hence the
      ``rw`` suffix) to do so.

      Every access from Python will read from or write to the static C++ field
      while performing a suitable conversion (using :ref:`type casters
      <type_casters>`, :ref:`bindings <bindings>`, or :ref:`wrappers
      <wrappers>`) as determined by its type.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`
      that are forwarded to the anonymous functions used to construct the
      property

      **Example**:

      .. code-block:: cpp

         struct A { inline static int value = 5; };

         nb::class_<A>(m, "A")
             // Enable mutable access to the static field A::value
             .def_rw_static("value", &A::value);

   .. cpp:function:: template <typename D, typename... Extra> class_ &def_ro_static(const char * name, D* p, const Extra &...extra)

      Bind the *static* field `p` and assign it to the class member `name`.
      nanobind constructs a class ``property`` object with *read-only* access
      (hence the ``ro`` suffix) to do so.

      Every access from Python will read the static C++ field while performing
      a suitable conversion (using :ref:`type casters <type_casters>`,
      :ref:`bindings <bindings>`, or :ref:`wrappers <wrappers>`) as determined
      by its type.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`
      that are forwarded to the anonymous functions used to construct the
      property

      **Example**:

      .. code-block:: cpp

         struct A { inline static int value = 5; };

         nb::class_<A>(m, "A")
             // Enable read-only access to the static field A::value
             .def_ro_static("value", &A::value);

   .. cpp:function:: template <typename Getter, typename Setter, typename... Extra> class_ &def_prop_rw_static(const char * name, Getter &&getter, Setter &&setter, const Extra &...extra)

      Construct a *mutable* (hence the ``rw`` suffix) Python ``property`` and
      assign it to the class member `name`. Every read access will call the
      function ``getter``  with `T`'s Python type object, and every write access will
      call the ``setter`` with `T`'s Python type object and value to be assigned.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      Note that this function implicitly assigns the
      :cpp:enumerator:`rv_policy::reference` return value policy to
      `getter` (as opposed to the usual
      :cpp:enumerator:`rv_policy::automatic`). Provide an explicit return value
      policy as part of the `extra` argument to override this.

      **Example**: the example below uses `def_prop_rw_static` to expose a
      static C++ setter/getter pair as a more "Pythonic" property:

      .. code-block:: cpp

         class A {
         public:
            static void set_value(int value) { s_value = value; }
            static int value() { return s_value; }
         private:
            inline static int s_value = 5;
         };

         nb::class_<A>(m, "A")
             .def_prop_rw_static("value",
                 [](nb::handle /*unused*/) { return A::value() ; },
                 [](nb::handle /*unused*/, int value) { A::set_value(value); });

   .. cpp:function:: template <typename Getter, typename... Extra> class_ &def_prop_ro_static(const char * name, Getter &&getter, const Extra &...extra)

      Construct a *read-only* (hence the ``ro`` suffix) Python ``property`` and
      assign it to the class member `name`. Every read access will call the
      function ``getter``  with `T`'s Python type object.

      The variable length `extra` parameter can be used to pass a docstring and
      other :ref:`function binding annotations <function_binding_annotations>`.

      Note that this function implicitly assigns the
      :cpp:enumerator:`rv_policy::reference` return value policy to
      `getter` (as opposed to the usual
      :cpp:enumerator:`rv_policy::automatic`). Provide an explicit return value
      policy as part of the `extra` argument to override this.

      **Example**: the example below uses `def_prop_ro_static` to expose a
      static C++ getter as a more "Pythonic" property:

      .. code-block:: cpp

         class A {
         public:
            static int value() { return s_value; }
         private:
            inline static int s_value = 5;
         };

         nb::class_<A>(m, "A")
             .def_prop_ro_static("value",
                 [](nb::handle /*unused*/) { return A::value() ; });

   .. cpp:function:: template <detail::op_id id, detail::op_type ot, typename L, typename R, typename... Extra> class_ &def(const detail::op_<id, ot, L, R> &op, const Extra&... extra)

      This interface provides convenient syntax sugar to replace relatively
      length method bindings with shorter operator bindings. To use it, you
      will need an extra include directive:

      .. code-block:: cpp

         #include <nanobind/operators.h>

      Below is an example type with three arithmetic operators in C++ (unary
      negation and 2 binary subtraction overloads) along with corresponding
      bindings.

      **Example**:

      .. code-block:: cpp

         struct A {
            float value;

            A operator-() const { return { -value }; }
            A operator-(const A &o) const { return { value - o.value }; }
            A operator-(float o) const { return { value - o }; }
         };

         nb::class_<A>(m, "A")
             .def(nb::init<float>())
             .def(-nb::self)
             .def(nb::self - nb::self)
             .def(nb::self - float());


      Bind an arithmetic or comparison operator expressed in short-hand form (e.g., ``.def(nb::self + nb::self)``).

   .. cpp:function:: template <detail::op_id id, detail::op_type ot, typename L, typename R, typename... Extra> class_ &def_cast(const detail::op_<id, ot, L, R> &op, const Extra&... extra)

      Like the above ``.def()`` variant, but furthermore cast the result of the operation back to `T`.


.. cpp:class:: template <typename T> enum_ : public class_<T>

   Class binding helper for scoped and unscoped C++ enumerations.

   .. cpp:function:: template <typename... Extra> NB_INLINE enum_(handle scope, const char * name, const Extra &...extra)

      Bind the enumeration of type `T` to the identifier `name` within the
      scope `scope`. The variable length `extra` parameter can be used to pass
      a docstring and other :ref:`enum binding annotations
      <enum_binding_annotations>` such as :cpp:class:`is_arithmetic`.

   .. cpp:function:: enum_ &value(const char * name, T value, const char * doc = nullptr)

      Add the entry `value` to the enumeration using the identifier `name`,
      potentially with a docstring provided via `doc` (optional).

   .. cpp:function:: enum_ &export_values()

      Export all entries of the enumeration into the parent scope.

.. cpp:class:: template <typename T> exception : public object

   Class binding helper for declaring new Python exception types

   .. cpp:function:: exception(handle scope, const char * name, handle base = PyExc_Exception)

      Create a new exception type identified by `name` that derives from
      `base`, and install it in `scope`. The constructor also calls
      :cpp:func:`register_exception_translator()` to register a new exception
      translator that converts caught C++ exceptions of type `T` into the
      newly created Python equivalent.

.. cpp:struct:: template <typename... Args> init

   nanobind uses this simple helper class to capture the signature of a
   constructor. It is only meant to be used in binding declarations done via
   :cpp:func:`class_::def()`.

   Sometimes, it is necessary to bind constructors that don't exist in the
   underlying C++ type (meaning that they are specific to the Python bindings).
   Because `init` only works for existing C++ constructors, this requires
   a manual workaround noting that

   .. code-block:: cpp

      nb::class_<MyType>(m, "MyType")
          .def(nb::init<const char*, int>());

   is syntax sugar for the following lower-level implementation using
   "`placement new <https://en.wikipedia.org/wiki/Placement_syntax>`_":

   .. code-block:: cpp

      nb::class_<MyType>(m, "MyType")
          .def("__init__",
               [](MyType* t, const char* arg0, int arg1) {
                   new (t) MyType(arg0, arg1);
               });

   The provided lambda function will be called with a pointer to uninitialized
   memory that has already been allocated (this memory region is co-located
   with the Python object for reasons of efficiency). The lambda function can
   then either run an in-place constructor and return normally (in which case
   the instance is assumed to be correctly constructed) or fail by raising an
   exception.

.. cpp:struct:: template <typename Arg> init_implicit

   See :cpp:class:`init` for detail on binding constructorts. The main
   difference between :cpp:class:`init`  and `init_implicit` is that the latter
   only supports constructors taking a single argument `Arg`, and that it marks
   the constructor as usable for implicit conversions from `Arg`.

   Sometimes, it is necessary to bind implicit conversion-capable constructors
   that don't exist in the underlying C++ type (meaning that they are specific
   to the Python bindings). This can be done manually noting that

   .. code-block:: cpp

      nb::class_<MyType>(m, "MyType")
          .def(nb::init_implicit<const char*, int>());

   can be replaced by the lower-level code

   .. code-block:: cpp

       nb::class_<MyType>(m, "MyType")
           .def("__init__",
                [](MyType* t, const char* arg0) {
                    new (t) MyType(arg0);
                });

       nb::implicitly_convertible<const char*, MyType>();


GIL Management
--------------

These two `RAII
<https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization>`_ helper
classes acquire and release the *Global Interpreter Lock* (GIL) in a given
scope. The :cpp:struct:`gil_scoped_release` helper is often combined with the
:cpp:struct:`call_guard`, as in

.. code-block:: cpp

    m.def("expensive", &expensive, nb::call_guard<nb::gil_scoped_release>());

This releases the interpreter lock while `expensive` is running, which permits
running it in parallel from multiple Python threads.

.. cpp:struct:: gil_scoped_acquire

   .. cpp:function:: gil_scoped_acquire()

      Acquire the GIL

   .. cpp:function:: ~gil_scoped_acquire()

      Release the GIL

.. cpp:struct:: gil_scoped_release

   .. cpp:function:: gil_scoped_release()

      Release the GIL (**must** be currently held)

   .. cpp:function:: ~gil_scoped_release()

      Reacquire the GIL

Low-level type and instance access
----------------------------------

nanobind exposes a low-level interface to provide fine-grained control over
the sequence of steps that instantiates a Python object wrapping a C++
instance. An thorough explanation of these features is provided in a
:ref:`separate section <lowlevel>`. The function listing below merely
summarizes their signatures.

Type objects
^^^^^^^^^^^^

.. cpp:function:: bool type_check(handle h)

   Returns ``true`` if ``h`` is a type that was previously bound via
   :cpp:class:`class_`.

.. cpp:function:: size_t type_size(handle h)

   Assuming that `h` represents a bound type (see :cpp:func:`type_check`),
   return its size in bytes.

.. cpp:function:: size_t type_align(handle h)

   Assuming that `h` represents a bound type (see :cpp:func:`type_check`),
   return its alignment in bytes.

.. cpp:function:: const std::type_info& type_info(handle h)

   Assuming that `h` represents a bound type (see :cpp:func:`type_check`),
   return its C++ RTTI record.

.. cpp:function:: template <typename T> T &type_supplement(handle h)

   Return a reference to supplemental data stashed in a type object.
   The type ``T`` must exactly match the type specified in the
   :cpp:class:`nb::supplement\<T\> <supplement>` annotation used when
   creating the type; no type check is performed, and accessing an
   incorrect supplement type may crash the interpreter.
   See :cpp:class:`supplement`.

.. cpp:function:: str type_name(handle h)

   Return the full (module-qualified) name of a type object as a Python string.

Instances
^^^^^^^^^

.. cpp:function:: bool inst_check(handle h)

   Returns ``true`` if `h` represents an instance of a type that was
   previously bound via :cpp:class:`class_`.

.. cpp:function:: template <typename T> T * inst_ptr(handle h)

   Assuming that `h` represents an instance of a type that was previously bound
   via :cpp:class:`class_`, return a pointer to the C++ instance.

   The function *does not check* that `T` is consistent with the type of `h`.

.. cpp:function:: object inst_alloc(handle h)

   Assuming that `h` represents a bound type (see :cpp:func:`type_check`),
   allocate an uninitialized Python object of type `h` and return it.

.. cpp:function:: object inst_wrap(handle h, void * p)

   Assuming that `h` represents an instance of a type that was previously bound
   via :cpp:class:`class_`, create an object of type `h` that wraps an existing
   C++ instace `p`.

.. cpp:function:: void inst_zero(handle h)

   Zero-initialize the contents of `h`.

.. cpp:function:: bool inst_ready(handle h)

   Query the *ready* field of the object `h`.

.. cpp:function:: void inst_mark_ready(handle h)

   Mark the object `h` as *ready*.

.. cpp:function:: std::pair<bool, bool> inst_state(handle h)

   Query the *ready* and *destruct* fields of the object `h`.

.. cpp:function:: void inst_set_state(handle h, bool ready, bool destruct)

   Set the *ready* and *destruct* fields of the object `h`.

.. cpp:function:: void inst_destruct(handle h)

   Destruct the object `h`.

.. cpp:function:: void inst_copy(handle dst, handle src)

   Move-construct the contents of `src` into `dst` and mark `dst` as *ready*.

.. cpp:function:: void inst_move(handle dst, handle src)

   Copy-construct the contents of `src` into `dst` and mark `dst` as *ready*.

.. cpp:function:: str inst_name(handle h)

   Return the full (module-qualified) name of the instance's type object as a Python string.


Global flags
------------

.. cpp:function:: void set_leak_warnings(bool value) noexcept

   By default, nanobind loudly complains when any nanobind instances, types, or
   functions are still alive when the Python interpreter shuts down. Call this
   function to disable or re-enable leak warnings.

.. cpp:function:: void set_implicit_cast_warnings(bool value) noexcept

   By default, nanobind loudly complains when it attempts to perform an
   implicit conversion, and when that conversion is not successful. Call this
   function to disable or re-enable the warnings.


Miscellaneous
-------------

.. cpp:function:: str repr(handle h)

   Return a stringified version of the provided Python object.
   Equivalent to ``repr(h)`` in Python.

.. cpp:function:: void print(handle value, handle end = handle(), handle file = handle())

   Invoke the Python ``print()`` function to print the object `value`. If desired,
   a line ending `end` and file handle `file` can be specified.

.. cpp:function:: void print(const char * str, handle end = handle(), handle file = handle())

   Invoke the Python ``print()`` function to print the null-terminated C-style
   string `str` that is encoded using UTF-8 encoding.  If desired, a line
   ending `end` and file handle `file` can be specified.

.. cpp:function:: iterator iter(handle h)

   Equivalent to ``iter(h)`` in Python.

.. cpp:function:: object none()

   Return an object representing the value ``None``.

.. cpp:function:: dict builtins()

   Return the ``__builtins__`` dictionary.

.. cpp:function:: template <typename Source, typename Target> void implicitly_convertible()

   Indicate that the type `Source` is implicitly convertible into `Target`
   (which must refer to a type that was previously bound via
   :cpp:class:`class_`).

   *Note*: the :cpp:struct:`init_implicit` interface generates more compact
   code and should be preferred, i.e., use

   .. code-block:: cpp

      nb::class_<Target>(m, "Target")
          .def(nb::init_implicit<Source>());

   instead of

   .. code-block:: cpp

      nb::class_<Target>(m, "Target")
          .def(nb::init<Source>());

      nb::implicitly_convertible<Target, Source>();

   The function is provided for reasons of compatibility with pybind11, and as
   an escape hatch to enable use cases where :cpp:struct:`init_implicit`
   is not available (e.g., for custom binding-specific constructors that don't
   exist in `Target` type).

.. cpp:struct:: template <typename T, typename D> typed

    This helper class provides an an API for overriding the type
    annotation of a function argument or return value in generated
    docstrings. It is particularly helpful when the type signature is
    not obvious and must be computed at compile time.  Otherwise, the
    :cpp:class:`raw_doc` attribute provides a simpler alternative for
    taking full control of docstrings and type annotations.

    Consider the following binding that iterates over a Python list.

    .. code-block:: cpp

       m.def("f", [](nb::list l) {
           for (handle h : l) {
               // ...
           }
       });

    Suppose that ``f`` expects a list of ``Foo`` objects, which is not clear
    from the signature. To improve the function signature, use the
    ``nb::typed<T, D>`` wrapper class to pass the argument.

    The template argument `T` should be set to the original argument type, and
    `D` points to a helper class that will be used to compute the type name at
    compile time. Any access to the list ``l`` must be replaced by ``l.value``:

    .. code-block:: cpp

       m.def("f", [](nb::typed<nb::list, FooListName> l) {
           for (nb::handle h : l.value) {
               // ...
           }
       });

    In this simple example, the ``FooListName`` type can be defined as follows:

    .. code-block:: cpp

       struct FooListName {
           static constexpr auto Name =
               nb::detail::const_name("list[") +
               nb::detail::const_name<Foo>() +
               nb::detail::const_name("]");
       };

    More generally, `D` can be a templated class with partial overloads,
    which allows for advanced constructions.

    .. cpp:member:: T value

       Wrapped value of the `typed` parameter.
