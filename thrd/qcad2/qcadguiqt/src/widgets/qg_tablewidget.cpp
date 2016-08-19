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

#include "qg_tablewidget.h"

#include <QMouseEvent>

#include "rs_debug.h"

/**
 * Default Constructor. 
 */
QG_TableWidget::QG_TableWidget(QWidget* parent)
        : QTableWidget(parent) {
    itemPressed = NULL;
}

/**
 * Destructor
 */
QG_TableWidget::~QG_TableWidget() {}


void QG_TableWidget::contextMenuEvent(QContextMenuEvent* e) {
    RS_DEBUG->print("QG_TableWidget::contextMenuEvent");
    if (e!=NULL) {
        RS_DEBUG->print("QG_TableWidget::contextMenuEvent: e!=NULL");
        QTableWidgetItem* item = itemAt(e->pos());
        if (item!=NULL) {
            RS_DEBUG->print("QG_TableWidget::contextMenuEvent: item: %s", 
                (const char*)item->text().toLatin1());
            setCurrentItem(item);
            emit itemClicked(item);
        }
    }
    e->ignore();
}

void QG_TableWidget::mousePressEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_TableWidget::mousePressEvent: %d/%d", 
        e->x(), e->y());

    if (e->x() < iconSize().width()) {
        itemPressed = itemAt(e->pos());
    }
    else {
        RS_DEBUG->print("QTableWidget::mousePressEvent");
        e->ignore();
        QTableWidget::mousePressEvent(e);
    }
}
    
void QG_TableWidget::mouseReleaseEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_TableWidget::mouseReleaseEvent: %d/%d", 
        e->x(), e->y());
    
    if (e->x() < iconSize().width()) {
        QTableWidgetItem* item = itemAt(e->pos());
        if (item!=NULL) {
            RS_DEBUG->print("QG_TableWidget::mouseReleaseEvent: item: %s", 
                (const char*)item->text().toLatin1());
        }
        if (item!=NULL && item==itemPressed) {
            emit iconClicked(e->x(), item);
        }
    }
    else {
        RS_DEBUG->print("QTableWidget::mouseReleaseEvent");
        e->ignore();
        QTableWidget::mouseReleaseEvent(e);
    }
}


void QG_TableWidget::mouseMoveEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_TableWidget::mouseMoveEvent");

    if (e->pos().x() < iconSize().width()) {
    }
    else {
        RS_DEBUG->print("QTableWidget::mouseMoveEvent");
        e->ignore();
        QTableWidget::mouseMoveEvent(e);
    }
}
