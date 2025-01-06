#include "pch.h"
#include "Angles.h"

float Degree::MaxDegree = 6.28f;

Degree::Degree(float _angle)
    : angle(_angle)
{
}

Degree::Degree(Radian _radian)
    : angle(Radian::ToDegree(_radian))
{
}

Degree::operator Radian() const
{
    Radian radian{ 3.141592f * angle / 180.f };
    return radian;
}
Degree::operator float() const
{
    return angle;
}
Degree::operator float*()
{
    return &angle;
}

Radian Degree::ToRadian(Degree _degree)
{
    return 3.141592f * _degree.angle / 180.f;
}

Radian::Radian(float _angle)
    : angle(_angle)
{
}

Radian::Radian(Degree _degree)
    : angle(Degree::ToRadian(_degree))
{
}

Radian::operator Degree() const
{
    Degree degree = Degree(180.f * angle / 3.141592f);
    return degree;
}
Radian::operator float() const
{
    return angle;
}
Radian::operator float* ()
{
    return &angle;
}

Degree Radian::ToDegree(Radian _radian)
{
    return  Degree(180.f * _radian.angle / 3.141592f);
}
