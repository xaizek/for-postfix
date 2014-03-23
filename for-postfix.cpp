#include <iostream>

#include <llvm/Support/CommandLine.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory toolCategory("for-postfix options");

static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

static StatementMatcher incMatcher =
    forStmt(                          // for ([init]; [condition]; [increment])
        hasIncrement(                 // "increment" part of for-loop
            unaryOperator(            // any unary op, e.g. *, &, --
                hasOperatorName("++") // exact unary op: ++
            ).bind("op")              // bind matched unary op to "op" name
        )
    );

class MatchHelper : public MatchFinder::MatchCallback
{
public:
    virtual void run(const MatchFinder::MatchResult &result)
    {
        using namespace clang;

        typedef UnaryOperator UnOp;

        if (const UnOp *op = result.Nodes.getNodeAs<UnOp>("op")) {
            if (op->isPostfix()) {
                op->dump();
                std::cout << '\n';
            }
        }
    }
};

int
main(int argc, const char *argv[])
{
    CommonOptionsParser optionsParser(argc, argv, toolCategory);
    ClangTool tool(optionsParser.getCompilations(),
                   optionsParser.getSourcePathList());

    MatchHelper helper;

    MatchFinder finder;
    finder.addMatcher(incMatcher, &helper);

    return tool.run(newFrontendActionFactory(&finder));
}
