.. _changelog:

.. cpp:namespace:: nanobind

Changelog
#########

nanobind uses a `semantic versioning <http://semver.org>`__ policy for its API.
It also has a separate ABI version that is *not* subject to semantic
versioning.

The ABI version is relevant whenever a type binding from one extension module
should be visible in another (also nanobind-based) extension module. In this
case, both modules must use the same nanobind ABI version, or they will be
isolated from each other. Releases that don't explicitly mention an ABI version
below inherit that of the preceding release.


Version 1.5.0 (TBA)
-------------------

* Support for exception chaining. (commit `041520
  <https://github.com/wjakob/nanobind/commit/0415208e83885dba038516d86c2f4cca5f81df5f>`__).
* The :cpp:func:`nb::list::append() <list::append>` method now performs perfect
  forwarding. (commit `2219d0
  <https://github.com/wjakob/nanobind/commit/2219d0b0fec5e6cc4fce96bc3dbad6bfa148a57d>`__).
* Many improvements to the handling of return value policies in
  :cpp:class:`nb::ndarray\<..\> <ndarray>` to avoid unnecessary copies. (commit `ffd22b
  <https://github.com/wjakob/nanobind/commit/ffd22b069ba95a546baeca0bdb6711fb9059cad8>`__,
  `a79575
  <https://github.com/wjakob/nanobind/commit/a79575165134c72c0a26e46772290d0404eae7a3>`__,
  and `6f0c3f
  <https://github.com/wjakob/nanobind/commit/6f0c3feaf088e78c75f2abee90164f20446eba08>`__).
* The :cpp:class:`nb::ndarray\<..\> <ndarray>` class now has an additional
  convenience constructor that takes the shape and (optionally) strides using
  ``std::initializer_list``. (commit `de1117
  <https://github.com/wjakob/nanobind/commit/de111766b21fe893a41cd4614a346b0da251f7f2>`__).
* Inference of ``automatic*`` return value policy was entirely moved to the
  base C++ class type caster. (commit `1ff9df
  <https://github.com/wjakob/nanobind/commit/1ff9df03fb56a16f56854b4cecd1f388f73d3b53>`__).
* Switch to the new Python 3.12 error status API if available. (commit `36751c
  <https://github.com/wjakob/nanobind/commit/36751cb05994a96a3801bf511c846a7bc68e2f09>`__).
* Various minor fixes and improvements.
* ABI version 10.

Version 1.4.0 (June 8, 2023)
----------------------------

* Improved the efficiency of the function dispatch loop. (PR `#227
  <https://github.com/wjakob/nanobind/pull/227>`__).
* Significant improvements to the Eigen type casters (generalized stride
  handling to avoid unnecessary copies, support for conversion via
  ``nb::cast()``, many refinements to the  ``Eigen::Ref<T>`` interface). (PR
  `#215 <https://github.com/wjakob/nanobind/pull/215>`__).
* Added a ``NB_DOMAIN`` parameter to :cmake:command:`nanobind_add_module` which
  can isolate extensions from each other to avoid binding clashes. See the
  associated :ref:`FAQ entry <type-visibility>` for details. (commit `977119
  <https://github.com/wjakob/nanobind/commit/977119c4797db7decf8064cf118afde768ff8fab>`__).
* Reduced the severity of nanobind encountering a duplicate type binding
  (commits `f3b0e6
  <https://github.com/wjakob/nanobind/commit/f3b0e6cbd69a4adcdc31dbe0b844370b1b60dbcf>`__,
  and `2c9124
  <https://github.com/wjakob/nanobind/commit/2c9124bbbe736881fa8f9f33ea7817c98b43bf8b>`__).
* Support for pickling/unpickling nanobind objects. (commit `59843e
  <https://github.com/wjakob/nanobind/commit/59843e09bc6e8f2b0338829a44cf71e25f76cba3>`__).
* ABI version 9.

Version 1.3.2 (June 2, 2023)
----------------------------

* Fixed compilation on 32 bit processors (only ``i686`` tested so far).
  (PR `#224 <https://github.com/wjakob/nanobind/pull/224>`__).
* Fixed compilation on PyPy 3.8. (commit `cd8135
  <https://github.com/wjakob/nanobind/commit/cd8135baa1da1213252272b5c9ecbf909e947597>`__).
* Reduced binary bloat of musllinux wheels. (commit `f52513
  <https://github.com/wjakob/nanobind/commit/f525139a80d173feaea5518e842aceeb6ceec5cf>`__).

Version 1.3.1 (May 31, 2023)
----------------------------

* CMake build system improvements for stable ABI wheel generation. 
  (PR `#222 <https://github.com/wjakob/nanobind/pull/222>`__).

Version 1.3.0 (May 31, 2023)
----------------------------

This is a big release. The sections below cover added features, efficiency
improvements, and miscellaneous fixes and improvements.

New features
^^^^^^^^^^^^
* nanobind now supports binding types that inherit from
  ``std::enable_shared_from_this<T>``. See the :ref:`advanced section
  on object ownership <enable_shared_from_this>` for more details.
  (PR `#212 <https://github.com/wjakob/nanobind/pull/212>`__).
* Added a type caster between Python ``datetime``/``timedelta`` objects and
  C++ ``std::chrono::duration``/``std::chrono::time_point``, ported
  from pybind11. (PR `#175 <https://github.com/wjakob/nanobind/pull/175>`__).
* The :cpp:class:`nb::ndarray\<..\> <ndarray>` class can now use the buffer
  protocol to receive and return arrays representing read-only memory. (PR
  `#217 <https://github.com/wjakob/nanobind/pull/217>`__).
* Added :cpp:func:`nb::python_error::discard_as_unraisable()
  <python_error::discard_as_unraisable>` as a wrapper around
  ``PyErr_WriteUnraisable()``. (PR `#175
  <https://github.com/wjakob/nanobind/pull/175>`__).

Efficiency improvements:
^^^^^^^^^^^^^^^^^^^^^^^^

* Reduced the per-instance overhead of nanobind by 1 pointer and simplified the
  internal hash table types to crunch ``libnanobind``. (commit `de018d
  <https://github.com/wjakob/nanobind/commit/de018db2d17905564703f1ade4aa201a22f8551f>`__).
* Supplemental type data specified via :cpp:class:`nb::supplement\<T\>()
  <supplement>` is now stored directly within the type object instead of being
  referenced through an indirection. (commit `d82ca9
  <https://github.com/wjakob/nanobind/commit/d82ca9c14191e74dd35dd5bf15fc90f5230319fb>`__).
* Reduced the number of exception-related exports to further crunch
  ``libnanobind``. (commit `763962
  <https://github.com/wjakob/nanobind/commit/763962b8ce76414148089ef6a68cff97d7cc66ce>`__).
* Reduced the size of nanobind type objects by 5 pointers. (PR `#194
  <https://github.com/wjakob/nanobind/pull/194>`__, `#195
  <https://github.com/wjakob/nanobind/pull/195>`__, and commit `d82ca9
  <https://github.com/wjakob/nanobind/commit/d82ca9c14191e74dd35dd5bf15fc90f5230319fb>`__).
* Internal nanobind types (``nb_type``, ``nb_static_property``, ``nb_ndarray``)
  are now constructed on demand. This reduces the size of the ``libnanobind``
  component in static (``NB_STATIC``) builds when those features are not used.
  (commits `95e45a
  <https://github.com/wjakob/nanobind/commit/95e45a4027dcbce935091533f7d41bf59e3e5fe1>`__,
  `375083
  <https://github.com/wjakob/nanobind/commit/37508386a1f8c346d17a0353c8152940aacde9c2>`__,
  and `e033c8
  <https://github.com/wjakob/nanobind/commit/e033c8fab4a14cbb9c5b0e08b1bdf49af2a9cb22>`__).
* Added a small function cache to improve code generation in limited API
  builds. (commit `f0f4aa
  <https://github.com/wjakob/nanobind/commit/f0f42a564995ba3bd573282674d1a6d636a048c8>`__).
* Refined compiler and linker flags across platforms to ensure compact binaries
  especially in ``NB_STATIC`` builds. (commit `5ead9f
  <https://github.com/wjakob/nanobind/commit/5ead9ff348a2ef0df8231e6480607a5b0623a16b>`__)
* nanobind enums now take advantage of :ref:`supplemental data <supplement>`
  to improve the speed of object and name lookups. Note that this prevents
  use of ``nb::supplement<T>()`` with enums for other purposes.
  (PR `#195 <https://github.com/wjakob/nanobind/pull/195>`__).

Miscellaneous fixes and improvements
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Use the new `PEP-697 <https://peps.python.org/pep-0697/>`__ interface to
  access data in type objects when compiling stable ABI3 wheels. This improves
  forward compatibility (the Python team may at some point significantly
  refactor the layout and internals of type objects). (PR `#211
  <https://github.com/wjakob/nanobind/pull/211>`__):
* Added introspection attributes ``__self__`` and ``__func__`` to nanobind
  bound methods, to make them more like regular Python bound methods.
  Fixed a bug where ``some_obj.method.__call__()`` would behave differently
  than ``some_obj.method()``.
  (PR `#216 <https://github.com/wjakob/nanobind/pull/216>`__).
* Updated the implementation of :cpp:class:`nb::enum_ <enum_>` so it does
  not take advantage of any private nanobind type details. As a side effect,
  the construct ``nb::class_<T>(..., nb::is_enum(...))`` is no longer permitted;
  use ``nb::enum_<T>(...)`` instead.
  (PR `#195 <https://github.com/wjakob/nanobind/pull/195>`__).
* Added the :cpp:class:`nb::type_slots_callback` class binding annotation,
  similar to :cpp:class:`nb::type_slots` but allowing more dynamic choices.
  (PR `#195 <https://github.com/wjakob/nanobind/pull/195>`__).
* nanobind type objects now treat attributes specially whose names
  begin with ``@``. These attributes can be set once, but not
  rebound or deleted.  This safeguard allows a borrowed reference to
  the attribute value to be safely stashed in the type supplement,
  allowing arbitrary Python data associated with the type to be accessed
  without a dictionary lookup while keeping this data visible to the
  garbage collector.  (PR `#195 <https://github.com/wjakob/nanobind/pull/195>`__).
* Fixed surprising behavior in enumeration comparisons and arithmetic
  (PR `#207 <https://github.com/wjakob/nanobind/pull/207>`__):

  * Enum equality comparisons (``==`` and ``!=``) now can only be true
    if both operands have the same enum type, or if one is an enum and
    the other is an :py:class:`int`. This resolves some confusing
    results and ensures that enumerators of different types have a
    distinct identity, which is important if they're being put into
    the same set or used as keys in the same dictionary. All of the
    following were previously true but will now evaluate as false:

    * ``FooEnum(1) == BarEnum(1)``
    * ``FooEnum(1) == 1.2``
    * ``FooEnum(1) == "1"``

  * Enum ordering comparisons (``<``, ``<=``, ``>=``, ``>``) and
    arithmetic operations (when using the :cpp:struct:`is_arithmetic`
    annotation) now require that any non-enum operand be a Python number
    (an object that defines ``__int__``, ``__float__``, and/or ``__index__``)
    and will avoid truncating non-integer operands to integers. Note that
    unlike with equality comparisons, ordering and arithmetic operations
    *do* still permit two operands that are enums of different types.
    Some examples of changed behavior:

    * ``FooEnum(1) < 1.2`` is now true (used to be false)
    * ``FooEnum(2) * 1.5`` is now 3.0 (used to be 2)
    * ``FooEnum(3) - "2"`` now raises an exception (used to be 1)

  * Enum comparisons and arithmetic operations with unsupported types
    now return `NotImplemented` rather than raising an exception.
    This means equality comparisons such as ``some_enum == None`` will
    return unequal rather than failing; order comparisons such as
    ``some_enum < None`` will still fail, but now with a more
    informative error.

* ABI version 8.

Version 1.2.0 (April 24, 2023)
------------------------------

* Improvements to the internal C++ → Python instance map data structure to improve
  performance and address type confusion when returning previously registered instances.
  (commit `716354 <https://github.com/wjakob/nanobind/commit/716354f0ed6123d6a19fcabb077b72a17b4ddf79>`__,
  discussion `189 <https://github.com/wjakob/nanobind/discussions/189>`__).
* Added up-to-date nanobind benchmarks on Linux including comparisons to Cython.
  (commit `834cf3
  <https://github.com/wjakob/nanobind/commit/834cf36ce12ffe6470dcffecd21341377c56cee1>`__
  and `39e163
  <https://github.com/wjakob/nanobind/commit/e9e163ec55de995a68a34fafda2e96ff06532658>`__).
* Removed the superfluous ``nb_enum`` metaclass.
  (commit `9c1985 <https://github.com/wjakob/nanobind/commit/9c19850471be70a22114826f6c0edceee99ff40b>`__).
* Fixed a corner case that prevented ``nb::cast<char>`` from working.
  (commit `9ae320 <https://github.com/wjakob/nanobind/commit/9ae32054d9a6ad17af15994dc51138eb88f71f92>`__).

Version 1.1.1 (April 6, 2023)
-----------------------------

* Added documentation on packaging and distributing nanobind modules. (commit
  `0715b2
  <https://github.com/wjakob/nanobind/commit/0715b278ba806cf13cf63e41d62438481e7b73b8>`__).
* Made the conversion :cpp:func:`handle::operator bool() <handle::operator
  bool>` explicit. (PR `#173 <https://github.com/wjakob/nanobind/pull/173>`__).
* Support :cpp:class:`nb::typed\<..\> <typed>` in return values. (PR `#174
  <https://github.com/wjakob/nanobind/pull/174>`__).
* Tweaks to definitions in ``nb_types.h`` to improve compatibility with further
  C++ compilers (that said, there is no change about the official set of
  supported compilers). (commit `b8bd10
  <https://github.com/wjakob/nanobind/commit/b8bd1086e9b20da8a81a954f03e7947bee5422fd>`__)

Version 1.1.0 (April 5, 2023)
-----------------------------

* Added :cpp:func:`size <ndarray::size>`, :cpp:func:`shape_ptr
  <ndarray::shape_ptr>`, :cpp:func:`stride_ptr <ndarray::stride_ptr>` members
  to to the :cpp:class:`nb::ndarray\<..\> <ndarray>` class. (PR `#161
  <https://github.com/wjakob/nanobind/pull/161>`__).
* Allow macros in :c:macro:`NB_MODULE(..) <NB_MODULE>` name parameter. (PR
  `#168 <https://github.com/wjakob/nanobind/pull/168>`__).
* The :cpp:class:`nb::ndarray\<..\> <ndarray>` interface is more tolerant when
  converting Python (PyTorch/NumPy/..) arrays with a size-0 dimension that have
  mismatched strides. (PR `#162
  <https://github.com/wjakob/nanobind/pull/162>`__).
* Removed the ``<anonymous>`` label from docstrings of anonymous functions,
  which caused issues in MyPy. (PR `#172
  <https://github.com/wjakob/nanobind/pull/172>`__).
* Fixed an issue in the propagation of return value policies that broke
  user-provided/custom policies in properties (PR `#170
  <https://github.com/wjakob/nanobind/pull/170>`__).
* The Eigen interface now converts 1x1 matrices to 1x1 NumPy arrays instead of
  scalars. (commit `445781
  <https://github.com/wjakob/nanobind/commit/445781fc2cf2fa326cc22e8fd483e8e4a7bf6cf5>`__).
* The ``nanobind`` package now has a simple command line interface. (commit
  `d5ccc8
  <https://github.com/wjakob/nanobind/commit/d5ccc8844b29ca6cd5188ffd8d16e034bcee9f73>`__).

Version 1.0.0 (March 28, 2023)
------------------------------

* Nanobind now has a logo. (commit `b65d31
  <https://github.com/wjakob/nanobind/commit/b65d3b134d8b9f8d153b51d87751d09a12e4235b>`__).
* Fixed a subtle issue involving function/method properties and the IPython
  command line interface. (PR `#151
  <https://github.com/wjakob/nanobind/pull/151>`__).
* Added a boolean type to the :cpp:class:`nb::ndarray\<..\> <ndarray>`
  interface. (PR `#150 <https://github.com/wjakob/nanobind/pull/150>`__).
* Minor fixes and improvements.


Version 0.3.1 (March 8, 2023)
-----------------------------

* Added a type caster for ``std::filesystem::path``. (PR `#138
  <https://github.com/wjakob/nanobind/pull/138>`__ and commit `0b05cd
  <https://github.com/wjakob/nanobind/commit/0b05cde8bd8685ab42328660da03cc4ee66e3ba2>`__).
* Fixed technical issues involving implicit conversions (commits `022935
  <https://github.com/wjakob/nanobind/commit/022935cbb92dfb1d02f90546bf6b34013f90e9e5>`__
  and `5aefe3
  <https://github.com/wjakob/nanobind/commit/5aefe36e3e07b5b98a6be7c0f3ce28a236fe2330>`__)
  and construction of type hierarchies with custom garbage collection hooks
  (commit `022935
  <https://github.com/wjakob/nanobind/commit/7b3e893e1c14d95f7b3fc838657e6f9ce520d609>`__).
* Re-enabled the 'chained fixups' linker optimization for recent macOS
  deployment targets. (commit `2f29ec
  <https://github.com/wjakob/nanobind/commit/2f29ec7d5fbebd5f55fb52da297c8d197279f659>`__).

Version 0.3.0 (March 8, 2023)
-----------------------------

* Botched release, replaced by 0.3.1 on the same day.

Version 0.2.0 (March 3, 2023)
-----------------------------
* Nanobind now features documentation on `readthedocs
  <https://nanobind.readthedocs.io>`__.
* The documentation process revealed a number of inconsistencies in the
  :cpp:func:`class_\<T\>::def* <class_::def>` naming scheme. nanobind will from
  now on use the following shortened and more logical interface:

  .. list-table::
    :widths: 40 60
    :header-rows: 1

    * - Type
      - method
    * - Methods & constructors
      - :cpp:func:`.def() <class_::def>`
    * - Fields
      - :cpp:func:`.def_ro() <class_::def_ro>`,
        :cpp:func:`.def_rw() <class_::def_rw>`
    * - Properties
      - :cpp:func:`.def_prop_ro() <class_::def_prop_ro>`,
        :cpp:func:`.def_prop_rw() <class_::def_prop_rw>`
    * - Static methods
      - :cpp:func:`.def_static() <class_::def_static>`
    * - Static fields
      - :cpp:func:`.def_ro_static() <class_::def_ro_static>`,
        :cpp:func:`.def_rw_static() <class_::def_rw_static>`
    * - Static properties
      - :cpp:func:`.def_prop_ro_static() <class_::def_prop_ro_static>`,
        :cpp:func:`.def_prop_rw_static() <class_::def_prop_rw_static>`

  Compatibility wrappers with deprecation warnings were also added to help port
  existing code. They will be removed when nanobind reaches version 1.0.
  (commits `cb0dc3
  <https://github.com/wjakob/nanobind/commit/cb0dc392b656fd9d0c85c56dc51a9be1de06e176>`__
  and `b5ed96
  <https://github.com/wjakob/nanobind/commit/b5ed696a7a68c9c9adc4d3aa3c6f4adb5b7defeb>`__)
* The ``nb::tensor<..>`` class has been renamed to :cpp:class:`nb::ndarray\<..\> <ndarray>`,
  and it is now located in a different header file (``nanobind/ndarray.h``). A
  compatibility wrappers with a deprecation warning was retained in the
  original header file. It will be removed when nanobind reaches version 1.0.
  (commit `a6ab8b
  <https://github.com/wjakob/nanobind/commit/a6ab8b06dd3316ac53fbed143c346c2b73c31b75>`__).
* Dropped the first two arguments of the :c:macro:`NB_OVERRIDE_*()
  <NB_OVERRIDE>` macros that turned out to be unnecessary in nanobind. (commit
  `22bc21
  <https://github.com/wjakob/nanobind/commit/22bc21b97cd2bbe060d7fb42d374bde72d973ada>`__).
* Added casters for dense matrix/array types from the `Eigen library
  <https://eigen.tuxfamily.org/index.php?title=Main_Page>`__. (PR `#120
  <https://github.com/wjakob/nanobind/pull/120>`__).
* Added casters for sparse matrix/array types from the `Eigen library
  <https://eigen.tuxfamily.org/index.php?title=Main_Page>`__. (PR `#126
  <https://github.com/wjakob/nanobind/pull/126>`_).
* Implemented `nb::bind_vector\<T\>() <bind_vector>` analogous to similar
  functionality in pybind11. (commit `f2df8a
  <https://github.com/wjakob/nanobind/commit/f2df8a90fbfb06ee03a79b0dd85fa0e266efeaa9>`__).
* Implemented :cpp:func:`nb::bind_map\<T\>() <bind_map>` analogous to
  similar functionality in pybind11. (PR `#114
  <https://github.com/wjakob/nanobind/pull/114>`__).
* nanobind now :ref:`automatically downcasts <automatic_downcasting>`
  polymorphic objects in return values analogous to pybind11. (commit `cab96a
  <https://github.com/wjakob/nanobind/commit/cab96a9160e0e1a626bc3e4f9fcddcad31e0f727>`__).
* nanobind now supports :ref:`tag-based polymorphism <tag_based_polymorphism>`.
  (commit `6ade94
  <https://github.com/wjakob/nanobind/commit/6ade94b8e5a2388d66fc9df6f81603c65108cbcc>`__).
* Updated tuple/list iterator to satisfy the ``std::forward_iterator`` concept.
  (PR `#117 <https://github.com/wjakob/nanobind/pull/117>`__).
* Fixed issues with non-writeable tensors in NumPy. (commit `25cc3c
  <https://github.com/wjakob/nanobind/commit/25cc3ccbd1174e7cfc4eef1d1e7206cc38e854ca>`__).
* Removed use of some C++20 features from the codebase. This now makes it
  possible to use nanobind on  Visual Studio 2017 and GCC 7.3.1 (used on RHEL 7).
  (PR `#115 <https://github.com/wjakob/nanobind/pull/115>`__).
* Added the :cpp:class:`nb::typed\<...\> <typed>` wrapper to override the type signature of an
  argument in a bound function in the generated docstring. (commit `b3404c4
  <https://github.com/wjakob/nanobind/commit/b3404c4f347981bce7f4c7a9bac762656bed8385>`__).
* Added an :cpp:func:`nb::implicit_convertible\<A, B\>() <implicitly_convertible>` function analogous to the one in
  pybind11. (commit `aba4af
  <https://github.com/wjakob/nanobind/commit/aba4af06992f14e21e5b7b379e7986e939316da4>`__).
* Updated :cpp:func:`nb::make*_iterator\<..\>() <make_iterator>` so that it returns references of elements, not
  copies. (commit `8916f5
  <https://github.com/wjakob/nanobind/commit/8916f51ad1a25318b5c9fcb07c153f6b72a43bd2>`__).
* Changed the CMake build system so that the library component
  (``libnanobind``) is now compiled statically by default. (commit `8418a4
  <https://github.com/wjakob/nanobind/commit/8418a4aa93d19d7b9714b8d9473539b46cbed508>`__).
* Switched shared library linking on macOS back to a two-level namespace.
  (commit `fe4965
  <https://github.com/wjakob/nanobind/commit/fe4965369435bf7c0925bddf610553d0bb516e27>`__).
* Various minor fixes and improvements.
* ABI version 7.

Version 0.1.0 (January 3, 2023)
-------------------------------

* Allow nanobind methods on non-nanobind) classes. (PR `#104
  <https://github.com/wjakob/nanobind/pull/104>`__).
* Fix dangling `tp_members` pointer in type initialization. (PR `#99
  <https://github.com/wjakob/nanobind/pull/99>`__).
* Added a runtime setting to suppress leak warnings. (PR `#109
  <https://github.com/wjakob/nanobind/pull/109>`__).
* Added the ability to hash ``nb::enum_<..>`` instances (PR `#106
  <https://github.com/wjakob/nanobind/pull/106>`__).
* Fixed the signature of ``nb::enum_<..>::export_values()``. (commit `714d17
  <https://github.com/wjakob/nanobind/commit/714d17e71aa405c7633e0bd798a8bdb7b8916fa1>`__).
* Double-check GIL status when performing reference counting operations in
  debug mode. (commit `a1b245
  <https://github.com/wjakob/nanobind/commit/a1b245fcf210fbfb10d7eb19dc2dc31255d3f561>`__).
* Fixed a reference leak that occurred when module initialization fails.
  (commit `adfa9e
  <https://github.com/wjakob/nanobind/commit/adfa9e547be5575f025d92abeae2e649a690760a>`__).
* Improved robustness of ``nb::tensor<..>`` caster. (commit `633672
  <https://github.com/wjakob/nanobind/commit/633672cd154c0ef13f96fee84c2291562f4ce3d3>`__).
* Upgraded the internally used ``tsl::robin_map<>`` hash table to address a
  rare `overflow issue <https://github.com/Tessil/robin-map/issues/52>`__
  discovered in this codebase. (commit `3b81b1
  <https://github.com/wjakob/nanobind/commit/3b81b18577e243118a659b524d4de9500a320312>`__).
* Various minor fixes and improvements.
* ABI version 6.

Version 0.0.9 (Nov 23, 2022)
----------------------------

* PyPy 7.3.10 or newer is now supported subject to `certain limitations
  <https://github.com/wjakob/nanobind/blob/master/docs/pypy.rst>`__. (commits
  `f935f93
  <https://github.com/wjakob/nanobind/commit/f935f93b9d532a5ef1f385445f328d61eb2af97f>`__
  and `b343bbd
  <https://github.com/wjakob/nanobind/commit/b343bbd11c12b55bbc00492445c743cae18b298f>`__).
* Three changes that reduce the binary size and improve runtime performance of
  binding libraries. (commits `07b4e1fc
  <https://github.com/wjakob/nanobind/commit/07b4e1fc9e94eeaf5e9c2f4a63bdb275a25c82c6>`__,
  `9a803796
  <https://github.com/wjakob/nanobind/commit/9a803796cb05824f9df7593edb984130d20d3755>`__,
  and `cba4d285
  <https://github.com/wjakob/nanobind/commit/cba4d285f4e23b888dfcccc656c221414138a2b7>`__).
* Fixed a reference leak in ``python_error::what()`` (commit `61393ad
  <https://github.com/wjakob/nanobind/commit/61393ad3ce3bc68d195a1496422df43d5fb45ec0>`__).
* Adopted a new policy for function type annotations. (commit `c855c90 <https://github.com/wjakob/nanobind/commit/c855c90fc91d180f7c904c612766af6a84c017e3>`__).
* Improved the effectiveness of link-time-optimization when building extension modules
  with the ``NB_STATIC`` flag. This leads to smaller binaries. (commit `f64d2b9
  <https://github.com/wjakob/nanobind/commit/f64d2b9bb558afe28cf6909e4fa47ebf720f62b3>`__).
* Nanobind now relies on standard mechanisms to inherit the ``tp_traverse`` and
  ``tp_clear`` type slots instead of trying to reimplement the underlying
  CPython logic (commit `efa09a6b
  <https://github.com/wjakob/nanobind/commit/efa09a6bf6ac27f790b2c96389c2da42d4bc176b>`__).
* Moved nanobind internal data structures from ``builtins`` to Python
  interpreter state dictionary. (issue `#96
  <https://github.com/wjakob/nanobind/issues/96>`__, commit `ca23da7
  <https://github.com/wjakob/nanobind/commit/ca23da72ce71a45318f1e59474c9c2906fce5154>`__).
* Various minor fixes and improvements.


Version 0.0.8 (Oct 27, 2022)
----------------------------

* Caster for ``std::array<..>``. (commit `be34b16
  <https://github.com/wjakob/nanobind/commit/be34b165c6a0bed08e477755644f96759b9ed69a>`__).
* Caster for ``std::set<..>`` and ``std::unordered_set`` (PR `#87
  <https://github.com/wjakob/nanobind/pull/87>`__).
* Ported ``nb::make[_key_,_value]_iterator()`` from pybind11. (commit `34d0be1
  <https://github.com/wjakob/nanobind/commit/34d0be1bbeb54b8265456fd3a4a50e98f93fe6d4>`__).
* Caster for untyped ``void *`` pointers. (commit `6455fff
  <https://github.com/wjakob/nanobind/commit/6455fff7be5be2867063ea8138cf10e1d9f3065f>`__).
* Exploit move constructors in ``nb::class_<T>::def_readwrite()`` and
  ``nb::class_<T>::def_readwrite_static()`` (PR `#94
  <https://github.com/wjakob/nanobind/pull/94>`__).
* Redesign of the ``std::function<>`` caster to enable cyclic garbage collector
  traversal through inter-language callbacks (PR `#95
  <https://github.com/wjakob/nanobind/pull/95>`__).
* New interface for specifying custom type slots during Python type
  construction. (commit `38ba18a
  <https://github.com/wjakob/nanobind/commit/38ba18a835cfcd561efb4b4c640ee5c6d525decb>`__).
* Fixed potential undefined behavior related to ``nb_func`` garbage collection by
  Python's cyclic garbage collector. (commit `662e1b9
  <https://github.com/wjakob/nanobind/commit/662e1b9311e693f84c58799a67064d4a44bb706a>`__).
* Added a workaround for spurious reference leak warnings caused by other
  extension modules in conjunction with ``typing.py`` (commit `5e11e80
  <https://github.com/wjakob/nanobind/commit/5e11e8032f777c0a34abd437dc6e84a909907c91>`__).
* Various minor fixes and improvements.
* ABI version 5.

Version 0.0.7 (Oct 14, 2022)
----------------------------

* Fixed a regression involving function docstrings in ``pydoc``. (commit
  `384f4a
  <https://github.com/wjakob/nanobind/commit/384f4ada1f3f08486fb03427227878ddbbcaad43>`__).

Version 0.0.6 (Oct 14, 2022)
----------------------------

* Fixed undefined behavior that could lead to crashes when nanobind types were
  freed. (commit `39266e
  <https://github.com/wjakob/nanobind/commit/39266ef0b0ccd7fa3e9237243a6c97ba8db2cd2a>`__).
* Refactored nanobind so that it works with ``Py_LIMITED_API`` (PR `#37 <https://github.com/wjakob/nanobind/pull/37>`__).
* Dynamic instance attributes (PR `#38 <https://github.com/wjakob/nanobind/pull/38>`__).
* Intrusive pointer support (PR `#43 <https://github.com/wjakob/nanobind/pull/43>`__).
* Byte string support (PR `#62 <https://github.com/wjakob/nanobind/pull/62>`__).
* Casters for ``std::variant<..>`` and ``std::optional<..>`` (PR `#67 <https://github.com/wjakob/nanobind/pull/67>`__).
* Casters for ``std::map<..>`` and ``std::unordered_map<..>`` (PR `#73 <https://github.com/wjakob/nanobind/pull/73>`__).
* Caster for ``std::string_view<..>`` (PR `#68 <https://github.com/wjakob/nanobind/pull/68>`__).
* Custom exception support (commit `41b7da <https://github.com/wjakob/nanobind/commit/41b7da33f1bc5c583bb98df66bdac2a058ec5c15>`__).
* Register nanobind functions with Python's cyclic garbage collector (PR `#86 <https://github.com/wjakob/nanobind/pull/86>`__).
* Various minor fixes and improvements.
* ABI version 3.

Version 0.0.5 (May 13, 2022)
----------------------------

* Enumeration export.
* Implicit number conversion for numpy scalars.
* Various minor fixes and improvements.

Version 0.0.4 (May 13, 2022)
----------------------------

* Botched release, replaced by 0.0.5 on the same day.

Version 0.0.3 (Apr 14, 2022)
----------------------------

* DLPack support.
* Iterators for various Python type wrappers.
* Low-level interface to instance creation.
* Docstring generation improvements.
* Various minor fixes and improvements.

Version 0.0.2 (Mar 10, 2022)
----------------------------

* Initial release of the nanobind codebase.
* ABI version 1.

Version 0.0.1 (Feb 21, 2022)
----------------------------

* Placeholder package on PyPI.
