#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

#define MAX 40000

using namespace std;

double PI = acos(-1);

vector<complex<double>> FFT(vector<complex<double>> a, int flag)
{
    int N = a.size();
    vector<complex<double>> y;
    if(N == 1)
    {
        y.push_back(a[0]);
        return y;
    }

    vector<complex<double>> a_even;
    vector<complex<double>> a_odd;
    for(int i = 0; i < N/2; i++)
    {   
        a_even.push_back(a[i*2]);
        a_odd.push_back(a[i*2+1]);
    } 
    vector<complex<double>> S0 = FFT(a_even, flag);
    vector<complex<double>> S1 = FFT(a_odd, flag);

    complex<double> w_n(cos(PI*2/N), sin(PI*2/N));
    complex<double> w(1,0);
    for(int i = 0; i < N/2; i++)
    {
        if(flag == 1)
            y.push_back(S0[i] + w * S1[i]);
        else
            y.push_back((S0[i] + w * S1[i]));    
        w = (flag==1)?(w * w_n):(w / w_n);
    }
    w = complex<double>(1,0);
    for(int i = 0;i < N/2; i++)
    {
        if(flag == 1)
            y.push_back(S0[i] - w * S1[i]);
        else
            y.push_back((S0[i] - w * S1[i])); 
         w = (flag==1)?(w * w_n):(w / w_n);  
    }

    return y;
}   


int main()
{
    int n;
    cin >> n;

    vector<complex<double>> a;
    vector<complex<double>> b;
    int data;
    for(int i = 0; i <= n; i++)
    {
        cin >> data;
        a.push_back(data);
    }
    for(int i = 0; i <= n; i++)
    {
        cin >> data;
        b.push_back(data);
    }

    int extend_n;
    for(int k = 0; ;k++)
    {
        if(pow(2,k) < 2*(n+1) && pow(2,k+1) >= 2*(n+1) )
        {
            extend_n = (int)pow(2,k+1);
            break;
        }    
    }
    // TODO: delete me!
    cout << extend_n << endl;

    for(int i = n+1; i < extend_n; i++)
    {
        a.push_back(0);
        b.push_back(0);
    }

    vector<complex<double>> a_point = FFT(a,1);
    vector<complex<double>> b_point = FFT(b,1);

    // TODO: delete me!
    for (int i = 0; i < a_point.size(); i++) {
        printf("%d ", int(a_point[i].real()));
    }
    printf("\n");
    for (int i = 0; i < b_point.size(); i++) {
        printf("%d ", int(b_point[i].real()));
    }
    printf("\n");

    vector<complex<double>> c_point;
    for(int i = 0; i < a_point.size(); i++)
        c_point.push_back(a_point[i] * b_point[i]);

    // TODO: delete me!
    for (int i = 0; i < c_point.size(); i++) {
        printf("%d ", int(c_point[i].real()));
    }
    printf("\n");
    
    vector<complex<double>> c_exp = FFT(c_point, -1);

    // TODO: delete me!
    for (int i = 0; i < c_exp.size(); i++) {
        printf("%d ", int(c_exp[i].real()));
    }
    printf("\n\n\n");

    for(int i = 0; i <= 2*n; i++)
    {    
        double c_i = (c_exp[i]/complex<double>(extend_n,0)).real();
        if( fabs(c_i) > 0.001 )
            cout << (int)round(c_i) << ' ';   
        else
            cout << 0 << ' ';    
    }     
    cout << endl;
}
