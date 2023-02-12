#include <gtest/gtest.h>
#include <vector>

#include "../src/range_queries/generic_segment_tree.h"
namespace {
class XorSegmentTreeTests : public ::testing::Test {
 protected:
  void SetUp() override {

  }

  std::vector<int> arr = std::vector<int>{1,2,3,4,5,6,7};
  gx::segment_tree<int> xor_tree_ = gx::segment_tree<int>::make_lazy(arr, std::bit_xor<>());
};

TEST_F(XorSegmentTreeTests, QueryXorReduction) {
  EXPECT_EQ(xor_tree_.query_range(1,5), 6);
}
}
