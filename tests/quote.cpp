#include <gtest/gtest.h>

#include <cstring>
#include <mutex>
#include "quote.hpp"

using namespace fx;

class QuoteTest : public ::testing::Test {
  protected:
    void SetUp() override {
        q.timestamp_ = 123456789000000;
        q.bidPrice_ = 987650;
        q.askPrice_ = 555444;
        q.bidVolume_ = 100;
        q.askVolume_ = 9999;
        q.key_ = 9999666667777888;
    }

    Quote q;
};

/*
Dummy json file line:

{"_id":{"$oid":"5b6ac3d663bfd384de2361c1"},"entryID":"1533723599042","key":{"$numberLong":"1375301640029223812"},"quoteType":{"$numberInt":"-1"},"time":{"$numberLong":"1533723600198000000"},"bid":{"$numberInt":"1563910"},"ask":{"$numberInt":"0"},"bidVolume":{"$numberInt":"25000000"},"askVolume":{"$numberInt":"0"},"depth":{"$numberInt":"0"},"positionNumber":{"$numberInt":"0"},"compID":"FXVEN","validTime":{"$numberLong":"3606000000"}}
*/

TEST_F(QuoteTest, QuoteInitialization) {
    const std::string line{
        "{\"_id\":{\"$oid\":\"abcde\"},\"entryID\":\"100100\",\"key\":{\"$numberLong\":\"9999666667777888\"},\"quoteType\":{\"$numberInt\":\"-1\"},\"time\":{\"$numberLong\":\"123456789000000\"},\"bid\":{\"$numberInt\":\"987650\"},\"ask\":{\"$numberInt\":\"555444\"},\"bidVolume\":{\"$numberInt\":\"100\"},\"askVolume\":{\"$numberInt\":\"9999\"},\"depth\":{\"$numberInt\":\"0\"},\"positionNumber\":{\"$numberInt\":\"0\"},\"compID\":\"FXVEN\",\"validTime\":{\"$numberLong\":\"3606000000\"}}"
    };

    Quote test_quote(line);
    EXPECT_EQ(test_quote.timestamp_, q.timestamp_);
    EXPECT_EQ(test_quote.bidPrice_, q.bidPrice_);
    EXPECT_EQ(test_quote.askPrice_, q.askPrice_);
    EXPECT_EQ(test_quote.bidVolume_, q.bidVolume_);
    EXPECT_EQ(test_quote.askVolume_, q.askVolume_);
}