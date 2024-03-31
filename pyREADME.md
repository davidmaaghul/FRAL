### Fast Random Access Log (FRAL)
FRAL is a concurrency-friendly log structure allocated over shared memory. Reads are random access and writes
are non-blocking. The current shared-memory framework uses memory-mapped files. The core engine is written in C++ and
Python bindings are provided for higher level and less performance-constrained usage.

### Example Usage
```Python
# Appender Process
ral_A = FRAL("test.bin")
test_bytes = "TEST".encode()

test_blob = ral_A.allocate(len(test_bytes))
test_blob[:len(test_bytes)] = test_bytes
print(bytes(test_blob).decode())
ral_A.append(test_blob)
```
```Python
# Reader Process
ral_B = FRAL("test.bin")

while True:
    test_blob2 = ral_B[0]
    if test_blob2:
        break
        
print(bytes(test_blob2).decode())
test_blob2[0:1] = 'D'.encode()

test_blob3 = ral_B[0]
print(bytes(test_blob3).decode())
```

### More Info:
See the project repo