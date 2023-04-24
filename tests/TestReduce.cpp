#include "gtest/gtest.h"
#include "src/Reduce.h"

TEST(HelloTest, GetGreet) {
  EXPECT_EQ(get_greet("Bazel"), "Hello Bazel");
}