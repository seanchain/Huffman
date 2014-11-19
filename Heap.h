#ifndef HEAP_H
#define HEAP_H

template <typename E>
class Heap {
private:
    // Pointer to the heap array
    E* heap;
    // Maximum size of the heap
    int maxsize;
    // Number of elements now in the heap
    int n;

    /* Helper function to put element in its correct place */
    void siftdown(int pos) {
        while (!isLeaf(pos)) {
            int lc = leftchild(pos);
            int rc = rightchild(pos);

            if ((rc<n) && heap[rc]->weight()<heap[lc]->weight()) {
                lc = rc;
            }
            if (heap[pos]->weight() < heap[lc]->weight()) {
                return;
            }

            swap(heap, pos, lc);
            pos = lc;
        }
    }

    /* Swap function */
    void swap(E arr[], int elemcomp1, int elemcomp2) {
        E temp = arr[elemcomp1];
        arr[elemcomp1] = arr[elemcomp2];
        arr[elemcomp2] = temp;
    }

public:
    Heap(E* h, int num, int max) {
        heap = h;
        n = num;
        maxsize = max;
        buildHeap();
    }

    int size() const {
        return n;
    }

    bool isLeaf(int pos) const {
        return ((pos>=n/2) && (pos<n));
    }

    int leftchild(int pos) const {
        return 2*pos+1;
    }

    int rightchild(int pos) const {
        return 2*pos+2;
    }

    int parent(int pos) const {
        return (pos-1)/2;
    }

    void buildHeap() {
        for (int i=n/2-1; i>=0; i--) {
            siftdown(i);
        }
    }

    void insert(const E& it) {
        if (n>maxsize) {
            //cout << "Full Heap!" << endl;
            return;
        }

        int curr = n++;
        heap[curr] = it;

        while ((curr!=0) && (heap[curr]->weight()<heap[parent(curr)]->weight())) {
            swap(heap, curr, parent(curr));
            curr = parent(curr);
        }
    }

    E removefirst() {
        swap(heap, 0, --n);
        if (n != 0) {
            siftdown(0);
        }

        return heap[n];
    }
};

#endif // HEAP_H
