/**
 * @brief A set of methods used to selectively capture info
 *  like Timestamps, Bid Prices, Ask Prices, etc. from every line
 *  in the input JSON dump. The parsed info is then packaged into
 *  a Quote object.
 *
 */
#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <type_traits>
#include "constants.hpp"

#define LIKELY(x) (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

namespace fx {

struct Timestamp { using type = int64_t; };
struct Bid { using type = int32_t; };
struct Ask { using type = int32_t; };
struct BidVolume { using type = int32_t; };
struct AskVolume { using type = int32_t; };
struct Key { using type = int64_t; };

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
                                        if constexpr (std::is_same_v<T, Timestamp> ||
                                                std::is_same_v<T, Key>) {
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
auto parse<Timestamp>(const std::string& line) {
    constexpr char TIME_STR[]{"\"time\":"};
    return snip<Timestamp>(line, TIME_STR);
}

template <>
auto parse<Bid>(const std::string& line) {
    constexpr char BID_STR[]{"\"bid\":"};
    return snip<Bid>(line, BID_STR);
}

template <>
auto parse<Ask>(const std::string& line) {
    constexpr char ASK_STR[]{"\"ask\":"};
    return snip<Ask>(line, ASK_STR);
}

template <>
auto parse<BidVolume>(const std::string& line) {
    constexpr char BID_VOLUME_STR[]{"\"bidVolume\":"};
    return snip<BidVolume>(line, BID_VOLUME_STR);
}

template <>
auto parse<AskVolume>(const std::string& line) {
    constexpr char ASK_VOLUME_STR[]{"\"askVolume\":"};
    return snip<AskVolume>(line, ASK_VOLUME_STR);
}

template <>
auto parse<Key>(const std::string& line) {
    constexpr char KEY_STR[]{"\"key\":"};
    return snip<Key>(line, KEY_STR);
}

}
#endif
