#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <string>

namespace gx {

constexpr int int_max = 1 << 30;
 
__attribute__ ((const))
static inline uint32_t next_power_of_2(uint32_t x)
{
    return 1 << (32 - __builtin_clz(x - 1));
}

/**
* A data structure for $O(log(N))$ range-based queries and updates on some array.
* Supports any associative binary operation.
*/
template <class T>
class segment_tree {
	using BinaryOperation = std::function<T(T,T)>;
public:
    segment_tree (const std::vector<T> &vec, BinaryOperation &&func)
    : size_(next_power_of_2(vec.size())), op(std::move(func)) {
        tree.resize(2*size_, T{});
		std::copy(vec.begin(), vec.end(), tree.begin() + size_);

		// build the tree 
        for (int i = size_-1; i >= 1; --i) {
            tree[i] = op(tree[left_child(i)], tree[right_child(i)]);
        }
    }
    segment_tree() = delete; // must construct from a vector and a binop
    ~segment_tree() = default;

	// initialize the lazy array so that the tree can be lazily updated
	void make_lazy() { lazy.resize(size_, T{}); }
	
    /**
     * @brief Update array[index] to the provided value.
     */
    void update(int index, T &&value) {
        tree[index + size_] = value;
        for (int i = (index + size_)/2; i >= 1; i /= 2) {
            tree[i] = op(tree[left_child(i)], tree[right_child(i)]);
        }
    }
 
    T query_range (int left, int right) const {
        return recursive_query_range(1, 0, size_-1, left, right);
    }
 
private:
    BinaryOperation op;
    std::vector<T> tree, lazy;
    const int size_; // the number of elements in the original array
 
    T recursive_query_range(int node, int low, int high, int left, int right) const {
        // three cases:
        // 1. [left, right] encompasses [low, high]
        if (low >= left and high <= right) {
            return tree[node];
        }
        // 2. [low, high] and [left, right] are disjoint
        if (high < left || low > right) {
            return T{};
        }
 
        int middle = (low + high) / 2;
        return op(
            recursive_query_range(left_child(node), low, middle, left, right),
            recursive_query_range(right_child(node), middle+1, high, left, right)
        );
    }
    int left_child(int index) const { return 2*index; }
    int right_child(int index) const { return 2*index+1; }
};
} // namespace gx

