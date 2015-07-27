/*****************************************************************
 * $Id: gv_grid.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Oct 20, 2008 6:57:55 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_grid.h 7863
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_GRID_H
#define GV_GRID_H

#include <QtWidgets>
#include "rb_string.h"
#include "RVector.h"

class GV_Drawing;

/**
 * This class represents a grid. Grids can be drawn on graphic
 * views and snappers can snap to the grid points.
 */
class GV_Grid {

public:
    GV_Grid(GV_Drawing* dwg);
    ~GV_Grid();

    void update(const QRectF& rect, double scaleFactor);
    
    /**
     * @return Array of all visible grid points.
     */
    RVector* getPoints() {
        return pt;
    }
    
    /**
     * @return Number of visible grid points.
     */
    int count() {
        return number;
    }

    /**
     * @return Current grid spacing.
     */
    //double getSpacing() {
    //    return spacing;
    //}
    
    /**
     * @return Current meta grid spacing.
     */
    //double getMetaSpacing() {
    //    return metaSpacing;
    //}

    /**
     * @return Grid info for status widget.
     */
    RB_String getInfo() {
        return RB_String("%1 / %2").arg(spacing).arg(metaSpacing);
    }

    /**
     * @return Meta grid positions in X.
     */
    double* getMetaX() {
        return metaX;
    }
    
    /**
     * @return Number of visible meta grid lines in X.
     */
    int countMetaX() {
        return numMetaX;
    }
    
    /**
     * @return Meta grid positions in Y.
     */
    double* getMetaY() {
        return metaY;
    }
    
    /**
     * @return Meta grid positions in Y.
     */
    double* getMetaYIso() {
        return metaYIso;
    }

    /**
     * @return Number of visible meta grid lines in Y.
     */
    int countMetaY() {
        return numMetaY;
    }

    double getMetaXMin() {
        return metaXMin;
    }
    double getMetaXMax() {
        return metaXMax;
    }

    double getMetaYMin() {
        return metaYMin;
    }
    double getMetaYMax() {
        return metaYMax;
    }

    /**
     * @return Current grid spacing (in x).
     */
    double getSpacing() {
        return spacing;
    }

    int getMetaFactorX() {
        return metaFactorX;
    }
    int getMetaFactorY() {
        return metaFactorY;
    }

protected:
	//! Drawing this grid is connected to
	GV_Drawing* mDrawing;
    
    //! Current grid spacing
    double spacing;
    //! Current meta grid spacing
    double metaSpacing;
    
    //! Pointer to array of grid points
    RVector* pt;
    //! Number of points in the array
    int number;
    //! Meta grid positions in X
    double* metaX;
    //! Number of meta grid lines in X
    int numMetaX;
    //! Meta grid positions in Y
    double* metaY;
    double* metaYIso;
    //! Number of meta grid lines in Y
    int numMetaY;
    double metaXMin;
    double metaXMax;
    double metaYMin;
    double metaYMax;

    int metaFactorX;
    int metaFactorY;

};

#endif
