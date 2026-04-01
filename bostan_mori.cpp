#include<bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
using tiii = tuple<int, int, int>;
using ldb = long double;
//#define double ldb

template<class T>
ostream& operator<<(ostream& os, const pair<T, T> pr) {
  return os << pr.first << ' ' << pr.second;
}
template<class T, size_t N>
ostream& operator<<(ostream& os, const array<T, N> &arr) {
  for(const T &X : arr)
    os << X << ' ';
  return os;
}
template<class T>
ostream& operator<<(ostream& os, const vector<T> &vec) {
  for(const T &X : vec)
    os << X << ' ';
  return os;
}

//////////////////////////////////////////////////
//template name: mint
//author: __Shioko(Misuki)
//last update: 2022/06/01
//note: inversion only works when MOD is a prime
struct mint {
  static const long long MOD = 998244353;
  long long _val;

  mint(long long init = 0) {
    _val = init % MOD;
    (*this).norm();
  }

  mint POW(long long index) {
    if (index == 0)
      return mint(1ll);
    mint base = *this;
    mint res = (base == 0ll ? 0ll : 1ll);
    while(index) {
      if (index & 1)
        res *= base;
      base *= base, index >>= 1;
    }
    return res;
  }

  mint inv() { return (*this).POW(MOD - 2); }

  mint& norm() {
    if (_val >= MOD)
      _val -= MOD;
    if (_val < 0)
      _val += MOD;
    return *this;
  }

  mint& operator+=(mint b) {
    _val += b._val;
    return (*this).norm();
  }
  mint& operator-=(mint b) {
    _val -= b._val;
    return (*this).norm();
  }
  mint& operator*=(mint b) {
    _val = (_val * b._val) % MOD;
    return *this;
  }
  mint& operator/=(mint b) {
    _val = (_val * b.inv()._val) % MOD;
    return *this;
  }

  mint& operator++() {
    _val += 1;
    return (*this).norm();
  }
  mint& operator--() {
    _val -= 1;
    return (*this).norm();
  }
  mint operator++(signed) {
    mint tmp = *this;
    ++(*this);
    return tmp;
  }
  mint operator--(signed) {
    mint tmp = *this;
    --(*this);
    return tmp;
  }

  mint operator-() { return mint(-_val); }
  bool operator==(mint b) { return _val == b._val; }
  bool operator!=(mint b) { return _val != b._val; }

  friend mint operator+(mint a, mint b) { return a += b; }
  friend mint operator-(mint a, mint b) { return a -= b; }
  friend mint operator*(mint a, mint b) { return a *= b; }
  friend mint operator/(mint a, mint b) { return a /= b; }

  friend ostream& operator<<(ostream& os, const mint& b) {
    return os << b._val;
  }
  friend istream& operator>>(istream& is, mint& b) {
    long long val;
    is >> val;
    b = mint(val);
    return is;
  }
};
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//template name: FPS
//author: __Shioko(Misuki)
//last update: 2023/01/24
//include: mint(with NTT-able MOD)

bool init_NTT = false;

struct FPS : vector<mint> {

  using vector<mint>::vector;

  static const int K = 23, C = 119, R = 3;
  static mint w[K + 1], w_inv[K + 1];
  static bool init_NTT;

  void NTT(bool inverse) {
    if (!init_NTT) {
      w[K] = mint(R).POW(C);
      for(int i = K - 1; i >= 0; i--)
        w[i] = w[i + 1] * w[i + 1];
      for(int i = 0; i <= K; i++)
        w_inv[i] = 1 / w[i];

      init_NTT = true;
    }
    int n = this -> size();
    FPS tmp = *this;
    for(int i = 0; i < this -> size(); i++) {
      int idx = 0;
      int lgn = __lg(n);
      for(int j = lgn - 1; j >= 0; j--)
        idx = (idx << 1) | ((i >> (lgn - j - 1)) & 1);
      (*this)[idx] = tmp[i];
    }

    for(int l = 2; l <= n; l <<= 1) {
      mint w_l = (inverse ? w_inv[__lg(l)] : w[__lg(l)]);
      for(int i = 0; i < n; i += l) {
        mint w = 1;
        for(int j = 0; j < (l >> 1); j += 1) {
          mint t = (*this)[i + j + l / 2] * w;
          (*this)[i + j + l / 2] = (*this)[i + j] - t;
          (*this)[i + j] = (*this)[i + j] + t;
          w *= w_l;
        }
      }
    }

    if (inverse) {
      mint INV = mint(1) / n;
      for(int i = 0; i < n; i++)
        (*this)[i] *= INV;
    }
  }
  FPS& operator+=(FPS &b) {
    if (this -> size() < b.size()) this -> resize(b.size(), 0);
    for(int i = 0; i < b.size(); i++)
      (*this)[i] += b[i];
    return *this;
  }
  FPS& operator-=(FPS &b) {
    if (this -> size() < b.size()) this -> resize(b.size(), 0);
    for(int i = 0; i < b.size(); i++)
      (*this)[i] -= b[i];
    return *this;
  }
  FPS& operator*=(FPS b) {
    int mxSz = (int)this -> size() + (int)b.size() - 1;
    int n = (mxSz == 1) ? 2 : (1 << (__lg(mxSz - 1) + 1));

    this -> resize(n, 0);
    b.resize(n, 0);

    this -> NTT(false);
    b.NTT(false);
    for(int i = 0; i < n; i++)
      (*this)[i] *= b[i];
    this -> NTT(true);
    this -> resize(mxSz);
    return *this;
  }
  FPS& operator*=(mint b) {
    for(int i = 0; i < this -> size(); i++)
      (*this)[i] *= b;
    return (*this);
  }

  FPS integral() {
    vector<mint> Inv(this -> size() + 1);
    Inv[1] = 1;
    for(int i = 2; i < Inv.size(); i++)
      Inv[i] = (mint::MOD - mint::MOD / i) * Inv[mint::MOD % i];
    FPS Q(this -> size() + 1, 0);
    for(int i = 0; i < this -> size(); i++)
      Q[i + 1] = (*this)[i] * Inv[i + 1];

    return Q;
  }

  FPS derivative() {
    assert(!this -> empty());

    FPS Q((int)(this -> size()) - 1);
    for(int i = 1; i < this -> size(); i++)
      Q[i - 1] = (*this)[i] * i;

    return Q;
  }

  FPS inv(int k) { // 1 / FPS (mod x^k)
    assert(!this -> empty() and (*this)[0] != 0);

    FPS Q(1, 1 / (*this)[0]);
    for(int i = 1; (1 << (i - 1)) < k; i++) {
      FPS P = (*this);
      P.resize(1 << i, 0);
      Q = Q * (FPS(1, 2) - P * Q);
      Q.resize(1 << i, 0);
    }
    Q.resize(k);

    return Q;
  }

  FPS log(int k) {
    assert(!this -> empty() and (*this)[0] == 1);

    FPS Q = *this;
    Q = (Q.derivative() * Q.inv(k));
    Q.resize(k - 1);
    return Q.integral();
  }

  FPS exp(int k) {
    assert(!this -> empty() and (*this)[0] == 0);

    FPS Q(1, 1);
    for(int i = 1; (1 << (i - 1)) < k; i++) {
      FPS P = (*this);
      P.resize(1 << i, 0);
      Q = Q * (FPS(1, 1) + P - Q.log(1 << i));
      Q.resize(1 << i, 0);
    }
    Q.resize(k);

    return Q;
  }

  FPS pow(long long idx, int k) {
    if (idx == 0) {
      FPS res(k, 0);
      res[0] = 1;
      return res;
    }
    for(int i = 0; i < this -> size() and i * idx < k; i++) {
      if ((*this)[i] != 0) {
        mint Inv = 1 / (*this)[i];
        FPS Q((int)this -> size() - i);
        for(int j = i; j < this -> size(); j++)
          Q[j - i] = (*this)[j] * Inv;
        Q = (Q.log(k) * idx).exp(k);
        FPS Q2(k, 0);
        mint Pow = (*this)[i].POW(idx);
        for(int j = 0; j + i * idx < k; j++)
          Q2[j + i * idx] = Q[j] * Pow;
        return Q2;
      }
    }
    return FPS(k, 0);
  }

  friend FPS operator+(FPS a, FPS b) { return a += b; }
  friend FPS operator-(FPS a, FPS b) { return a -= b; }
  friend FPS operator*(FPS a, FPS b) { return a *= b; }
  friend FPS operator*(FPS a, mint b) { return a *= b; }
};

bool FPS::init_NTT = false;
mint FPS::w[K + 1];
mint FPS::w_inv[K + 1];
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//template name: Bostan-Mori
//author: __Shioko(Misuki)
//last update: 2023/01/25
//include: mint(with NTT-able MOD), FPS

mint BostanMori(FPS P, FPS Q, long long k) {
  assert(!P.empty() and !Q.empty() and Q[0] != 0);

  int d = max((int)P.size(), (int)Q.size() - 1);
  P.resize(d, 0);
  Q.resize(d + 1, 0);

  int sz = (2 * d + 1 == 1 ? 2 : (1 << __lg(2 * d) + 1));
  while(k) {
    FPS Qneg(sz);
    for(int i = 0; i < Q.size(); i++)
      Qneg[i] = Q[i] * ((i & 1) ? -1 : 1);
    Qneg.NTT(false);

    FPS U(sz), V(sz);
    for(int i = 0; i < P.size(); i++)
      U[i] = P[i];
    for(int i = 0; i < Q.size(); i++)
      V[i] = Q[i];

    U.NTT(false);
    V.NTT(false);
    for(int i = 0; i < sz; i++)
      U[i] *= Qneg[i];
    for(int i = 0; i < sz; i++)
      V[i] *= Qneg[i];
    U.NTT(true);
    V.NTT(true);

    for(int i = k & 1; i <= 2 * d - 1; i += 2)
      P[i >> 1] = U[i];
    for(int i = 0; i <= 2 * d; i += 2)
      Q[i >> 1] = V[i];
    k >>= 1;
  }

  return P[0] / Q[0];
}

//////////////////////////////////////////////////