/****************************************************************************
** $Id: qg_scrollbar.h 5087 2007-03-29 09:48:14Z andrew $
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

#ifndef QG_SCROLLBAR_H
#define QG_SCROLLBAR_H

#include <QScrollBar>
#include <QWheelEvent>
#include "cad_global.h"

/**
 * A small wrapper for the Qt scrollbar. This class offers a slot
 * for scroll events.
 */
class CAD_EXPORT QG_ScrollBar: public QScrollBar {
    Q_OBJECT

public:
    QG_ScrollBar(QWidget* parent=0)
            : QScrollBar(parent) {}
    QG_ScrollBar(Qt::Orientation orientation,
                 QWidget* parent=0)
            : QScrollBar(orientation, parent) {}

public slots:
    void slotWheelEvent(QWheelEvent* e) {
        wheelEvent(e);
    }
};

#endif

