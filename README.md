Overview
---
This is a single-header implementation of optional references in C++ as a safe, zero-overhead wrapper over raw pointers.

Non-owning optional references to objects in C++ are traditionally returned in the form of raw pointers. In general, the use of raw pointers in C++ has been steadily beaten back to being almost nonexistant outside of C interop as they are inherently unsafe, but the standard does not currently provide a good way of avoiding raw "observer" pointers, so I wrote a class for it based on the interface of `std::optional`.

Example
---
```
int value = 42;
dl::optional_reference<int> opt1 = value;
auto opt2 = dl::opt_cref(value) // deduced to dl::optional_reference<const int>;
std::cout << opt1.ref(); // 42
std::cout << *opt2; // 42

dl::optional_reference<int> empty = dl::nullref;
std::cout << empty.ref() // bad_optional_reference_access exception
std::cout << *empty // Unchecked null dereferencing fun
```
Note
---
The class as provided here requires C++14 for the relaxed `constexpr` expressions for optional_reference::ref, but should work on earlier versions if that is removed.

License
---
Provided under the MIT license.
