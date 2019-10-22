#include "expr.h"
#include "functions.h"
void parse_define(std::string & line, std::map<std::string, Function *> & fmap) {
  if (!check_mismatch(line)) {
    std::cerr << "parenthes mismatch"
              << "\n";
    exit(0);
  }
  skipSpace(line);

  if (line.empty()) {
    std::cerr << "wrong command line" << std::endl;
    exit(0);
  }
  std::string temp;
  temp = takechar(line);  //should be ( now
  if (temp != "(") {
    std::cerr << "format part should have ( but have" << temp << "\n";
    exit(0);
  }
  skipSpace(line);
  std::string fname;
  fname = takechar(line);  //fname
  if (existFunc(fname, fmap)) {
    std::cerr << "already have function in list,redefined"
              << "\n";
    exit(0);
  }
  std::vector<Variable> vars_list;
  std::string token;
  while (token != ")") {
    skipSpace(line);
    token = takechar(line);
    if (token == ")") {
      break;
    }
    Variable v(token);
    vars_list.push_back(v);
  }
  temp.clear();
  skipSpace(line);

  temp = takechar(line);  //should be = now
  skipSpace(temp);
  if (temp != "=") {
    std::cerr << "should have =  in the middle but have" << temp << "\n";
    exit(0);
  }
  //now begin to parse rhs part;
  std::string defline;
  defline = line;
  //initialize function and set variable value to 0
  Function * newfunc = new Function(vars_list, defline);
  fmap.insert(std::pair<std::string, Function *>(fname, newfunc));
  //check unrecognized str in defline
  if (!valid_defline(fname, fmap)) {
    std::cerr << "unrecognized str in defline"
              << "\n";
    exit(0);
  }
  //check repeated paremeter in lhs
  if (checkrepeated(fname, fmap)) {
    std::cerr << "repeated vars in input"
              << "\n";
    exit(0);
  }

  //output
  std::cout << "defined " << fname;
  // print id
  std::cout << "(";
  for (size_t i = 0; i < vars_list.size() - 1; i++) {
    std::cout << vars_list[i].toString() << " ";
  }
  std::cout << vars_list[vars_list.size() - 1].toString() << ")"
            << "\n";
}
