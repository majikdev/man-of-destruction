#ifndef MATHS_H
#define MATHS_H

#include <cmath>

// Mathematical constants.

constexpr float pi = 3.141592f;      // The approximate value of pi.
constexpr float radian = 57.295779f; // Radian expressed in degrees.
constexpr float degree = 0.017453f;  // Degree expressed in radians.
constexpr float epsilon = 0.0001f;   // Small float for comparisons.

// Get the maximum of two values.

template<typename T>
inline T Max(T first, T second)
{
    return (first > second) ? first : second;
}

// Get the minimum of two values.

template<typename T>
inline T Min(T first, T second)
{
    return (first < second) ? first : second;
}

// Clamp a value to a range.

template<typename T>
inline T Clamp(T value, T min, T max)
{
    return Max(Min(value, max), min);
}

// Linearly interpolate between two values.

template<typename T>
inline T Lerp(T start, T end, float value)
{
    return start + (end - start) * Min(value, 1.0f);
}

// Snap a float value to a grid.

inline float Snap(float value, float step)
{
    return roundf(value / step) * step;
}

// Two-component float vector.

struct vector2f
{
public:
    vector2f();
    vector2f(float x, float y);

    bool IsZero() const;
    bool IsNearlyZero() const;

    float Length() const;
    float SqrLength() const;
    float Rotation() const;

    vector2f Normalised() const;
    vector2f Rotated(float angle) const;
    vector2f operator-() const;

    vector2f operator+(const vector2f& other) const;
    vector2f operator-(const vector2f& other) const;
    vector2f operator*(const vector2f& other) const;
    vector2f operator/(const vector2f& other) const;

    vector2f operator*(float value) const;
    vector2f operator/(float value) const;

    vector2f& operator=(const vector2f& other);
    vector2f& operator+=(const vector2f& other);
    vector2f& operator-=(const vector2f& other);
    vector2f& operator*=(const vector2f& other);
    vector2f& operator/=(const vector2f& other);

    vector2f& operator*=(float value);
    vector2f& operator/=(float value);

public:
    float x, y;

    static vector2f zero;
};

#endif