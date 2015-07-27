#ifndef GV_ORTHOGRID_H
#define GV_ORTHOGRID_H

#include "rb_grid.h"

class GV_GraphicsView;
class RB_Ruler;



/**
 * \brief Grid representation.
 *
 * This class is responsible for updating the grid and snapping to the grid.
 *
 * \scriptable
 * \ingroup grids
 */
class GV_OrthoGrid : public RB_Grid {
public:
    GV_OrthoGrid();
    ~GV_OrthoGrid();

    RVector snapToGrid(GV_GraphicsView& view, const RVector& positionUcs);
    virtual void update(GV_GraphicsView& view);
    virtual void paint(GV_GraphicsView& view);
    virtual void paintMetaGrid(GV_GraphicsView& view);

    virtual void paintRuler(GV_GraphicsView& view, RB_Ruler& ruler);

    virtual QString getInfoText();

    // This is just an idea. This might update the grid for
    // an entity that is viewed as elevation view. This might be at
    // any angle.
    //void updateGrid(RB_Entity& entity);

private:
    double inchAutoscale(double x, double idealSpacing);

private:
    RB_Box viewBox;
    RB_Box gridBox;
    RB_Box metaGridBox;
    RVector spacing;
    RVector minSpacing;
    RVector metaSpacing;
    bool scaleGrid;
    int minPixelSpacing;
    GV2::Unit unit;
};

Q_DECLARE_METATYPE(GV_OrthoGrid*)

#endif
