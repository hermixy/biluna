#include "RGraphicsView.h"
#include "RSettings.h"
#include "RSnapAuto.h"
#include "RSnapEnd.h"
#include "RSnapGrid.h"
#include "RSnapIntersection.h"
#include "RSnapMiddle.h"
#include "RSnapOnEntity.h"
#include "RSnapReference.h"


RVector RSnapAuto::snap(const RVector& position, RGraphicsView& view) {
    entityIds.clear();
    RVector ret;
    int rangePixels = RSettings::getSnapRange();
    double range = view.mapDistanceFromView(rangePixels);

    // intersection
    RSnapIntersection snapIntersection;
    ret = snapIntersection.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        entityIds = snapIntersection.getEntityIds();
        return ret;
    }
    
    // endpoint
    RSnapEnd snapEnd;
    ret = snapEnd.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        entityIds = snapEnd.getEntityIds();
        return ret;
    }

    // middle
    RSnapMiddle snapMiddle;
    ret = snapMiddle.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        entityIds = snapMiddle.getEntityIds();
        return ret;
    }

    // reference
    RSnapReference snapReference;
    ret = snapReference.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        entityIds = snapReference.getEntityIds();
        return ret;
    }
    
    // grid
    RSnapGrid snapGrid;
    ret = snapGrid.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        return ret;
    }

    // on entity
    RSnapOnEntity snapOnEntity;
    ret = snapOnEntity.snap(position, view);
    if (ret.isValid() && ret.getDistanceTo(position) < range) {
        //snapStatus = RS2::ActionSnapGrid;
        entityIds = snapOnEntity.getEntityIds();
        return ret;
    }

    // free (no snap)
    return position;
}
