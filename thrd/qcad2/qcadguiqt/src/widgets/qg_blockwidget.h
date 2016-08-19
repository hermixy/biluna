/****************************************************************************
** $Id: qg_blockwidget.h 7073 2007-11-13 09:39:35Z andrew $
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

#ifndef QG_BLOCKWIDGET_H
#define QG_BLOCKWIDGET_H

#include <qwidget.h>
#include <QContextMenuEvent>
#include <QPixmap>
#include <QKeyEvent>

#include "rs_blocklist.h"
#include "rs_blocklistlistener.h"

//#include "rs_actionhandler.h"
#include "qg_listwidget.h"

/**
 * This is the Qt implementation of a widget which can view a 
 * block list.
 */
class QG_BlockWidget: public QWidget, public RS_BlockListListener {
    Q_OBJECT

public:
    QG_BlockWidget(/*RS_ActionHandler* ah,*/ QWidget* parent,
                   Qt::WFlags f = 0);
    ~QG_BlockWidget();

    void setBlockList(RS_BlockList* blockList) {
        this->blockList = blockList;
        updateBlocks();
    }

    RS_BlockList* getBlockList() {
        return blockList;
    }

    void updateBlocks();
    void highlightBlock(RS_Block* block);

    virtual void blockAdded(RS_Block*) {
        RS_DEBUG->print(RS_Debug::D_TESTING, "QG_BlockWidget::blockAdded");
        updateBlocks();
    }
    virtual void blockEdited(RS_Block*) {
        updateBlocks();
    }
    virtual void blockRemoved(RS_Block*) {
        updateBlocks();
    }
    virtual void blockToggled(RS_Block*) {
        updateBlocks();
    }

    /**
     * Only used by unit tests.
     */
    QListWidget* getListBox() {
        return listBox;
    }
    
    virtual QSize sizeHint() const {
        return QSize(150, 400);
    }

signals:
    void escape();

public slots:
    void slotActivated();
    void slotIconClicked(int x, QListWidgetItem* item);

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    virtual void keyPressEvent(QKeyEvent* e);

private:
    RS_BlockList* blockList;
    QG_ListWidget* listBox;
    RS_Block* currentBlock;
    //RS_ActionHandler* actionHandler;
};

#endif
