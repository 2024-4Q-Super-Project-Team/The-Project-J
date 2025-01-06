#pragma once

class Radian;

class Degree
{
private:
    float angle = 0.f;
public:
    static float MaxDegree;
public:
    Degree() = default;
    Degree(float _angle);
    Degree(Radian _radian);
    operator Radian() const;
    operator float() const;
    operator float*();
public:
    static Radian ToRadian(Degree _degree);

    Degree operator+()
    {
        return Degree(angle);
    }
    Degree operator-()
    {
        return Degree(-angle);
    }
    Degree& operator=(const float& _angle) {
        angle = _angle;
        return *this;
    }
    Degree& operator+=(const Degree& _val) {
        angle += _val.angle;
        return *this;
    }
    Degree& operator-=(const Degree& _val) {
        angle -= _val.angle;
        return *this;
    }
    Degree& operator*=(const Degree& _val) {
        angle *= _val.angle;
        return *this;
    }
    Degree& operator/=(const Degree& _val) {
        angle /= _val.angle;
        return *this;
    }
    Degree operator+(const Degree& _val)
    {
        return Degree(angle + _val.angle);
    }
    Degree operator-(const Degree& _val)
    {
        return Degree(angle - _val.angle);
    }
    Degree operator*(const Degree& _val)
    {
        return Degree(angle * _val.angle);
    }
    Degree operator/(const Degree& _val)
    {
        return Degree(angle / _val.angle);
    }
    bool operator==(const Degree& _val)
    {
        return (angle == _val.angle);
    }
    bool operator!=(const Degree& _val)
    {
        return (angle != _val.angle);
    }
};

class Radian
{
private:
    float angle = 0.0f;
public:
    Radian() = default;
    Radian(float _angle);
    Radian(Degree _degree);
    operator Degree() const;
    operator float() const;
    operator float* ();
public:
    static Degree ToDegree(Radian _radian);

    Radian operator+()
    {
        return Radian(angle);
    }
    Radian operator-()
    {
        return Radian(-angle);
    }
    Radian& operator=(const float& _angle) {
        angle = _angle;
        return *this;
    }
    Radian& operator+=(const Radian& _val) {
        angle += _val.angle;
        return *this;
    }
    Radian& operator-=(const Radian& _val) {
        angle -= _val.angle;
        return *this;
    }
    Radian& operator*=(const Radian& _val) {
        angle *= _val.angle;
        return *this;
    }
    Radian& operator/=(const Radian& _val) {
        angle /= _val.angle;
        return *this;
    }
    Radian operator+(const Radian& _val)
    {
        return Radian(angle + _val.angle);
    }
    Radian operator-(const Radian& _val)
    {
        return Radian(angle - _val.angle);
    }
    Radian operator*(const Radian& _val)
    {
        return Radian(angle * _val.angle);
    }
    Radian operator/(const Radian& _val)
    {
        return Radian(angle / _val.angle);
    }
    bool operator==(const Radian& _val)
    {
        return (angle == _val.angle);
    }
    bool operator!=(const Radian& _val)
    {
        return (angle != _val.angle);
    }
};