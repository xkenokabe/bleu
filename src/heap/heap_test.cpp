﻿// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "gtest/gtest.h"

extern "C" {
#include "heap.h"
}

TEST(HeapTest, HowToUse) {
  char* c = (char*)heap->New(128);

  EXPECT_TRUE(c != NULL);
  for (int i = 0; i < 128; ++i) EXPECT_EQ(0, c[i]) << "Failure at index " << i;

  heap->Delete((void**)&c);

  EXPECT_EQ(NULL, c);
}

TEST(HeapTest, DeleteWithNull) {
  heap->Delete(NULL);

  SUCCEED();
}

TEST(HeapTest, DeleteMultipleTimes) {
  char* c = (char*)heap->New(128);

  heap->Delete((void**)&c);
  heap->Delete((void**)&c);

  SUCCEED();
}

namespace {
bool is_over_limit;
int given_usage;
void WarningSpy(int usage) {
  is_over_limit = true;
  given_usage = usage;
}
}  // namespace
TEST(HeapTest, WarnWhenOverUsageLimit) {
  is_over_limit = false;
  given_usage = 0;
  heap->ClearUsage();

  heap->SetUsageWarning(128, WarningSpy);

  void* v1 = (char*)heap->New(1);
  EXPECT_FALSE(is_over_limit);
  void* v2 = (char*)heap->New(128);
  EXPECT_TRUE(is_over_limit);
  EXPECT_GE(given_usage, 1 + 128);  // Actual usage is implementation-dependent.

  heap->Delete((void**)&v1);
  heap->Delete((void**)&v2);
}
