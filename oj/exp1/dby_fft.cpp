#include <iostream>
#include <complex>

// #define TEST

using namespace std;

double PI = acos(-1);

class poly
{
private:
	/* data */
public:
	complex<double> *a;
	int extended_n;
	poly(int n)
	{
		extended_n = 1;
		while (extended_n <= 2 * n)
		{
			extended_n *= 2;
		}
		a = new complex<double>[extended_n];
	};

	~poly()
	{
		delete a;
	};

	void operator*(const poly &b)
	{
		for (int i = 0; i < extended_n; i++)
		{
			a[i] *= b.a[i];
		}
	}

	void PRINT(int n)
	{
		for (int i = 0; i <= 2 * n; i++)
		{
			printf("%d ", (int)(a[i].real() / extended_n + 0.5));
		}
	}

	void fft(int n)
	{
		if (n == 1)
		{
			return;
		}
		complex<double> wn(cos(2 * PI / n), sin(2 * PI / n)), w(1, 0);
		poly a0(n / 2), a1(n / 2);
		for (int i = 0; i < n / 2; i++)
		{
			a0.a[i] = a[2 * i], a1.a[i] = a[2 * i + 1];
		}
		a0.fft(n / 2), a1.fft(n / 2);
		for (int i = 0; i < n / 2; i++, w *= wn)
		{
			a[i] = a0.a[i] + w * a1.a[i], a[i + n / 2] = a0.a[i] - w * a1.a[i];
		}
	}

	void reverse_dft(int n)
	{
		if (n == 1)
		{
			return;
		}
		complex<double> wn(cos(-2 * PI / n), sin(-2 * PI / n)), w(1, 0);
		poly a0(n / 2), a1(n / 2);
		for (int i = 0; i < n / 2; i++)
		{
			a0.a[i] = a[2 * i], a1.a[i] = a[2 * i + 1];
		}
		a0.reverse_dft(n / 2), a1.reverse_dft(n / 2);
		for (int i = 0; i < n / 2; i++, w *= wn)
		{
			a[i] = a0.a[i] + w * a1.a[i], a[i + n / 2] = a0.a[i] - w * a1.a[i];
		}
	}
};

int main(void)
{
	int n;
	cin >> n;
	poly f(n), d(n);

	for (int i = 0; i <= n; i++)
	{
		cin >> f.a[i];
	}
	for (int i = 0; i <= n; i++)
	{

		cin >> d.a[i];
	}
	f.fft(f.extended_n);
	d.fft(f.extended_n);
	f *d;
	f.reverse_dft(f.extended_n);
	f.PRINT(n);
	return 0;
}
