#include <cstdlib>

inline void
doNothing()
{
    // do nothing on purpose
}

int
main(void)
{
    for (int i = 0; i < 10; i--) {
        doNothing();
    }

    for (int i = 0; i < 10; --i) {
        doNothing();
    }

    return EXIT_SUCCESS;
}
