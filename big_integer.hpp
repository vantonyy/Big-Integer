#pragma once

#include <algorithm>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <cmath>

class BigInteger
{
public:
        //Constructors
        BigInteger();
        BigInteger(int);
        BigInteger(const char*);
        BigInteger(const std::string&);
public:
        //Arithmetic operators
        BigInteger operator+(const BigInteger&) const;
        BigInteger operator-(const BigInteger&) const;
        BigInteger operator*(const BigInteger&) const;
        BigInteger operator/(const BigInteger&) const;
        BigInteger operator/(int) const;
        BigInteger operator%(int) const;
        //Comparison operators
        bool operator<(const BigInteger&) const;
        bool operator>(const BigInteger&) const;
        bool operator<=(const BigInteger&) const;
        bool operator>=(const BigInteger&) const;
        bool operator==(const BigInteger&) const;
        bool operator!=(const BigInteger&) const;
        //Conversion operator
        operator std::string() const;
private:
        template <typename Value>
        static std::string toString(const Value&);
        static int toInt(const std::string&);
        static int toInt(char);
        static char toChar(int);
        static int mismatch(const std::string&, const std::string&);
        static void normalize(std::string&);

        static std::string sum(const std::string&, const std::string&);
        static std::string sub(const std::string&, const std::string&);
        static std::string mul(const std::string&, const std::string&);
        static std::string div(const std::string&, int);
private:
        //Friend functions
        friend std::ostream& operator <<(std::ostream&, const BigInteger&);
        friend std::istream& operator <<(std::istream&, BigInteger&);
private:
        //Data members
        std::string m_value;
};

//Implementation of BigInteger class
BigInteger::BigInteger()
        : m_value()
{
}

BigInteger::BigInteger(int n)
        : m_value(toString(n))
{
}

BigInteger::BigInteger(const char* n)
        : m_value(n)
{
}

BigInteger::BigInteger(const std::string& n)
        : m_value(n)
{
}

std::string BigInteger::sum(const std::string& l, const std::string& r)
{
        assert(l.size() >= r.size());
        std::string res;
        size_t max_size = l.size();
        size_t min_size = r.size();
        res.reserve(max_size + 1);
        int remainder = 0;
        for ( int i = min_size - 1, j = max_size - 1; i >= 0; --i, --j ) {
                int s = toInt(l[j]) + toInt(r[i]) + remainder;
                remainder = s > 9;
                res.push_back(toChar(s % 10));
        }
        for ( int i = max_size - min_size - 1; i >= 0; --i ) {
                int s = toInt(l[i]) + remainder;
                remainder = s > 9;
                res.push_back(toChar(s % 10));
        }
        if ( 1 == remainder ) {
                res.push_back('1');
        }
        std::reverse(res.begin(), res.end());
        return res;
}

std::string BigInteger::sub(const std::string& l, const std::string& r)
{
        assert(l.size() >= r.size());
        std::string res;
        size_t max_size = l.size();
        size_t min_size = r.size();
        res.reserve(max_size + 1);
        int remainder = 0;
        for ( int i = min_size - 1, j = max_size - 1; i >= 0; --i, --j ) {
                int d = toInt(l[j]) - toInt(r[i]) - remainder;
                remainder = d < 0;
                res.push_back(toChar(1 == remainder ? d + 10 : d));
        }
        for ( int i = max_size - min_size - 1; i >= 0; --i ) {
                int d = toInt(l[i]) - remainder;
                remainder = d < 0;
                res.push_back(toChar(1 == remainder ? d + 10 : d));
        }
        std::reverse(res.begin(), res.end());
        normalize(res);
        return res;
}

std::string BigInteger::mul(const std::string& l, const std::string& r)
{
        assert(l.size() >= r.size());
        std::string res;
        size_t max_size = l.size();
        size_t min_size = r.size();
        int remainder = 0;
        for ( int i = min_size - 1; i >= 0; --i ) {
                std::string tmp;
                tmp.reserve(max_size * 2);
                for ( int j = max_size - 1; j >= 0; --j ) {
                        int m = toInt(l[j]) * toInt(r[i]) + remainder;
                        remainder = m / 10;
                        tmp.push_back(toChar(m % 10));
                }
                if (0 != remainder) {
                        tmp.push_back(toChar(remainder));
                }
                std::reverse(tmp.begin(), tmp.end());
                tmp.resize(tmp.size() + min_size - 1 - i, '0');
                res = (res.size() >= tmp.size()) ? sum(res, tmp) : sum(tmp, res);
                remainder = 0;
        }
        return res;
}

std::string BigInteger::div(const std::string& l, int r)
{
        std::string res;
        int size = l.size();
        int i = 0;
        std::string tmp;
        tmp.reserve(size);
        tmp.push_back(l[i]);
        while ( i != size && toInt(tmp) < r ) {
                tmp.push_back(l[++i]);
        }
        while( i < size ) {
                int n = toInt(tmp);
                res += toString(n / r);
                tmp = toString(n % r);
                tmp.push_back(l[++i]);
        }
        return res;
}

BigInteger BigInteger::operator+(const BigInteger& r) const
{
        return r.m_value.size() >= m_value.size() ? sum(r.m_value, m_value) : sum(m_value, r.m_value);
}

BigInteger BigInteger::operator-(const BigInteger& r) const
{
        return r.m_value.size() > m_value.size() ? sub(r.m_value, m_value) : sub(m_value, r.m_value);
}

BigInteger BigInteger::operator*(const BigInteger& r) const
{
        return r.m_value.size() >= m_value.size() ? mul(r.m_value, m_value) : mul(m_value, r.m_value);
}

BigInteger BigInteger::operator/(int r) const
{
        return div(m_value, r);
}

BigInteger BigInteger::operator/(const BigInteger& r) const
{
        //TODO: optimize
        if ( r > *this ) {
                return 0;
        }
        BigInteger tmp = 1;
        while ( tmp * r < *this ) {
                tmp = tmp + 1;
        }
        return tmp - 1;
}

BigInteger BigInteger::operator%(int r) const
{
        BigInteger tmp = *this / r;
        return *this - tmp * r;
}

bool BigInteger::operator<(const BigInteger& r) const
{
        if ( m_value.size() != r.m_value.size() ) {
                return m_value.size() < r.m_value.size();
        }
        return mismatch(m_value, r.m_value) < 0;
}

bool BigInteger::operator<=(const BigInteger& r) const
{
        return *this < r || *this == r;
}

bool BigInteger::operator>(const BigInteger& r) const
{
        if ( m_value.size() != r.m_value.size() ) {
                return m_value.size() > r.m_value.size();
        }
        return mismatch(m_value, r.m_value) > 0;
}

bool BigInteger::operator>=(const BigInteger& r) const
{
        return *this > r || *this == r;
}

bool BigInteger::operator==(const BigInteger& r) const
{
        if ( m_value.size() != r.m_value.size() ) {
                return false;
        }
        return mismatch(m_value, r.m_value) == 0;
}

bool BigInteger::operator!=(const BigInteger& r) const
{
        return !(*this == r);
}

BigInteger::operator std::string() const
{
        return m_value;
}

template <typename Value>
std::string BigInteger::toString(const Value& v)
{
        std::stringstream str;
        str << v;
        return str.str();
}

int BigInteger::toInt(const std::string& v)
{
        return atoi(v.c_str());
}

int BigInteger::toInt(char c)
{
        return static_cast<int>(c - '0');
}

char BigInteger::toChar(int i)
{
        return static_cast<char>(i + '0');
}

int BigInteger::mismatch(const std::string& n1, const std::string& n2)
{
        assert( n1.size() == n2.size() );
        std::string::const_iterator i = n1.begin();
        std::string::const_iterator j = n2.begin();
        while ( i != n1.end() && *i == *j ) {
                ++i;
                ++j;
        }
        return i != n1.end() ? *i - *j : 0;
}

void BigInteger::normalize(std::string& n)
{
        while ( !n.empty() && '0' == *n.begin() ) {
                n.erase(n.begin());
        }
}

//Implementation of friend functions
std::ostream& operator <<(std::ostream& out, const BigInteger& i)
{
        out << i.m_value;
        return out;
}

std::istream& operator <<(std::istream& in, BigInteger& i)
{
        in >> i.m_value;
        return in;
}
