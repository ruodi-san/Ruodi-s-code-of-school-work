#include "parse.h"
//recursive loop to solve the ans
double recursive(std::string fname,
                 double step,
                 size_t dim,

                 std::vector<double> lower,
                 std::vector<double> currval,
                 std::vector<double> higher,
                 std::map<std::string, Function *> & fmap) {
  std::string currstr;
  double ans = 0;
  std::string str;
  currval[0] -= step;
  for (int i = 0; i < (higher[dim] - lower[dim] + step / 2) / step; i++) {
    double x = step + currval[dim];
    currval[dim] = x;
    if (dim == higher.size() - 1) {
      currstr = "( " + fname + " " + varstostr(currval) + ")";
      //caculate f at each point
      ans += parse_test_helper(currstr, fmap);
    }
    else {
      double temp_ans = recursive(fname, step, dim + 1, lower, currval, higher, fmap);
      ans += temp_ans;
    }
  }
  currval[dim] = lower[dim];
  return ans;
}
void parse_numint(std::string & line, std::map<std::string, Function *> & fmap) {
  std::string fname;
  skipSpace(line);
  fname = takechar(line);
  if (!existFunc(fname, fmap)) {
    std::cerr << "do not have such function,cannot parse_numint"
              << "\n";
    exit(0);
  }
  std::vector<double> lower;
  std::vector<double> higher;
  std::vector<double> currval;
  std::stringstream defline;
  defline << line;
  std::string stepstr;
  defline >> stepstr;
  double step = stod(stepstr);
  if (step < 0) {
    std::cerr << "step size should be a positive"
              << "\n";
    exit(0);
  }
  std::string temp;
  size_t sz = fmap[fname]->getsize();
  for (size_t i = 0; i < 2 * sz; i++) {
    defline >> temp;
    try {
      if (i % 2 == 0) {
        lower.push_back(stod(temp) + step / 2);
        currval.push_back(stod(temp) + step / 2);
      }
      if (i % 2 != 0) {
        higher.push_back(stod(temp));
      }
      temp.clear();
    }
    catch (std::invalid_argument) {
      std::cerr << "args should be number"
                << "\n";
      exit(0);
    }
  }
  //check whether low element is lower than higher bound
  if (!valid_bound(lower, higher)) {
    std::cerr << "lower bound should be lower than high bound"
              << "\n";
    exit(0);
  }
  //if there are other elements in input, invalid input and exit
  if (defline.rdbuf()->in_avail() != 0) {
    std::string s;
    defline >> s;
    std::cerr << "too many args for numint parse"
              << "\n";
    exit(0);
  }

  double ans = recursive(fname, step, 0, lower, currval, higher, fmap) * pow(step, lower.size());
  std::cout << "numint " << fname << " = " << ans << "\n";
}
