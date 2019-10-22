#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__
#include "expr.h"
bool existFunc(std::string fname, std::map<std::string, Function *> & fmap);
Function * findfunc(std::string fname, std::map<std::string, Function *> & fmap);
bool isValidOp(std::string op, std::map<std::string, Function *> & fmap);
bool isNum(std::string str);
void skipSpace(std::string & line);
std::string takechar(std::string & line);
bool check_mismatch(std::string input);
bool existvars(std::string token, std::string fname, std::map<std::string, Function *> & fmap);
size_t split_test(std::string line);
void freemap(std::map<std::string, Function *> & fmap);
bool valid_defline(std::string fname, std::map<std::string, Function *> & fmap);
bool checkrepeated(std::string fname, std::map<std::string, Function *> & fmap);
std::string varstostr(std::vector<double> vars);
bool valid_bound(std::vector<double> lower, std::vector<double> higher);
#endif
