#include <iostream>
#include <bitset>
#include <cstdlib>
#include <vector>

// Compute at compile time the minimum number of bits needed to store n
constexpr unsigned int bits_needed(unsigned long long n) {
    return n < 2 ? n : 1 + bits_needed(n >> 1);
}

constexpr unsigned int pow(unsigned int a, unsigned int p) {
    unsigned int ret = 1;
    unsigned int mul = a;
    while (p > 0) {
        if (p & 1) ret *= mul;
        mul = mul * mul;
        p >>= 1;
    }
    return ret;
}

constexpr unsigned int sqrt(unsigned int a) {
    unsigned int lb = 0;
    unsigned int ub = INT32_MAX;
    while (ub - lb > 1) {
        unsigned int mid = (ub + lb) / 2;
        if (mid * mid > a) ub = mid;
        else lb = mid;
    }
    return ub;
}

constexpr unsigned int compute_depth(unsigned int degree, unsigned int num) {
    unsigned int mul = 1;
    unsigned int depth = 0;
    for (; depth < 100; depth++) {
        if (mul >= num) return depth;
        mul *= degree;
    }
    return depth;
}

constexpr unsigned int nodes_num(unsigned int degree, unsigned depth) {
    unsigned int mul = 1;
    unsigned int num = 0;
    for (unsigned int d = 0; d <= depth; d++) {
        num += mul;
        mul *= degree;
    }
    return num;
}

unsigned long long get_element(const std::vector<unsigned long long> &data, unsigned int element_width, unsigned int i) {
    // [start_bit_idx, end_bit_idx]
    unsigned int start_bit_idx = element_width * i;
    unsigned int end_bit_idx = start_bit_idx + element_width - 1;
    unsigned int word_width = sizeof(unsigned long long) * 8;

    unsigned int relative_start_idx = start_bit_idx % word_width;
    unsigned int relative_end_idx = end_bit_idx % word_width;

    unsigned int ret = 0;
    if (relative_end_idx < relative_start_idx) {
        unsigned long long lower = data[start_bit_idx / word_width];
        unsigned long long upper = data[start_bit_idx / word_width + 1];
        unsigned int lower_width = word_width - relative_start_idx;
        lower >>= relative_start_idx;
        upper <<= word_width - relative_end_idx - 1;
        upper >>= (word_width - relative_end_idx - 1) - lower_width;
        ret = lower | upper;
    } else {
        unsigned long long value = data[start_bit_idx / word_width];
        value >>= relative_start_idx;
        value <<= relative_start_idx + (word_width - relative_end_idx - 1);
        value >>= relative_start_idx + (word_width - relative_end_idx - 1);
        ret = value;
    }

    return ret;
}

void set_element(std::vector<unsigned long long> &data, unsigned int element_width, unsigned int i, unsigned long long assigned) {
    auto clear_bit = [](unsigned long long &w, unsigned int j) { w &= (~(1LLu << j)); };
    auto set_bit = [](unsigned long long &w, unsigned int j) { w |= (1LLu << j); };

    // [start_bit_idx, end_bit_idx]
    unsigned int start_bit_idx = element_width * i;
    unsigned int end_bit_idx = start_bit_idx + element_width - 1;
    unsigned int word_width = sizeof(unsigned long long) * 8;

    unsigned int relative_start_idx = start_bit_idx % word_width;
    unsigned int relative_end_idx = end_bit_idx % word_width;

    if (relative_end_idx < relative_start_idx) {
        unsigned long long lower = data[start_bit_idx / word_width];
        unsigned long long upper = data[start_bit_idx / word_width + 1];
        for (int j = relative_start_idx; j < word_width; j++) {
            if (assigned & 1u) set_bit(lower, j);
            else clear_bit(lower, j);
            assigned >>= 1u;
        }
        for (int j = 0; j <= relative_end_idx; j++) {
            if (assigned & 1u) set_bit(upper, j);
            else clear_bit(upper, j);
            assigned >>= 1u;
        }
        data[start_bit_idx / word_width] = lower;
        data[start_bit_idx / word_width + 1] = upper;
    } else {
        unsigned long long value = data[start_bit_idx / word_width];
        for (int j = relative_start_idx; j <= relative_end_idx; j++) {
            if (assigned & 1u) set_bit(value, j);
            else clear_bit(value, j);
            assigned >>= 1u;
        }
        data[start_bit_idx / word_width] = value;
    }
}

// Set `N` to be the input bits size
constexpr unsigned long long N = 100000;
constexpr unsigned int logN = bits_needed(N) - 1;

constexpr unsigned int l = logN * logN; // The number of 1 in a large block
constexpr unsigned int s = logN / 2; // The size of a small block
constexpr unsigned int n = N;

constexpr unsigned int threshold = 16 * s * s * s * s; // (logN)^4
constexpr unsigned int tree_degree = sqrt(logN);
constexpr unsigned int max_small_blocks_num = threshold / s;
constexpr unsigned int tree_depth = compute_depth(tree_degree, max_small_blocks_num);
constexpr unsigned int non_leaf_nodes_num = nodes_num(tree_degree, tree_depth) - max_small_blocks_num;

constexpr unsigned int Tree_element_width = bits_needed(l);
constexpr unsigned int Tree_elements_num = nodes_num(tree_degree, tree_depth);
constexpr unsigned int Tree_n = Tree_element_width * Tree_elements_num;

constexpr unsigned int Pos_element_width = bits_needed(n);
constexpr unsigned int Pos_elements_num = l;
constexpr unsigned int Pos_n = Pos_element_width * Pos_elements_num;

class BitVector {
private:
    void count_blocks_num(const std::bitset<n> &bits) {
        tmp_block_types.clear();
        unsigned int bits_cnt = 0;
        unsigned int cnt = 0;
        for (int i = 0; i < n; i++) {
            cnt++;
            bits_cnt += bits[i];
            if (bits_cnt == l) {
                if (cnt >= threshold) {
                    sparse_blocks_num++;
                    tmp_block_types.push_back({false, cnt});
                } else {
                    dense_blocks_num++;
                    tmp_block_types.push_back({true, cnt});
                }
                bits_cnt = 0;
                cnt = 0;
            }
        }
        if (cnt > 0) {
            if (cnt >= threshold) {
                sparse_blocks_num++;
                tmp_block_types.push_back({false, cnt});
            } else {
                dense_blocks_num++;
                tmp_block_types.push_back({true, cnt});
            }
        }

        unsigned int blocks_num = sparse_blocks_num + dense_blocks_num;
        Ptr_element_width = bits_needed(std::max(sparse_blocks_num, dense_blocks_num));
        Ptr_elements_num = blocks_num;
        Ptr_n = Ptr_element_width * Ptr_elements_num;

        Type_element_width = 1;
        Type_elements_num = blocks_num;
        Type_n = Type_element_width * Type_elements_num;
    }

    // Return B[i]
    unsigned int get_b(unsigned int i) {
        return get_element(B, 1, i);
    }

    // B[i] = value
    void set_b(unsigned int i, unsigned int value) {
        set_element(B, 1, i, value);
    }

    unsigned int get_ptr(unsigned int i) {
        return get_element(Ptr, Ptr_element_width, i);
    }

    void set_ptr(unsigned int i, unsigned int value) {
        set_element(Ptr, Ptr_element_width, i, value);
    }

    unsigned int get_type(unsigned int i) {
        return get_element(Type, Type_element_width, i);
    }

    void set_type(unsigned int i, unsigned int value) {
        set_element(Type, Type_element_width, i, value);
    }

    unsigned int get_tree_node(unsigned int tree_idx, unsigned int node_idx) {
        return get_element(Tree, Tree_element_width, tree_idx * Tree_elements_num + node_idx);
    }

    unsigned int get_tree_nodes(unsigned int tree_idx, unsigned int node_idx, unsigned int batch_size) {
        unsigned int start_node_idx = tree_idx * Tree_elements_num + node_idx;
        unsigned int end_node_idx = start_node_idx + batch_size - 1;

        if (start_node_idx % batch_size == 0) {
            return get_element(Tree, Tree_element_width * batch_size, start_node_idx / batch_size);
        } else {
            unsigned long long lower = get_element(Tree, Tree_element_width * batch_size, start_node_idx / batch_size);
            unsigned long long upper = get_element(Tree, Tree_element_width * batch_size, start_node_idx / batch_size + 1);
            lower >>= (start_node_idx % batch_size) * Tree_element_width;

        }
        // return get_element(Tree, Tree_element_width, tree_idx * Tree_elements_num + node_idx);
    }

    void set_tree_node(unsigned int tree_idx, unsigned int node_idx, unsigned int value) {
        set_element(Tree, Tree_element_width, tree_idx * Tree_elements_num + node_idx, value);
    }

    unsigned int get_pos(unsigned int block_idx, unsigned int bit_idx) {
        return get_element(Pos, Pos_element_width, block_idx * Pos_elements_num + bit_idx);
    }

    void set_pos(unsigned int block_idx, unsigned int bit_idx, unsigned int value) {
        set_element(Pos, Pos_element_width, block_idx * Pos_elements_num + bit_idx, value);
    }
public:
    std::vector<unsigned long long> B;
    std::vector<unsigned long long> Ptr;
    std::vector<unsigned long long> Type;
    std::vector<unsigned long long> Tree;
    std::vector<unsigned long long> Pos;

    unsigned int Ptr_element_width;
    unsigned int Ptr_elements_num;
    unsigned int Ptr_n;

    unsigned int Type_element_width;
    unsigned int Type_elements_num;
    unsigned int Type_n;

    unsigned int sparse_blocks_num;
    unsigned int dense_blocks_num;

    std::vector<std::pair<bool, unsigned int>> tmp_block_types; // <dense?, block size>

    BitVector(const std::bitset<n> &bits) {
        count_blocks_num(bits);

        unsigned int word = sizeof(unsigned long long) * 8;
        unsigned int whole_tree_size = Tree_n * dense_blocks_num;
        unsigned int whole_pos_size = Pos_n * sparse_blocks_num;
        B.resize(n / word + (word - n % word), 0);
        Ptr.resize(Ptr_n / word + (word - Ptr_n % word), 0);
        Type.resize(Type_n / word + (word - Type_n % word), 0);
        Tree.resize(whole_tree_size/ word + (word - whole_tree_size % word), 0);
        Pos.resize(whole_pos_size / word + (word - whole_pos_size % word), 0);

        for (int i = 0; i < n; i++) set_b(bits[i], i);

        unsigned int global_bit_idx = 0;
        unsigned int tree_idx = 0;
        unsigned int pos_block_idx = 0;

        for (unsigned int block_idx = 0; block_idx < sparse_blocks_num + dense_blocks_num; block_idx++) {
            bool is_dense = tmp_block_types[block_idx].first;
            unsigned int block_size = tmp_block_types[block_idx].second;

            if (is_dense) {
                // Tree initialization
            } else {
                unsigned int pos_idx = 0;
                for (unsigned int local_bit_idx = 0; local_bit_idx < block_size; local_bit_idx++) {
                    unsigned int bit_idx = global_bit_idx + local_bit_idx;
                    if (get_b(bit_idx)) set_pos(block_idx, pos_idx++, bit_idx);
                }
                set_ptr(block_idx, pos_block_idx);
                set_type(block_idx, 0);
                pos_block_idx++;
            }

            global_bit_idx += block_size;
        }
    }
};

int main() {
    return 0;
}
