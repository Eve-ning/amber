#ifndef COMMON_H
#define COMMON_H

template <typename T>
class QVector;

class Common
{
public:
    // Squeezes the value to the defined MIN and MAX limits below and returns it
    double clipValue(double value, bool is_bpm);
    QVector<double> clipValueV(QVector<double> valueV, bool isBpm);

    // Define "global" variables
    constexpr static double SV_MIN = 0.01;
    constexpr static double SV_MAX = 10.0;
    constexpr static double SV_DEFAULT = 1.0;
    constexpr static double SV_STEPSIZE = 0.1;

    constexpr static double BPM_MIN = 0.01;
    constexpr static double BPM_MAX = 10000000; // 10,000,000
    constexpr static double BPM_DEFAULT = 120.0;
    constexpr static double BPM_STEPSIZE = 1.0;

    constexpr static double OFFSET_MIN = 0.0;
    constexpr static double OFFSET_MAX = 10000000; // 10,000,000
    constexpr static double OFFSET_DEFAULT = 0.0;
    constexpr static double OFFSET_INTERVAL_DEFAULT = 1000.0;

    long double MATH_PI = 3.14159265358979323846l;

    // This value is to convert vertical slider values to val
    double VS_TO_VAL = 100.0;
};

#endif // COMMON_H
