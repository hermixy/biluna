/*****************************************************************
 * $Id: gv_eventhandlerqt.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Sept 5, 2011 4:59:20 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_eventhandlerqt.h"

#include "rb_mdiwindow.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsViewQt.h"
#include "RRulerQt.h"

/**
 * Constructor.
 */
GV_EventHandlerQt::GV_EventHandlerQt() : QObject() {

}

/**
 * Destructor.
 */
GV_EventHandlerQt::~GV_EventHandlerQt() {
    RB_DEBUG->print("GV_EventHandlerQt::~GV_EventHandlerQt: OK");
}

void GV_EventHandlerQt::setGraphicsView(RGraphicsViewQt* view) {
    mGraphicsView = view;
}

void GV_EventHandlerQt::setMdiChild(RB_MdiWindow* mdiW) {
    mMdiW = mdiW;
}

void GV_EventHandlerQt::setDocumentInterface(RDocumentInterface* di) {
    mDocumentInterface = di;
    mDocument = &(di->getDocument());
}

void GV_EventHandlerQt::setScrollBar(QScrollBar* sb) {
    if (sb->orientation() == Qt::Horizontal) {
        mHsb = sb;
    } else {
        mVsb = sb;
    }
}

void GV_EventHandlerQt::setRuler(RRulerQt *ruler) {
    if (ruler->getOrientation() == Qt::Horizontal) {
        mHRuler = ruler;
    } else {
        mVRuler = ruler;
    }
}

void GV_EventHandlerQt::horizontalScrolled(int pos) {
    bool blocked = mGraphicsView->signalsBlocked();
    mGraphicsView->blockSignals(true);
    RVector off = mGraphicsView->getOffset();
    off.x = -(pos / mGraphicsView->getFactor());
    mGraphicsView->setOffset(off);
    mGraphicsView->blockSignals(blocked);
    mHRuler->updateViewport();
}

void GV_EventHandlerQt::verticalScrolled(int pos) {
    bool blocked = mGraphicsView->signalsBlocked();
    mGraphicsView->blockSignals(true);
    RVector off = mGraphicsView->getOffset();
    off.y = (pos / mGraphicsView->getFactor());
    mGraphicsView->setOffset(off);
    mGraphicsView->blockSignals(blocked);
    mVRuler->updateViewport();
}

void GV_EventHandlerQt::viewportChanged() {
    // From File/new.js
    mHsb->blockSignals(true);
    mVsb->blockSignals(true);

    RBox box = mDocument->getBoundingBox();
    double min = box.getMinimum().x * mGraphicsView->getFactor() - 800;
    double max = box.getMaximum().x * mGraphicsView->getFactor()
                    - mGraphicsView->getWidth() + 800;
    mHsb->setRange(min, max);
    mHsb->setPageStep(mGraphicsView->getWidth());
    mHsb->setValue(-mGraphicsView->getOffset().x * mGraphicsView->getFactor());

    min = mGraphicsView->getHeight() - box.getMaximum().y
                    * mGraphicsView->getFactor() - 800;
    max = 800 - box.getMinimum().y * mGraphicsView->getFactor();
    mVsb->setRange(min, max);
    mVsb->setPageStep(mGraphicsView->getHeight());
    mVsb->setValue(mGraphicsView->getOffset().y * mGraphicsView->getFactor());

    mHsb->blockSignals(false);
    mVsb->blockSignals(false);

    // rulers
    mHRuler->updateViewport();
    mVRuler->updateViewport();

    // info label
//    var infoLabel = this.mdiChild.findChild("InfoLabel");
//    var grid = this.graphicsView.getGrid();
//    if (grid != undefined) {
//            infoLabel.text = grid.getInfoText();
//    }

}
