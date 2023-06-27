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
    constexpr char OUTPUT_FILENAME[]{"output.txt"};
    std::ofstream output_file(OUTPUT_FILENAME, std::ios::trunc);
    if (! output_file.is_open()) {
        std::cerr << "Error: Unable to open output file: '" << OUTPUT_FILENAME << "'\n";
        return;
    }
    quote::write_file_header(output_file);

    std::vector<std::shared_ptr<Quote>> quotes_vec;
    auto i{0ll}, start_time{0ll}, begin{0ll};
    auto bid_sum{0ll}, ask_sum{0ll};

    for (std::string line; std::getline(input_file, line); ++i) {
        auto qptr = std::make_shared<Quote>(line);
        if (i == 0) {   // unlikely?
            start_time = qptr->timestamp_;
            bid_sum += qptr->bidPrice_;
            ask_sum += qptr->askPrice_;
            quotes_vec.push_back(qptr);
            continue;
        }
        if (qptr->timestamp_ - start_time > THIRTY_MINS) {
            // dispatch background threads
            quote::stats(quotes_vec, begin, i, bid_sum, ask_sum, output_file);
            begin = i;
            start_time = qptr->timestamp_;
            bid_sum = 0ll;
            ask_sum = 0ll;
        }
        bid_sum += qptr->bidPrice_;
        ask_sum += qptr->askPrice_;
        quotes_vec.push_back(qptr);
        // process remaining
    }

    input_file.close();
    output_file.close();
}

}
#endif
