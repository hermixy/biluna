/****************************************************************************
** $Id: qg_listwidget.cpp 2550 2005-08-03 00:19:25Z andrew $
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

#include "qg_listwidget.h"

#include <QMouseEvent>

#include "rs_debug.h"

/**
 * Default Constructor. 
 */
QG_ListWidget::QG_ListWidget(QWidget* parent)
        : QListWidget(parent) {
    itemPressed = NULL;
}

/**
 * Destructor
 */
QG_ListWidget::~QG_ListWidget() {}



void QG_ListWidget::contextMenuEvent(QContextMenuEvent* e) {
    RS_DEBUG->print("QG_ListWidget::contextMenuEvent");
    if (e!=NULL) {
        RS_DEBUG->print("QG_ListWidget::contextMenuEvent: e!=NULL");
        QListWidgetItem* item = itemAt(e->pos());
        if (item!=NULL) {
            RS_DEBUG->print("QG_ListWidget::contextMenuEvent: item: %s", 
                (const char*)item->text().toLatin1());
            setCurrentItem(item);
            emit itemClicked(item);
        }
    }
    e->ignore();
}



void QG_ListWidget::mousePressEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_ListWidget::mousePressEvent");
    if (e->x() < iconSize().width()) {
        itemPressed = itemAt(e->pos());
    }
    else {
        RS_DEBUG->print("QListWidget::mousePressEvent");
        e->ignore();
        QListWidget::mousePressEvent(e);
    }
}



void QG_ListWidget::mouseReleaseEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_ListWidget::mouseReleaseEvent: %d/%d", 
        e->x(), e->y());
    
    if (e->x() < iconSize().width()) {
        QListWidgetItem* item = itemAt(e->pos());
        if (item!=NULL) {
            RS_DEBUG->print("QG_ListWidget::mouseReleaseEvent: item: %s", 
                (const char*)item->text().toLatin1());
        }
        if (item!=NULL && item==itemPressed) {
            emit iconClicked(e->x(), item);
        }
    }
    else {
        RS_DEBUG->print("QListWidget::mouseReleaseEvent");
        e->ignore();
        QListWidget::mouseReleaseEvent(e);
    }
}



void QG_ListWidget::mouseMoveEvent(QMouseEvent* e) {
    RS_DEBUG->print("QG_ListWidget::mouseMoveEvent");
    if (e->pos().x() < iconSize().width()) {
    }
    else {
        RS_DEBUG->print("QListWidget::mouseMoveEvent");
        e->ignore();
        QListWidget::mouseMoveEvent(e);
    }
}
