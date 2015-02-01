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
 * Bijective functor.
 */
class Homography : public Functor
{
public:
    Homography(const Functor &fn, const Functor &inv)
        : apply(fn), inverse(inv) { };

    virtual Coord operator()(Coord input) const {
        return apply(input);
    };

    const Functor &apply;
    const Functor &inverse;
};

/**
 * Scale a coordinate by a factor.
 */
class Scale : public Functor
{
public:
    Scale(float f)
        : xfactor(f), yfactor(f) { };
    Scale(float x, float y)
        : xfactor(x), yfactor(y) { };
    virtual Coord operator()(Coord input) const {
        return Coord(input.x * xfactor, input.y * yfactor);
    };
    float xfactor;
    float yfactor;
};

/**
 * Shift a coordinate by an offset.
 */
class Shift : public Functor
{
public:
    Shift(float f)
        : xoffset(f), yoffset(f) { };
    Shift(float x, float y)
        : xoffset(x), yoffset(y) { };
    virtual Coord operator()(Coord input) const {
        return Coord(input.x + xoffset, input.y + yoffset);
    };
    float xoffset;
    float yoffset;
};

/**
 * Transform vertex positions in the range [-0.5, 0.5] to
 * texture coordinates in the range [0, 1].
 */
class PosToTexCoord : public Functor
{
public:
    PosToTexCoord()
        : shift(0.5) { };
    virtual Coord operator()(Coord input) const {
        return shift(input);
    };
    Shift shift;
};

/**
 * Transform texture coordinates in the range [0, 1] to
 * vertex positions in the range [-0.5, 0.5].
 */
class TexCoordToPos : public Functor
{
public:
    TexCoordToPos()
        : shift(-0.5) { };
    virtual Coord operator()(Coord input) const {
        return shift(input);
    };
    Shift shift;
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

/**
 * Fish-eye homography.
 */
class Fisheye : public Homography
{
public:
    Fisheye() : Homography(apply, inverse) { };
    Fish apply;
    FishInverse inverse;
};
