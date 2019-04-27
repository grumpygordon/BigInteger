#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cstddef>
#include <iosfwd>
#include <cstdint>
#include <vector>

typedef uint32_t (*func)(uint32_t a, uint32_t b);

struct big_integer {
    big_integer();

    big_integer(big_integer const &a);

    big_integer(int a);

    big_integer(uint32_t);

    explicit big_integer(std::string const &a);

    ~big_integer();

    big_integer &operator=(big_integer const &a);

    big_integer &operator+=(big_integer const &a);

    big_integer &operator-=(big_integer const &a);

    big_integer &operator*=(big_integer const &a);

    big_integer &operator/=(big_integer const &a);

    big_integer &operator/=(uint32_t a);

    big_integer &operator/=(int a);

    big_integer &operator%=(big_integer const &a);

    big_integer &operator&=(big_integer const &a);

    big_integer &operator|=(big_integer const &a);

    big_integer &operator^=(big_integer const &a);

    big_integer &operator<<=(int a);

    big_integer &operator>>=(int a);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend big_integer operator+(big_integer a, big_integer const &b);

	friend big_integer minus(big_integer a, big_integer const &b);

    friend big_integer operator-(big_integer a, big_integer const &b);

    friend big_integer operator*(big_integer a, big_integer const &b);

    friend big_integer operator/(big_integer a, big_integer const &b);

    friend big_integer operator/(big_integer a, uint32_t b);

    friend big_integer operator/(big_integer a, int b);

    friend big_integer operator%(big_integer a, big_integer const &b);

    friend big_integer operator&(big_integer a, big_integer const &b);

    friend big_integer operator|(big_integer a, big_integer const &b);

    friend big_integer operator^(big_integer a, big_integer const &b);

    friend big_integer operator<<(big_integer a, int b);

    friend big_integer operator>>(big_integer a, int b);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);

    friend big_integer abs(big_integer a);

    friend void normalize(big_integer &a);

    friend big_integer bit_operation(big_integer a, big_integer const &b, func bit_op);

	friend std::ostream &operator<<(std::ostream &s, big_integer const &a);
public:
	size_t size() const;

private:
    std::vector<uint32_t> number;
    bool sign;

};

#endif // BIG_INTEGER_H
