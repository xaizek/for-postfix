#include <llvm/Support/CommandLine.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory toolCategory("for-postfix options");

int
main(int argc, const char *argv[])
{
    CommonOptionsParser optionsParser(argc, argv, toolCategory);
    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    MatchFinder finder;

    return tool.run(newFrontendActionFactory(&finder));
}
