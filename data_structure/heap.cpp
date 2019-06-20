#include <iostream>
#include <vector>
using namespace std;

class Heap {
    vector<int> heap;
    int sz = 0;

public:
    Heap(int default_n) {
        heap.resize(default_n);
    }

    void push(int x) {
        int i = sz++;
        if (i >= heap.size()) heap.resize(sz * 2);
        while (i > 0) {
            int p = (i - 1) / 2;
            if (heap[p] <= x) break;
            heap[i] = heap[p];
            i = p;
        }
        heap[i] = x;
    }

    int pop() {
        int ret = heap[0];
        int x = heap[--sz];

        int i = 0;
        while (2 * i + 1 < sz) {
            int a = 2 * i + 1;
            int b = 2 * i + 2;
            if (b < sz && heap[b] < heap[a]) a = b;

            if (heap[a] >= a) break;

            heap[i] = heap[a];
            i = a;
        }

        heap[i] = x;
        return ret;
    }

    bool empty() {
        return sz == 0;
    }
};

int main() {
    auto heap = new Heap(1);

    heap->push(1);
    heap->push(3);
    heap->push(2);
    while (!heap->empty()) {
        cout << heap->pop() << endl;
    }
}