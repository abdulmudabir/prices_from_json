#ifndef __QUOTE_HPP__
#define __QUOTE_HPP__

#include "utils.hpp"
#include <memory>
#include <algorithm>
#include <iomanip>

namespace fx {

using namespace constants;

struct Quote {
    Quote(const std::string& line) {
        timestamp_ = parse<Timestamp>(line);
        bidPrice_ = parse<Bid>(line);
        askPrice_ = parse<Ask>(line);
        bidVolume_ = parse<BidVolume>(line);
        askVolume_ = parse<AskVolume>(line);
        key_ = parse<Key>(line);
    }

    int64_t timestamp_{0ll};
    int32_t bidPrice_{0};
    int32_t askPrice_{0};
    int32_t bidVolume_{0};
    int32_t askVolume_{0};
    int64_t key_{0ll};
};

namespace quote {

struct BidComparator {
    auto operator()(const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs) const {
        return lhs->bidPrice_ < rhs->bidPrice_;
    }
};

struct AskComparator {
    auto operator()(const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs) const {
        return lhs->askPrice_ < rhs->askPrice_;
    }
};

auto stats = [](std::vector<std::shared_ptr<Quote>>& quotes, std::size_t startIdx, std::size_t endIdx, int64_t bidSum, int64_t askSum, std::ofstream& ofs) {
    auto total_quotes = quotes.size();

    std::sort(quotes.begin() + startIdx, quotes.begin() + endIdx, BidComparator());
    auto min_bid = (quotes[0]->bidPrice_ + 0.0) / PRICE_SCALE;
    auto min_bid_vol = quotes[0]->bidVolume_ / VOLUME_SCALE;
    auto max_bid = (quotes[total_quotes - 1]->bidPrice_ + 0.0) / PRICE_SCALE;
    auto max_bid_vol = quotes[total_quotes - 1]->bidVolume_ / VOLUME_SCALE;
    auto mean_bid = (bidSum + 0.0) / PRICE_SCALE / total_quotes;
    auto median_bid{0.0};
    if (quotes.size() % 2) {
        median_bid = (quotes[total_quotes / 2]->bidPrice_ + 0.0) / PRICE_SCALE;
    } else {
        median_bid = (quotes[total_quotes / 2]->bidPrice_ + quotes[total_quotes / 2 - 1]->bidPrice_ + 0.0) / PRICE_SCALE / 2.0;
    }

    std::sort(quotes.begin() + startIdx, quotes.begin() + endIdx, AskComparator());
    auto min_ask = (quotes[0]->askPrice_ + 0.0) / PRICE_SCALE;
    auto min_ask_vol = quotes[0]->askVolume_ / VOLUME_SCALE;
    auto max_ask = (quotes[total_quotes - 1]->askPrice_ + 0.0) / PRICE_SCALE;
    auto max_ask_vol = quotes[total_quotes - 1]->askVolume_ / VOLUME_SCALE;
    auto mean_ask = (askSum + 0.0) / PRICE_SCALE / total_quotes;
    auto median_ask{0.0};
    if (quotes.size() % 2) {
        median_ask = (quotes[total_quotes / 2]->askPrice_ + 0.0) / PRICE_SCALE;
    } else {
        median_ask = (quotes[total_quotes / 2]->askPrice_ + quotes[total_quotes / 2 - 1]->askPrice_ + 0.0) / PRICE_SCALE / 2.0;
    }

    constexpr auto NUM_WIDTH{15}, EXTRA_NUM_WIDTH{16}, NUM_PRECISION{5};
    ofs << std::fixed << std::setprecision(NUM_PRECISION);
    ofs << std::left << std::setw(NUM_WIDTH) << min_bid
        << std::left << std::setw(EXTRA_NUM_WIDTH) << min_bid_vol
        << std::left << std::setw(NUM_WIDTH) << max_bid
        << std::left << std::setw(EXTRA_NUM_WIDTH) << max_bid_vol
        << std::left << std::setw(NUM_WIDTH) << min_ask
        << std::left << std::setw(EXTRA_NUM_WIDTH) << min_ask_vol
        << std::left << std::setw(NUM_WIDTH) << max_ask
        << std::left << std::setw(EXTRA_NUM_WIDTH) << max_ask_vol
        << std::left << std::setw(NUM_WIDTH) << mean_bid
        << std::left << std::setw(NUM_WIDTH) << mean_ask
        << std::left << std::setw(NUM_WIDTH) << median_bid
        << std::left << std::setw(NUM_WIDTH) << median_ask << '\n';
};

auto write_file_header = [](std::ofstream& ofs) {
    constexpr auto COLUMN_WIDTH{15}, EXTRA_COLUMN_WIDTH{16};
    constexpr char MIN_BID[]{"Min Bid"};
    constexpr char MIN_BID_VOL[]{"Min Bid Volume"};
    constexpr char MAX_BID[]{"Max Bid"};
    constexpr char MAX_BID_VOL[]{"Max Bid Volume"};
    constexpr char MIN_ASK[]{"Min Ask"};
    constexpr char MIN_ASK_VOL[]{"Min Ask Volume"};
    constexpr char MAX_ASK[]{"Max Ask"};
    constexpr char MAX_ASK_VOL[]{"Max Ask Volume"};
    constexpr char MEAN_BID[]{"Mean Bid"};
    constexpr char MEAN_ASK[]{"Mean Ask"};
    constexpr char MEDIAN_BID[]{"Median Bid"};
    constexpr char MEDIAN_ASK[]{"Median Ask"};

    ofs << std::setw(180) << std::setfill('=') << "";
    ofs << std::setfill(' ') << std::endl;

    ofs << std::left << std::setw(COLUMN_WIDTH) << MIN_BID
        << std::left << std::setw(EXTRA_COLUMN_WIDTH) << MIN_BID_VOL
        << std::left << std::setw(COLUMN_WIDTH) << MAX_BID
        << std::left << std::setw(EXTRA_COLUMN_WIDTH) << MAX_BID_VOL
        << std::left << std::setw(COLUMN_WIDTH) << MIN_ASK
        << std::left << std::setw(EXTRA_COLUMN_WIDTH) << MIN_ASK_VOL
        << std::left << std::setw(COLUMN_WIDTH) << MAX_ASK
        << std::left << std::setw(EXTRA_COLUMN_WIDTH) << MAX_ASK_VOL
        << std::left << std::setw(COLUMN_WIDTH) << MEAN_BID
        << std::left << std::setw(COLUMN_WIDTH) << MEAN_ASK
        << std::left << std::setw(COLUMN_WIDTH) << MEDIAN_BID
        << std::left << std::setw(COLUMN_WIDTH) << MEDIAN_ASK << '\n';

    ofs << std::setw(180) << std::setfill('=') << "";
    ofs << std::setfill(' ') << std::endl;
};

}   // namespace quote
}   // namespace fx
#endif
