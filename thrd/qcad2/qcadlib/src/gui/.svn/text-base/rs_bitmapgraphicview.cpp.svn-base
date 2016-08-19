/*****************************************************************************
**  $Id: rs_staticgraphicview.cpp 2444 2005-07-02 22:54:42Z andrew $
**
**  This is part of the QCad Qt GUI
**  Copyright (C) 2001 Andrew Mustun
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License (version 2) as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#include "rs_bitmapgraphicview.h"

#include "rs_painterqt.h"
#include "rs_img.h"
#include "rs_pixmap.h"


/**
 * Constructor.
 *
 * @param w Width
 * @param h Height
 */
RS_BitmapGraphicView::RS_BitmapGraphicView(int w, int h) :
    RS_StaticGraphicView(w, h, NULL) {

    setBackground(RS_Color(255,255,255));
    
    buffer = new RS_Pixmap(QSize(getWidth(), getHeight()));
    setPainter(new RS_PainterQt(buffer));
    
    ((RS_PainterQt*)painter)->setBackground(RS_Color(255,255,255));
    clear();

    updateGrid();
}


RS_BitmapGraphicView::~RS_BitmapGraphicView() {
    RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BitmapGraphicView::~RS_BitmapGraphicView: deleting buffer..");
    if (((RS_PainterQt*)painter)->isActive()) {
        ((RS_PainterQt*)painter)->end();
    }
    destroyPainter();
    delete buffer;
    RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BitmapGraphicView::~RS_BitmapGraphicView: OK");
}


void RS_BitmapGraphicView::clear() {
    ((RS_PainterQt*)painter)->eraseRect(0,0, getWidth(), getHeight());
}


bool RS_BitmapGraphicView::save(const RS_String& fileName) {
    bool ret;

    RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BitmapGraphicView::save");

    RS_Img img;
    img = buffer->toImage();
    ret = img.save(fileName, "PNG");
    /*if (((RS_PainterQt*)painter)->isActive()) {
        ((RS_PainterQt*)painter)->end();
    }*/
    
    RS_DEBUG->print(RS_Debug::D_TESTING, "RS_BitmapGraphicView::save: OK");

    return ret;
}

    
bool RS_BitmapGraphicView::save(const RS_String& fileName, 
    RS_EntityContainer* cont, bool autoZoom) {
    
    setContainer(cont);
    if (autoZoom) {
        zoomAuto(false);
    }
    drawEntity(cont);
    drawRelativeZero();

    return save(fileName);
}

