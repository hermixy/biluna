#include "RGraphicsView.h"
#include "RMouseEvent.h"
#include "RSnapFree.h"

RVector RSnapFree::snap(const RVector& position, RGraphicsView& /*view*/) {
    return position;
}
