set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(for-postfix for-postfix.cpp)
target_link_libraries(for-postfix clangTooling clangBasic clangASTMatchers)
