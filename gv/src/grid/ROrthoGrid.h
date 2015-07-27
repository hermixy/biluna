#ifndef RORTHOGRID_H
#define RORTHOGRID_H

#include "RGrid.h"

class RGraphicsView;
class RRuler;



/**
 * \brief Grid representation.
 *
 * This class is responsible for updating the grid and snapping to the grid.
 *
 * \scriptable
 * \ingroup grids
 */
class ROrthoGrid : public RGrid {
public:
    ROrthoGrid();
    ~ROrthoGrid();

    RVector snapToGrid(RGraphicsView& view, const RVector& positionUcs);
    virtual void update(RGraphicsView& view);
    virtual void paint(RGraphicsView& view);
    virtual void paintMetaGrid(RGraphicsView& view);

    virtual void paintRuler(RGraphicsView& view, RRuler& ruler);

    virtual QString getInfoText();

    // This is just an idea. This might update the grid for
    // an entity that is viewed as elevation view. This might be at
    // any angle.
    //void updateGrid(REntity& entity);

private:
    double inchAutoscale(double x, double idealSpacing);

private:
    RBox viewBox;
    RBox gridBox;
    RBox metaGridBox;
    RVector spacing;
    RVector minSpacing;
    RVector metaSpacing;
    bool scaleGrid;
    int minPixelSpacing;
    RS::Unit unit;
};

Q_DECLARE_METATYPE(ROrthoGrid*)

#endif
