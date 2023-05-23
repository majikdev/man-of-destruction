#include "maths.h"

vector2f vector2f::zero = vector2f();

// Create a vector.

vector2f::vector2f()
    : x(0.0f), y(0.0f)
{}

vector2f::vector2f(float x, float y)
    : x(x), y(y)
{}

// Check if this vector is zero.

bool vector2f::IsZero() const
{
    return x == 0.0f && y == 0.0f;
}

// Check if this vector is nearly zero.

bool vector2f::IsNearlyZero() const
{
    return abs(x) < epsilon && abs(y) < epsilon;
}

// Get this vector's length.

float vector2f::Length() const
{
    return sqrtf(x * x + y * y);
}

// Get this vector's squared length.

float vector2f::SqrLength() const
{
    return x * x + y * y;
}

// Get this vector's angle in degrees.

float vector2f::Rotation() const
{
    vector2f normal = Normalised();
    float angle = atan2(normal.y, normal.x) * radian;

    // Return the rotation normalised to a range of 0-360.

    return angle >= 0 ? angle : angle + 360.0f;
}

// Derive a normalised vector.

vector2f vector2f::Normalised() const
{
    if (IsZero())
    {
        return vector2f::zero;
    }

    // If not zero, return the normal of this vector.

    float length = Length();

    return {x / length, y / length};
}

// Derive a rotated vector.

vector2f vector2f::Rotated(float angle) const
{
    float sine = sinf(angle * degree);
    float cosine = cosf(angle * degree);

    vector2f a(cosine, sine);
    vector2f b(-sine, cosine);

    return a * x + b * y;
}

// Derive an inverted vector.

vector2f vector2f::operator-() const
{
    return {-x, -y};
}

// Binary arithmetic operators.

vector2f vector2f::operator+(const vector2f& other) const
{
    return {x + other.x, y + other.y};
}

vector2f vector2f::operator-(const vector2f& other) const
{
    return {x - other.x, y - other.y};
}

vector2f vector2f::operator*(const vector2f& other) const
{
    return {x * other.x, y * other.y};
}

vector2f vector2f::operator/(const vector2f& other) const
{
    return {x / other.x, y / other.y};
}

vector2f vector2f::operator*(float value) const
{
    return {x * value, y * value};
}

vector2f vector2f::operator/(float value) const
{
    return {x / value, y / value};
}

// Assignment operators.

vector2f& vector2f::operator=(const vector2f& other) = default;

vector2f& vector2f::operator+=(const vector2f& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

vector2f& vector2f::operator-=(const vector2f& other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

vector2f& vector2f::operator*=(const vector2f& other)
{
    x *= other.x;
    y *= other.y;

    return *this;
}

vector2f& vector2f::operator/=(const vector2f& other)
{
    x /= other.x;
    y /= other.y;

    return *this;
}

vector2f& vector2f::operator*=(float value)
{
    x *= value;
    y *= value;

    return *this;
}

vector2f& vector2f::operator/=(float value)
{
    x /= value;
    y /= value;

    return *this;
}