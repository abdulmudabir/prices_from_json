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
    Quote() = default;

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

namespace {

constexpr auto COLUMN_WIDTH{15};
constexpr auto EXTRA_COLUMN_WIDTH{16};
constexpr auto RANGE_WIDTH{20};
constexpr auto BORDER_WIDTH{195};

}   // anonymous

auto stats = [](std::vector<std::shared_ptr<Quote>>& quotes, std::size_t startIdx, std::size_t endIdx, int64_t bidSum, int64_t askSum, std::FILE* ofp
#ifndef SERIAL
    , std::mutex& outputMutex
#endif
    ) {
    auto total_quotes = endIdx - startIdx;

    std::sort(quotes.begin() + startIdx, quotes.begin() + endIdx, BidComparator());
    auto min_bid = (quotes[startIdx]->bidPrice_ + 0.0) / PRICE_SCALE;
    auto min_bid_vol = quotes[startIdx]->bidVolume_ / VOLUME_SCALE;
    auto max_bid = (quotes[endIdx - 1]->bidPrice_ + 0.0) / PRICE_SCALE;
    auto max_bid_vol = quotes[endIdx - 1]->bidVolume_ / VOLUME_SCALE;
    auto mean_bid = (bidSum + 0.0) / PRICE_SCALE / total_quotes;
    auto median_bid{0.0};
    if (total_quotes % 2) {
        median_bid = (quotes[startIdx + total_quotes / 2]->bidPrice_ + 0.0) / PRICE_SCALE;
    } else {
        median_bid = (quotes[startIdx + total_quotes / 2]->bidPrice_ + quotes[startIdx + total_quotes / 2 - 1]->bidPrice_ + 0.0) / PRICE_SCALE / 2.0;
    }

    std::sort(quotes.begin() + startIdx, quotes.begin() + endIdx, AskComparator());
    auto min_ask = (quotes[startIdx]->askPrice_ + 0.0) / PRICE_SCALE;
    auto min_ask_vol = quotes[startIdx]->askVolume_ / VOLUME_SCALE;
    auto max_ask = (quotes[endIdx - 1]->askPrice_ + 0.0) / PRICE_SCALE;
    auto max_ask_vol = quotes[endIdx - 1]->askVolume_ / VOLUME_SCALE;
    auto mean_ask = (askSum + 0.0) / PRICE_SCALE / total_quotes;
    auto median_ask{0.0};
    if (total_quotes % 2) {
        median_ask = (quotes[startIdx + total_quotes / 2]->askPrice_ + 0.0) / PRICE_SCALE;
    } else {
        median_ask = (quotes[startIdx + total_quotes / 2]->askPrice_ + quotes[startIdx + total_quotes / 2 - 1]->askPrice_ + 0.0) / PRICE_SCALE / 2.0;
    }

#ifndef SERIAL
    {
        std::unique_lock lk(outputMutex);
#endif
        char range_buf[EXTRA_COLUMN_WIDTH]{};
        std::snprintf(range_buf, EXTRA_COLUMN_WIDTH, "[%lu-%lu]", startIdx, endIdx - 1);
        std::fprintf(ofp, "%-*s%-*.6f%-*lld%-*.6f%-*lld%-*.6f%-*lld%-*.6f%-*lld%-*.6f%-*.6f%-*.6f%-*.6f\n",
            RANGE_WIDTH, range_buf,
            COLUMN_WIDTH, min_bid,
            COLUMN_WIDTH, min_bid_vol,
            COLUMN_WIDTH, max_bid,
            EXTRA_COLUMN_WIDTH, max_bid_vol,
            COLUMN_WIDTH, min_ask,
            EXTRA_COLUMN_WIDTH, min_ask_vol,
            COLUMN_WIDTH, max_ask,
            EXTRA_COLUMN_WIDTH, max_ask_vol,
            COLUMN_WIDTH, mean_bid,
            COLUMN_WIDTH, mean_ask,
            COLUMN_WIDTH, median_bid,
            COLUMN_WIDTH, median_ask
        );
#ifndef SERIAL
    }
#endif
};

auto write_file_header = [](std::FILE* ofp) {
    constexpr char QUOTE_RANGE[]{"Quote Range"};
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

    auto border_fill = [ofp]() {
        for (auto i = 0; i < BORDER_WIDTH; ++i) {
            std::fprintf(ofp, "%c", '=');
        }
        std::fputc('\n', ofp);
    };

    border_fill();

    std::fprintf(ofp, "%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s%-*s\n",
        RANGE_WIDTH, QUOTE_RANGE,
        COLUMN_WIDTH, MIN_BID,
        EXTRA_COLUMN_WIDTH, MIN_BID_VOL,
        COLUMN_WIDTH, MAX_BID,
        EXTRA_COLUMN_WIDTH, MAX_BID_VOL,
        COLUMN_WIDTH, MIN_ASK,
        EXTRA_COLUMN_WIDTH, MIN_ASK_VOL,
        COLUMN_WIDTH, MAX_ASK,
        EXTRA_COLUMN_WIDTH, MAX_ASK_VOL,
        COLUMN_WIDTH, MEAN_BID,
        COLUMN_WIDTH, MEAN_ASK,
        COLUMN_WIDTH, MEDIAN_BID,
        COLUMN_WIDTH, MEDIAN_ASK
    );

    border_fill();
};

}   // namespace quote
}   // namespace fx
#endif
