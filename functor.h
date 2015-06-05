#include "point.h"

#include <QDebug>

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
    virtual Point operator()(Point input) const {
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
    virtual Point operator()(Point input) const {
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
    Homography() : apply(), inverse() { };

    virtual Point operator()(Point input) const {
        return apply(input);
    };

    Functor apply;
    Functor inverse;
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
    virtual Point operator()(Point input) const {
        return Point(input.x * xfactor, input.y * yfactor);
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
    virtual Point operator()(Point input) const {
        return Point(input.x + xoffset, input.y + yoffset);
    };
    float xoffset;
    float yoffset;
};

/**
 * Clamp a coordinate to a range.
 */
class Clamp : public Functor
{
public:
    Clamp(float xminimum, float xmaximum, float yminimum, float ymaximum)
     : xmin(xminimum), xmax(xmaximum), ymin(yminimum), ymax(ymaximum) { };
    virtual Point operator()(Point input) const {
        float x = input.x;
        float y = input.y;

        if(x < xmin) {
            x = xmin;
        } else if(x > xmax) {
            x = xmax;
        }

        if(y < ymin) {
            y = ymin;
        } else if(y > ymax) {
            y = ymax;
        }

        return Point(x, y);
    };

    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

/**
 * Transform vertex positions in the range [-0.5, 0.5] to
 * texture coordinates in the range [0, 1].
 */
class PosToTexCoord : public Functor
{
public:
    PosToTexCoord() : clamp(-0.5, 0.5, -0.5, 0.5), shift(0.5) { };
    virtual Point operator()(Point input) const {
        return shift(input);
    };
    Clamp clamp;
    Shift shift;
};

/**
 * Transform texture coordinates in the range [0, 1] to
 * vertex positions in the range [-0.5, 0.5].
 */
class TexCoordToPos : public Functor
{
public:
    TexCoordToPos() : clamp(-0.5, 0.5, -0.5, 0.5), shift(-0.5) { };
    virtual Point operator()(Point input) const {
        return shift(input);
    };
    Clamp clamp;
    Shift shift;
};

/**
 * Fish-eye transformation.
 */
class Fish : public Functor
{
public:
    Fish() { };
    virtual Point operator()(Point input) const {
        Polar p = input.toPolar();
        GLfloat s = 0.3390;
        GLfloat lambda = 3.8342;
        GLfloat rr = s * log(1 + lambda * p.r);
        Polar pp = Polar(rr, p.theta);
        return pp.toPoint();
    };
};

/**
 * Reverse fish-eye transformation.
 */
class FishInverse : public Functor
{
public:
    FishInverse() { };
    virtual Point operator()(Point input) const {
        Polar p = input.toPolar();
        GLfloat s = 0.3390;
        GLfloat lambda = 3.8342;
        GLfloat rr = (exp(p.r / s) - 1) / lambda;
        Polar pp = Polar(rr, p.theta);
        return pp.toPoint();
    };
};

/**
 * Fish-eye homography.
 */
class Fisheye : public Homography
{
public:
    Fisheye() : Homography(), apply(), inverse() { };

    virtual Point operator()(Point input) const {
        return apply(input);
    };

    Fish apply;
    FishInverse inverse;
};
