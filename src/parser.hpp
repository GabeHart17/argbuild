#ifndef PARSER_HEADER
#define PARSER_HEADER
#include "tokenizer.hpp"
#include <string>
#include <vector>
#include <memory>

enum class OpType {
  o_or,
  o_xor,
  o_and,
  o_not,
  o_lookup,
  o_err,
  o_null
};

class ParseTreeNode {
private:
  OpType operator_;
  std::string text_;
  std::vector<std::shared_ptr<ParseTreeNode>> children_;

public:
  ParseTreeNode(OpType op, std::string text) : operator_(op), text_(text) {}
  ParseTreeNode(OpType op) : operator_(op), text_("") {}
  ParseTreeNode() : operator_(OpType::o_null), text_("") {}
  virtual ~ParseTreeNode() {}

  void add_child(std::shared_ptr<ParseTreeNode> child) {
    children_.push_back(child);
  }
  void add_child(ParseTreeNode& node) {
    children_.push_back(std::make_shared<ParseTreeNode>(node));
  }

  bool is_null() { return operator_ == OpType::o_null; }
  bool is_err() { return operator_ == OpType::o_err; }
  operator bool() { return is_null() || is_err(); }
};

class ExprParser {
private:
  std::vector<Token>::iterator next_;
  const std::vector<Token>& tokens_;
  std::string name_;
  ParseTreeNode tree_;

  bool consume_token(TokenType type) {
    if (next_->type == type) {
      next_++;
      return true;
    }
    return false;
  }

  ParseTreeNode E0() {
    ParseTreeNode fst = E1();
    ParseTreeNode snd;
    if (consume_token(TokenType::t_or)) {
      snd = E0();
    } else {
      return fst;
    }
    if (snd.is_null()) {
      return ParseTreeNode(OpType::o_err, "unknown parse error");
    }
    ParseTreeNode res(OpType::o_or);
    res.add_child(fst);
    res.add_child(snd);
    return res;
  }
  ParseTreeNode E1() {
    ParseTreeNode fst = E2();
    ParseTreeNode snd;
    if (consume_token(TokenType::t_xor)) {
      snd = E1();
    } else {
      return fst;
    }
    if (snd.is_null()) {
      return ParseTreeNode(OpType::o_err, "unknown parse error");
    }
    ParseTreeNode res(OpType::o_xor);
    res.add_child(fst);
    res.add_child(snd);
    return res;
  }
  ParseTreeNode E2() {
    ParseTreeNode fst = E3();
    ParseTreeNode snd;
    if (consume_token(TokenType::t_and)) {
      snd = E2();
    } else {
      return fst;
    }
    if (snd.is_null()) {
      return ParseTreeNode(OpType::o_err, "unknown parse error");
    }
    ParseTreeNode res(OpType::o_and);
    res.add_child(fst);
    res.add_child(snd);
    return res;
  }
  ParseTreeNode E3() {
    if (consume_token(TokenType::t_not)) {
      ParseTreeNode res(OpType::o_not);
      ParseTreeNode e4 = E4();
      res.add_child(e4);
      return res;
    }
    return E4();
  }
  ParseTreeNode E4() {
    if (consume_token(TokenType::t_parenl)) {
      ParseTreeNode res = E0();
      if (consume_token(TokenType::t_parenr)) {
        return res;
      } else {
        return ParseTreeNode(OpType::o_err, "did not find expected token ')'");
      }
    } else {
      std::string txt = next_->text;
      if (consume_token(TokenType::t_ident)) {
        return ParseTreeNode(OpType::o_lookup, txt);
      }
      return ParseTreeNode(OpType::o_err, "did not find expected identifier");
    }
  }

public:
  ExprParser(std::string name, std::vector<Token>& tokens) :
    name_(name), next_(tokens.begin()), tokens_(tokens) {
      tree_ = E0();
    }
  virtual ~ExprParser() {}
};


#endif
