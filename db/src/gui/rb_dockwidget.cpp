/*****************************************************************
 * $Id: rb_dockwidget.cpp 1806 2012-11-29 15:18:34Z rutger $
 * Created: Dec 29, 2010 12:34:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dockwidget.h"

#include <QCloseEvent>
#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_mainwindow.h"


// int RB_DockWidget::mUntitledNumber = 0;

/**
 * Constructor
 * @param mw main window
 * @param mwgt main widget to be shown on MDI window
 */
RB_DockWidget::RB_DockWidget(RB_MainWindow* mw, RB_Widget* mwgt) : QDockWidget(mw) {
    RB_DEBUG->print("RB_DockWidget::RB_DockWidget()");

    if (!mw || !mwgt) return;
    mMainWindow = mw;
    mCentralWidget = mwgt;

    setWidget(mCentralWidget);
    setObjectName("RB_DockWidget");
    setWindowIcon(QIcon(":/images/biluna16.png"));
    setAttribute(Qt::WA_DeleteOnClose, false);

    connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)),
            this, SLOT(slotWindowStateHasChanged(Qt::WindowStates, Qt::WindowStates)));
//    connect(mCentralWidget, SIGNAL(modificationChanged(bool)),
//            this, SLOT(slotChangedModification(bool)));
    connect(mCentralWidget, SIGNAL(widgetClose()), this, SLOT(close()));

//    this->setAttribute(Qt::WA_DeleteOnClose);
}

/**
 * Destructor
 */
RB_DockWidget::~RB_DockWidget() {
    // delete mWidget; not required since mWidget is created with RB_DockWidget as parent
    RB_DEBUG->print("RB_DockWidget::~RB_DockWidget() OK");
}

/**
 * Get ID of current document, such as table, drawing from database
 * @return ID of document
 */
RB_String RB_DockWidget::getId() const {
    return mCentralWidget->getId();
}

/**
 * Get name of current document, such as table, drawing from filename or
 * window type in case of unique window
 * @return name of document or window type, shown as window title
 */
RB_String RB_DockWidget::getName() const {
    return mCentralWidget->getName();
}

/**
 * Get type of central widget (enumerator), which is used as identifier
 * when managing the open or active windows in the MDI area.
 * @return type of central widget
 */
int RB_DockWidget::getWidgetType() const {
    return mCentralWidget->getWidgetType();
}

/**
 * Set type of central widget
 * @param type type of widget (enumerator)
 */
void RB_DockWidget::setWidgetType(int type) {
    mCentralWidget->setWidgetType(type);
}

/**
 * Get central widget parent type. The parent type is used by the MDI windows to
 * identify the corresponding dockwindows for creation and/or activation
 * @return parent widget type
 */
int RB_DockWidget::getParentWidgetType() const {
    return mCentralWidget->getParentWidgetType();
}

/**
 * Set parent widget type, parent widget type is a MDI window widget type
 * @param type parent widget type
 */
void RB_DockWidget::setParentWidgetType(int type) {
    mCentralWidget->setParentWidgetType(type);
}

/**
 * Get perspective type, for example DB and LDT. This is about the same as
 * the engineering departments
 * @return perspective type
 */
RB2::PerspectiveType RB_DockWidget::getPerspectiveType() const {
    if (!mCentralWidget) return RB2::PerspectiveDefault; // TODO: command dockwidget to be with central widget
    return mCentralWidget->getPerspectiveType();
}

/**
 * Get central widget
 * @return central widget
 */
RB_Widget* RB_DockWidget::getWidget() {
    return mCentralWidget;
}

///**
// * Set central widget
// */
//void RB_DockWidget::setWidget(RB_Widget* wdgt) {
//    mCentralWidget = wdgt;
//    QDockWidget::setWidget(wdgt);
//}

/**
 * Show window. Does nothing, only QDockWidget::show();
 */
void RB_DockWidget::show() {
//    updateWindowTitle(false);
    QDockWidget::show();
}

/**
 * Window state change
 * @param oldState
 * @param newState
 */
void RB_DockWidget::slotWindowStateHasChanged(Qt::WindowStates /* oldState */,
                                         Qt::WindowStates newState) {
    if (newState & Qt::WindowActive) {
        emit refreshToggledActions(mCentralWidget->getPerspectiveType());
        mCentralWidget->setEnabled(true);
    } else {
        mCentralWidget->setEnabled(false);
    }
}

/**
 * Set window is modified state when model/document is changed or saved
 * @param changed true will add asterisk, false will remove asterisk
 */
//void  RB_DockWidget::slotChangedModification(bool changed){
//    updateWindowTitle(changed);
//}

/**
 * Close event of MDI window
 * @param event event
 */
void RB_DockWidget::closeEvent(QCloseEvent* e) {
    if (isWindowModified()) {
        if (mCentralWidget->closeWidget()) {
            e->accept();
        } else {
            e->ignore();
        }
    } else {
        mCentralWidget->closeWidget();
        e->accept();
    }
}

//void RB_DockWidget::updateWindowTitle(bool changed) {
//    if (mCentralWidget->getName().isEmpty()) {
//        RB_String strNr = RB_String::number(mUntitledNumber);
//        setWindowTitle(tr("Untitled") + strNr + "[*]");
//        setWindowModified(changed);
//    } else {
//        // placeholder [*] always required
//        setWindowTitle(mCentralWidget->getName() + "[*]");
//        setWindowModified(changed);
//    }
//}
