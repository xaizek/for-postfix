#include <cstdlib>

#include <llvm/Support/CommandLine.h>

#include <clang/Tooling/CommonOptionsParser.h>

using namespace clang::tooling;

static llvm::cl::OptionCategory toolCategory("for-postfix options");

int
main(int argc, const char *argv[])
{
    CommonOptionsParser optionsParser(argc, argv, toolCategory);

    return EXIT_SUCCESS;
}
