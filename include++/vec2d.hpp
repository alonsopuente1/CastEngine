#pragma once

#include <cmath>

struct vec2d
{
    float x = 0.f, y = 0.f;

    // normal constructors

    vec2d();
    vec2d(float a);
    vec2d(float _x, float _y);

    // copy/move assignment/constructors

    vec2d& operator=(const vec2d& other);

    vec2d(const vec2d& other) = default;

    // math operations

    vec2d operator+(const vec2d& other) const;
    vec2d operator-(const vec2d& b) const;
    vec2d operator*(float mag) const;
    vec2d operator/(float mag) const;

    vec2d& operator+=(const vec2d& other);
    vec2d& operator-=(const vec2d& other);
    vec2d& operator*=(float mag);
    vec2d operator/=(float mag);

    // equality operations

    bool operator==(const vec2d& other) const;
    bool operator!=(const vec2d& other) const;

    // public helper methods

    float GetMagnitude() const;

    void SetMagnitude(float mag);
    
    vec2d GetPerpendicular() const;

    static vec2d AngToVec(float angle);

    void Normalise();

    void Rotate(float angle);

    float DotProduct(const vec2d& other) const;


};

inline float DegToRad(float deg)
{
    return deg * (acosf(-1) / 180.f);
}

inline float RadToDeg(float rad)
{
    return rad * (180.f / acosf(-1));
}