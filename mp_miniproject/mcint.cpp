#include "parse.h"
void parse_mcint(std::string & line, std::map<std::string, Function *> fmap) {
  std::string fname;
  skipSpace(line);
  fname = takechar(line);
  if (!existFunc(fname, fmap)) {
    std::cerr << "do not have such function,cannot parse_numint"
              << "\n";
    exit(0);
  }
  std::vector<double> low;
  std::vector<double> high;
  std::stringstream defline;
  defline << line;
  std::string num;
  defline >> num;
  int inum;  //num of random numbers
  try {
    inum = stoi(num);
  }
  catch (std::invalid_argument) {
    std::cerr << "num should be integer"
              << "\n";
    exit(0);
  }
  if (inum < 0) {
    std::cerr << "mcint num should be a integer"
              << "\n";
    exit(0);
  }
  std::string temp;
  size_t dim = fmap[fname]->getsize();
  for (size_t i = 0; i < 2 * dim; i++) {
    defline >> temp;
    try {
      if (i % 2 == 0) {
        low.push_back(stod(temp));
      }
      if (i % 2 != 0) {
        high.push_back(stod(temp));
      }
      temp.clear();
    }
    catch (std::invalid_argument) {
      std::cerr << "args should be number"
                << "\n";
      exit(0);
    }
  }
  //if there are other elements in input, invalid input and exit
  if (defline.rdbuf()->in_avail() != 0) {
    std::string s;
    defline >> s;
    std::cerr << "too many args for mcint parse" << s << "~";
    exit(0);
  }
  //check whether low element is lower than higher bound
  if (!valid_bound(low, high)) {
    std::cerr << "lower bound should be lower than high bound"
              << "\n";
    exit(0);
  }
  srand((unsigned)time(NULL));
  double ans = 0;

  std::string str;
  std::string currstr;
  for (size_t i = 0; i < (size_t)inum; i++) {
    for (size_t j = 0; j < dim; j++) {
      double rannum = ((double)rand() / (RAND_MAX) * (high[j] - low[j]) + low[j]);

      str += " " + std::to_string(rannum);
    }
    currstr = "(" + fname + " " + str + ")";

    str.clear();
    ans += parse_test_helper(currstr, fmap);
  }
  ans = ans / inum;
  for (size_t j = 0; j < dim; j++) {
    ans *= (high[j] - low[j]);
  }

  std::cout << "mcint: " << fname << " = " << ans << "\n";
}
