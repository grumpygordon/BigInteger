#include "big_integer.h"
#include "vector.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <iostream>

big_integer::big_integer() : number(1), sign(false) {
    number[0] = 0;
}

big_integer::big_integer(big_integer const &other) : number(other.number), sign(other.sign) {}

big_integer::big_integer(int a) : number(1), sign(a < 0) {
    number[0] = (a == INT32_MIN) ? (uint32_t) a : abs(a);
}

big_integer::big_integer(uint32_t a) : number(1), sign(false) {
    number[0] = a;
}

big_integer::big_integer(std::string const &s) {
    if (s == "")
        return;
    number = {0};
    sign = 0;
    for (size_t i = (s[0] == '-'); i < s.size(); i++) {
        *this *= 10;
        *this += s[i] - '0';
    }
    sign = (*this != 0 && s[0] == '-');
}

big_integer::~big_integer() = default;

big_integer &big_integer::operator=(big_integer const &a) {
    sign = a.sign;
    number = a.number;
    return *this;
}

big_integer &big_integer::operator+=(big_integer const &a) {
    return *this = *this + a;
}

big_integer &big_integer::operator-=(big_integer const &a) {
    return *this = *this - a;
}

big_integer &big_integer::operator*=(big_integer const &a) {
    return *this = *this * a;
}

big_integer &big_integer::operator/=(big_integer const &a) {
    return *this = *this / a;
}

big_integer &big_integer::operator/=(uint32_t a) {
    return *this = *this / a;
}

big_integer &big_integer::operator/=(int a) {
    return *this = *this / a;
}

big_integer &big_integer::operator%=(big_integer const &a) {
    return *this = *this % a;
}

big_integer &big_integer::operator&=(big_integer const &a) {
    return *this = *this & a;
}

big_integer &big_integer::operator|=(big_integer const &a) {
    return *this = *this | a;
}

big_integer &big_integer::operator^=(big_integer const &a) {
    return *this = *this ^ a;
}

big_integer &big_integer::operator<<=(int a) {
    return *this = *this << a;
}

big_integer &big_integer::operator>>=(int a) {
    return *this = *this >> a;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    if (number.size() == 1 && number[0] == 0)
        return big_integer(*this);
    big_integer tmp(*this);
    tmp.sign ^= 1;
    return tmp;
}

big_integer big_integer::operator~() const {
    return -(*this + 1);
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer big_integer::operator++(int) {
    big_integer res = *this;
    ++*this;
    return res;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
}

big_integer big_integer::operator--(int) {
    big_integer res = *this;
    --*this;
    return res;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return a.sign == b.sign && a.number == b.number;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign ^ b.sign) {
        return a.sign;
	}
	if (a.number.size() != b.number.size())
		return a.sign ^ (a.number.size() < b.number.size());
	for (ptrdiff_t i = a.number.size() - 1; i >= 0; i--)
		if (a.number[i] != b.number[i])
			return a.sign ^ (a.number[i] < b.number[i]);
	return 0;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

std::string to_string(big_integer const &a) {
    std::string s;
    big_integer w(a);
	const char pw = (1ll << 32) % 10;
    while (w != 0) {
		char digit = 0;
        for (ptrdiff_t i = w.number.size() - 1; i >= 0; i--) {
            digit = (digit * pw + w.number[i]) % 10;
        }
        s += std::to_string((uint32_t)digit);
        w /= 10;
    }
    if (s == "")
		s = "0";
    if (a.sign)
        s += '-';
    std::reverse(s.begin(), s.end());
    return s;
}

big_integer abs(big_integer a) {
    a.sign = false;
    return a;
}

void normalize(big_integer &a) {
    while (a.number.size() > 1 && a.number.back() == 0)
        a.number.pop_back();
    if (a.number.size() == 1 && a.number[0] == 0)
        a.sign = false;
}

big_integer operator+(big_integer a, big_integer const &b) {
    big_integer abs_a = abs(a);
    big_integer abs_b = abs(b);
    if (a.sign && b.sign)
        return -(abs_a + abs_b);
    if (a.sign && !b.sign)
        return abs_b - abs_a;
    if (!a.sign && b.sign)
        return abs_a - abs_b;

    uint32_t w = 0;
    uint64_t s;
    for (size_t i = 0; i < std::max(a.number.size(), b.number.size()) || w > 0; i++) {
        if (i == a.number.size())
            a.number.push_back(0);
        s = (uint64_t) a.number[i] + w + (i < b.number.size() ? b.number[i] : 0);
        a.number[i] = (uint32_t) s;
        w = s >> 32;
    }
    return a;
}

big_integer minus(big_integer a, big_integer const &b) {
	big_integer abs_a = abs(a);
    big_integer abs_b = abs(b);
    if (a.sign && b.sign)
        return minus(abs_b, abs_a);
    if (a.sign && !b.sign)
        return -(abs_a + abs_b);
    if (!a.sign && b.sign)
        return abs_a + abs_b;
    if (a < b)
        return -minus(abs_b, abs_a);
    bool w = 0;
    uint64_t s;
    for (size_t i = 0; i < b.number.size() || w > 0; i++) {
        s = (uint64_t) a.number[i] - w - (i < b.number.size() ? b.number[i] : 0);
        a.number[i] = (uint32_t) s;
        w = s >> 63;
    }
    return a;
}

big_integer operator-(big_integer a, big_integer const &b) {
    big_integer c = minus(a, b);
    normalize(c);
    return c;
}

big_integer operator*(big_integer a, big_integer const &b) {
    big_integer c;
	if (a == 0 || b == 0)
		return 0;
	c.sign = (a.sign ^ b.sign);
    c.number.resize(a.number.size() + b.number.size());
	uint32_t w;
	uint64_t s;
    for (size_t i = 0; i < a.number.size(); i++) {
        w = 0;
        for (size_t j = 0; j < b.number.size() || w > 0; j++) {
            s = c.number[i + j] + (uint64_t) a.number[i] * (j < b.number.size() ? b.number[j] : 0) + w;
            c.number[i + j] = (uint32_t) s;
            w = s >> 32;
        }
    }
    normalize(c);
    return c;
}

big_integer operator/(big_integer a, uint32_t b) {
    uint32_t w = 0;
    uint64_t s;
    for (ptrdiff_t i = a.number.size() - 1; i >= 0; i--) {
        s = (uint64_t(w) << 32) + a.number[i];
        a.number[i] = s / b;
        w = s % b;
    }
    normalize(a);
    return a;
}

big_integer operator/(big_integer a, int b) {
    big_integer abs_a = abs(a);
    uint32_t abs_b = (b == INT32_MIN) ? (uint32_t) b : abs(b);
    big_integer c = abs_a / abs_b;
    c.sign = (a.sign ^ (b < 0));
    normalize(c);
    return c;
}

typedef __uint128_t int128;

big_integer operator/(big_integer a, big_integer const &b) {
    big_integer abs_a = abs(a);
    big_integer abs_b = abs(b);
    if (a.sign != b.sign) {
        return -(abs_a / abs_b);
	}
	if (abs_a < abs_b) {
        return 0;
	}
    if (abs_b.number.size() == 1) {
        return abs_a / abs_b.number[0];
	}
	uint32_t f = (uint64_t(1) << 32) / (abs_b.number.back() + 1);
	big_integer norm_a = abs_a * f;
	big_integer norm_b = abs_b * f;
	size_t n = norm_b.size(), m = norm_a.size() - n;
	big_integer c;
	c.number.resize(m + 1);
	norm_a.number.push_back(0);
	int128 d1, d2 = (int128(norm_b.number.back()) << 32) ^ norm_b.number[norm_b.size() - 2];
	for (ptrdiff_t i = m; i >= 0; i--) {
		d1 = (int128(norm_a.number.back()) << 64) ^
			 (int128(norm_a.number[norm_a.size() - 2]) << 32) ^
			 norm_a.number[norm_a.size() - 3];
		if (d1 >= d2) {
			uint32_t w = std::min(d1 / d2, (int128)UINT32_MAX);
			norm_a = minus(norm_a, w * norm_b << 32 * i);
			if (norm_a.sign) {
				norm_a += norm_b << 32 * i;
				w--;
			}
			c.number[i] = w;
		}
		norm_a.number.pop_back();
	}
    normalize(c);
    return c;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a - (a / b) * b;
}

uint32_t and_(uint32_t a, uint32_t b) {
    return a & b;
}

uint32_t or_(uint32_t a, uint32_t b) {
    return a | b;
}

uint32_t xor_(uint32_t a, uint32_t b) {
    return a ^ b;
}

typedef uint32_t (*func)(uint32_t a, uint32_t b);

big_integer bit_operation(big_integer a, big_integer const &b, func f) {
    big_integer c;
    c.number.resize(std::max(a.size(), b.size()));
	auto ref = [&] (big_integer x) {
		big_integer inv_x = x;
		if (x.sign) {
			inv_x++;
			for (size_t i = 0; i < inv_x.number.size(); i++)
				inv_x.number[i] = ~inv_x.number[i];
		}
		return inv_x;
	};
    big_integer inv_a = ref(a), inv_b = ref(b);
	if (inv_a.size() < inv_b.size())
		std::swap(inv_a, inv_b);
	while (inv_a.size() > inv_b.size())
		inv_b.number.push_back(b.sign * UINT32_MAX);
    for (size_t i = 0; i < std::max(inv_a.number.size(), inv_b.number.size()); i++)
        c.number[i] = (*f)(inv_a.number[i], inv_b.number[i]);
    c.sign = (*f)(a.sign, b.sign);
    if (c.sign) {
		for (size_t i = 0; i != c.number.size(); i++)
			c.number[i] = ~c.number[i];
		c--;
    }
    return c;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return bit_operation(a, b, and_);
}

big_integer operator|(big_integer a, big_integer const &b) {
    return bit_operation(a, b, or_);
}

big_integer operator^(big_integer a, big_integer const &b) {
    return bit_operation(a, b, xor_);
}
#include <cassert>
big_integer operator<<(big_integer a, int b) {
	if (a == 0)
		return 0;
    size_t sh = b & 31;
    uint32_t w = 0, s;
    for (size_t i = 0; i < a.number.size(); i++) {
        s = ((a.number[i] << sh) ^ w);
		if (sh == 0)
			w = 0;
		else
			w = (a.number[i] >> (32 - sh));
		a.number[i] = s;
    }
    if (w > 0) {
        a.number.push_back(w);
	}
	vector<uint32_t> z((b >> 5) + a.size(), 0);
	for (size_t i = 0; i != a.number.size(); i++)
		z[i + (b >> 5)] = a.number[i];
	z.swap(a.number);
    return a;
}

big_integer operator>>(big_integer a, int b) {
    size_t sh = (b & 31);
    uint32_t w = 0;
    uint64_t s;
	for (size_t i = (b >> 5); i < a.number.size(); i++)
		a.number[i - (b >> 5)] = a.number[i];
	a.number.resize(std::max(size_t(0), a.size() - (b >> 5)));
    for (ptrdiff_t i = a.number.size() - 1; i >= 0; i--) {
        s = (a.number[i] << (32 - sh));
        a.number[i] = ((a.number[i] >> sh) | w);
        w = (uint32_t) s;
    }
    normalize(a);
    if (a.sign)
        a--;
    return a;
}

size_t big_integer::size() const {
	return number.size();
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}
