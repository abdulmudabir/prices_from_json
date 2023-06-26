#ifndef __PARSER_INL__
#define __PARSER_INL__

#include <fstream>
#include <vector>
#include <cstring>
#include "quote.hpp"

namespace fx {

using namespace constants;

inline
void Parser::Run() {
    std::ifstream input_file(jsonFileName_.c_str());
    if (! input_file.is_open()) {
        std::cerr << "Error: Unable to open input file: '" << jsonFileName_ << "'\n";
        return;
    }

    std::vector<Quote> quotes;
    auto i{0ll}, begin{0ll};
    for (std::string line; std::getline(input_file, line); ++i) {
        Quote q(line);
        if (i == 0) {   // unlikely?
            begin = q.timestamp_;
            quotes.push_back(q);
            continue;
        }
        if (q.timestamp_ - begin > THIRTY_MINS) {
            // dispatch background threads
        }
        quotes.push_back(q);
    }
    input_file.close();
}

}
#endif
