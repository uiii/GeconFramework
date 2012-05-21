#include "Fraction.hpp"

#include<algorithm>

namespace Gecon
{
    Fraction::Fraction(int nominator, int denominator):
        nominator(nominator), denominator(denominator)
    {
        int a = nominator;
        int b = denominator;
        while(a != 0)
        {
            b = b % a;
            std::swap(a, b);
        }

        nominator /= b;
        denominator /= b;
    }

    bool operator==(const Fraction& first, const Fraction& second)
    {
        return double(first.nominator) / double(first.denominator)
            == double(second.nominator) / double(second.denominator);
    }

    bool operator!=(const Fraction& first, const Fraction& second)
    {
        return double(first.nominator) / double(first.denominator)
            != double(second.nominator) / double(second.denominator);
    }

    bool operator>(const Fraction& first, const Fraction& second)
    {
        return double(first.nominator) / double(first.denominator)
            > double(second.nominator) / double(second.denominator);
    }

    bool operator<(const Fraction& first, const Fraction& second)
    {
        return double(first.nominator) / double(first.denominator)
            < double(second.nominator) / double(second.denominator);
    }

} // namespace Gecon
