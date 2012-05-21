#ifndef GECON_FRACTION_HPP
#define GECON_FRACTION_HPP

namespace Gecon
{
    struct Fraction
    {
        Fraction(int nominator, int denominator);

        int nominator;
        int denominator;
    };

    bool operator==(const Fraction& first, const Fraction& second);
    bool operator!=(const Fraction& first, const Fraction& second);
    bool operator>(const Fraction& first, const Fraction& second);
    bool operator<(const Fraction& first, const Fraction& second);
} // namespace Gecon

#endif // GECON_FRACTION_HPP
