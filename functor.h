#include "coord.h"

class Combinator;

class Functor
{
public:
    Functor() { };
    virtual Coord operator()(Coord input) {
        return input;
    };
    virtual Functor chain(Functor &fn);
};

class Combinator : public Functor
{
public:
    Combinator(Functor &fn1, Functor &fn2)
        : functor1(fn1), functor2(fn2) { };
    virtual Coord operator()(Coord input) {
        return functor2(functor1(input));
    };
    Functor functor1;
    Functor functor2;
};

class Fish : public Functor
{
public:
    Fish() { };
    virtual Coord operator()(Coord input) {
        PolarCoord p = input.toPolarCoord();
        GLfloat rr = p.r - 0.5 * p.r * p.r;
        PolarCoord pp = PolarCoord(rr, p.theta);
        return pp.toCoord();
    };
};

class FishInverse : public Functor
{
public:
    FishInverse() { };
    virtual Coord operator()(Coord input) {
        PolarCoord p = input.toPolarCoord();
        GLfloat rr = p.r + 0.1 * p.r * p.r;
        PolarCoord pp = PolarCoord(rr, p.theta);
        return pp.toCoord();
    };
};
