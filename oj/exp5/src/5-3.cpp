#include <memory.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct BigInteger {
    int d[2100];
    int len;
    int sign;

    inline bool greater_eq(BigInteger a, BigInteger b, int last_dg) {
        // 被除数a以下标last_dg为最低位, 是否可以减去除数b而保持非负

        // 有可能被除数剩余的部分比除数长，这个情况下最多多出 1 位，故如此判断即可
        int len = b.len;
        if (a.d[last_dg + len] > 0)
            return true;
        for (int i = len - 1; i >= 0; i--) {
            if (a.d[last_dg + i] > b.d[i]) {
                return true;
            } else if (a.d[last_dg + i] < b.d[i]) {
                return false;
            }
        }
        return true;
    }

    BigInteger() {
        memset(d, 0, sizeof(d));
        len = 0;
        sign = 1;
    }
    BigInteger(string s) {
        if (s[0] == '-') {
            sign = -1;
            len = s.size() - 1;
            for (int i = 0; i < len; i++) {
                d[i] = s[len - i] - '0';
            }
        } else {
            sign = 1;
            len = s.size();
            for (int i = 0; i < len; i++) {
                d[i] = s[len - i - 1] - '0';
            }
        }
    }
    BigInteger(int i) {
        BigInteger(to_string(i));
    }
    void operator=(const BigInteger &bi) {
        memset(d, 0, sizeof(d));
        len = bi.len;
        for (int i = 0; i < len; i++) {
            d[i] = bi.d[i];
        }
        sign = bi.sign;
    }
    bool operator==(const BigInteger &bi) {
        if (len != bi.len) {
            return false;
        } else {
            for (int i = len - 1; i >= 0; i--) {
                if (d[i] != bi.d[i]) {
                    return false;
                }
            }
            return true;
        }
    }
    bool operator>(const BigInteger &bi) {
        // 比较的是绝对值大小
        if (len > bi.len) {
            return true;
        } else if (len < bi.len) {
            return false;
        } else {
            for (int i = len - 1; i >= 0; i--) {
                if (d[i] > bi.d[i]) {
                    return true;
                } else if (d[i] < bi.d[i]) {
                    return false;
                }
            }
            return false;
        }
    }
    bool operator<(const BigInteger &bi) {
        if (len < bi.len) {
            return true;
        } else if (len > bi.len) {
            return false;
        } else {
            for (int i = len - 1; i >= 0; i--) {
                if (d[i] < bi.d[i]) {
                    return true;
                } else if (d[i] > bi.d[i]) {
                    return false;
                }
            }
            return false;
        }
    }
    BigInteger operator-(BigInteger &bi) {
        // 两个大 正 整数相减, 且被减数(self)不小于减数(bi)
        BigInteger ret;
        BigInteger self = (*this);
        for (int i = 0; i < self.len || i < bi.len; i++) {
            if (self.d[i] < bi.d[i]) {
                int j = i + 1;
                while (self.d[j] == 0 && j < len) {
                    j++;
                }
                if (j < len) {
                    // d[j] > 0
                    self.d[j]--;
                    j--;
                    while (j > i) {
                        self.d[j] += 9;
                        j--;
                    }
                    self.d[i] += 10;
                }
            }
            ret.d[ret.len++] = self.d[i] - bi.d[i];
        }
        while (ret.len - 1 >= 1 && ret.d[ret.len - 1] == 0) {
            ret.len--;
        }
        return ret;
    }
    BigInteger operator+(BigInteger &bi) {
        // 两个大整数相加
        BigInteger ret;
        if (sign == bi.sign) {
            ret.sign = sign;
            int carry = 0;
            for (int i = 0; i < len || i < bi.len; i++) {
                int tmp = d[i] + bi.d[i] + carry;
                ret.d[ret.len++] = tmp % 10;
                carry = tmp / 10;
            }
            if (carry > 0) {
                ret.d[ret.len++] = carry;
            }
        } else {
            // sign != bi.sign
            if ((*this) > bi) {
                ret = (*this) - bi;
                ret.sign = sign;
            } else if ((*this) < bi) {
                ret = bi - (*this);
                ret.sign = bi.sign;
            }
        }
        return ret;
    }
    BigInteger operator*(int i32) {
        // 大整数与普通integer的乘法
        BigInteger bi;
        int carry = 0;

        if (i32 * (*this).sign < 0) {
            bi.sign = -1;
        }
        if (i32 < 0) {
            i32 = -i32;
        }

        for (int i = 0; i < len; i++) {
            int tmp = d[i] * i32 + carry;
            bi.d[bi.len++] = tmp % 10;
            carry = tmp / 10;
        }
        while (carry != 0) {
            bi.d[bi.len++] = carry % 10;
            carry /= 10;
        }
        return bi;
    }
    BigInteger operator/(int i32) {
        // 大整数与普通integer的除法
        BigInteger bi;
        if ((*this).sign * i32 < 0) {
            bi.sign = -1;
        }
        if (i32 < 0) {
            i32 = -i32;
        }
        bi.len = len;
        int r = 0;
        for (int i = len - 1; i >= 0; i--) {
            r = r * 10 + d[i];
            if (r < i32) {
                bi.d[i] = 0;
            } else {
                bi.d[i] = r / i32;
                r = r % i32;
            }
        }
        while (bi.len - 1 >= 1 && bi.d[bi.len - 1] == 0) {
            bi.len--;
        }
        return bi;
    }
    BigInteger operator%(const int &i32) {
        BigInteger divisor = (*this) / i32;
        BigInteger product = divisor * i32;
        return ((*this) - product);
    }
    BigInteger operator*(const BigInteger &bi) {
        // 大整数与大整数的乘法
        BigInteger ret;
        if (len == 0 || bi.len == 0) {
            return ret;
        }
        ret.len = len + bi.len;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < bi.len; j++) {
                int newval = ret.d[i + j] + d[i] * bi.d[j];
                int carry = newval / 10;
                ret.d[i + j] = newval % 10;

                for (int k = i + j + 1; carry > 0 && k < ret.len; k++) {
                    ret.d[k - 1] = newval % 10;
                    newval = ret.d[k] + carry;
                    ret.d[k] = newval % 10;
                    carry = newval / 10;
                }
            }
        }
        for (int k = ret.len - 1; ret.d[k] == 0; k--) {
            ret.len--;
        }
        if (sign != bi.sign) {
            ret.sign = -1;
        } else {
            ret.sign = 1;
        }
        return ret;
    }
    pair<BigInteger, BigInteger> operator/(const BigInteger &bi) {
        // 大整数与大整数的除法
        if (bi.len == 0) {
            cout << "Error: divisor cannot be zero!" << endl;
            exit(-1);
        }
        BigInteger q, r;
        r = (*this);
        BigInteger ten("10");
        // 使用减法计算除法
        for (int i = r.len - bi.len; i >= 0; i--) {
            // 计算商的第i位
            while (greater_eq(r, bi, i)) {
                // 若可以减, 则减
                for (int j = 0; j < bi.len; j++) {
                    r.d[i + j] -= bi.d[j];
                    if (r.d[i + j] < 0) {
                        r.d[i + j + 1] -= 1;
                        r.d[i + j] += 10;
                    }
                }
                q.d[i] += 1;
                for (int k = r.len - 1; k >= 0 && r.d[k] == 0; k--) {
                    r.len--;
                }
                for (int k = q.len; q.d[k] > 0; k++) {
                    q.len++;
                }
            }
        }
        return make_pair(q, r);
    }
    BigInteger operator%(BigInteger &bi) {
        // 大整数与大整数的取模
        if ((*this).sign == -1) {
            BigInteger tmp = (*this) + bi;
            return (tmp / bi).second;
        }
        return (*this / bi).second;
    }
};

void print(const BigInteger &bi) {
    if (bi.len == 0) {
        cout << 0 << endl;
        return;
    }
    for (int i = bi.len - 1; i >= 0; i--) {
        cout << bi.d[i];
    }
    cout << endl;
}

int Big2int(const BigInteger &bi) {
    if (bi.len == 0) {
        return 0;
    }
    int ret = 0;
    int radix = 1;
    for (int i = 0; i < bi.len; i++) {
        ret += radix * bi.d[i];
        radix *= 10;
    }
    if (bi.sign == -1) {
        ret = -ret;
    }
    return ret;
}

BigInteger extend_gcd(BigInteger a, BigInteger b, int &x, int &y) {
    // 假设a > b
    // if (b == 0) {
    if (b.len == 0 || (b.len == 1 && b.d[0] == 0)) {
        x = 1;
        y = 0;
        return a;
    }
    BigInteger tmp = a % b;
    BigInteger gcd = extend_gcd(b, tmp, y, x);
    int tmp2 = Big2int((a / b).first * x);
    y = y - tmp2;
    return gcd;
}

BigInteger extend_gcd(BigInteger a, int b, int &x, int &y) {
    // 假设a > b
    
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    // BigInteger gcd = extend_gcd(b, a%b, y, x);
    BigInteger bb(to_string(b));
    BigInteger gcd = extend_gcd(bb, Big2int(a % bb), y, x);
    y = y - Big2int((a / b) * x);
    return gcd;
    
}

BigInteger ChinaRemainder(vector<int> &r, vector<int> &p, int k) {
    BigInteger M("1");
    int BOUND = 20000;
    int Mint = 1;
    for (int i = 0; i < k; i++) {
        Mint *= p[i];
        if (Mint > BOUND) {
            M = M * Mint;
            Mint = 1;
        }
        // M = M * p[i];
    }
    M = M * Mint;

    BigInteger ans("0");
    int s, t;
    BigInteger m;
    for (int i = 0; i < k; i++) {
        m = M / p[i];
        extend_gcd(m, p[i], t, s);
        BigInteger tmp = (m * (t * r[i]));
        ans = (ans + tmp + M) % M;
    }
    return (ans + M) % M;
}

int main() {
    int k;
    cin >> k;
    vector<int> p(k);
    vector<int> r(k);
    for (int i = 0; i < k; i++) {
        cin >> r[i] >> p[i];
    }

    // test BigInteger
    /*
    BigInteger bi1("100");
    BigInteger bi2("25");
    print(bi1 + bi2);
    print(bi1 - bi2);
    print(bi1 * bi2);
    print((bi1 / bi2).first);
    print(bi1 % bi2);

    BigInteger bi1("-1");
    BigInteger bi2("33");
    print(bi1 % bi2);
    */

    BigInteger result = ChinaRemainder(r, p, k);
    print(result);
    return 0;
}