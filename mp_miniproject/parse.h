#ifndef __PARSE_H__
#define __PARSE_H__
#include "expr.h"
#include "functions.h"
double makeFunc(std::string op,
                std::vector<Expression *> & Id,
                std::map<std::string, Function *> & fmap);
Expression * makeExpr(std::string op,
                      std::vector<Expression *> & Id,
                      std::map<std::string, Function *> & fmap);
Expression * parseOp(std::string & line,
                     std::string name,
                     std::map<std::string, Function *> & fmap);
Expression * parse(std::string & line, std::string name, std::map<std::string, Function *> & fmap);
double eval_test(std::string & line, std::map<std::string, Function *> & fmap);
double parse_test_helper(std::string & line, std::map<std::string, Function *> & fmap);

#endif
