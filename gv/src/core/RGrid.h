#ifndef RGRID_H
#define RGRID_H

#include "RVector.h"
#include "RBox.h"
#include "RRuler.h"

class RGraphicsView;

/**
 * Abstract base class for grid implementations. A grid class
 * can for example implement an orthogonal grid or a polar grid.
 *
 * \scriptable
 * \ingroup core
 */
class RGrid {
public:
    RGrid() {}
    virtual ~RGrid() {}

    /**
     * Snaps the given UCS position to the grid.
     */
    virtual RVector snapToGrid(RGraphicsView& view, const RVector& positionUcs) = 0;

    /**
     * This is called by the graphics view this grid is attached to
     * whenever the view port changes and the grid needs to be
     * updated.
     */
    virtual void update(RGraphicsView& view) = 0;

    /**
     * This is called by the graphics view this grid is attached to
     * whenever the grid should be painted.
     */
    virtual void paint(RGraphicsView& view) = 0;

    /**
     * This is called by the graphics view this grid is attached to
     * whenever the meta grid should be painted.
     */
    virtual void paintMetaGrid(RGraphicsView& view) = 0;

    virtual void paintRuler(RGraphicsView& view, RRuler& ruler) = 0;

    virtual QString getInfoText() = 0;
};

Q_DECLARE_METATYPE(RGrid*)

#endif
