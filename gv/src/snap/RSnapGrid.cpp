#include "RGraphicsView.h"
#include "RGrid.h"
#include "RMouseEvent.h"
#include "RSnapGrid.h"



RVector RSnapGrid::snap(const RVector& position, RGraphicsView& view) {
    RGrid* grid = view.getGrid();

    if (grid==NULL) {
        RVector ret = position;
        ret.valid = false;
        return ret;
    }

    return grid->snapToGrid(view, position);
}
