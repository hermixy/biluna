/****************************************************************************
** $Id: rs_layer.cpp 4862 2007-03-07 15:32:00Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#include "rs_layer.h"

#include <iostream>

#include "rs_color.h"

#include "rs_math.h"


/**
 * Constructor.
 */
RS_Layer::RS_Layer(const RS_String& name) {
    setName(name);

    data.pen.setLineType(RS2::SolidLine);
    data.pen.setWidth(RS2::Width00);
    data.pen.setColor(RS_Color(0,0,0));
    data.frozen = false;
    data.locked = false;
}

RS_Layer::RS_Layer(const RS_LayerData& data) {
    this->data = data;
}
