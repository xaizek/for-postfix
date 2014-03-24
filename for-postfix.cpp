#include <iostream>

#include <llvm/Support/CommandLine.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::OptionCategory toolCategory("for-postfix options");

static llvm::cl::extrahelp commonHelp(CommonOptionsParser::HelpMessage);

static StatementMatcher builtinMatcher =
    forStmt(                          // for ([init]; [condition]; [increment])
        hasIncrement(                 // "increment" part of for-loop
            unaryOperator(            // any unary op, e.g. *, &, --

            ).bind("op")              // bind matched unary op to "op" name
        )
    );

static StatementMatcher opMatcher =
    forStmt(                          // for ([init]; [condition]; [increment])
        hasIncrement(                 // "increment" part of for-loop
            operatorCallExpr(         // call of overloaded operator
                argumentCountIs(2)    // that requires two arguments
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
        typedef CXXOperatorCallExpr Call;

        if (const UnOp *op = result.Nodes.getNodeAs<UnOp>("op")) {
            if (op->isIncrementDecrementOp() && op->isPostfix()) {
                printOut(result, op);
            }
        } else if (const Call *op = result.Nodes.getNodeAs<Call>("op")) {
            const OverloadedOperatorKind opKind = op->getOperator();

            if (opKind == OO_PlusPlus || opKind == OO_MinusMinus) {
                printOut(result, op);
            }
        }
    }

private:
    void printOut(const MatchFinder::MatchResult &result,
                  const clang::Expr *expr) const
    {
        expr->dump();
        std::cout << '\n';
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
    finder.addMatcher(builtinMatcher, &helper);
    finder.addMatcher(opMatcher, &helper);

    return tool.run(newFrontendActionFactory(&finder));
}
