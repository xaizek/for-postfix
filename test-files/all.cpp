#include <cstdlib>

#include <vector>

inline void
doNothing()
{
    // do nothing on purpose
}

int
main()
{
    for (int i = 0; i < 5; i--) {
        doNothing();
    }

    for (int i = 0; i < 5; --i) {
        doNothing();
    }

    for (int i = 0; i < 5; i++) {
        doNothing();
    }

    for (int i = 0; i < 5; ++i) {
        doNothing();
    }

    std::vector<int> v;
    typedef std::vector<int> IntVector;

    for (IntVector::const_iterator cit = v.begin(); cit != v.end(); cit++) {
        doNothing();
    }

    for (IntVector::const_iterator cit = v.begin(); cit != v.end(); ++cit) {
        doNothing();
    }

    for (IntVector::const_iterator cit = v.begin(); cit != v.end(); cit--) {
        doNothing();
    }

    for (IntVector::const_iterator cit = v.begin(); cit != v.end(); --cit) {
        doNothing();
    }

    return EXIT_SUCCESS;
}
