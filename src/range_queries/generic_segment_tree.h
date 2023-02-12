#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace gx {

constexpr int int_max = 1 << 30;

__attribute__((const)) static inline uint32_t next_power_of_2(uint32_t x) {
  return 1 << (32 - __builtin_clz(x - 1));
}

/**
 * A data structure for $O(log(N))$ range-based queries and updates on some
 * array. Supports any associative binary operation.
 */
template <class T> class segment_tree {
  using BinaryOperation = std::function<T(T, T)>;

public:
  segment_tree(const std::vector<T> &vec, BinaryOperation &&func) : size_(next_power_of_2(vec.size())), op(std::move(func)) {
    tree.resize(2 * size_, T{});
    std::copy(vec.begin(), vec.end(), tree.begin() + size_);

    for (int i = size_ - 1; i >= 1; --i) {
      tree[i] = op(tree[left_child(i)], tree[right_child(i)]);
    }
  }

  // factory method for instantiating a lazy segment tree
  static segment_tree<T> make_lazy(const std::vector<T> &vec, BinaryOperation &&func) {
    segment_tree lazyTree = segment_tree(vec, std::move(func));
    lazyTree.lazy.resize(2 * lazyTree.size_, T{});
    lazyTree.update_range(0, lazyTree.size_, T{});
    return lazyTree;
  }

  /**
   * @brief Update values in our range from [left, right] to the given value.
   *
   * @param left The left bound of the subrange to update.
   * @param right The right bound of the subrange to update.
   * @param val The target value each element will be updated to.
   */
  void update_range(int left, int right, T val) {
    update_range_lazy(1, 0, size_ - 1, left, right, val);
  }

  T query_point(int node) {
    return query_range(0, node) - query_range(0, node - 1);
  }

  /**
   * @brief Update array[index] to the provided value.
   */
  void update(int index, T &&value) {
    tree[index + size_] = value;
    for (int i = (index + size_) / 2; i >= 1; i /= 2) {
      tree[i] = op(tree[left_child(i)], tree[right_child(i)]);
    }
  }

  T query_range(int left, int right) {
    // printf("Querying %d %d\n", left, right);
    return recursive_query_range(1, 0, size_ - 1, left, right);
  }

private:
  BinaryOperation op;
  std::vector<T> tree, lazy;
  int size_; // the number of elements in the original array

  // Private implementation behind `update_range`, allowing for a simpler API
  void update_range_lazy(int node, int start, int end, int left, int right, T val) {
    if (lazy[node] != 0) {
      tree[node] += (end - start + 1) * lazy[node];

      if (start != end) {
        lazy[left_child(node)] += lazy[node];
        lazy[right_child(node)] += lazy[node];
      }

      lazy[node] = 0;
    }

    if (end < left || start > right) {
      return;
    }

    if (start >= left && end <= right) {
      tree[node] += (end - start + 1) * val;
      if (start != end) {
        lazy[left_child(node)] += val;
        lazy[right_child(node)] += val;
      }
      return;
    }

    int mid = (start + end) / 2;
    update_range_lazy(left_child(node), start, mid, left, right, val);
    update_range_lazy(right_child(node), mid + 1, end, left, right, val);
    tree[node] = tree[left_child(node)] + tree[right_child(node)];
  }

  // Private implementation behind `query_range`, allowing for a simpler API.
  T recursive_query_range(int node, int low, int high, int left, int right) {
    if (lazy.size() != 0) {
      if (lazy[node] != 0) {
        tree[node] += (high - low + 1) * lazy[node];
        if (low != high) {
          lazy[left_child(node)] += lazy[node];
          lazy[right_child(node)] += lazy[node];
        }
        lazy[node] = 0;
      }
    }
    // three cases:
    // 1. [left, right] encompasses [low, high]
    if (low >= left and high <= right) {
      // printf("\tCase 1\n");
      return tree[node];
    }

    // 2. [low, high] and [left, right] are disjoint
    if (high < left || low > right) {
      // printf("\tCase 2\n");
      return T{};
    }

    int middle = (low + high) / 2;
    // printf("\tNo case\n");

    return op(recursive_query_range(left_child(node), low, middle, left, right),
              recursive_query_range(right_child(node), middle + 1, high, left,
                                    right));
  }
  int left_child(int index) const { return 2 * index; }
  int right_child(int index) const { return 2 * index + 1; }
};
} // namespace gx
