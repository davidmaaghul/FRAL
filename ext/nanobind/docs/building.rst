.. _building:

Setting up a build system
#########################

This section assumes that you have followed the instructions to :ref:`install
<installing>` nanobind. The easiest way to compile a nanobind-based extension
involves a CMake-based build system. Other build systems can likely be used as
well, but they are not officially supported.

Here, we will create a new package from scratch. If you already have an
existing CMake build system, it should be straightforward to merge some of the
following snippets into it.

Preliminaries
-------------

Begin by creating a new file named ``CMakeLists.txt`` in the root directory of
your project. It should start with the following lines that declare a project
name and tested CMake version range. The third line line searches for Python >=
3.8 including the ``Development.Module`` component required by nanobind.

.. code-block:: cmake

    project(my_project) # Replace 'my_project' with the name of your project
    cmake_minimum_required(VERSION 3.18...3.22)
    find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)

Add the following lines below. They configure CMake to perform an optimized
*release* build by default unless another build type is specified. Without this
addition, binding code may run slowly and produce large binaries.

.. code-block:: cmake

   if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
     set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
     set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
   endif()

Finding nanobind
----------------

Next, we must inform CMake about the presence of nanobind so that it can load
the functionality needed to compile extension modules. The details of this
step depend on *how you installed* nanobind, in the :ref:`previous section
<installing>`.

1. If you installed nanobind as a Pip or Conda package, append the following
   lines at the end of ``CMakeLists.txt``. They query the package to determine
   its installation path and then import it.

   .. code-block:: cmake

       # Detect the installed nanobind package and import it into CMake
       execute_process(
         COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
         OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
       list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
       find_package(nanobind CONFIG REQUIRED)

2. If you installed nanobind as a `Git submodule
   <https://git-scm.com/book/en/v2/Git-Tools-Submodules>`_, append the
   following lines at the end of ``CMakeLists.txt`` to point CMake to the
   directory where nanobind is checked out.

   .. code-block:: cmake

       add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanobind)

Building an extension
---------------------

Finally, we are ready to build an extension! Append the following line at the end of
``CMakeLists.txt``. It will compile a new extension named ``my_ext`` from the
source code contained in the file ``my_ext.cpp``.

.. code-block:: cmake

    nanobind_add_module(my_ext my_ext.cpp)

:cmake:command:`nanobind_add_module` resembles standard CMake commands like
``add_executable()`` and ``add_library()``. Any number of source code and
header files can be declared when the extension is more complex and spread out
over multiple files.

The :ref:`next section <basics>` will review the contents of example module
implementation in ``my_ext.cpp``.
