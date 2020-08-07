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

// Set `N` to be the input bits size
constexpr unsigned long long N = 10000000;
constexpr unsigned int logN = bits_needed(N) - 1;

constexpr unsigned int s = logN / 2; // the number of small blocks
constexpr unsigned int l = s * 2 * logN; // the number of large blocks
constexpr unsigned int n = (N / l + 1) * l; // the actual number of input bits

constexpr unsigned int Bs_internal_num = l / s;
constexpr unsigned int Bs_element_width = bits_needed(s * Bs_internal_num);
constexpr unsigned int Bs_elements_num = n / s;
constexpr unsigned int Bs_n = Bs_element_width * Bs_elements_num;

constexpr unsigned int Bl_element_width = bits_needed(n);
constexpr unsigned int Bl_elements_num = n / l;
constexpr unsigned int Bl_n = Bl_element_width * Bl_elements_num;

constexpr unsigned int lookup_element_width = bits_needed(s);
constexpr unsigned int lookup_n = bits_needed(s) * pow(2, s) * s;

unsigned int get_element(const std::vector<unsigned long long> &data, unsigned int element_width, unsigned int i) {
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

void set_element(std::vector<unsigned long long> &data, unsigned int element_width, unsigned int i, unsigned int assigned) {
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

class BitVector {
private:
    void construct_Bs() {
        for (int bl_idx = 0; bl_idx < Bl_elements_num; bl_idx++) {
            for (int bs_local_idx = 0; bs_local_idx < Bs_internal_num; bs_local_idx++) {
                unsigned int bs_global_idx = bs_local_idx + bl_idx * Bs_internal_num;

                if (bs_local_idx == 0) {
                    set_bs_bits(bs_global_idx, 0);
                    continue;
                }

                unsigned int bits_num = get_bs(bs_global_idx - 1);
                for (int i = s * (bs_global_idx - 1); i < s * bs_global_idx; i++) if (get_b(i)) bits_num++;
                set_bs_bits(bs_global_idx, bits_num);
            }
        }
    }

    void construct_Bl() {
        set_bl_bits(0, 0);
        for (int bl_idx = 1; bl_idx < Bl_elements_num; bl_idx++) {
            unsigned int bits_num = get_bl(bl_idx - 1) + get_bs(bl_idx * Bs_internal_num - 1);
            for (int i = (bl_idx * Bs_internal_num - 1) * s; i < bl_idx * Bs_internal_num * s; i++)
                if (get_b(i)) bits_num++;
            set_bl_bits(bl_idx, bits_num);
        }
    }

    void construct_lookup() {
        for (unsigned int w = 0; w < pow(2, s); w++) {
            for (unsigned int j = 0; j < s; j++) {
                set_lookup_bits(w, j, (j > 0 ? get_lookup(w, j - 1) : 0) + ((w >> j) & 1));
            }
        }
    }

    // Return B[i]
    unsigned int get_b(unsigned int i) {
        return get_element(B, 1, i);
    }

    // B[i] = value
    void set_b(unsigned int i, unsigned int value) {
        set_element(B, 1, i, value);
    }

    // Return Bl[i]
    unsigned int get_bl(unsigned int i) {
        return get_element(Bl, Bl_element_width, i);
    }

    // Bl[i] = value
    void set_bl_bits(unsigned int i, unsigned int value) {
        set_element(Bl, Bl_element_width, i, value);
    }

    // Return Bs[i]
    unsigned int get_bs(unsigned int i) {
        return get_element(Bs, Bs_element_width, i);
    }

    // Bs[i] = value
    void set_bs_bits(unsigned int i, unsigned int value) {
        set_element(Bs, Bs_element_width, i, value);
    }

    // Return lookup[w][j]
    // w = the number of bits in the current small block
    // j = local position in the current small block
    // return = the number of bits in small_block[0..j]
    unsigned int get_lookup(unsigned int w, unsigned int j) {
        return get_element(lookup_table, lookup_element_width, w * s + j);
    }

    // lookup[w][j] = value
    void set_lookup_bits(unsigned int w, unsigned int j, unsigned int value) {
        set_element(lookup_table, lookup_element_width, w * s + j, value);
    }

    std::vector<unsigned long long> B;
    std::vector<unsigned long long> Bl;
    std::vector<unsigned long long> Bs;
    std::vector<unsigned long long> lookup_table;
public:
    BitVector(std::bitset<n> bits) {
        unsigned int word = sizeof(unsigned long long) * 8;
        B.resize(n / word + (word - n % word), 0);
        Bl.resize(Bl_n / word + (word - Bl_n % word), 0);
        Bs.resize(Bs_n / word + (word - Bs_n % word), 0);
        lookup_table.resize(lookup_n / word + (word - lookup_n % word), 0);

        for (int i = 0; i < n; i++) set_b(i, bits[i]);
        construct_Bs();
        construct_Bl();
        construct_lookup();
    }

    unsigned int rank(unsigned int i) {
        unsigned int x = i / s; // index in Bs
        unsigned int y = x / Bs_internal_num; // index in Bl
        unsigned int w = get_element(B, s, x); // Value of the current small block
        unsigned int j = i % s; // local position in the current small block
        return get_bl(y) + get_bs(x) + get_lookup(w, j);
    }
};

int main() {
    std::cout << "n=" << n << " ,s=" << s << " ,l=" << l << std::endl;
    std::cout << "Big block total bits: " << Bl_n << std::endl;
    std::cout << "Small block total bits: " << Bs_n << std::endl;
    std::cout << "Lookup table total bits: " << lookup_n << std::endl;

    std::bitset<n> bits;
    for (int i = 0; i < n; i++) {
        if ((double) std::rand() / RAND_MAX > 0.5) bits[i] = 1;
        else bits[i] = 0;
    }
    BitVector bv(bits);

    unsigned int rank = 0;
    for (int i = 0; i < n; i++) {
        rank += bits[i];
        unsigned int computed = bv.rank(i);
        if (computed == rank) continue;
        std::cout << "Test failed at " << i << "th test case. " << n - 1 - i << " tests remain" << std::endl;
        return 0;
    }
    std::cout << "Test passed" << std::endl;

    return 0;
}
