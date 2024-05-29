#include "analyzer.h"
#include "coords.h"
#include "domain.h"
#include "lexer.h"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ast.h"

Analyzer::~Analyzer() {}


// Pattern ::= ( int | var | ident | string | '(' Pattern ')' ) Pattern | .

std::unique_ptr<ast::Pattern> Analyzer::ParsePattern() {

  std::unique_ptr<ast::Pattern> node = std::make_unique<ast::Pattern>();
  
  
  switch (this->curr.get_kind()) {

  case DomainTag::INTEGER:
    node->add_part(std::make_unique<ast::Integer>(std::stoi(*this->attr)));
    break;
  case DomainTag::VAR:
    node->add_part(std::make_unique<ast::Var>(*this->attr));
    break;
  case DomainTag::IDENT:
    node->add_part(std::make_unique<ast::Ident>(*this->attr));
    break;
  case DomainTag::STRING:
    node->add_part(std::make_unique<ast::String>(*this->attr));
    break;
  case DomainTag::O_BRACKET:
    this->next_token();
    
      
    node->add_part(std::make_unique<ast::GroupedPattern>(this->ParsePattern()));
    
    if (this->curr.get_kind() != DomainTag::C_BRACKET) {
      this->print_err(DomainTag::C_BRACKET);
      throw "Syntax error";
    
    }
    
    break;
  default:
    return node;
  }
    this->next_token();
    node->append(this->ParsePattern());
    
    return node;
  
}


// Result ::= ( int | var | ident | string | '(' Result ')' ) Result | .

std::unique_ptr<ast::Result> Analyzer::ParseResult() {
  
  std::unique_ptr<ast::Result> node = std::make_unique<ast::Result>();
  
  switch (this->curr.get_kind()) {
  case DomainTag::O_ANGLE_BRACKET:
    this->next_token();

     
    if (this->curr.get_kind() != DomainTag::IDENT) {
      this->print_err(DomainTag::IDENT);
      throw "Syntax error";
    }
    {
      std::string ident = *this->attr;
      this->next_token();
      std::unique_ptr<ast::Result> result = this->ParseResult();
      node->add_part(std::make_unique<ast::FuncCall>(ident, std::move(result)));
    }
    
    if (this->curr.get_kind() != DomainTag::C_ANGLE_BRACKET) {
      this->print_err(DomainTag::C_ANGLE_BRACKET);
      throw "Syntax error";
    }

    break;
  case DomainTag::INTEGER:
    node->add_part(std::make_unique<ast::Integer>(std::stoi(*this->attr)));
    break;
  case DomainTag::VAR:
    node->add_part(std::make_unique<ast::Var>(*this->attr));
    break;
  case DomainTag::IDENT:
    node->add_part(std::make_unique<ast::Ident>(*this->attr));
    break;
  case DomainTag::STRING:
    node->add_part(std::make_unique<ast::String>(*this->attr));
    break;
  case DomainTag::O_BRACKET:
    this->next_token();
      
    node->add_part(std::make_unique<ast::GroupedResult>(this->ParseResult()));
    if (this->curr.get_kind() != DomainTag::C_BRACKET) {
      this->print_err(DomainTag::C_BRACKET);
      throw "Syntax error";
    }

    break;

  default:
    return node;
  }
  
  this->next_token();
  node->append(this->ParseResult());
  return node;
}

// BodyPartRhs ::= ',' Result ':' (Pattern BodyPartRhs | '{' FuncBody '}' ';') | '=' Result ';' . 

std::pair<std::unique_ptr<ast::SentenceRhs>, std::vector<std::unique_ptr<ast::Condition>>>  Analyzer::ParseBodyPartRhs() {
  
  std::unique_ptr<ast::SentenceRhs> node = std::make_unique<ast::SentenceRhs>();
  std::vector<std::unique_ptr<ast::Condition>> conditions;
  
  if (this->curr.get_kind() == DomainTag::COMMA) {
    this->next_token();
    std::unique_ptr<ast::Result> result = this->ParseResult(); 
    
    if (this->curr.get_kind() != DomainTag::COLON) {
      this->print_err(DomainTag::COLON);
      throw "Syntax error";
    }
  
    this->next_token();
    
    if (this->curr.get_kind() == DomainTag::O_CURLY_BRACKET) {
      this->next_token();
      node = std::make_unique<ast::SentenceRhsBlock>(std::move(result), this->ParseFuncBody());
      
      if (this->curr.get_kind() != DomainTag::C_CURLY_BRACKET) {
        this->print_err(DomainTag::C_CURLY_BRACKET);
        throw "Syntax error";
      }
      
      this->next_token();
      
      if (this->curr.get_kind() != DomainTag::SEMICOLON) {
        this->print_err(DomainTag::SEMICOLON);
        throw "Syntax error";
      }
      
      this->next_token();
    } else {
        conditions.push_back(std::make_unique<ast::Condition>( this->ParsePattern(), std::move(result)));
        auto pair = this->ParseBodyPartRhs();
        node = std::move(pair.first);
        for (int i = 0; i < pair.second.size(); ++i) {
          conditions.push_back(std::move(pair.second[i]));
        }
      }
  } else if (this->curr.get_kind() == DomainTag::EQUAL) {
    this->next_token();
    std::unique_ptr<ast::Result> result = this->ParseResult();
    node = std::make_unique<ast::SentenceRhsResult>(std::move(result));

    if (this->curr.get_kind() != DomainTag::SEMICOLON) {
      this->print_err(DomainTag::SEMICOLON);
      throw "Syntax error";
    }
    this->next_token();
  } else {
    std::cout << "ParseBodyPartRhs() ERROR! Current Token: " << this->curr << std::endl;
    throw "Syntax error";
  }

  
  return std::make_pair(std::move(node), std::move(conditions));
}


// FuncBody ::= Pattern BodyPartRhs FuncBody | .

std::unique_ptr<ast::FuncBody> Analyzer::ParseFuncBody() {
  std::unique_ptr<ast::FuncBody> node = std::make_unique<ast::FuncBody>();
  std::unique_ptr<ast::Sentence> sentence = std::make_unique<ast::Sentence>();
  std::unique_ptr<ast::SentenceLhs> lhs = std::make_unique<ast::SentenceLhs>();
  std::unique_ptr<ast::SentenceRhs> rhs = std::make_unique<ast::SentenceRhs>();

  bool findPattern = false;
  
  switch (this->curr.get_kind()) {
  case DomainTag::INTEGER:
    findPattern = true;
    break;
  case DomainTag::VAR:
    findPattern = true;
    break;
  case DomainTag::IDENT:
    findPattern = true;
    break;
  case DomainTag::STRING:
    findPattern = true;
    break;
  case DomainTag::O_BRACKET:
    findPattern = true;
    break;
  }

  
  if (findPattern) {
    lhs->set_pattern(this->ParsePattern());
  } else {
    lhs->set_pattern(std::make_unique<ast::Pattern>());
  }
  
  if (this->curr.get_kind() == DomainTag::COMMA || this->curr.get_kind() == DomainTag::EQUAL) {
      auto pair = this->ParseBodyPartRhs();
      rhs = std::move(pair.first);
      lhs->set_conditions(std::move(pair.second));
  } else if (findPattern) {
      this->print_err(DomainTag::EQUAL);
      throw "Syntax error";
  } else {
      return node;
  }
  
  
  node->add_sentence(std::make_unique<ast::Sentence>(std::move(lhs), std::move(rhs)));
  node->append(this->ParseFuncBody());
  return node;
}


// Func ::= ident '{' FuncBody '}' .

std::unique_ptr<ast::Func> Analyzer::ParseFunc() {
  
  std::unique_ptr<ast::Func> node = std::make_unique<ast::Func>();
  
  if (this->curr.get_kind() != DomainTag::IDENT) {
    this->print_err(DomainTag::IDENT);
    throw "Syntax error";
  }

  node->set_ident(*this->attr);
  
  this->next_token();
  
  
  if (this->curr.get_kind() != DomainTag::O_CURLY_BRACKET) {
    this->print_err(DomainTag::O_CURLY_BRACKET);
    throw "Syntax error";
  }
  

  this->next_token();
  
  std::unique_ptr<ast::FuncBody> body = this->ParseFuncBody();
  node->set_body(std::move(body));
  if (this->curr.get_kind() != DomainTag::C_CURLY_BRACKET) {
    this->print_err(DomainTag::C_CURLY_BRACKET);
    throw "Syntax error";
  }
  
  this->next_token();
  return node;
}

// Start ::= Func Start | .

std::unique_ptr<ast::Programm> Analyzer::ParseStart() {
  
  std::unique_ptr<ast::Programm> node = std::make_unique<ast::Programm>();
  
  if (this->curr.get_kind() == DomainTag::_EOF) {
    return node;
  }
  
  node->push_func(this->ParseFunc());
  node->append(this->ParseStart());

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


std::unique_ptr<ast::Programm> Analyzer::analyze() {
  return this->ParseStart();
}
