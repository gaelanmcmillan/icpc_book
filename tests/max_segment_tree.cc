#include <gtest/gtest.h>
#include <vector>

#include "../src/range_queries/generic_segment_tree.h"

namespace {

class MaxSegmentTreeTests : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  std::vector<int> arr = std::vector<int>{1,2,3,4,5,6,7};
  gx::segment_tree<int> max_tree_ = gx::segment_tree<int>::make_lazy(arr, [](int a, int b) { return a > b? a : b; });
};

TEST_F(MaxSegmentTreeTests, QueryMax) {
  EXPECT_EQ(max_tree_.query_range(1,4), 5);
}
}
