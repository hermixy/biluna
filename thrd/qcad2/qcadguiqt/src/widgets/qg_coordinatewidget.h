/****************************************************************************
** $Id: qg_colorbox.h 2495 2005-07-24 17:46:28Z andrew $
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

#ifndef QG_COORDINATEWIDGET_H
#define QG_COORDINATEWIDGET_H

#include <QWidget>

#include "rs.h"

#include "ui_qg_coordinatewidget.h"

class RS_Graphic;
class RS_Vector;

/**
 * Widget that displays the current mouse position.
 * Usually shown in the status bar of an application.
 */
class QG_CoordinateWidget: public QWidget, private Ui::QG_CoordinateWidget {
    Q_OBJECT

public:
    QG_CoordinateWidget(QWidget* parent=0);
    virtual ~QG_CoordinateWidget();

    void setGraphic(RS_Graphic* graphic);
    void setCoordinates(const RS_Vector& abs,
        const RS_Vector& rel, bool updateFormat);

private:
    RS_Graphic* graphic;
    int prec;
    RS2::LinearFormat format;
    int aprec;
    RS2::AngleFormat aformat;
};

#endif

