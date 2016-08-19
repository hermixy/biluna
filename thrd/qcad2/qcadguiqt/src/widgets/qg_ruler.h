/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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

#ifndef QG_RULERWIDGET_H
#define QG_RULERWIDGET_H

#include <QFrame>

#include "rs.h"

class RS_GraphicView;

/**
 * Widget that displays the current mouse position.
 * Usually shown in the status bar of an application.
 */
class QG_Ruler: public QFrame {
    Q_OBJECT

public:
    QG_Ruler(bool horizontal, QWidget* parent=0);
    virtual ~QG_Ruler();

    void setGraphicView(RS_GraphicView* graphicView);

    bool isHorizontal() const {
        return horizontal;
    }

    virtual QSize sizeHint() const {
        if (isHorizontal()) {
            return QSize(100, 20);
        }
        else {
            return QSize(20, 100);
        }
    }

protected:
    void paintEvent(QPaintEvent *);
    void paintHorizontalLabel(QPainter& painter, double mx, bool hasLabel, int labelWidth, int prec);
    void paintVerticalLabel(QPainter& painter, double my, bool hasLabel, int labelWidth, int prec);

private:
    RS_GraphicView* graphicView;
    bool horizontal;
};

#endif

