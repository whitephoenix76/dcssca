/**
 * @file
 * @brief Stepdown functions.
**/

#include "AppHdr.h"

#include "stepdown.h"

#include <cmath>

#include "random.h"

#ifdef __ANDROID__
#include <android/log.h>
double log2(double n)
{
    return log(n) / log(2); // :(
}
#endif

double _stepup2(double value, double divisor, double exponent, double multiplier);

double _stepup2(double value, double divisor, double exponent, double multiplier)
{
    return multiplier * pow(value / divisor, exponent);
}

double _stepup(double multiplier, double base, double value, double divisor);

double _stepup(double multiplier, double base, double value, double divisor)
{
    return multiplier * pow(base, value / divisor);
}

int stepup(int value, int multiplier, int base, int divisor)
{
    return _stepup(multiplier, base, value, divisor);
}

int stepup2(double value, int divisor, int exponent, int multiplier)
{
    return _stepup2(value, divisor, exponent, multiplier);
}

double stepdown(double value, double step, double base)
{
    return step * log(1 + value / step) / log(base);
}

int qpow(int value, int num, int denom, int power)
{
    return (double)value * pow((double)num/(double)denom, power);
}

int stepdown(int value, int step, rounding_type rounding, int max, double base)
{
    double ret = stepdown((double) value, double(step), double(base));

    if (max > 0 && ret > max)
        return max;

    // Randomised rounding
    if (rounding == ROUND_RANDOM)
    {
        double intpart;
        double fracpart = modf(ret, &intpart);
        if (decimal_chance(fracpart))
            ++intpart;
        return intpart;
    }

    return ret + (rounding == ROUND_CLOSE ? 0.5 : 0);
}

// Deprecated definition. Call directly stepdown instead.
int stepdown_value(int base_value, int stepping, int first_step,
                   int /*last_step*/, int ceiling_value)
{
    // Disabling max used to be -1.
    if (ceiling_value < 0)
        ceiling_value = 0;

    if (ceiling_value && ceiling_value < first_step)
        return min(base_value, ceiling_value);
    if (base_value < first_step)
        return base_value;

    const int diff = first_step - stepping;
    // Since diff < first_step, we can assume here that ceiling_value > diff
    // or ceiling_value == 0.
    return diff + stepdown(base_value - diff, stepping, ROUND_DOWN,
                           ceiling_value ? ceiling_value - diff : 0);
}
