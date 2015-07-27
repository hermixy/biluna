/*****************************************************************
 * $Id: gv_actionzoomscroll.cpp 831 2008-10-07 19:54:45Z rutger $
 * Created: Oct 5, 2008 1:11:26 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomscroll.cpp 8386
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rs_actionzoomscroll.h"



GV_ActionZoomScroll::GV_ActionZoomScroll(/*RS2::Direction direction,*/
		GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionInterface(dwg, gv) {

//    this->direction = direction;
}


/*
void GV_ActionZoomScroll::trigger() {
    graphicView->zoomScroll(direction);
    finish();
}



void GV_ActionZoomScroll::init(int status) {
    GV_ActionInterface::init(status);
    trigger();
}
*/
