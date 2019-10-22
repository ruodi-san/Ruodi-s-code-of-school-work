#include "parse.h"
//this module calculate value for valid test function
//return (double) result of function

double makeFunc(std::string op,
                std::vector<Expression *> & Id,
                std::map<std::string, Function *> & fmap) {
  //defined function name
  if (fmap[op]->getsize() != Id.size()) {
    std::cerr << "should have same size for assignment"
              << "\n";
    exit(0);
  }
  //set value for args in function
  std::vector<double> ids_val;
  for (size_t i = 0; i < fmap[op]->getsize(); i++) {
    ids_val.push_back(Id[i]->evaluate());
  }
  fmap[op]->setvars(ids_val);
  for (auto it = Id.begin(); it != Id.end(); ++it) {
    delete *it;
  }
  std::string defline = fmap[op]->getdefline();
  //parse function until it's broken down to basic op
  Expression * funcexpr = parse(defline, op, fmap);
  if (funcexpr == NULL) {
    freemap(fmap);
    exit(0);
  }
  double result = funcexpr->evaluate();
  delete funcexpr;
  return result;
}

//return to the numexpression or basic op expression
Expression * makeExpr(std::string op,
                      std::vector<Expression *> & Id,
                      std::map<std::string, Function *> & fmap) {
  //if op is function
  if (existFunc(op, fmap)) {
    double f_ans = makeFunc(op, Id, fmap);

    return new NumExpression(f_ans);
  }
  //return to operators, take two args
  if (Id.size() == 2) {
    if (op == "+") {
      return new PlusExpression(Id[0], Id[1]);
    }
    if (op == "-") {
      return new MinusExpression(Id[0], Id[1]);
    }
    if (op == "*") {
      return new TimesExpression(Id[0], Id[1]);
    }
    if (op == "/") {
      return new DivExpression(Id[0], Id[1]);
    }
    if (op == "%") {
      return new ModExpression(Id[0], Id[1]);
    }
    if (op == "pow") {
      return new Pow(Id[0], Id[1]);
    }
  }
  //return to operator_for_one_arg, take one arg
  if (Id.size() == 1) {
    if (op == "sin") {
      return new SinExpression(Id[0]);
    }
    if (op == "cos") {
      return new CosExpression(Id[0]);
    }
    if (op == "log") {
      return new Log(Id[0]);
    }
    if (op == "sqrt") {
      return new Sqrt(Id[0]);
    }
  }

  //unrecognizable char
  else
    std::cerr << "can not recognize such op" << op << "*"
              << "\n";
  return NULL;
}

Expression * parseOp(std::string & line,
                     std::string name,
                     std::map<std::string, Function *> & fmap) {
  if (fmap[name] == NULL) {
    std::cerr << "no function correspond to name"
              << "\n";
    return NULL;
  }
  //operator or fname
  std::string id;
  //ids represent args(num or variable) in (op args args...)
  std::vector<Expression *> ids;
  std::string temp;
  skipSpace(line);
  id = takechar(line);  //id should be a opearator or fname now
  if (!isValidOp(id, fmap)) {
    std::cerr << "op is not valid: " << id << "\n";
    return NULL;
  }
  skipSpace(line);
  while (line[0] != ')') {
    ids.push_back(parse(line, name, fmap));
    skipSpace(line);
    if (line[0] == ')') {
      line.erase(0, 1);
      return makeExpr(id, ids, fmap);
    }
  }
  std::cerr << "did not find )"
            << "\n";
  return NULL;
}
Expression * parse(std::string & line, std::string name, std::map<std::string, Function *> & fmap) {
  if (fmap[name] == NULL) {
    std::cerr << "no function correspond to name"
              << "\n";
    return NULL;
  }
  skipSpace(line);
  std::string token;
  //if this is empty
  if (line.empty()) {
    std::cerr << "defline empty"
              << "\n";
    return NULL;
  }
  token = takechar(line);
  //if it is the begin of function
  if (token == "(") {
    return parseOp(line, name, fmap);
  }
  //end of a function , jump out of recursion

  else {
    //num
    if (isNum(token)) {
      if (token.empty()) {
        return NULL;
      }
      return new NumExpression(stod(token));
    }

    //variable
    std::vector<std::string> var_list = fmap[name]->getvar_list();
    auto index = std::find(var_list.begin(), var_list.end(), token);
    if (index == var_list.end()) {
      //invalid token
      std::cerr << "no such variable in defined function"
                << "\n";
      return NULL;
    }
    //valid variable
    return new NumExpression(fmap[name]->returnvalue(token));
  }
}

double eval_test(std::string & line, std::map<std::string, Function *> & fmap) {
  skipSpace(line);
  std::string val;
  double result;
  std::vector<double> values;
  std::string fname = takechar(line);  //fname now
  //output for test format

  if (!isValidOp(fname, fmap)) {
    std::cerr << "variable in test line is not valid op"
              << "\n";
    exit(0);
  }
  skipSpace(line);
  while (line[0] != ')') {
    skipSpace(line);
    //if there is another function in test line
    if (line[0] == '(') {
      result = parse_test_helper(line, fmap);

      if (line[0] == ')') {
        values.push_back(result);
        line.erase(0, 1);
      }
      else {
        std::cerr << "wrong input in eval_test"
                  << "\n";
        exit(0);
      }
    }
    //if it is num
    else {
      try {
        val = takechar(line);
        values.push_back(stod(val));
      }
      catch (std::invalid_argument) {
        std::cerr << "val should be number in this case"
                  << "\n";
        exit(0);
      }
    }
    skipSpace(line);
  }

  if (values.size() != fmap[fname]->getsize()) {
    std::cerr << "different size, cannot test"
              << "\n";
    exit(0);
  }
  fmap[fname]->setvars(values);
  std::string defline = fmap[fname]->getdefline();
  Expression * funceval = parse(defline, fname, fmap);
  if (funceval == NULL) {
    freemap(fmap);
    exit(0);
  }
  double ans = funceval->evaluate();
  delete funceval;
  return ans;
}
double parse_test_helper(std::string & line, std::map<std::string, Function *> & fmap) {
  skipSpace(line);
  std::string temp;
  if (line.empty()) {
    std::cerr << "invalid test input, nothing to check"
              << "\n";
    exit(0);
  }
  temp = takechar(line);  //temp should be ( now

  if (temp != "(") {
    std::cerr << "should have ( in evaluate line"
              << "\n";
    exit(0);
  }
  return eval_test(line, fmap);
}
