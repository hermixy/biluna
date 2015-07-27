/*****************************************************************
 * $Id: gv_actionzoomautoy.cpp 831 2008-10-07 19:54:45Z rutger $
 * Created: Oct 5, 2008 12:46:38 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomautoy.cpp 8361
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomautoy.h"



GV_ActionZoomAutoY::GV_ActionZoomAutoY(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionInterface(dwg, gv) {}


/*
void GV_ActionZoomAutoY::trigger() {
    graphicView->zoomAutoY(false);
    finish();
}



void GV_ActionZoomAutoY::init(int status) {
    GV_ActionInterface::init(status);
    trigger();
}
*/
// EOF
