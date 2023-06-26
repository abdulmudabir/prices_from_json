#ifndef __QUOTE_HPP__
#define __QUOTE_HPP__

#include "utils.hpp"

namespace fx {

struct Quote {
    Quote(const std::string& line) {
        timestamp_ = parse<LongLongTime>(line);
        bidPrice_ = parse<IntBid>(line) / PRICE_SCALE;
        askPrice_ = parse<IntAsk>(line) / PRICE_SCALE;
        bidVolume_ = parse<IntBidVolume>(line) / VOLUME_SCALE;
        askVolume_ = parse<IntAskVolume>(line) / VOLUME_SCALE;
        key_ = parse<LongLongKey>(line);
    }

    int64_t timestamp_{0ll};
    double bidPrice_{0.0};
    double askPrice_{0.0};
    int32_t bidVolume_{0};
    int32_t askVolume_{0};
    int64_t key_{0ll};
};

}
#endif
