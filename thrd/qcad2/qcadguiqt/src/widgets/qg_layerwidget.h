/****************************************************************************
** $Id: qg_layerwidget.h 9470 2008-03-28 11:11:11Z andrew $
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

#ifndef QG_LAYERWIDGET_H
#define QG_LAYERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QContextMenuEvent>
#include <QPixmap>
#include <QKeyEvent>

#include "rs_layerlist.h"

//#include "rs_actionhandler.h"
#include "qg_listwidget.h"


/**
 * This is the Qt implementation of a widget which can view a 
 * layer list and provides a user interface for basic layer actions.
 */
class QG_LayerWidget: public QWidget {
    Q_OBJECT

public:
    QG_LayerWidget(/*RS_ActionHandler* ah,*/ QWidget* parent,
                   Qt::WindowFlags f = 0);
    ~QG_LayerWidget();

    void setLayerList(RS_LayerList* layerList);

    void updateUiList();
    void activateLayer(RS_Layer* layer);
    void activateLayer(const QString& name);

public slots:
    void slotUpdated() {
        updateUiList();
    }
    void slotLayerActivated(RS_Layer* layer);
    void slotLayerAdded(RS_Layer* layer) {
        RS_DEBUG->print("QG_LayerWidget::layerAdded");
        updateUiList();
        activateLayer(layer);
    }
    void slotLayerEdited(RS_Layer*) {
        updateUiList();
    }
    void slotLayerRemoved(RS_Layer*) {
        updateUiList();
        activateLayer(layerList->at(0));
    }
    void slotLayerToggled(RS_Layer*) {
        updateUiList();
    }
    void slotLayerListChanged() {
        updateUiList();
    }
    void slotLayerActivatedByUser();
    void slotIconClicked(int x, QListWidgetItem* item);
    void slotItemDoubleClicked(QListWidgetItem* item);

public:
    QListWidget* getListBox() {
        return listBox;
    }

    virtual QSize sizeHint() const {
        return QSize(150, 400);
    }

signals:
    void escape();

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    virtual void keyPressEvent(QKeyEvent* e);

private:
    QG_ListWidget* listBox;
    bool updating;
    RS_LayerList* layerList;
};

#endif
