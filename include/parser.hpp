#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <iostream>
#include <config.hpp>
#include "thread_pool.hpp"

namespace fx {

class Parser {
  public:
    Parser(const char* filename) {
        jsonFileName_.append(filename);
#ifdef POOL
        constexpr auto NUM_THREADS{4};
        pool_ = std::make_unique<ThreadPool>(NUM_THREADS);
#endif
    }
    void Run();
  private:
    std::string jsonFileName_{fx::PROJECT_ROOT_DIR};
#ifndef SERIAL
    std::unique_ptr<ThreadPool> pool_;
    std::mutex outputMutex_;
#endif
};

}
#include "parser.inl"
#endif
