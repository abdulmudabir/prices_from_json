/**
 * @brief class Parser
 *
 * The Parser object is responsible for fully reading the MongoDB json dump and
 * publishing the mean, median, max, min for every 30-minute time interval
 * of Quotes to an output text file.
 */
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
        constexpr auto NUM_THREADS{4};  // 4 were benchmarked as most-performant
        pool_ = std::make_unique<ThreadPool>(NUM_THREADS);
#endif
    }
    void Run();
  private:
    std::string jsonFileName_{fx::PROJECT_ROOT_DIR};
#ifndef SERIAL
#ifdef POOL
    std::unique_ptr<ThreadPool> pool_;
#endif
    std::mutex outputMutex_;
#endif
};

}
#include "parser.inl"
#endif
