/****************************************************************************
** $Id: rs_actionfileprintpreview.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionfileprintpreview.h"
//Added by qt3to4:
#include <QPixmap>

/**
 * Constructor.
 */
RS_ActionFilePrintPreview::RS_ActionFilePrintPreview(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface(
                    container, graphicView) {
    showOptions();
}



RS_ActionFilePrintPreview::~RS_ActionFilePrintPreview() {}


RS_GuiAction* RS_ActionFilePrintPreview::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionFilePrintPreview::getName(), 
            "fileprintpreview",
                             tr("Print Pre&view"));
        action->setStatusTip(tr("Shows a preview of a print"));
        action->setCheckable(true);
    }
    return action;
}


void RS_ActionFilePrintPreview::init(int status) {
    RS_ActionInterface::init(status);
    showOptions();
}




void RS_ActionFilePrintPreview::trigger() {}



void RS_ActionFilePrintPreview::mouseMoveEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case Moving:
        v2 = graphicView->toGraph(e->x(), e->y());
        if (graphic!=NULL) {
            RS_Vector pinsbase = graphic->getPaperInsertionBase();

            double scale = graphic->getPaperScale();

            graphic->setPaperInsertionBase(pinsbase-v2*scale+v1*scale);
        }
        v1 = v2;
        graphicView->redraw();
        break;

    default:
        break;
    }
}



void RS_ActionFilePrintPreview::mousePressEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case Neutral:
            v1 = graphicView->toGraph(e->x(), e->y());
            setStatus(Moving);
            break;

        default:
            break;
        }
    }
}


void RS_ActionFilePrintPreview::mouseReleaseEvent(RS_MouseEvent* e) {
    switch (getStatus()) {
    case Moving:
        setStatus(Neutral);
        break;

    default:
        //deleteSnapper();
        RS_DIALOGFACTORY->requestPreviousMenu();
#if QT_VERSION>=0x030000

        e->accept();
#endif

        break;
    }
}



void RS_ActionFilePrintPreview::coordinateEvent(RS_CoordinateEvent* ) {}



void RS_ActionFilePrintPreview::commandEvent(RS_CommandEvent* ) {}



RS_StringList RS_ActionFilePrintPreview::getAvailableCommands() {
    RS_StringList cmd = RS_ActionInterface::getAvailableCommands();
    return cmd;
}


void RS_ActionFilePrintPreview::updateMouseButtonHints() {}



void RS_ActionFilePrintPreview::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::MovingHandCursor);
}



void RS_ActionFilePrintPreview::updateToolBar() {}


void RS_ActionFilePrintPreview::center() {
    if (graphic!=NULL) {
        graphic->centerToPage();
        graphicView->redraw();
    }
}


void RS_ActionFilePrintPreview::fit() {
    if (graphic!=NULL) {
        graphic->fitToPage();
        graphicView->zoomAuto();
        //graphicView->redraw();
    }
}


void RS_ActionFilePrintPreview::setScale(double f) {
    if (graphic!=NULL) {
        graphic->setPaperScale(f);
        graphicView->redraw();
    }
}



double RS_ActionFilePrintPreview::getScale() {
    double ret = 1.0;
    if (graphic!=NULL) {
        ret = graphic->getPaperScale();
    }
    return ret;
}



void RS_ActionFilePrintPreview::setBlackWhite(bool bw) {
    if (bw) {
        graphicView->setDrawingMode(RS2::ModeBW);
    } else {
        graphicView->setDrawingMode(RS2::ModeFull);
    }
    graphicView->redraw();
}


RS2::Unit RS_ActionFilePrintPreview::getUnit() {
    if (graphic!=NULL) {
        return graphic->getUnit();
    } else {
        return RS2::None;
    }
}

