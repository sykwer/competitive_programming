#include <iostream>
#include <bitset>
#include <cstdlib>

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
constexpr unsigned long long N = 10000;
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

// For test
unsigned int naive_select(std::bitset<n> bits, unsigned int i) {
    unsigned int ret = 0;
    for (int j = 0; j <= i; j++) if (bits[j]) ret++;
    return ret;
}

// Drop bits outside the range [right, left)
// e.g. project_range(0b101101, 1, 4) returns 6
template<unsigned int W>
unsigned long project_range(std::bitset<W> bits, unsigned int right, unsigned int left) {
    bits >>= right;
    bits <<= W - left + right;
    bits >>= W - left + right;
    return bits.to_ulong();
}

template<unsigned int W>
void assign_bits(std::bitset<W> &bits, unsigned int idx, unsigned int width, unsigned int num) {
    for (int i = 0; i < width; i++) {
        if (i + idx >= W) break;
        if ((num >> i) & 1) bits[i + idx] = 1;
        else bits[i + idx] = 0;
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

                unsigned int bits_num = get_bs_bits(bs_global_idx - 1);
                for (int i = s * (bs_global_idx - 1); i < s * bs_global_idx; i++) if (bits[i]) bits_num++;
                set_bs_bits(bs_global_idx, bits_num);
            }
        }
    }

    void construct_Bl() {
        set_bl_bits(0, 0);
        for (int bl_idx = 1; bl_idx < Bl_elements_num; bl_idx++) {
            unsigned int bits_num = get_bl_bits(bl_idx - 1) + get_bs_bits(bl_idx * Bs_internal_num - 1);
            for (int i = (bl_idx * Bs_internal_num - 1) * s; i < bl_idx * Bs_internal_num * s; i++) if (bits[i]) bits_num++;
            set_bl_bits(bl_idx, bits_num);
        }
    }

    void construct_lookup() {
        for (unsigned int w = 0; w < pow(2, s); w++) {
            for (unsigned int j = 0; j < s; j++) {
                set_lookup_bits(w, j, (j > 0 ? get_lookup_bits(w, j-1) : 0) + ((w >> j) & 1));
            }
        }
    }

    // Return Bl[i]
    unsigned int get_bl_bits(unsigned int i) {
        return project_range<Bl_n>(bl_bits, i * Bl_element_width, (i+1) * Bl_element_width);
    }

    // Bl[i] = value
    void set_bl_bits(unsigned int i, unsigned int value) {
        assign_bits<Bl_n>(bl_bits, i * Bl_element_width, Bl_element_width, value);
    }

    // Return Bs[i]
    unsigned int get_bs_bits(unsigned int i) {
        return project_range<Bs_n>(bs_bits, i * Bs_element_width, (i+1) * Bs_element_width);
    }

    // Bs[i] = value
    void set_bs_bits(unsigned int i, unsigned int value) {
        assign_bits<Bs_n>(bs_bits, i * Bs_element_width, Bs_element_width, value);
    }

    // Return lookup[w][j]
    // w = the number of bits in the current small block
    // j = local position in the current small block
    // return = the number of bits in small_block[0..j]
    unsigned int get_lookup_bits(unsigned int w, unsigned int j) {
        return project_range<lookup_n>(lookup_bits, (w*s+j) * lookup_element_width, (w*s+j+1) * lookup_element_width);
    }

    // lookup[w][j] = value
    void set_lookup_bits(unsigned int w, unsigned int j, unsigned int value) {
        assign_bits<lookup_n>(lookup_bits, (w*s+j) * lookup_element_width, lookup_element_width, value);
    }

    std::bitset<n> bits;
    std::bitset<Bl_n> bl_bits;
    std::bitset<Bs_n> bs_bits;
    std::bitset<lookup_n> lookup_bits;
public:
    BitVector(std::bitset<n> bits) : bits(bits) {
        construct_Bs();
        construct_Bl();
        construct_lookup();
    }

    unsigned int rank(unsigned int i) {
        unsigned int x = i / s; // index in Bs
        unsigned int y = x / Bs_internal_num; // index in Bl
        unsigned int w = project_range<n>(bits, x * s, (x + 1) * s); // bits num in the current small block
        unsigned int j = i % s; // local position in the current small block
        return get_bl_bits(y) + get_bs_bits(x) + get_lookup_bits(w, j);
    }
};

int main() {
    std::cout << "n=" << n << " ,s=" << s << " ,l=" << l << std::endl;
    std::cout << "Big block total bits: " << Bl_n << std::endl;
    std::cout << "Small block total bits: " << Bs_n << std::endl;
    std::cout << "Lookup table total bits: " << lookup_n << std::endl;

    std::bitset<n> bits;
    for (int i = 0; i < n; i++) {
        if ((double)rand() / RAND_MAX > 0.5) bits[i] = 1;
        else bits[i] = 0;
    }
    BitVector bv(bits);

    for (int i = 0; i < n; i++) {
        if (naive_select(bits, i) == bv.rank(i)) continue;
        std::cout << "Test failed at " << i << "th test case. " << n-1-i << " tests remain" << std::endl;
        return 0;
    }
    std::cout << "Test passed" << std::endl;

    return 0;
}
