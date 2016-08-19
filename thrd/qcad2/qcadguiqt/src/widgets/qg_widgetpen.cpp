/****************************************************************************
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

#include "qg_widgetpen.h"

/**
 * Constructor
 */
QG_WidgetPen::QG_WidgetPen(QWidget* parent) : QWidget(parent) {
    setupUi(this);
}


/**
 * Destructor
 */
QG_WidgetPen::~QG_WidgetPen() {
}


void QG_WidgetPen::setPen(RS_Pen pen, bool showByLayer, 
                          bool showUnchanged, const QString& title) {
    cbColor->init(showByLayer, showUnchanged);
    cbWidth->init(showByLayer, showUnchanged);
    cbLineType->init(showByLayer, showUnchanged);
    if (!showUnchanged) {
       cbColor->setColor(pen.getColor());
       cbWidth->setWidth(pen.getWidth());
       cbLineType->setLineType(pen.getLineType());
    }

    if (!title.isEmpty()) {
        gbPen->setTitle(title);
    }
}

RS_Pen QG_WidgetPen::getPen() {
    RS_Pen pen;

    pen.setColor(cbColor->getColor());
    pen.setWidth(cbWidth->getWidth());
    pen.setLineType(cbLineType->getLineType());

    return pen;
}

bool QG_WidgetPen::isColorUnchanged() {
    return cbColor->isUnchanged();
}

bool QG_WidgetPen::isLineTypeUnchanged() {
    return cbLineType->isUnchanged();
}

bool QG_WidgetPen::isWidthUnchanged() {
    return cbWidth->isUnchanged();
}
