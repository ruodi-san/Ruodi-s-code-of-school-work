#include "parse.h"
//parse test command
void parse_test(std::string & line, std::map<std::string, Function *> & fmap) {
  if (line.empty()) {
    std::cerr << "line empty"
              << "\n";
    return;
  }
  if (!check_mismatch(line)) {
    std::cerr << "parenthes mismatch"
              << "\n";
    return;
  }

  skipSpace(line);
  //1. if format: test num num
  if (line[0] != '(') {
    std::stringstream ss;
    std::string s;
    ss << line;
    ss >> s;
    try {
      double s1 = stod(s);
      s.clear();
      ss >> s;
      double s2 = stod(s);
      if (ss.rdbuf()->in_avail() != 0) {
        std::cerr << "too many args for test parse"
                  << "\n";
        return;
      }
      if (s1 == s2) {
        std::cout << s1 << " = " << s2 << " [correct] "
                  << "\n";
      }
      else
        std::cout << s1 << " = " << s2 << " [INCORRECT: expected " << s1 << "]\n";
    }
    catch (std::invalid_argument) {
      std::cerr << "test num num should be numbers"
                << "\n";
      return;
    }

    if (ss.rdbuf()->in_avail() != 0) {
      std::cerr << "too many args for test parse"
                << "\n";
      return;
    }
  }
  else {  // if format:(f args (g args args))  rans
    std::string totest;
    std::string ex_ans;
    size_t count = split_test(line);
    totest = line.substr(0, count);
    std::cout << totest;
    double lans = parse_test_helper(totest, fmap);
    ex_ans = line.substr(count + 1);

    skipSpace(ex_ans);
    double rans;
    //2. format: (f args args) constant
    //check the expected ans is a num or expression
    if (isNum(ex_ans)) {
      rans = stod(ex_ans);
    }
    //3. format: (f args args) (g args args)
    else {
      rans = parse_test_helper(ex_ans, fmap);
    }
    /*    if (lans == NAN || rans == NAN) {
      freemap(fmap);
      exit(0);
      }*/
    //output

    if (std::abs(lans - rans) < 0.0000000000001) {
      std::cout << " = " << lans << " [correct] "
                << "\n";
    }
    else {
      std::cout << " = " << rans << " [INCORRECT: expected " << lans << "]\n";
    }
  }
}
