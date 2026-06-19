
#include "castengine/vec2d.hpp"

vec2d::vec2d() : x(0.0f), y(0.0f) {}
vec2d::vec2d(float a) : x(a), y(a) {}
vec2d::vec2d(float _x, float _y) : x(_x), y(_y) {}

// copy/move assignment/constructors

vec2d& vec2d::operator=(const vec2d& other)
{
    x = other.x;
    y = other.y;

    return *this;
}

// math operations
vec2d vec2d::operator+(const vec2d& other) const
{
    return vec2d(x + other.x, y + other.y);
}    

vec2d& vec2d::operator+=(const vec2d& other)
{
    *this = *this + other;
    return *this;
}

vec2d vec2d::operator-(const vec2d &b) const
{
    return vec2d(x - b.x, y - b.y);
}


vec2d& vec2d::operator-=(const vec2d& other)
{
    *this = *this - other;
    return *this;
}

vec2d vec2d::operator*(float mag) const
{
    return vec2d(x * mag, y * mag);
}

vec2d& vec2d::operator*=(float mag)
{
    *this = *this * mag;
    return *this;
}
vec2d vec2d::operator/(float mag) const
{
    return vec2d(x / mag, y / mag);
}
vec2d vec2d::operator/=(float mag)
{
    *this = *this / mag;
    return *this;
}
// equality operations
bool vec2d::operator==(const vec2d& other) const 
{
    return x == other.x && y == other.y;
}
bool vec2d::operator!=(const vec2d& other) const 
{
    return !(*this == other);        
}
// public helper methods
float vec2d::GetMagnitude() const
{
    return sqrtf((x * x) + (y * y));
}
void vec2d::SetMagnitude(float mag)
{
    if(GetMagnitude() == 0.0f)
        return;
    *this *= (mag / GetMagnitude());
}    

vec2d vec2d::GetPerpendicular() const
{
    return vec2d(y, -x);
}
vec2d vec2d::AngToVec(float angle)
{
    return vec2d(cosf(angle), sinf(angle));
}
void vec2d::Normalise()
{
    *this /= GetMagnitude();
}
void vec2d::Rotate(float angle)
{
    *this = vec2d(x * cosf(angle) - y * sinf(angle), x * sinf(angle) + y * cosf(angle));
}
float vec2d::DotProduct(const vec2d& other) const 
{
    return x * other.x + y * other.y;
}

float vec2d::GetAngle() const
{
    return atan2f(y, x);
}
