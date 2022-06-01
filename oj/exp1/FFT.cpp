#include <math.h>
#include <stdio.h>
#include <complex>
using namespace std;

double PI = acos(-1);

void DFT(complex<double>* A, int n, complex<double>* y) {
    /* 输入n是数组A的长度，应为2的正数幂
     * 输出y是数组A的DFT
     */
    if (n == 1) {
        y[0] = A[0];
        return;
    }

    complex<double> wn(cos(2 * PI / n), sin(2 * PI / n));
    complex<double> w(1, 0);

    complex<double>* a0 = new complex<double>[n / 2];
    complex<double>* a1 = new complex<double>[n / 2];
    for (int i = 0; i < n; i = i + 2) {
        a0[i / 2] = A[i];
    }
    for (int i = 1, j = 0; i < n; i = i + 2, j++) {
        a1[j] = A[i];
    }

    complex<double>* y0 = new complex<double>[n / 2];
    complex<double>* y1 = new complex<double>[n / 2];
    DFT(a0, n / 2, y0);
    DFT(a1, n / 2, y1);

    for (int k = 0; k < n / 2; k++) {
        y[k] = y0[k] + w * y1[k];
        y[k + n / 2] = y0[k] - w * y1[k];
        w = w * wn;
    }
}

void InverseDFT(complex<double>* y, int n, complex<double>* A) {
    /* 输入n是数组A的长度，应为2的正数幂
     * 输出A是y的逆DFT
     */
    if (n == 1) {
        A[0] = y[0];
        return;
    }

    complex<double> wn(cos(2 * PI / n), sin(2 * PI / n));  // 注意这里与DFT里的设置略有不同
    complex<double> w(1, 0);

    complex<double>* y0 = new complex<double>[n / 2];
    complex<double>* y1 = new complex<double>[n / 2];
    for (int i = 0; i < n; i = i + 2) {
        y0[i / 2] = y[i];
    }
    for (int i = 1, j = 0; i < n; i = i + 2, j++) {
        y1[j] = y[i];
    }

    complex<double>* a0 = new complex<double>[n / 2];
    complex<double>* a1 = new complex<double>[n / 2];
    InverseDFT(y0, n / 2, a0);
    InverseDFT(y1, n / 2, a1);

    for (int k = 0; k < n / 2; k++) {
        A[k] = (a0[k] + w * a1[k]);
        A[k + n / 2] = (a0[k] - w * a1[k]);
        w = w / wn;
    }
}

int main() {
    int n;
    scanf("%d", &n);

    int count = 0;
    int result_len;
    while (true) {
        if (pow(2, count-1) < 2*n && pow(2, count) >= 2*n) {
            result_len = (int)pow(2, count);
            break;
        }
        count++;
    }

    complex<double>*A = new complex<double>[result_len], *B = new complex<double>[result_len], *C = new complex<double>[result_len];
    complex<double>*A1 = new complex<double>[result_len], *B1 = new complex<double>[result_len], *C1 = new complex<double>[result_len];

    for (int i = 0; i <= n; i++) {
        int temp;
        scanf("%d", &temp);
        A[i].real(temp);
        A[i].imag(0.0);
    }
    for (int i = n + 1; i <= result_len; i++) {
        A[i].real(0.0);
        A[i].imag(0.0);
    }
    for (int i = 0; i <= n; i++) {
        int temp;
        scanf("%d", &temp);
        B[i].real(temp);
        B[i].imag(0.0);
    }
    for (int i = n + 1; i <= result_len; i++) {
        B[i].real(0.0);
        B[i].imag(0.0);
    }

    DFT(A, result_len, A1);
    DFT(B, result_len, B1);  

    for (int i = 0; i <= result_len; i++) {
        C1[i] = A1[i] * B1[i];
    }

    InverseDFT(C1, result_len, C);

    for (int i = 0; i <= 2 * n; i++) {
        complex<double> complex_len(result_len, 0.0);
        double c = (C[i] / complex_len).real();
        if (fabs(c) > 10e-6) {
            printf("%d ", int(round(c)));
        } else {
            printf("0 ");
        }
    }
    printf("\n");
}