

#include <istream>

#include "parse.h"
//include define test numint mcint and gradient function
void parse_define(std::string & line, std::map<std::string, Function *> & fmap);
void parse_test(std::string & line, std::map<std::string, Function *> & fmap);
void parse_numint(std::string & line, std::map<std::string, Function *> & fmap);
void parse_mcint(std::string & line, std::map<std::string, Function *> fmap);
void parse_gradient(std::string & line, std::map<std::string, Function *> & fmap);

int main() {
  std::string line;
  std::map<std::string, Function *> fmap;
  while (getline(std::cin, line)) {
    //store one line of command read from cin
    std::string command;
    command = takechar(line);
    if (command == "define") {
      try {
        parse_define(line, fmap);
      }
      catch (std::exception & e) {
        std::cerr << "invalid define";
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else if (command == "test") {
      try {
        parse_test(line, fmap);
      }
      catch (std::exception & e) {
        std::cerr << "invalid test";
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else if (command == "numint") {
      try {
        parse_numint(line, fmap);
      }
      catch (std::exception & e) {
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else if (command == "mcint") {
      try {
        parse_mcint(line, fmap);
      }
      catch (std::exception & e) {
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else if (command == "max") {
      line = line + " 1";
      try {
        parse_gradient(line, fmap);
      }
      catch (std::exception & e) {
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else if (command == "min") {
      line = line + " -1";
      try {
        parse_gradient(line, fmap);
      }
      catch (std::exception & e) {
        freemap(fmap);
        return EXIT_FAILURE;
      }
    }
    else {
      std::cout << "invalid command"
                << "\n";
      freemap(fmap);
      return EXIT_FAILURE;
    }
  }
  freemap(fmap);
  return EXIT_SUCCESS;
}
