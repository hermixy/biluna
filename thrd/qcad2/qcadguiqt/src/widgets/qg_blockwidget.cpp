/****************************************************************************
** $Id: qg_blockwidget.cpp 8363 2008-01-23 15:21:36Z andrew $
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

#include "qg_blockwidget.h"

#include <qlayout.h>
#include <qtooltip.h>
#include <QMenu>
#include <QToolButton>
#include <qlabel.h>
#include <qcursor.h>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QContextMenuEvent>
#include <QVBoxLayout>

#include "rs_actionblockshideall.h"
#include "rs_actionblocksshowall.h"
#include "rs_actionblocksadd.h"
#include "rs_actionblocksremove.h"
#include "rs_actionblocksattributes.h"
#include "rs_actionblocksedit.h"
#include "rs_actionblocksinsert.h"
#include "rs_actionblockstoggleview.h"

/**
 * Constructor.
 */
QG_BlockWidget::QG_BlockWidget(/*RS_ActionHandler* ah,*/ QWidget* parent,
                               Qt::WFlags f)
        : QWidget(parent, f) {

    //actionHandler = ah;
    blockList = NULL;
    currentBlock = NULL;
    
    listBox = new QG_ListWidget(this);
    listBox->setSelectionMode(QAbstractItemView::SingleSelection);
    listBox->setFocusPolicy(Qt::NoFocus);
    listBox->setIconSize(QPixmap(":/qcad/images/visibleblock.png").size()+QSize(6,0));

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setMargin(0);
    lay->setSpacing(0);

    QHBoxLayout* layButtons = new QHBoxLayout(NULL);
    layButtons->setMargin(0);
    layButtons->setSpacing(0);
    QHBoxLayout* layButtons2 = new QHBoxLayout(NULL);
    layButtons2->setMargin(0);
    layButtons2->setSpacing(0);
    QToolButton* but;
    // show all blocks:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksShowAll::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // hide all blocks:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksHideAll::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // add block:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksAdd::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // remove block:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksRemove::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // edit attributes:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksAttributes::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // edit block:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksEdit::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons2->addWidget(but);
    // insert block:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionBlocksInsert::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons2->addWidget(but);

    lay->addLayout(layButtons);
    lay->addLayout(layButtons2);
    lay->addWidget(listBox);

    connect(listBox, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotActivated()));
    connect(listBox, SIGNAL(iconClicked(int, QListWidgetItem*)),
        this, SLOT(slotIconClicked(int, QListWidgetItem*)));

}



/**
 * Destructor
 */
QG_BlockWidget::~QG_BlockWidget() {
    delete listBox;
}



/**
 * Updates the block box from the blocks in the graphic.
 */
void QG_BlockWidget::updateBlocks() {
    RS_DEBUG->print("QG_BlockWidget::updateBlocks");

    RS_Block* activeBlock = NULL;
    if (blockList!=NULL) {
        if (currentBlock==NULL) {
            activeBlock = blockList->getActive();
        }
        else {
            activeBlock = currentBlock;
        }
    } else {
        activeBlock = NULL;
    }

    listBox->clear();

    if (blockList==NULL) {
        RS_DEBUG->print("QG_BlockWidget::updateBlocks(): blockList is NULL");
        return;
    }

    for (int i=0; i<blockList->count(); ++i) {
        RS_Block* blk = blockList->at(i);
        RS_DEBUG->print("QG_BlockWidget::updateBlocks(): adding block: '%s'", 
            (const char*)blk->getName().toLatin1());
        QListWidgetItem* item = new QListWidgetItem(blk->getName(), listBox);
        if (!blk->isFrozen()) {
            item->setIcon(QIcon(":/qcad/images/visibleblock.png"));
        } else {
            item->setIcon(QIcon(":/qcad/images/hiddenblock.png"));
        }
    }

    listBox->sortItems();
    
    highlightBlock(activeBlock);

    RS_DEBUG->print("QG_BlockWidget::updateBlocks: OK");
}



/**
 * Highlights (activates) the given block and makes it 
 * the active block in the blocklist.
 */
void QG_BlockWidget::highlightBlock(RS_Block* block) {
    RS_DEBUG->print("QG_BlockWidget::highlightBlock");

    if (block==NULL || listBox==NULL) {
        return;
    }

    if (blockList!=NULL) {
        blockList->activate(block);
    }
    QString name = block->getName();
    RS_DEBUG->print("QG_BlockWidget::highlightBlock: name: %s", 
        (const char*)name.toLatin1());

    for (int i=0; i<listBox->count(); ++i) {
        RS_DEBUG->print("QG_BlockWidget::highlightBlock: comparing: %d", i);
        if (listBox->item(i)->text()==name) {
            listBox->setCurrentRow(i);
            break;
        }
    }
    RS_DEBUG->print("QG_BlockWidget::highlightBlock: OK");
}


/**
 * Called when the user activates (highlights) a block.
 */
void QG_BlockWidget::slotActivated() {
    RS_DEBUG->print("QG_BlockWidget::slotActivated()");

    QListWidgetItem* item = NULL;
    QList<QListWidgetItem*> itemList = listBox->selectedItems();
    if (!itemList.isEmpty()) {
        item = itemList.first();
    }

    if (blockList==NULL || item==NULL) {
        return;
    }

    blockList->activate(item->text());
}

    
    
/**
 * Called for every mouse click.
 */
void QG_BlockWidget::slotIconClicked(int /*x*/, QListWidgetItem* item) {
    RS_DEBUG->print("QG_BlockWidget::slotIconClicked");

    currentBlock = NULL;

    // temporarily activate block that was clicked
    if (item!=NULL && blockList!=NULL) {
        RS_DEBUG->print("QG_BlockWidget::slotIconClicked: %s", 
            (const char*)item->text().toLatin1());
        currentBlock = blockList->getActive();
        blockList->activate(item->text());
    }
    
    // toggle view of block:
    //actionHandler->slotBlocksToggleView();
    //RS_ActionBlocksToggleView::createGuiAction();
    //RS_GuiAction::triggerByType(RS2::ActionBlocksToggleView);

    RS_ActionBlocksToggleView::factory();

    currentBlock = NULL;
}


/**
 * Shows a context menu for the block widget. Launched with a right click.
 */
void QG_BlockWidget::contextMenuEvent(QContextMenuEvent* e) {
    RS_DEBUG->print("QG_BlockWidget::contextMenuEvent");

    QMenu* contextMenu = new QMenu(this);
    contextMenu->addAction(RS_ActionBlocksShowAll::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksHideAll::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksAdd::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksRemove::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksAttributes::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksEdit::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksInsert::createGuiAction());
    contextMenu->addAction(RS_ActionBlocksToggleView::createGuiAction());

    /*
    contextMenu->addAction( tr("&Defreeze all Blocks"), actionHandler,
                             SLOT(slotBlocksDefreezeAll()));
    contextMenu->addAction( tr("&Freeze all Blocks"), actionHandler,
                             SLOT(slotBlocksFreezeAll()));
    contextMenu->addAction( tr("&Add Block"), actionHandler,
                             SLOT(slotBlocksAdd()));
    contextMenu->addAction( tr("&Remove Block"), actionHandler,
                             SLOT(slotBlocksRemove()));
    contextMenu->addAction( tr("&Rename Block"), actionHandler,
                             SLOT(slotBlocksAttributes()));
    contextMenu->addAction( tr("&Edit Block"), actionHandler,
                             SLOT(slotBlocksEdit()));
    contextMenu->addAction( tr("&Insert Block"), actionHandler,
                             SLOT(slotBlocksInsert()));
    contextMenu->addAction( tr("&Toggle Visibility"), actionHandler,
                             SLOT(slotBlocksToggleView()));
    contextMenu->addAction( tr("&Create New Block"), actionHandler,
                             SLOT(slotBlocksCreate()));
    */
    contextMenu->exec(QCursor::pos());
    delete contextMenu;

    e->accept();
}



/**
 * Escape releases focus.
 */
void QG_BlockWidget::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        emit escape();
        break;

    default:
        QWidget::keyPressEvent(e);
        break;
    }
}

