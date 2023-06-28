/**
 * @brief Implementation details of the Parser object.
 *
 * This is just a separation of the implementation of the Run()
 * member function of Parser.
 */
#ifndef __PARSER_INL__
#define __PARSER_INL__

#include <fstream>
#include <cstdio>
#include <cstring>
#include "quote.hpp"
#include <future>

namespace fx {

using namespace constants;

/**
 * @brief Run() allows the option to either serially or parallelly process
 * batches of Quotes from the JSON quotes-dump file.
 *
 * The presence of the SERIAL, POOL, ASYNC macros is intended to help with
 * performance assessments for the batch-processing. The user may determine the type of
 * processing at compile-time using cmake flags.
 *
 * @return void
 */
inline
void Parser::Run() {
    std::ifstream input_file(jsonFileName_.c_str());
    if (! input_file.is_open()) {
        std::cerr << "Error: Unable to open input file: '" << jsonFileName_ << "'\n";
        return;
    }
    constexpr char OUTPUT_FILENAME[]{"output.txt"};
    auto output_file = std::fopen(OUTPUT_FILENAME, "w");
    if (! output_file) {
        std::cerr << "Error: Unable to open output file: '" << OUTPUT_FILENAME << "'\n";
        return;
    }
    quote::write_file_header(output_file);

    std::vector<std::shared_ptr<Quote>> quotes_vec;
    auto i{0ll}, start_time{0ll}, begin{0ll};
    auto bid_sum{0ll}, ask_sum{0ll};

    for (std::string line; std::getline(input_file, line); ++i) {
        auto qptr = std::make_shared<Quote>(line);
        if UNLIKELY(i == 0) {
            start_time = qptr->timestamp_;
            bid_sum += qptr->bidPrice_;
            ask_sum += qptr->askPrice_;
            quotes_vec.push_back(qptr);
            continue;
        }
        if (qptr->timestamp_ - start_time > THIRTY_MINS) {
#ifdef SERIAL
            quote::stats(quotes_vec, begin, i, bid_sum, ask_sum, output_file);
#elif defined(POOL)
            pool_->Submit(quote::stats, quotes_vec, begin, i, bid_sum, ask_sum, output_file, std::ref(outputMutex_));
#elif defined(ASYNC)
            static_cast<void>(std::async(std::launch::async, quote::stats, std::ref(quotes_vec), begin, i, bid_sum, ask_sum, output_file, std::ref(outputMutex_)));
            // deliberately ignore return-type
#endif
            begin = i;
            start_time = qptr->timestamp_;
            bid_sum = 0ll;
            ask_sum = 0ll;
        }
        bid_sum += qptr->bidPrice_;
        ask_sum += qptr->askPrice_;
        quotes_vec.push_back(qptr);
    }
    if (quotes_vec[i - 1]->timestamp_ - start_time < THIRTY_MINS) { // process remaining quotes
#ifdef SERIAL
        quote::stats(quotes_vec, begin, i, bid_sum, ask_sum, output_file);
#elif defined(POOL)
        pool_->Submit(quote::stats, quotes_vec, begin, i, bid_sum, ask_sum, output_file, std::ref(outputMutex_));
#elif defined(ASYNC)
        static_cast<void>(std::async(std::launch::async, quote::stats, std::ref(quotes_vec), begin, i, bid_sum, ask_sum, output_file, std::ref(outputMutex_)));
#endif
    }

#ifdef POOL
    pool_->Shutdown();
#endif
    input_file.close();
    std::fclose(output_file);
}

}   // namespace fx
#endif
