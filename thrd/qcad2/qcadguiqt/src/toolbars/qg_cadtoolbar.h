/****************************************************************************
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

#ifndef QG_CADTOOLBAR_H
#define QG_CADTOOLBAR_H

#include <QWidget>

#ifdef RS_PROF
#include "qg_cadtoolbarpolylines.h"
#endif

#include "rs_actioninterface.h"
#include "ui_qg_cadtoolbar.h"

class RS_ActionInterface;

//class RS_ActionHandler;
class QG_CadToolBarArcs;
class QG_CadToolBarCircles;
class QG_CadToolBarDim;
class QG_CadToolBarEllipses;
class QG_CadToolBarInfo;
class QG_CadToolBarMain;
class QG_CadToolBarModify;
class QG_CadToolBarLines;
class QG_CadToolBarPoints;
class QG_CadToolBarSelect;
class QG_CadToolBarSnap;
class QG_CadToolBarSplines;

/**
 *
 */
class QG_CadToolBar: public QWidget, private Ui::QG_CadToolBar {
    Q_OBJECT

public:
    QG_CadToolBar(QWidget* parent=0);
    virtual ~QG_CadToolBar();
    
    //RS_ActionHandler* getActionHandler();
    
public slots:
    void back();
    void forceNext();
    void mouseReleaseEvent(QMouseEvent* e);
    void contextMenuEvent(QContextMenuEvent* e);
    void createSubToolBars(/*RS_ActionHandler* ah*/);
    void showToolBar(int id);
    void showToolBarMain();
    void showToolBarPoints();
    void showToolBarLines();
    void showToolBarArcs();
    void showToolBarCircles();
    void showToolBarEllipses();
    void showToolBarSplines();
#ifdef RS_PROF
    void showToolBarPolylines();
#endif
    void showToolBarInfo();
    void showToolBarModify();
    void showToolBarSnap();
    void showToolBarDim();
    void showToolBarSelect();

public:
    void showToolBarSelect(RS_ActionInterface* (*f)());

signals:
    void signalBack();
    void signalNext();

private:
    QWidget* currentTb;
    //RS_ActionHandler* actionHandler;

    QG_CadToolBarMain* tbMain;
    
    QG_CadToolBarPoints* tbPoints;
    QG_CadToolBarLines* tbLines;
    QG_CadToolBarArcs* tbArcs;
    QG_CadToolBarCircles* tbCircles;
    QG_CadToolBarEllipses* tbEllipses;
    QG_CadToolBarSplines* tbSplines;
#ifdef RS_PROF
    QG_CadToolBarPolylines* tbPolylines;
#endif

    QG_CadToolBarDim* tbDim;

    QG_CadToolBarSnap* tbSnap;

    QG_CadToolBarInfo* tbInfo;
    QG_CadToolBarModify* tbModify;
    QG_CadToolBarSelect* tbSelect;
};

#endif

