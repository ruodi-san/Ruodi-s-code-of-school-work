#include "parse.h"
//return distance from two vectors
double distance(std::vector<double> v1, std::vector<double> v2) {
  if (v1.size() != v2.size()) {
    std::cerr << "v1 and v2 have different length"
              << "\n";
    exit(0);
  }
  double dis = 0;
  for (size_t i = 0; i < v1.size(); i++) {
    dis += pow(v1[i] - v2[i], 2);
  }
  return sqrt(dis);
}

//return gradient of a point
std::vector<double> grad(std::string fname,
                         std::vector<double> point,
                         std::map<std::string, Function *> & fmap) {
  std::vector<double> ans;
  double h = 0.0001;  //set h=0.0001 to calculate gradient
  std::string str;
  std::string hstr;
  std::vector<double> next;
  next = point;

  for (size_t i = 0; i < point.size(); i++) {
    next = point;
    next[i] = point[i] + h;
    str = "(" + fname + " " + varstostr(point) + ")";
    hstr = "(" + fname + " " + varstostr(next) + ")";
    ans.push_back((parse_test_helper(hstr, fmap) - parse_test_helper(str, fmap)) / h);
  }
  return ans;
}
//check whether function convert ,if so return point
//if not ,exit
std::vector<double> check_grad(std::string fname,
                               double gamma,
                               double converge_dis,
                               std::vector<double> p,
                               int step,
                               int sign,
                               std::map<std::string, Function *> & fmap) {
  std::vector<double> next_p(p);
  for (int count = 0; count < step; count++) {
    p = next_p;
    for (size_t i = 0; i < p.size(); i++) {
      next_p[i] = p[i] + sign * gamma * grad(fname, p, fmap)[i];
    }
    //if converge, return point directly
    if (distance(p, next_p) < converge_dis) {
      return p;
    }
  }
  //use out steps, print out a messege and return the last point
  std::cout << "This function " << fname << " doesn't converge : ";
  return p;
}

void parse_gradient(std::string & line, std::map<std::string, Function *> & fmap) {
  std::stringstream ss;
  ss << line;
  //fname should be an existed function
  std::string fname;
  ss >> fname;
  if (!existFunc(fname, fmap)) {
    std::cerr << "function undefined, cannot parse gradient"
              << "\n";
    return;
  }
  //gamma shoud be a positive number
  std::string gamma;
  ss >> gamma;
  try {
    double checksign = stod(gamma);
    if (checksign <= 0) {
      std::cerr << "gamma should be a positive"
                << "\n";
      exit(0);
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "gamma should be a number"
              << "\n";
    exit(0);
  }
  //converge distance should be a positive number
  std::string converge_dis;
  ss >> converge_dis;
  try {
    double checksign = stod(gamma);
    if (checksign <= 0) {
      std::cerr << "converge_dis should be a positive"
                << "\n";
      exit(0);
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "converge_dis should be a number"
              << "\n";
    exit(0);
  }
  // point should be a vector of number
  std::vector<double> point;
  std::string p;
  try {
    for (size_t i = 0; i < fmap[fname]->getsize(); i++) {
      ss >> p;
      point.push_back(stod(p));
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "points in gradient_parse should be a number"
              << "\n";
    exit(0);
  }
  //step should be a positive integer
  std::string step;
  ss >> step;
  try {
    int stepi = stoi(step);
    if (stepi <= 0) {
      std::cerr << "step should be positive"
                << "\n";
      exit(0);
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "step should be a integer"
              << "\n";
    exit(0);
  }
  //sign to check max or min command should be 1 for max and -1 for min
  std::string signstr;
  ss >> signstr;
  try {
    int sign = stoi(signstr);
    if (sign != 1 && sign != -1) {
      std::cerr << "sign should be -1 or 1"
                << "\n";
      exit(0);
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "sign should be a integer"
              << "\n";
    exit(0);
  }
  //if there are other elements in input, invalid input and exit
  if (ss.rdbuf()->in_avail() != 0) {
    std::string s;
    ss >> s;
    std::cerr << "too many args for gradient parse" << s << "~";
    exit(0);
  }
  //compute max or min of f and decide whether f converge
  std::vector<double> ans =
      check_grad(fname, stod(gamma), stod(converge_dis), point, stoi(step), stoi(signstr), fmap);
  std::string anslist;
  for (size_t i = 0; i < ans.size() - 1; i++) {
    anslist += std::to_string(ans[1]) + " ,";
  }
  anslist = "(" + anslist + std::to_string(ans[ans.size() - 1]) + ")";
  //if we want to find the max of f
  if (signstr == "1") {
    std::cout << "max " << fname << " = " << anslist << "\n";
  }
  //if we want to find the min of f
  else {
    std::cout << "min " << fname << " = " << anslist << "\n";
  }
}
