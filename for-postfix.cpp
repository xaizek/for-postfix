/*
 * for-postfix
 *
 * Copyright (C) 2014 xaizek.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <iostream>

#include <llvm/Support/CommandLine.h>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>

#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

using namespace clang;
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
                  const Expr *expr) const
    {
        FullSourceLoc fullLoc(expr->getLocStart(), *result.SourceManager);

        const std::string &fileName = result.SourceManager->getFilename(fullLoc);
        const unsigned int lineNum = fullLoc.getSpellingLineNumber();

        std::cout << fileName
                  << ":"
                  << lineNum
                  << ":"
                  << "dangerous use of postfix operator"
                  << '\n';
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

    return tool.run(newFrontendActionFactory(&finder).get());
}
