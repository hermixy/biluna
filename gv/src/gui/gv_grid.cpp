/*****************************************************************
 * $Id: gv_grid.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 20, 2008 7:00:11 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_grid.cpp 9109
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_grid.h"

//#include "rs_units.h"
//#include "rs_graphic.h"
#include "gv_drawing.h"
// #include "gv_gvmdi_old.h"
#include "gv.h"
#include "rb_debug.h"
#include "RMath.h"
#include "rb_settings.h"

/**
 * Constructor.
 */
GV_Grid::GV_Grid(GV_Drawing* dwg) {
	RB_DEBUG->print("GV_Grid::GV_Grid()");
    mDrawing = dwg;
    
    pt = NULL;
    number = 0;
    metaX = NULL;
    numMetaX = 0;
    metaY = NULL;
    metaYIso = NULL;
    numMetaY = 0;
    spacing = 1.0;
    metaSpacing = 10.0;
    
    metaFactorX = 1;
    metaFactorY = 1;
}


/**
 * Destructor.
 */
GV_Grid::~GV_Grid() {
	RB_DEBUG->print("GV_Grid::~GV_Grid()");
    if (pt!=NULL) {
        delete[] pt;
    }
    if (metaX!=NULL) {
        delete[] metaX;
    }
    if (metaY!=NULL) {
        delete[] metaY;
	}
	RB_DEBUG->print("GV_Grid::~GV_Grid() OK");
}


/**
 * Updates the grid point array.
 * @param rect rectangle of visible scene in scene coordinates
 * @param scaleFactor overall scale factor
 */
void GV_Grid::update(const QRectF& rect, double scaleFactor) {
    RB_DEBUG->print("GV_Grid::update() scaleFactor = %f %f, %f", 
    		scaleFactor, rect.x(), rect.y());
    
    // auto scale grid?
    RB_SETTINGS->beginGroup("/Appearance");
    bool scaleGrid = (bool)RB_SETTINGS->readNumEntry("/ScaleGrid", 1);
    int minGridSpacing = RB_SETTINGS->readNumEntry("/MinGridSpacing", 10);
    RB_SETTINGS->endGroup();
    
    // delete old grid:
    if (pt!=NULL) {
        delete[] pt;
        pt = NULL;
    }
    if (metaX!=NULL) {
        delete[] metaX;
        metaX = NULL;
    }
    if (metaY!=NULL) {
        delete[] metaY;
        metaY = NULL;
    }
    if (metaYIso!=NULL) {
        delete[] metaYIso;
        metaYIso = NULL;
    }
    number = 0;
    numMetaX = 0;
    numMetaY = 0;
    
    RVector gridWidth;
    RVector metaGridWidth;
    
	bool gridIsStandard = true; // false is isometric grid
    const double isoFactor = 2 * pow(3, 0.5);

    gridWidth.x = 0.000001;
    gridWidth.y = 0.000001;
    
    // auto scale grid
	if (scaleGrid) {
        // +0.0001 is correction for logFactor
		int logFactor = floor(log10(scaleFactor+0.0001));
		gridWidth.x = pow(10.0, (double)-logFactor + 1.0);
        gridWidth.y = gridWidth.x;
        gridWidth.x *= gridIsStandard ? 1.0 : isoFactor;
    }
    
    metaGridWidth.x = gridWidth.x*10;
    metaGridWidth.y = gridWidth.y*10;

    // for grid info:
    spacing = gridWidth.x;
    metaSpacing = metaGridWidth.x;
    
	double left = 0.0;
	double right = 0.0;
	double top = 0.0;
	double bottom = 0.0;

    if (gridWidth.x>1.0e-6 && gridWidth.y>1.0e-6) {
        // find grid boundaries
    	left = (int)(rect.x() / gridWidth.x) * gridWidth.x;
    	right = (int)((left + rect.width()) / gridWidth.x) * gridWidth.x;
    	top = (int)(rect.y() / gridWidth.y) * gridWidth.y;
    	bottom = (int)((top + rect.height()) / gridWidth.y) * gridWidth.y;
    	
        left -= gridWidth.x;
        right += gridWidth.x;
        top -= gridWidth.y;
        bottom += gridWidth.y;

        RB_DEBUG->print("GV_Grid::update: left: %f", left);
        RB_DEBUG->print("GV_Grid::update: right: %f", right);
        RB_DEBUG->print("GV_Grid::update: top: %f", top);
        RB_DEBUG->print("GV_Grid::update: bottom: %f", bottom);
        RB_DEBUG->print("GV_Grid::update: spacing: %f", spacing);
        RB_DEBUG->print("GV_Grid::update: metaSpacing: %f", metaSpacing);
        
        // calculate number of visible grid points
        const int numberX = (RMath::mround((right-left) / gridWidth.x) + 1);
        const int numberY = (RMath::mround((bottom-top) / gridWidth.y) + 1);
        number = numberX*numberY;

        // create grid array:
        if (number > 0 && number < 1000000) {

            pt = new RVector[number];
            bool numberIsEven = true;
            
            if (gridIsStandard) {
	            // standard grid
	            int i=0;
	            const double gwx = gridWidth.x;
	            const double gwy = gridWidth.y;
	            for (int y=0; y<numberY; ++y) {
	                const double top_gwy = top+y*gwy;
	                for (int x=0; x<numberX; ++x) {
	                    pt[i++].set(left+x*gwx, top_gwy);
	                }
	            }
            } else {
            	// isometric grid test
	            int i=0;
	            const double gwx = gridWidth.x;
	            const double gwy = gridWidth.y;
	            const double leftIso = left + 0.5 * gwx;
	            for (int y=0; y<numberY; ++y) {
	                const double top_gwy = top+y*gwy;
	                for (int x=0; x<numberX; ++x) {
	                	if (numberIsEven) {
	                		pt[i++].set(left+x*gwx, top_gwy);
	                	} else {
	                		pt[i++].set(leftIso+x*gwx, top_gwy);
	                	}
	                }
	            	numberIsEven = numberIsEven ? false : true;
	            }
            }
        } else {
            number = 0;
            pt = NULL;
        }
    }
    
    // find meta grid boundaries
    if (metaGridWidth.x>1.0e-6 && metaGridWidth.y>1.0e-6) {
    	metaXMin = (int)(left / metaGridWidth.x) * metaGridWidth.x;
    	metaXMax = (int)(right / metaGridWidth.x) * metaGridWidth.x;
    	metaYMin = (int)(top / metaGridWidth.y) * metaGridWidth.y;
    	metaYMax = (int)(bottom / metaGridWidth.y) * metaGridWidth.y;
    	
        metaXMin -= metaGridWidth.x;
        metaXMax += metaGridWidth.x;
        metaYMax += metaGridWidth.y;
        metaYMin -= metaGridWidth.y;

        RB_DEBUG->print("GV_Grid::update: metaXMin: %f", metaXMin);
        RB_DEBUG->print("GV_Grid::update: metaXMax: %f", metaXMax);
        RB_DEBUG->print("GV_Grid::update: metaYMin: %f", metaYMin);
        RB_DEBUG->print("GV_Grid::update: metaYMax: %f", metaYMax);

        // calculate number of visible meta grid lines:
        numMetaX = (RMath::mround((metaXMax-metaXMin) / metaGridWidth.x) + 1);
        numMetaY = (RMath::mround((metaYMax-metaYMin) / metaGridWidth.y) + 1);

        if (numMetaX>0 && numMetaY>0) {
            // create meta grid arrays:
            metaX = new double[numMetaX];
            metaY = new double[numMetaY];

            int i=0;
            for (int x=0; x<numMetaX; ++x) {
                metaX[i++] = metaXMin+x*metaGridWidth.x;
            }
            i=0;
            for (int y=0; y<numMetaY; ++y) {
                metaY[i++] = metaYMin+y*metaGridWidth.y;
            }
            
			if (!gridIsStandard) {
				// isometric grid
				metaYIso = new double[numMetaY];
				const int numberX = (RMath::mround((right-left) / gridWidth.x) + 1);
				i = 0;
				for (int y = 0; y < numMetaY; ++y) {
					metaYIso[i++] = metaYMin + y * metaGridWidth.y
									+ (pt[numberX-1].x - pt[0].x) * 0.5;
				}
			}
        } else {
            numMetaX = 0;
            metaX = NULL;
            numMetaY = 0;
            metaY = NULL;
        }
    }
    
    RB_DEBUG->print("GV_Grid::update: OK");
}


