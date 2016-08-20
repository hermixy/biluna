/****************************************************************************
** $Id: qg_layerwidget.cpp 9470 2008-03-28 11:11:11Z andrew $
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

#include "qg_layerwidget.h"

#include <QApplication>
#include <qtoolbutton.h>
#include <qcursor.h>
#include <qlabel.h>
#include <qlayout.h>
#include <QMenu>
#include <qtooltip.h>
#include <QPixmap>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QContextMenuEvent>
#include <QVBoxLayout>
#include <QScrollBar>

#include "rs_modification.h"

//#include "rs_actionfactory.h"
#include "rs_actionlayersadd.h"
#include "rs_actionlayersshowall.h"
#include "rs_actionlayershideall.h"
#include "rs_actionlayersedit.h"
#include "rs_actionlayersremove.h"
#include "rs_actionlayerstogglelock.h"
#include "rs_actionlayerstoggleview.h"

#ifdef RS_ARCH
#include "rs_archutils.h"
#endif

/**
 * Constructor.
 */
QG_LayerWidget::QG_LayerWidget(/*RS_ActionHandler* ah,*/ QWidget* parent,
                               Qt::WindowFlags f)
        : QWidget(parent, f) {

    //actionHandler = ah;
    layerList = NULL;
    updating = false;

    //RS_ActionFactory actionFactory(actionHandler, this);

    listBox = new QG_ListWidget(this);
    listBox->setSelectionMode(QAbstractItemView::SingleSelection);
    //listBox->setFocusPolicy(Qt::NoFocus);
    listBox->setIconSize(QPixmap(":/qcad/images/layerstatus_00.png").size() + QSize(6,0));
    //listBox->setAutoScroll(true);
    //listBox->setDragDropMode(QAbstractItemView::DropOnly);
    QPalette palette = listBox->palette();
    palette.setColor(QPalette::Highlight, Qt::darkBlue);
    palette.setColor(QPalette::HighlightedText, Qt::white);
    listBox->setPalette(palette);

    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setMargin(0);
    lay->setSpacing(0);

    QHBoxLayout* layButtons = new QHBoxLayout(NULL);
    layButtons->setMargin(0);
    layButtons->setSpacing(0);
    QToolButton* but;
    // show all layer:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionLayersShowAll::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // hide all layer:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionLayersHideAll::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // add layer:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionLayersAdd::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // remove layer:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionLayersRemove::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);
    // rename layer:
    but = new QToolButton(this);
    but->setDefaultAction(RS_ActionLayersEdit::createGuiAction());
    but->setMaximumSize(QSize(1024,22));
    layButtons->addWidget(but);

    lay->addLayout(layButtons);
    lay->addWidget(listBox);

    connect(listBox, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotLayerActivatedByUser()));
    connect(listBox, SIGNAL(iconClicked(int, QListWidgetItem*)),
        this, SLOT(slotIconClicked(int, QListWidgetItem*)));
    connect(listBox, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(slotItemDoubleClicked(QListWidgetItem*)));

    RS_DEBUG->print("QG_LayerWidget::QG_LayerWidget: sizeHint: %d/%d", 
        sizeHint().width(), sizeHint().height());
}



/**
 * Destructor
 */
QG_LayerWidget::~QG_LayerWidget() {
}



/**
 * Sets the layerlist this layer widget should show.
 *
 * @param showByBlock true: show the layer with the name "ByBlock" if
 *                    it exists.
 *                    false: don't show special layer "ByBlock"
 */
void QG_LayerWidget::setLayerList(RS_LayerList* layerList /*bool showByBlock*/) {
    this->layerList = layerList;
    updateUiList();
}



/**
 * Updates the layer list, based on the layers in the graphic.
 */
void QG_LayerWidget::updateUiList() {
    RS_DEBUG->print("");
    RS_DEBUG->print("QG_LayerWidget::updateUiList");
    
    updating = true;

    int pos = listBox->verticalScrollBar()->sliderPosition();

    listBox->clear();

    // remember active layer:
    RS_Layer* activeLayer = NULL;
    if (layerList!=NULL) {
        activeLayer = layerList->getActive();
    } else {
        activeLayer = NULL;
    }
    
    if (layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::updateUiList: layerList==NULL");
        return;
    }

    for (int i=0; i<layerList->count(); ++i) {
        RS_Layer* layer = layerList->at(i);

#ifdef RS_ARCH
        if (layer->getName().startsWith(RS_FLOOR_PREFIX)) {
            continue;
        }
#endif

        QListWidgetItem* item = new QListWidgetItem(layer->getName(), listBox);
        if (!layer->isFrozen()) {
            if (!layer->isLocked()) {
                item->setIcon(QIcon(":/qcad/images/layerstatus_10.png"));
            }
            else {
                item->setIcon(QIcon(":/qcad/images/layerstatus_11.png"));
            }
        } else {
            if (!layer->isLocked()) {
                item->setIcon(QIcon(":/qcad/images/layerstatus_00.png"));
            }
            else {
                item->setIcon(QIcon(":/qcad/images/layerstatus_01.png"));
            }
        }
    }
    
    listBox->sortItems();

    if (activeLayer!=NULL) {
        for (int i=0; i<(int)listBox->count(); ++i) {
            if (listBox->item(i)->text()==activeLayer->getName()) {
                listBox->setCurrentRow(i);
                break;
            }
        }
    }
    
    listBox->verticalScrollBar()->setSliderPosition(pos);
    
    updating = false;

    RS_DEBUG->print("QG_LayerWidget::updateUiList: OK");
}


/**
 * Highlights (activates) the given layer and makes it 
 * the active layer in the layerlist.
 */
void QG_LayerWidget::activateLayer(RS_Layer* layer) {
    RS_DEBUG->print("QG_LayerWidget::activateLayer");

    if (layer==NULL) {
        RS_DEBUG->print("QG_LayerWidget::activateLayer: abort");
        return;
    }

    activateLayer(layer->getName());

    RS_DEBUG->print("QG_LayerWidget::activateLayer: OK");
}



/**
 * Activates the given layer and makes it 
 * the active layer in the layerlist.
 */
void QG_LayerWidget::activateLayer(const QString& name) {
    RS_DEBUG->print("QG_LayerWidget::activateLayer");

    if (layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::activateLayer: abort");
        return;
    }

    layerList->activate(name/*, true*/);
}



/**
 * Called when the user activates (highlights) a layer.
 */
void QG_LayerWidget::slotLayerActivatedByUser() {
    if (updating) {
        return;
    }
    
    RS_DEBUG->print("QG_LayerWidget::slotLayerActivatedByUser");

    QListWidgetItem* item = NULL;
    QList<QListWidgetItem*> itemList = listBox->selectedItems();
    if (!itemList.isEmpty()) {
        item = itemList.first();
    }

    if (layerList==NULL || item==NULL) {
        RS_DEBUG->print("QG_LayerWidget::slotLayerActivatedByUser: abort");
        return;
    }
    
    layerList->activate(item->text());

    RS_DEBUG->print("QG_LayerWidget::slotLayerActivatedByUser: OK");
}


/**
 * Called for every mouse click.
 */
void QG_LayerWidget::slotIconClicked(int x, QListWidgetItem* item) {
   
    RS_DEBUG->print("QG_LayerWidget::slotIconClicked");

    // temporarily activate layer that was clicked
    if (item!=NULL && layerList!=NULL) {
        RS_DEBUG->print("QG_LayerWidget::slotIconClicked: %s", 
            (const char*)item->text().toLatin1());

        if (x<23) {
            layerList->toggle(item->text());
        }
        else if (x<34) {
            layerList->toggleLock(item->text());
        }
    }
}



/**
 * Shows a context menu for the layer widget. Launched with a right click.
 */
void QG_LayerWidget::contextMenuEvent(QContextMenuEvent *e) {
    
    //RS_ActionFactory actionFactory(actionHandler, this);
        
    QMenu* contextMenu = new QMenu(this);
    contextMenu->addAction(RS_ActionLayersHideAll::createGuiAction());
    contextMenu->addAction(RS_ActionLayersShowAll::createGuiAction());
    contextMenu->addAction(RS_ActionLayersAdd::createGuiAction());
    contextMenu->addAction(RS_ActionLayersRemove::createGuiAction());
    contextMenu->addAction(RS_ActionLayersEdit::createGuiAction());
    contextMenu->addAction(RS_ActionLayersToggleView::createGuiAction());
    contextMenu->addAction(RS_ActionLayersToggleLock::createGuiAction());

    contextMenu->exec(QCursor::pos());
    delete contextMenu;

    e->accept();
}


/**
 * Escape releases focus.
 */
void QG_LayerWidget::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    
    case Qt::Key_Escape:
        emit escape();
        break;
        
    default:
        QWidget::keyPressEvent(e);
        break;
    }
}


/**
 * Called when the layer is activated through an application or the user.
 */
void QG_LayerWidget::slotLayerActivated(RS_Layer* layer) {
    if (layer==NULL) {
        return;
    }
    
    for (int i=0; i<(int)listBox->count(); ++i) {
        if (listBox->item(i)->text()==layer->getName()) {
            listBox->setCurrentRow(i);
            break;
        }
    }

}
    
    

/**
 * On item double-click: apply layer to selection.
 */
void QG_LayerWidget::slotItemDoubleClicked(QListWidgetItem* item) {
    if (layerList==NULL) {
        RS_DEBUG->print("QG_LayerWidget::slotItemDoubleClicked: abort");
        return;
    }
    
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container==NULL || graphicView==NULL) {
        return;
    }

    RS_AttributesData data;
    data.pen = RS_Pen();
    data.layer = item->text();
    data.changeColor = false;
    data.changeLineType = false;
    data.changeWidth = false;
    data.changeLayer = true;

    RS_Modification m(*container, graphicView);
    m.changeAttributes(data);
}
