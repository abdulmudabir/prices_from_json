#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <iostream>
#include <config.hpp>

namespace fx {

class Parser {
  public:
    Parser(const char* filename) {
        jsonFileName_.append(filename);
    }
    void Run();
  private:
    std::string jsonFileName_{fx::PROJECT_ROOT_DIR};
};

}
#include "parser.inl"
#endif
