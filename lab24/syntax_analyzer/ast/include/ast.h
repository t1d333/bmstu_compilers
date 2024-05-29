#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace ast {
  
  class Node {
    public:
      virtual ~Node() = default;
      virtual void print() {};
    
  };

  class Pattern : public Node {
    private:
      std::vector<std::unique_ptr<Pattern>> parts; 
    public:
      virtual ~Pattern() {};
      Pattern(): Node(), parts(std::vector<std::unique_ptr<ast::Pattern>>()) {}
      void add_part(std::unique_ptr<Pattern> node) {
        this->parts.push_back(std::move(node));
      }
      
      virtual void print() override {
        std::cout << "{\"type\": \"pattern\", \"parts\" :[";
       for (int i = 0; i < parts.size(); ++i ) {
         parts[i]->print();
          if (i != parts.size() - 1) {
            std::cout << ", ";
          }
        } 
        std::cout << "]}";
      }
      
      std::vector<std::unique_ptr<Pattern>>& get_parts() {
        return this->parts;
      }

      
      void append(std::unique_ptr<Pattern> other) {
        for (int i = 0; i < other->parts.size(); ++i) { 
          this->parts.push_back(std::move(other->parts[i]));
        }
      }
      
  };

  class Result : public Node {
    private:
      std::vector<std::unique_ptr<Result>> parts; 
    public:
      Result(): parts(std::vector<std::unique_ptr<ast::Result>>()), Node() {}
      void add_part(std::unique_ptr<Result> node) {
        this->parts.push_back(std::move(node));
      }
      virtual ~Result() {};
       
      virtual void print() override {
        std::cout << "{\"type\": \"result\", \"parts\" :[";
       for (int i = 0; i < parts.size(); ++i ) {
         parts[i]->print();
          if (i != parts.size() - 1) {
            std::cout << ", ";
          }
        } 
        std::cout << "]}";
      }
      
      std::vector<std::unique_ptr<Result>>& get_parts() {
        return this->parts;
      }
      
      void append(std::unique_ptr<Result> other) {
        for (int i = 0; i < other->parts.size(); ++i) { 
          this->parts.push_back(std::move(other->parts[i]));
        }
      }

  };
  
  class GroupedPattern : public Pattern {
    private:
      std::unique_ptr<ast::Pattern> pattern;
    public:
      void print() {
        std::cout << "{\"type\": \"grouped_pattern\", \"pattern\" :";
        pattern->print();
        std::cout << "}";
      } 
      GroupedPattern(std::unique_ptr<Pattern> pattern): pattern(std::move(pattern)) {}
  };

  class GroupedResult : public Result {
    private:
      std::unique_ptr<ast::Result> result;
    public:
      
      void print() {
        std::cout << "{\"type\": \"grouped_result\", \"result\" :";
        result->print();
        std::cout << "}";
      } 
      GroupedResult(std::unique_ptr<Result> result): result(std::move(result)) {}
  };

  
  class Var : public Result, public Pattern {
    std::string var;
    public:
      Var(std::string var): var(var) {}
    public:
      void print() override {
       std::cout << "{\"type\": \"var\", \"name\": \"" << var << "\"}";
      }
      
  };
  
  class Ident: public Result, public Pattern {
    std::string ident;
    public:
      Ident(std::string ident): ident(ident) {}
      void print() override {
       std::cout << "{\"type\": \"ident\", \"name\": \"" << ident << "\"}";
      }
  };

  class Integer: public Result, public Pattern {
    private:
      int value;
    public:
      Integer(int value): value(value) {}
      void print() override {
       std::cout << "{\"type\": \"int\", \"value\": " << value << "}";
      }
  };
  
  class String: public Result, public Pattern {
    std::string value;

    public:
      String(std::string value): value(value) {}
      void print() override {
       std::cout << "{\"type\": \"string\", \"value\": \"" << value << "\"}";
      }
  };

  class FuncCall : public Result {
    private:
      std::string ident;
      std::unique_ptr<Result> result;
    public:
      FuncCall(std::string ident, std::unique_ptr<Result> result): Result(), ident(ident), result(std::move(result)) {}
      void print() override {
       std::cout << "{\"type\": \"func_call\", \"ident\": \"" << ident << "\", \"result\": ";
       result->print();
       std::cout << "}";
      }
      
  };

  class Condition : public Node {
    private:
      std::unique_ptr<Pattern> pattern;
      std::unique_ptr<Result> result;
    public:
      Condition(std::unique_ptr<Pattern> pattern, std::unique_ptr<Result> result): pattern(std::move(pattern)), result(std::move(result)) {}
      void print() {
       std::cout << "{\"type\": \"condition\", \"pattern\": ";
       pattern->print();
       std::cout << ", \"result\": ";
       result->print();
       std::cout << "}";
      }
  };

  class SentenceLhs : public Node {
    private:
      std::unique_ptr<Pattern> pattern;
      std::vector<std::unique_ptr<Condition>> conditions;
    public:
      void set_pattern(std::unique_ptr<Pattern> pattern) {
        this->pattern = std::move(pattern);
      }

      void push_condition(std::unique_ptr<Condition> cond) {
        this->conditions.push_back(std::move(cond));
      }
      
      void set_conditions(std::vector<std::unique_ptr<Condition>> conditions) {
        this->conditions = std::move(conditions);
      }

      void print() {
        std::cout << "{\"type\": \"sentence_lhs\", \"pattern\": ";
        pattern->print();
        std::cout << ", \"conditions\": [";
        for (int i = 0; i < conditions.size(); ++i) {
          conditions[i]->print();
          if (i != conditions.size() - 1) {
            std::cout << ", ";
          }
        }
        std::cout << "] }";
      }
  };
  
  class SentenceRhs : public Node {
    public:
      void print() override {
      }
  }; 

  
 
  class Sentence : public Node {
    private: 
      std::unique_ptr<SentenceLhs> lhs;   
      std::unique_ptr<SentenceRhs> rhs;   
    public:
      Sentence() {}
      Sentence(std::unique_ptr<SentenceLhs> lhs, std::unique_ptr<SentenceRhs> rhs): lhs(std::move(lhs)), rhs(std::move(rhs)) {}
      void set_lhs(std::unique_ptr<SentenceLhs> lhs) {
        this->lhs = std::move(lhs);
      }
      
      void set_rhs(std::unique_ptr<SentenceRhs> rhs) {
        this->rhs = std::move(rhs);
      }

      void print() {
        std::cout << "{\"type\": \"sentence\", \"lhs\": ";
        lhs->print();
        std::cout << ", \"rhs\": ";
        rhs->print();
        std::cout << "}";
      }
  };
 
  class FuncBody : public Node {
    private:
      std::vector<std::unique_ptr<Sentence>> sentencies;
    public:
      void add_sentence(std::unique_ptr<Sentence> sentence ) {
        this->sentencies.push_back(std::move(sentence));
      }
      
      std::vector<std::unique_ptr<Sentence>>& get_sentencies() {
        return this->sentencies;
      }
      
      void append(std::unique_ptr<FuncBody> other) {
        for (int i = 0; i < other->sentencies.size(); ++i) { 
          this->sentencies.push_back(std::move(other->sentencies[i]));
        }
      }

      void print() {
        std::cout << "{\"type\": \"func_body\", \"sentencies\": [ ";
        for (int i = 0; i < sentencies.size(); ++i) {
          sentencies[i]->print();
          if (i != sentencies.size() - 1) {
            std::cout << ", ";
          }
        }
        std::cout << "] }";
      }
  };
   
  class SentenceRhsBlock : public SentenceRhs {
    private:
      std::unique_ptr<Result> result;
      std::unique_ptr<FuncBody> body;
    public:
      SentenceRhsBlock(std::unique_ptr<Result> result, std::unique_ptr<FuncBody> body): result(std::move(result)), body(std::move(body)) {}
      void print() {
        std::cout << "{\"type\": \"sentence_rhs_block\", \"result\": ";
        result->print();
        std::cout << ", \"body\": ";
        body->print();
        std::cout << "}";
      }
  };
  
  class SentenceRhsResult : public SentenceRhs {
    private:
      std::unique_ptr<Result> result;
    public:
      SentenceRhsResult(std::unique_ptr<Result> result): result(std::move(result)) {}
    void print() {
      std::cout << "{\"type\": \"sentence_rhs_result\", \"result\": ";
      result->print();
      std::cout << "}";
    }
  };
  
 
 class Func : public Node {
   std::string ident;
   std::unique_ptr<FuncBody> body;
   public: 
    void set_ident(std::string ident)  {
      this->ident = ident;
    }
    
    void set_body(std::unique_ptr<FuncBody> body)  {
      this->body = std::move(body);
    }

    std::string get_ident() {
      return this->ident;
    }
    void print() {
      std::cout << "{\"type\": \"func\", \"ident\": \"" << this->ident << "\" , \"body\": ";
      body->print();
      std::cout << "}";
    }
  };

  
  
  class Programm : public Node {
    std::vector<std::unique_ptr<Func>> funcs; 
    public:
      void push_func(std::unique_ptr<Func> func) {
        this->funcs.push_back(std::move(func));
      }
      
      void append(std::unique_ptr<Programm> other) {
        for (int i = 0; i < other->funcs.size(); ++i) { 
          this->funcs.push_back(std::move(other->funcs[i]));
        }
      }

      void print() {
        std::cout << "{\"functions\": " << std::endl;
        
        std::cout << "[" << std::endl;
        for (int i = 0; i < this->funcs.size(); ++i) {
          std::cout << "\t";
          funcs[i]->print();
          if (i != this->funcs.size() - 1) {
            std::cout << "," << std::endl;
          }
        }
        std::cout << "]}";
      }
      
  };
};
