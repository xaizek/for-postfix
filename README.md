Brief description
-----------------

Clang-based standalone tool that detects for-loops with postfix operators.

Example
-------

Input:

```c++
#include <cstdlib>

#include <vector>

inline void
doNothing()
{
    // do nothing on purpose
}

int
main(void)
{
    std::vector<int> v;
    typedef std::vector<int> intVector;

    for (intVector::const_iterator cit = v.begin(); cit != v.end(); cit++) {
        doNothing();
    }

    for (intVector::const_iterator cit = v.begin(); cit != v.end(); ++cit) {
        doNothing();
    }

    for (intVector::const_iterator cit = v.begin(); cit != v.end(); cit--) {
        doNothing();
    }

    for (intVector::const_iterator cit = v.begin(); cit != v.end(); --cit) {
        doNothing();
    }

    return EXIT_SUCCESS;
}
```

Run command:

```sh
bin/for-postfix vector-all.cpp --
```

Output (paths are truncated):

```
.../vector-all.cpp:17:dangerous use of postfix operator
.../vector-all.cpp:25:dangerous use of postfix operator
```

Building
--------

1. Clone the repository to `llvm/tools/clang/tools/extra`.
2. Add `add_subdirectory(for-postfix)` to `CMakeLists.txt` in the same 
   directory.
3. Run `ninja for-postfix` inside your LLVM's `build/` directory.
