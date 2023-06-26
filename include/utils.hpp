#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <type_traits>
#include "constants.hpp"

namespace fx {

using namespace constants;

struct LongLongTime { using type = int64_t; };
struct IntBid { using type = int32_t; };
struct IntAsk { using type = int32_t; };
struct IntBidVolume { using type = int32_t; };
struct IntAskVolume { using type = int32_t; };
struct LongLongKey { using type = int64_t; };

template <typename T>
auto parse(const std::string& line) {
    return 0ll;
}

template <typename T>
auto snip(const std::string& line, const std::string& target) {
    typename T::type ret;
    auto pos{std::string::npos};

    if (pos = line.find(target);
            pos != std::string::npos) {
                pos += target.size();
                if (auto start = line.find_first_of(':', pos);
                        start != std::string::npos) {
                            start += std::strlen(":\"");
                            if (auto end = line.find_first_of(',', start);
                                    end != std::string::npos) {
                                        end -= std::strlen("\"}");
                                        if constexpr (std::is_same_v<T, LongLongTime>) {
                                            ret = std::stoll(std::string{&line[start], end - start});
                                        } else {
                                            ret = std::stol(std::string{&line[start], end - start});
                                        }
                            }
                }
    }
    return ret;
}

template <>
auto parse<LongLongTime>(const std::string& line) {
    constexpr char TIME_STR[]{"\"time\":"};
    return snip<LongLongTime>(line, TIME_STR);
}

template <>
auto parse<IntBid>(const std::string& line) {
    constexpr char BID_STR[]{"\"bid\":"};
    return snip<IntBid>(line, BID_STR);
}

template <>
auto parse<IntAsk>(const std::string& line) {
    constexpr char ASK_STR[]{"\"ask\":"};
    return snip<IntAsk>(line, ASK_STR);
}

template <>
auto parse<IntBidVolume>(const std::string& line) {
    constexpr char BID_VOLUME_STR[]{"\"bidVolume\":"};
    return snip<IntBidVolume>(line, BID_VOLUME_STR);
}

template <>
auto parse<IntAskVolume>(const std::string& line) {
    constexpr char ASK_VOLUME_STR[]{"\"askVolume\":"};
    return snip<IntAskVolume>(line, ASK_VOLUME_STR);
}

template <>
auto parse<LongLongKey>(const std::string& line) {
    constexpr char KEY_STR[]{"\"key\":"};
    return snip<LongLongKey>(line, KEY_STR);
}

}
#endif
