#include "functor.h"

Combinator Functor::chain(const Functor &fn) const {
    return Combinator(*this, fn);
}
