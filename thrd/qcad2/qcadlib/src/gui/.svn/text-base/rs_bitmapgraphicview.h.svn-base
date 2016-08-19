/*****************************************************************************
**  $Id: rs_staticgraphicview.h 2444 2005-07-02 22:54:42Z andrew $
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

#ifndef RS_BITMAPGRAPHICVIEW_H
#define RS_BITMAPGRAPHICVIEW_H

#include <QPixmap>
#include "rs_staticgraphicview.h"

/**
 * This is an implementation of a graphic viewer for drawing 
 * into bitmaps.
 */
class RS_BitmapGraphicView: public RS_StaticGraphicView {
public:
    RS_BitmapGraphicView(int w, int h);
    virtual ~RS_BitmapGraphicView();

    void clear();

    bool save(const RS_String& fileName);
    bool save(const RS_String& fileName, RS_EntityContainer* cont, bool autoZoom=true);

private:
    QPixmap* buffer;
};

#endif
