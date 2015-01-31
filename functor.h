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

class PosToTexCoord : public Functor
{
public:
    PosToTexCoord() { };
    virtual Coord operator()(Coord input) {
        return Coord(input.x + 0.5, input.y + 0.5);
    };
};

class TexCoordToPos : public Functor
{
public:
    TexCoordToPos() { };
    virtual Coord operator()(Coord input) {
        return Coord(input.x - 0.5, input.y - 0.5);
    };
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
        GLfloat rr = -sqrt(1 - 2 * p.r) + 1;
        PolarCoord pp = PolarCoord(rr, p.theta);
        return pp.toCoord();
    };
};
