typedef long long ll;

const int mod = 1000000007;

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

void FWHT(int P[], int n, bool inverse) {
    for (int len = 1; 2 * len <= n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                int u = P[i + j];
                int v = P[i + len + j];
                P[i + j] = (u + v) % mod;
                P[i + len + j] = (u - v + mod) % mod;
            }
        }
    }
    if (inverse) {
        int inv = Inv(n);
        for (int i = 0; i < n; i++)
            P[i] = ll(P[i]) * inv % mod;
    }
}
