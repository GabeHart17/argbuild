#include "tokenizer.hpp"
#include "parser.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  std::string expr = "abc&d |(e^ ~fg)";
  std::vector<Token> tokens = tokenize(expr);
  ExprParser parser("expr", tokens);

  std::string expr2 = "a & |b";
  std::vector<Token> tokens2 = tokenize(expr2);
  ExprParser parser2("expr2", tokens2);

  std::string expr3 = "a & b) | c";
  std::vector<Token> tokens3 = tokenize(expr3);
  ExprParser parser3("expr3", tokens3);

  std::string expr4 = "a & (b | c";
  std::vector<Token> tokens4 = tokenize(expr4);
  ExprParser parser4("expr4", tokens4);

  return 0;
}
