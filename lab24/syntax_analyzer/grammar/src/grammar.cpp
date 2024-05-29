#include "symbol.h"

std::string symbol_to_string(Symbol &s) {
  switch (s) {
    case Symbol::START:
      return "start";
    case Symbol::FUNC:
      return "func";
    case Symbol::FUNC_BODY:
      return "func_body";
    case Symbol::BODY_PART_RHS:
      return "body_part_rhs";
    case Symbol::BODY_PART_END:
      return "body_part_end";
    case Symbol::RESULT:
      return "result";
    case Symbol::RESULT_PART:
      return "result_part";
    case Symbol::PATTERN:
      return "pattern";
    case Symbol::PATTER_PART:
      return "pattern_part";
    default:
      return "undefined symbol";
  }
}
