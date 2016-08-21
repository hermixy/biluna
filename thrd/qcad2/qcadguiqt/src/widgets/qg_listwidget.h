/****************************************************************************
** $Id: qg_widthbox.h 2495 2005-07-24 17:46:28Z andrew $
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

#ifndef QG_LISTWIDGET_H
#define QG_LISTWIDGET_H

#include <qlistwidget.h>
#include "cad_global.h"

/**
 * A list widget that signals mouse click events.
 */
class CAD_EXPORT QG_ListWidget: public QListWidget {
    Q_OBJECT

public:
    QG_ListWidget(QWidget* parent=0);
    virtual ~QG_ListWidget();

protected:
    void contextMenuEvent(QContextMenuEvent* e);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

signals:
    void iconClicked(int x, QListWidgetItem* item);

private:
    QListWidgetItem* itemPressed;
};

#endif

