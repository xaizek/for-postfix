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
