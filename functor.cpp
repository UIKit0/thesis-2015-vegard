#include "functor.h"

Functor Functor::chain(Functor &fn) {
    return Combinator(*this, fn);
}
