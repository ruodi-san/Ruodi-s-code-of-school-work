#include "functions.h"
//some helper functions declared here
bool existFunc(std::string fname, std::map<std::string, Function *> & fmap) {
  std::map<std::string, Function *>::iterator it;
  for (it = fmap.begin(); it != fmap.end(); ++it) {
    if (fname == it->first) {
      return true;
    }
  }
  return false;
}

Function * findfunc(std::string fname, std::map<std::string, Function *> & fmap) {
  std::map<std::string, Function *>::iterator it;
  for (it = fmap.begin(); it != fmap.end(); ++it) {
    if (fname == it->first) {
      return it->second;
    }
  }
  std::cerr << "no such function in fmap list"
            << "\n";
  exit(0);
}

bool isValidOp(std::string op, std::map<std::string, Function *> & fmap) {
  size_t find;
  std::string valid_op = "+-*/%,pow,sin,cos,log,sqrt";
  find = valid_op.find(op);
  if (find != std::string::npos) {
    return true;
  }
  if (existFunc(op, fmap)) {
    return true;
  }
  return false;
}

bool isNum(std::string str) {
  std::stringstream sin(str);
  double d;
  char c;
  if (!(sin >> d))
    return false;
  if (sin >> c)
    return false;
  return true;
}
void skipSpace(std::string & line) {
  if (line.empty()) {
    return;
  }

  line.erase(0, line.find_first_not_of(" "));
  line.erase(line.find_last_not_of(" ") + 1);
}
//this function read one valid char from line and then erase that char from line
std::string takechar(std::string & line) {
  skipSpace(line);
  std::string str;
  size_t i = 0;
  if (line[0] == '(' || line[0] == ')' || line[0] == ' ' || line[0] == '=') {
    str = line.substr(0, 1);
    line.erase(0, 1);
    return str;
  }
  while (i < line.size()) {
    if (line[i] == '(' || line[i] == ')' || line[i] == ' ' || line[i] == '=') {
      str = line.substr(0, i);
      break;
    }
    i++;
  }
  skipSpace(str);
  line.erase(0, i);
  skipSpace(line);
  return str;
}
bool check_mismatch(std::string input) {
  int lnum = std::count(input.begin(), input.end(), '(');
  int rnum = std::count(input.begin(), input.end(), ')');
  if (lnum == rnum) {
    return true;
  }
  else {
    return false;
  }
}

bool existvars(std::string token, std::string fname, std::map<std::string, Function *> & fmap) {
  std::vector<std::string> var_list = fmap[fname]->getvar_list();
  auto index = std::find(var_list.begin(), var_list.end(), token);
  if (index != var_list.end()) {
    return true;
  }
  return false;
}
size_t split_test(std::string line) {
  size_t len = line.size();
  size_t count = 0;
  int count_l = 0;
  int count_r = 0;
  //count '(' and ')' to seperate two test functions
  for (size_t i = 0; i < len; i++) {
    if (line[i] == '(') {
      count_l++;
    }
    if (line[i] == ')') {
      count_r++;
    }
    count++;
    if (count_l == count_r) {
      break;
    }
  }
  return count;
}
void freemap(std::map<std::string, Function *> & fmap) {
  for (std::map<std::string, Function *>::iterator it = fmap.begin(); it != fmap.end(); ++it) {
    delete it->second;
  }
}
bool valid_defline(std::string fname, std::map<std::string, Function *> & fmap) {
  std::string line = fmap[fname]->getdefline();
  std::string str;
  while (!line.empty()) {
    std::string str = takechar(line);
    if (!existvars(str, fname, fmap) && !isValidOp(str, fmap) && (str != " ") && (str != "(") &&
        (str != ")") && !isNum(str))
      return false;
  }

  return true;
}
bool checkrepeated(std::string fname, std::map<std::string, Function *> & fmap) {
  std::vector<std::string> vars = fmap[fname]->getvar_list();
  for (size_t i = 0; i < vars.size() - 1; i++) {
    for (size_t j = i + 1; j < vars.size(); j++) {
      if (vars[i] == vars[j]) {
        return true;
      }
    }
  }
  return false;
}
std::string varstostr(std::vector<double> vars) {
  std::string str;
  for (size_t i = 0; i < vars.size() - 1; i++) {
    str = str + std::to_string(vars[i]) + " ";
  }
  str = str + std::to_string(vars[vars.size() - 1]);
  return str;
}
bool valid_bound(std::vector<double> lower, std::vector<double> higher) {
  if (lower.size() != higher.size()) {
    std::cerr << "two vectors have different szie"
              << "\n";
    return false;
  }
  for (size_t i = 0; i < lower.size(); i++) {
    if (higher[i] < lower[i]) {
      return false;
    }
  }
  return true;
}
