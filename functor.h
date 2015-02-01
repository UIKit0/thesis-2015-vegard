#include "coord.h"

class Combinator;

/**
 * Functor interface.
 */
class Functor
{
public:
    Functor() { };

    /**
     * Unity transformation.
     */
    virtual Coord operator()(Coord input) const {
        return input;
    };

    /**
     * Chain this functor together with another functor.
     * @param fn a functor to be applied after this functor.
     * @return a combined functor.
     */
    virtual Combinator chain(const Functor &fn) const;
};

/**
 * Combine two functors.
 */
class Combinator : public Functor
{
public:
    /**
     * Chain two functors together.
     * @param fn1 the first functor to be applied.
     * @param fn1 the second functor to be applied.
     * @return a combined functor.
     */
    Combinator(const Functor &fn1, const Functor &fn2)
        : functor1(fn1), functor2(fn2) { };

    /**
     * Combined transformation.
     */
    virtual Coord operator()(Coord input) const {
        return functor2(functor1(input));
    };

private:
    const Functor &functor1;
    const Functor &functor2;
};

/**
 * Transform vertex positions in the range [-0.5, 0.5] to
 * texture coordinates in the range [0, 1].
 */
class PosToTexCoord : public Functor
{
public:
    PosToTexCoord() { };
    virtual Coord operator()(Coord input) const {
        return Coord(input.x + 0.5, input.y + 0.5);
    };
};

/**
 * Transform texture coordinates in the range [0, 1] to
 * vertex positions in the range [-0.5, 0.5].
 */
class TexCoordToPos : public Functor
{
public:
    TexCoordToPos() { };
    virtual Coord operator()(Coord input) const {
        return Coord(input.x - 0.5, input.y - 0.5);
    };
};

/**
 * Fish-eye transformation.
 */
class Fish : public Functor
{
public:
    Fish() { };
    virtual Coord operator()(Coord input) const {
        PolarCoord p = input.toPolarCoord();
        GLfloat rr = p.r - 0.5 * p.r * p.r;
        PolarCoord pp = PolarCoord(rr, p.theta);
        return pp.toCoord();
    };
};

/**
 * Reverse fish-eye transformation.
 */
class FishInverse : public Functor
{
public:
    FishInverse() { };
    virtual Coord operator()(Coord input) const {
        PolarCoord p = input.toPolarCoord();
        GLfloat rr = -sqrt(1 - 2 * p.r) + 1;
        PolarCoord pp = PolarCoord(rr, p.theta);
        return pp.toCoord();
    };
};
