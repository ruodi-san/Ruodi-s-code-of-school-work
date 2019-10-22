#ifndef __EXPR_H___
#define __EXPR_H___
#include <string.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Expression
{
 public:
  virtual ~Expression() {}
  virtual double evaluate() const = 0;
};

class NumExpression : public Expression
{
 private:
  double num;

 public:
  NumExpression(double input) : num(input) {}

  virtual double evaluate() const { return num; }
  virtual ~NumExpression() {}
};

class Variable : public Expression
{
 private:
  std::string vname;
  double value;

 public:
  Variable(std::string name) : vname(name), value(0) {}
  Variable(std::string name, double v) : vname(name), value(v) {}
  std::string toString() const { return vname; }
  virtual double evaluate() const { return value; }
  void setvalue(double _value) { value = _value; }
  virtual ~Variable() {}
};

//operator for two args
class operators : public Expression
{
 protected:
  Expression * lhs;
  Expression * rhs;
  double lvalue;
  double rvalue;

 public:
  operators(Expression * lhs, Expression * rhs) :
      lhs(lhs),
      rhs(rhs),
      lvalue(lhs->evaluate()),
      rvalue(rhs->evaluate()) {}
  virtual double evaluate() const = 0;

  virtual ~operators() {
    delete lhs;
    delete rhs;
  }
};

class PlusExpression : public operators
{
 public:
  PlusExpression(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return lhs->evaluate() + rhs->evaluate(); }
  virtual ~PlusExpression() {}
};
class MinusExpression : public operators
{
 public:
  MinusExpression(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return lhs->evaluate() - rhs->evaluate(); }
  virtual ~MinusExpression() {}
};

class TimesExpression : public operators
{
 public:
  TimesExpression(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return lhs->evaluate() * rhs->evaluate(); }
  virtual ~TimesExpression() {}
};
class DivExpression : public operators
{
 public:
  DivExpression(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return lhs->evaluate() / rhs->evaluate(); }
  virtual ~DivExpression() {}
};
class ModExpression : public operators
{
 public:
  ModExpression(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return fmod(lhs->evaluate(), rhs->evaluate()); }
  virtual ~ModExpression() {}
};
class Pow : public operators
{
 public:
  Pow(Expression * lhs, Expression * rhs) : operators(lhs, rhs) {}
  virtual double evaluate() const { return pow(lhs->evaluate(), rhs->evaluate()); }
  virtual ~Pow() {}
};
//operator for one arg
class operator_for_onearg : public Expression
{
 protected:
  Expression * lhs;
  double lvalue;

 public:
  operator_for_onearg(Expression * lhs) : lhs(lhs), lvalue(lhs->evaluate()) {}
  virtual double evaluate() const = 0;

  virtual ~operator_for_onearg() { delete lhs; }
};
class SinExpression : public operator_for_onearg
{
 public:
  SinExpression(Expression * lhs) : operator_for_onearg(lhs) {}
  virtual double evaluate() const { return sin(lhs->evaluate()); }
  virtual ~SinExpression() {}
};
class CosExpression : public operator_for_onearg
{
 public:
  CosExpression(Expression * lhs) : operator_for_onearg(lhs) {}
  virtual double evaluate() const { return cos(lhs->evaluate()); }

  virtual ~CosExpression() {}
};
class Sqrt : public operator_for_onearg
{
 public:
  Sqrt(Expression * lhs) : operator_for_onearg(lhs) {}
  virtual double evaluate() const { return sqrt(lhs->evaluate()); }

  virtual ~Sqrt() {}
};
class Log : public operator_for_onearg
{
 public:
  Log(Expression * lhs) : operator_for_onearg(lhs) {}
  virtual double evaluate() const { return log(lhs->evaluate()); }

  virtual ~Log() {}
};

//function module

class Function
{
 private:
  std::vector<Variable> vars;
  std::string defline;

 public:
  Function() {}
  Function(std::vector<Variable> & _vars, std::string & _defline) :
      vars(_vars),
      defline(_defline) {}
  void printvalue() {
    std::cout << "value: ";
    for (size_t i = 0; i < vars.size(); i++) {
      std::cout << vars[i].toString() << " ";
    }
    std::cout << "\n";
  }

  std::string & getdefline() { return this->defline; }
  std::string getvname(int i) { return vars[i].toString(); }
  std::vector<std::string> getvar_list() {
    std::vector<Variable>::const_iterator it = vars.begin();
    std::vector<std::string> var_list;
    while (it != vars.end()) {
      var_list.push_back(it->toString());
      ++it;
    }
    return var_list;
  }

  int findvnamePos(std::string vname) {
    if (!checkexist(vname)) {
      std::cerr << "vname didn't exist in the var_list"
                << "\n";
    }
    std::vector<std::string> var_list = this->getvar_list();

    std::vector<std::string>::iterator vElement =
        std::find(var_list.begin(), var_list.end(), vname);
    int vPos = -1;
    if (vElement != var_list.end()) {
      vPos = distance(var_list.begin(), vElement);
    }
    return vPos;
  }

  void setvars(std::vector<double> values) {
    std::vector<double>::iterator it;
    size_t i = 0;
    for (it = values.begin(); it != values.end(); ++it) {
      vars[i].setvalue(*it);
      i++;
    }
  }

  bool checkexist(std::string token) {
    std::vector<std::string> var_list = this->getvar_list();

    auto index = std::find(var_list.begin(), var_list.end(), token);
    if (index != var_list.end()) {
      return true;
    }
    return false;
  }
  double returnvalue(std::string vname) {
    if (!checkexist(vname)) {
      std::cerr << "didn't have vname in the val_list, return none"
                << "\n";
    }
    int vpos = findvnamePos(vname);
    return vars[vpos].evaluate();
  }
  size_t getsize() { return vars.size(); }
};

#endif
