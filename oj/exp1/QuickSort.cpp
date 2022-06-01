#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct element{
    int i1;
    int i2;
    int i3;
    /*
    bool operator<=(const element &e) {
        if (i1 != e.i1) {
            return i1 < e.i1;
        } else if (i2 != e.i2) {
            return i2 < e.i2;
        } else {
            return i3 < e.i3;
        }
    }
    bool operator==(const element &e) {
        return (i1 == e.i1 && i2 == e.i2 && i3 == e.i3);
    }
    */
} element;

inline bool leq(const element &e1, const element &e2) {
    if (e1.i1 != e2.i1) {
            return e1.i1 < e2.i1;
        } else if (e1.i2 != e2.i2) {
            return e1.i2 < e2.i2;
        } else {
            return e1.i3 <= e2.i3;
        }
}

/*
inline void swap(element &e1, element &e2) {
    element temp = e1;
    e1 = e2;
    e2 = temp;
}
*/

int partition(element* array, int p, int r) {
    // srand(int(time(nullptr)));
    int k = rand() % (r-p+1) + p;
    element pivot = array[k];
    // swap(array[k], array[r]);
    element temp = array[k];
    array[k] = array[r];
    array[r] = temp;
    int i = p - 1;
    int j = p;
    for (; j < r; j++) {
        if (leq(array[j], pivot)) {
            i++;
            // swap(array[++i], array[j]);
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    // swap(array[i+1], array[r]);
    temp = array[i+1];
    array[i+1] = array[r];
    array[r] = temp;
    return i+1;
}

void QuickSort(element* array, int p, int r) {
    if (p >= r)
        return;
    int q = partition(array, p, r);
    QuickSort(array, p, q - 1);
    QuickSort(array, q + 1, r);
}

int main() {
    int n;
    scanf("%d", &n);
    element* array = new element[n];
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &array[i].i1, &array[i].i2, &array[i].i3);
    }
    srand(int(time(nullptr)));
    QuickSort(array, 0, n-1);
    for (int i = 0; i < n; i++) {
        printf("%d %d %d\n", array[i].i1, array[i].i2, array[i].i3);
    }
    return 0;
}