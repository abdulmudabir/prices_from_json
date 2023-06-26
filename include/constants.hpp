#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__

namespace fx::constants {

static constexpr auto NANOSECONDS_PER_SECOND = 1000000000ll;
static constexpr auto THIRTY_MINS = 30 * 60 * NANOSECONDS_PER_SECOND;
static constexpr auto PRICE_SCALE = 1000000.0;
static constexpr auto VOLUME_SCALE = 1000ull;

}
#endif
