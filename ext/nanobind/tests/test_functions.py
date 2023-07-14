import test_functions_ext as t
import pytest
import sys
import re


def fail_fn(): # used in test_30
    raise RuntimeError("Foo")


def test01_capture():
    # Functions with and without capture object of different sizes
    assert t.test_01() is None
    assert t.test_02(5, 3) == 2
    assert t.test_03(5, 3) == 44
    assert t.test_04() == 60


def test02_default_args():
    # Default arguments
    assert t.test_02() == 7
    assert t.test_02(7) == 6


def test03_kwargs():
    # Basic use of keyword arguments
    assert t.test_02(3, 5) == -2
    assert t.test_02(3, k=5) == -2
    assert t.test_02(k=5, j=3) == -2


def test04_overloads():
    assert t.test_05(0) == 1
    assert t.test_05(0.0) == 2

def test05_signature():
    assert t.test_01.__doc__ == 'test_01() -> None'
    assert t.test_02.__doc__ == 'test_02(j: int = 8, k: int = 1) -> int'
    assert t.test_05.__doc__ == (
        "test_05(arg: int, /) -> int\n"
        "test_05(arg: float, /) -> int\n"
        "\n"
        "Overloaded function.\n"
        "\n"
        "1. ``test_05(arg: int, /) -> int``\n"
        "\n"
        "doc_1\n"
        "\n"
        "2. ``test_05(arg: float, /) -> int``\n"
        "\n"
        "doc_2")

    if sys.version_info < (3, 9):
        assert t.test_07.__doc__ == (
            "test_07(arg0: int, arg1: int, /, *args, **kwargs) -> Tuple[int, int]\n"
            "test_07(a: int, b: int, *myargs, **mykwargs) -> Tuple[int, int]")
    else:
        assert t.test_07.__doc__ == (
            "test_07(arg0: int, arg1: int, /, *args, **kwargs) -> tuple[int, int]\n"
            "test_07(a: int, b: int, *myargs, **mykwargs) -> tuple[int, int]")

def test06_signature_error():
    with pytest.raises(TypeError) as excinfo:
        t.test_05("x", y=4)
    assert str(excinfo.value) == (
        "test_05(): incompatible function arguments. The "
        "following argument types are supported:\n"
        "    1. test_05(arg: int, /) -> int\n"
        "    2. test_05(arg: float, /) -> int\n\n"
        "Invoked with types: str, kwargs = { y: int }")


def test07_raises():
    with pytest.raises(RuntimeError) as excinfo:
        t.test_06()
    assert str(excinfo.value) == "oops!"


def test08_args_kwargs():
    assert t.test_07(1, 2) == (0, 0)
    assert t.test_07(a=1, b=2) == (0, 0)
    assert t.test_07(a=1, b=2, c=3) == (0, 1)
    assert t.test_07(1, 2, 3, c=4) == (1, 1)
    assert t.test_07(1, 2, 3, 4, c=5, d=5) == (2, 2)


def test09_maketuple():
    assert t.test_tuple() == ("Hello", 123)
    with pytest.raises(RuntimeError) as excinfo:
        assert t.test_bad_tuple()
    value = str(excinfo.value)
    assert value == "std::bad_cast" or value == 'bad cast'


def test10_cpp_call_simple():
    result = []
    def my_callable(a, b):
        result.append((a, b))

    t.test_call_2(my_callable)
    assert result == [(1, 2)]

    with pytest.raises(TypeError) as excinfo:
        t.test_call_1(my_callable)
    assert "my_callable() missing 1 required positional argument: 'b'" in str(excinfo.value)
    assert result == [(1, 2)]


def test11_call_complex():
    result = []
    def my_callable(*args, **kwargs):
        result.append((args, kwargs))

    t.test_call_extra(my_callable)
    assert result == [
        ((1, 2), {"extra" : 5})
    ]

    result.clear()
    t.test_call_extra(my_callable, 5, 6, hello="world")
    assert result == [
      ((1, 2, 5, 6), {"extra" : 5, "hello": "world"})
    ]


def test12_list_tuple_manipulation():
    li = [1, 5, 6, 7]
    t.test_list(li)
    assert li == [1, 5, 123, 7, 19]

    tu = (1, 5, 6, 7)
    assert t.test_tuple(tu) == 19
    assert tu == (1, 5, 6, 7)


def test13_call_guard():
    assert t.call_guard_value() == 0
    assert t.test_call_guard() == 1
    assert t.call_guard_value() == 2
    assert t.test_call_guard_wrapper_rvalue_ref(1) == 1
    assert not t.test_release_gil()


def test14_print(capsys):
    t.test_print()
    captured = capsys.readouterr()
    assert captured.out == "Test 1\nTest 2\n"


def test15_iter():
    assert t.test_iter(()) == []
    assert t.test_iter((1,)) == [1]
    assert t.test_iter((1, 2)) == [1, 2]
    assert t.test_iter((1, 2, 3)) == [1, 2, 3]


def test16_iter_tuple():
    assert t.test_iter_tuple(()) == []
    assert t.test_iter_tuple((1,)) == [1]
    assert t.test_iter_tuple((1, 2)) == [1, 2]
    assert t.test_iter_tuple((1, 2, 3)) == [1, 2, 3]


def test17_iter_tuple():
    assert t.test_iter_list([]) == []
    assert t.test_iter_list([1]) == [1]
    assert t.test_iter_list([1, 2]) == [1, 2]
    assert t.test_iter_list([1, 2, 3]) == [1, 2, 3]


def test18_raw_doc():
    assert t.test_08.__doc__ == 'raw'


def test19_type_check_manual():
    assert t.test_09.__doc__ == 'test_09(arg: type, /) -> bool'

    assert t.test_09(bool) is True
    assert t.test_09(int) is False
    with pytest.raises(TypeError) as excinfo:
        assert t.test_09(True)
    assert "incompatible function arguments" in str(excinfo.value)


def test20_dict_iterator():
    assert t.test_10({}) == {}
    assert t.test_10({1:2}) == {1:2}
    assert t.test_10({1:2, 3:4}) == {1:2, 3:4}
    assert t.test_10({1:2, 3:4, 'a': 'b'}) == {1:2, 3:4, 'a':'b'}


def test21_numpy_overloads():
    try:
        import numpy as np
    except ImportError:
        pytest.skip('numpy is missing')

    assert t.test_05(np.int32(0)) == 1
    assert t.test_05(np.float64(0.1)) == 2
    assert t.test_05(np.float64(0.0)) == 2
    assert t.test_11_sl(np.int32(5)) == 5
    assert t.test_11_ul(np.int32(5)) == 5
    assert t.test_11_sll(np.int32(5)) == 5
    assert t.test_11_ull(np.int32(5)) == 5


def test22_string_return():
    assert t.test_12("hello") == "hello"
    assert t.test_13() == "test"
    assert t.test_14("abc") == "abc"


def test23_byte_return():
    assert t.test_15(b"abc") == "abc"
    assert t.test_16("hello") == b"hello"
    assert t.test_17(b"four") == 4
    assert t.test_17(b"\x00\x00\x00\x00") == 4
    assert t.test_18("hello world", 5) == b"hello"


@pytest.mark.skipif(sys.version_info < (3, 9), reason="requires python3.9 or higher")
def test24_pydoc():
    import pydoc
    assert "test_05(arg: int, /)" in pydoc.render_doc(t)


def test25_int():
    assert t.test_19(5) == 128
    assert t.test_20("5") == 128
    assert t.test_21(5) == 5
    assert t.test_19.__doc__ == "test_19(arg: int, /) -> object"


def test26_capsule():
    p = t.test_22()
    assert 'capsule' in str(p) and 'nb_handle' in str(p)
    assert t.test_24(p) == 1
    p = t.test_23()
    assert p is None
    assert t.test_24(p) == 0


def test27_slice():
    s = slice(1, 10, 2)
    assert t.test_25(s) is s
    assert t.test_25.__doc__ == "test_25(arg: slice, /) -> slice"
    assert t.test_26() == slice(4)
    assert t.test_27() == slice(1, 10)
    assert t.test_28() == slice(5, -5, -2)


def test28_ellipsis():
    assert t.test_29(...) is ...
    assert t.test_29.__doc__ == "test_29(arg: EllipsisType, /) -> EllipsisType"


def test29_traceback():
    result = t.test_30(fail_fn)
    regexp = r'Traceback \(most recent call last\):\n.*\n  File "[^"]*", line 8, in fail_fn\n.*RuntimeError: Foo'
    matches = re.findall(regexp, result, re.MULTILINE | re.DOTALL)
    assert len(matches) == 1

def test30_noexcept():
    assert t.test_31(123) == 123
    assert t.test_32(123) == 123

@pytest.mark.parametrize("func", [ t.identity_i8,  t.identity_u8,
                                   t.identity_i16, t.identity_u16,
                                   t.identity_i32, t.identity_u32,
                                   t.identity_i64, t.identity_u64 ])
def test31_range(func):
    values = [
        0, -1, 1, 2**7, 2**7-1, 2**8, 2**8-1, 2**15, 2**15-1, 2**16, 2**16-1,
        2**29, 2**29-1, 2**30, 2**30-1, 2**31, 2**31-1, 2**32, 2**32-1, 2**63,
        2**63-1, 2**64, 2**64-1, 2**127, 2**127-1, 2**128, 2**128-1
    ]
    values += [-value for value in values]
    suffix = func.__name__[9:]

    if suffix[0] == 'u':
        range_min = 0
        range_max = 2**int(suffix[1:]) - 1
    else:
        range_min = -2**(int(suffix[1:])-1)
        range_max = -range_min - 1

    for value in values:
        if value < range_min or value > range_max:
            with pytest.raises(TypeError):
                value_out = func(value)
        else:
            value_out = func(value)
            assert value_out == value

def test33_method_on_non_nanobind_class():
    class AClass:
        def __init__(self):
            self.x = 42
    AClass.simple_method = t.test_33
    AClass.complex_method = t.test_34
    a = AClass()
    assert a.simple_method(7) == 49
    assert a.complex_method(y=2) == 84


def test34_module_docstring():
    assert t.__doc__ == 'function testcase'

def test35_return_capture():
    x = t.test_35()
    assert x() == 'Test Foo'

def test36_test_char():
    assert t.test_cast_char('c') == 'c'
    with pytest.raises(TypeError):
        assert t.test_cast_char('abc')
    with pytest.raises(RuntimeError):
        assert t.test_cast_char(123)

def test37_test_str():
    assert t.test_cast_str('c') == 'c'
    assert t.test_cast_str('abc') == 'abc'
    with pytest.raises(RuntimeError):
        assert t.test_cast_str(123)
