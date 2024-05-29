#include "analyzer.h"
#include "coords.h"
#include "domain.h"
#include "inner.h"
#include "leaf.h"
#include "lexer.h"
#include "symbol.h"
#include <iostream>
#include <memory>

Analyzer::~Analyzer() {}

std::shared_ptr<Node> Analyzer::ParsePattern() {

  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::PATTERN);
  
  switch (this->curr.get_kind()) {

  case DomainTag::INTEGER:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
  case DomainTag::VAR:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->print();
    node->add_child(this->ParsePattern());
    return node;
  case DomainTag::IDENT:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParsePattern());
    return node;
  case DomainTag::STRING:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParsePattern());
    return node;
  case DomainTag::O_BRACKET:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParsePattern());
    if (this->curr.get_kind() != DomainTag::C_BRACKET) {
      this->print_err(DomainTag::C_BRACKET);
      throw "Syntax error";
    }
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParsePattern());
    return node;
  default:
    return node;
  }
  
}

std::shared_ptr<Node> Analyzer::ParseResult() {
  
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::RESULT);
  
  switch (this->curr.get_kind()) {
  case DomainTag::INTEGER:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  case DomainTag::VAR:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  case DomainTag::IDENT:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  case DomainTag::STRING:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  case DomainTag::O_BRACKET:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    if (this->curr.get_kind() != DomainTag::C_BRACKET) {
      this->print_err(DomainTag::C_BRACKET);
      throw "Syntax error";
    }
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  case DomainTag::O_ANGLE_BRACKET:
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    if (this->curr.get_kind() != DomainTag::IDENT) {
      this->print_err(DomainTag::IDENT);
      throw "Syntax error";
    }
    
    node->add_child(std::make_shared<Leaf>(curr));
    this->next_token();
    node->add_child(this->ParseResult());

    if (this->curr.get_kind() != DomainTag::C_ANGLE_BRACKET) {
      this->print_err(DomainTag::C_CURLY_BRACKET);
      throw "Syntax error";
    }
    
    node->add_child(std::make_shared<Leaf>(curr));
    
    this->next_token();
    node->add_child(this->ParseResult());
    return node;
  default:
    return node;
  }
}

std::shared_ptr<Node> Analyzer::ParseBodyPartEnd() {
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::BODY_PART_END);
  
  if (this->curr.get_kind() == DomainTag::O_CURLY_BRACKET) {
    node->add_child(std::make_shared<Leaf>(this->curr));
    
    this->next_token();
    node->add_child(this->ParseFuncBody());

    if (this->curr.get_kind() != DomainTag::C_CURLY_BRACKET) {
      this->print_err(DomainTag::C_CURLY_BRACKET);
      throw "Syntax error";
    }
    node->add_child(std::make_shared<Leaf>(this->curr));
    
    this->next_token();
    
    if (this->curr.get_kind() != DomainTag::SEMICOLON) {
      this->print_err(DomainTag::SEMICOLON);
      throw "Syntax error";
    }
    
    node->add_child(std::make_shared<Leaf>(this->curr));
    this->next_token();
    
  } else {
    node->add_child(this->ParsePattern());
    node->add_child(this->ParseBodyPartRhs());
  }

  return node;
}
std::shared_ptr<Node> Analyzer::ParseBodyPartRhs() {
  
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::BODY_PART_RHS);
  
  if (this->curr.get_kind() == DomainTag::COMMA) {
    node->add_child(std::make_shared<Leaf>(this->curr));
    this->next_token();
    node->add_child(this->ParseResult());
    
    if (this->curr.get_kind() != DomainTag::COLON) {
      this->print_err(DomainTag::COLON);
      throw "Syntax error";
    }
  
    node->add_child(std::make_shared<Leaf>(this->curr));
    this->next_token();
    
    node->add_child(this->ParseBodyPartEnd());
  } else if (this->curr.get_kind() == DomainTag::EQUAL) {
    node->add_child(std::make_shared<Leaf>(this->curr));
    this->next_token();
    node->add_child(this->ParseResult());
    
    if (this->curr.get_kind() != DomainTag::SEMICOLON) {
      this->print_err(DomainTag::SEMICOLON);
      throw "Syntax error";
    }
    
    node->add_child(std::make_shared<Leaf>(this->curr));
    this->next_token();
  } else {
    std::cout << "ParseBodyPartRhs() ERROR! Current Token: " << this->curr << std::endl;
    throw "Syntax error";
  }

  return node;
}

std::shared_ptr<Node> Analyzer::ParseFuncBody() {
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::FUNC_BODY);

  switch (this->curr.get_kind()) {
  case DomainTag::INTEGER:
    node->add_child(this->ParsePattern());
    break;
  case DomainTag::VAR:
    node->add_child(this->ParsePattern());
    break;
  case DomainTag::IDENT:
    node->add_child(this->ParsePattern());
    break;
  case DomainTag::STRING:
    node->add_child(this->ParsePattern());
    break;
  case DomainTag::O_BRACKET:
    node->add_child(this->ParsePattern());
  }

  
  switch (this->curr.get_kind()) {
    case DomainTag::COMMA:
      node->add_child(this->ParseBodyPartRhs());
      break;
    case DomainTag::EQUAL:
      node->add_child(this->ParseBodyPartRhs());
      break;
    default:
      return node;
  }
  
  node->add_child(this->ParseFuncBody());
  return node;
}

std::shared_ptr<Node>  Analyzer::ParseFunc() {
  
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::FUNC);
  
  if (this->curr.get_kind() != DomainTag::IDENT) {
    this->print_err(DomainTag::IDENT);
    throw "Syntax error";
  }
  
  node->add_child(std::make_shared<Leaf>(this->curr));
  this->next_token();
  
  if (this->curr.get_kind() != DomainTag::O_CURLY_BRACKET) {
    this->print_err(DomainTag::O_CURLY_BRACKET);
    throw "Syntax error";
  }
  
  node->add_child(std::make_shared<Leaf>(this->curr));
  this->next_token();

  node->add_child(this->ParseFuncBody());
  
  if (this->curr.get_kind() != DomainTag::C_CURLY_BRACKET) {
    this->print_err(DomainTag::C_CURLY_BRACKET);
    throw "Syntax error";
  }
  
  node->add_child(std::make_shared<Leaf>(this->curr));
  this->next_token();
  return node;
}
std::shared_ptr<Node> Analyzer::ParseStart() {
  
  std::shared_ptr<Node> node = std::make_shared<Inner>(Symbol::START);
  
  if (this->curr.get_kind() == DomainTag::_EOF) {
    std::make_shared<Leaf>(this->curr)->print();
    node->add_child(std::make_shared<Leaf>(this->curr));
    return node;
  }
  
  node->add_child(this->ParseFunc());
  node->add_child(this->ParseStart());

  return node;
}
Analyzer::Analyzer()
    : lexer(std::make_unique<Lexer>(&std::cin)),
      coords(new Coords(0, 0, 0, 0)),
      attr(new std::string),
      curr(Token(this->lexer->next_token(this->coords, this->attr), *this->coords, *this->attr)) {}

Analyzer::Analyzer(std::istream *in)
    : lexer(std::make_unique<Lexer>(in)),
      coords(new Coords(0, 0, 0, 0)),
      attr(new std::string),
      curr(DomainTag::_EOF, *this->coords, *this->attr) {
      this->next_token();
      
    }


std::shared_ptr<Node> Analyzer::analyze() {
  return this->ParseStart();
}
