#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int mod = 998244353;
int root, root_1;
const int root_pw = 1<<19;
const int Maxm = root_pw;

int toPower(int a, int p)
{
    int res = 1;
    while (p) {
        if (p & 1) res = ll(res) * a % mod;
        p >>= 1; a = ll(a) * a % mod;
    }
    return res;
}

int Inv(int x) { return toPower(x, mod - 2); } 
 
void fft (vector <int> &a, bool invert) {
    int n = a.size();

    for (int i=1, j=0; i<n; ++i) {
        int bit = n >> 1;
        for (; j>=bit; bit>>=1)
            j -= bit;
        j += bit;
        if (i < j)
            swap (a[i], a[j]);
    }

    for (int len=2; len<=n; len<<=1) {
        int wlen = invert ? root_1 : root;
        for (int i=len; i<root_pw; i<<=1)
            wlen = int (wlen * 1ll * wlen % mod);
        for (int i=0; i<n; i+=len) {
            int w = 1;
            for (int j=0; j<len/2; ++j) {
                int u = a[i+j],  v = int (a[i+j+len/2] * 1ll * w % mod);
                a[i+j] = u+v < mod ? u+v : u+v-mod;
                a[i+j+len/2] = u-v >= 0 ? u-v : u-v+mod;
                w = int (w * 1ll * wlen % mod);
            }
        }
    }
    if (invert) {
        int nrev = Inv(n);
        for (int i=0; i<n; ++i)
            a[i] = int (a[i] * 1ll * nrev % mod);
    }
}

void Multiply(vector <int> &a, vector <int> &b)
{
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    a.resize(n);
    b.resize(n);
    fft(a, false);
    fft(b, false);
    for (int i = 0; i < n; i++)
        a[i] = ll(a[i]) * b[i] % mod;
    fft(a, true);
    while (a.size() > 1 && a.back() == 0)
        a.pop_back();
}

int main()
{
    root = toPower(3, 1904);
    root_1 = Inv(root);
    return 0;
}
