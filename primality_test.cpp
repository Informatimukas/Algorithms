// The algorithm is based on https://cp-algorithms.com/algebra/primality_tests.html

typedef __int128 lll;
typedef long long ll;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll binpower(ll base, ll e, ll mod) {
    ll result = 1;
    base %= mod;
    while (e) {
        if (e & 1)
            result = (lll)result * base % mod;
        base = (lll)base * base % mod;
        e >>= 1;
    }
    return result;
}

bool check_composite(ll n, ll a, ll d, int s) {
    ll x = binpower(a, d, n);
    if (x == 1 || x == n - 1)
        return false;
    for (int r = 1; r < s; r++) {
        x = (lll)x * x % n;
        if (x == n - 1)
            return false;
    }
    return true;
}

bool MillerRabin(ll n, int iter = 4) {
    if (n < 4)
        return n == 2 || n == 3;

    int s = 0;
    ll d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    for (int i = 0; i < iter; i++) {
        ll a = uniform_int_distribution<ll>(2, n - 2)(rng);
        if (check_composite(n, a, d, s))
            return false;
    }
    return true;
}
