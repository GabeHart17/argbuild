#ifndef EXPRESSION_HEADER
#define EXPRESSION_HEADER
#include <string>
#include <vector>
#include <regex>

enum class TokenType {
  t_not,
  t_and,
  t_or,
  t_xor,
  t_parenl,
  t_parenr,
  t_ident
};

struct Token {
public:
  TokenType type;
  std::string text;
  Token(TokenType ty, std::string tx) : type(ty), text(tx) {}
};

std::vector<Token> tokenize(std::string expr_text) {
  std::vector<Token> result;
  std::string next_text;
  for (char c : expr_text + " ") {
    if (std::regex_match(std::string(1, c), std::regex("\\s"))) {
      if (next_text.length() > 0) {
        result.push_back(Token(TokenType::t_ident, next_text));
        next_text = std::string();
      }
      continue;
    }
    TokenType ttype;
    switch (c) {
      case '|':
      ttype = TokenType::t_or;
      break;
      case '^':
      ttype = TokenType::t_xor;
      break;
      case '&':
      ttype = TokenType::t_and;
      break;
      case '~':
      ttype = TokenType::t_not;
      break;
      case '(':
      ttype = TokenType::t_parenl;
      break;
      case ')':
      ttype = TokenType::t_parenr;
      break;
      default:
      ttype = TokenType::t_ident;
    }
    if (ttype != TokenType::t_ident){
      if (next_text.length() > 0) {
        result.push_back(Token(TokenType::t_ident, next_text));
        next_text = std::string();
      }
      result.push_back(Token(ttype, std::string(1,c)));
    } else {
      next_text += c;
    }
  }
  return result;
}

#endif
