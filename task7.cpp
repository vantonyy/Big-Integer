#include "big_integer.hpp"

#include <iostream>
#include <string>

BigInteger calc_arithmetic_progression(const BigInteger& a_1, const BigInteger& n, const BigInteger d  = 1)
{
        BigInteger a_n = a_1 + d * (n - 1);
        BigInteger S_n = ((a_1 + a_n) * n) / 2;
        std::cout << (a_1 + a_n) * n<< "\n";
        return S_n;
}

int main()
{
        int n = 0;
        std::cout << "Input: N [1, 100]: ";
        std::cin >> n;
        if ( n < 1 || n > 100 ) {
                std::cout << "Invalid N, should be in range: [1, 100]" << std::endl;
                return 1;
        }
        std::string s1(n, '0');
        s1[0] = '1';
        std::string s2(n, '9');
        std::cout << "Result: " << calc_arithmetic_progression(s1, s2) << std::endl;
        return 0;
}
