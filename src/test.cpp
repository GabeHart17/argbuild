#include "tokenizer.hpp"
#include "parser.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  std::string expr = "abc&d |(e^ ~fg)";
  std::vector<Token> tokens = tokenize(expr);
  Parser parser("expr", tokens);
  // std::string expr2 = "a & b";
  // std::vector<Token> tokens2 = tokenize(expr2);
  // Parser parser2("expr2", tokens2);
  return 0;
}
