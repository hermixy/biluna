/*****************************************************************
 * $Id: rb_toolbar.cpp 1956 2013-07-27 16:03:14Z rutger $
 * Created: Jul 22, 2013 9:34:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_toolbar.h"

#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_mainwindow.h"



/**
 * Constructor
 * @param mw main window
 * @param mwgt main widget to be shown on MDI window
 */
RB_ToolBar::RB_ToolBar(RB_MainWindow* mw, RB_Widget* mwgt) : QToolBar(mw) {
    RB_DEBUG->print("RB_ToolBar::RB_ToolBar()");

    if (!mw || !mwgt) return;
    mMainWindow = mw;
    mCentralWidget = mwgt;

    setAttribute(Qt::WA_DeleteOnClose, false);
    setWidget(mCentralWidget);
    setWindowIcon(QIcon(":/images/biluna16.png"));

    connect(this, SIGNAL(windowStateChanged(Qt::WindowStates, Qt::WindowStates)),
            this, SLOT(slotWindowStateHasChanged(Qt::WindowStates, Qt::WindowStates)));
    connect(mCentralWidget, SIGNAL(widgetClose()), this, SLOT(close()));
}

/**
 * Destructor
 */
RB_ToolBar::~RB_ToolBar() {
    // delete mWidget; not required since mWidget is created with RB_ToolBar as parent
    RB_DEBUG->print("RB_ToolBar::~RB_ToolBar() OK");
}

/**
 * Get ID of current document, such as table, drawing from database
 * @return ID of document
 */
RB_String RB_ToolBar::getId() const {
    return mCentralWidget->getId();
}

/**
 * Get name of current document, such as table, drawing from filename or
 * window type in case of unique window
 * @return name of document or window type, shown as window title
 */
RB_String RB_ToolBar::getName() const {
    return mCentralWidget->getName();
}

/**
 * Get type of central widget (enumerator), which is used as identifier
 * when managing the open or active windows in the MDI area.
 * @return type of central widget
 */
int RB_ToolBar::getWidgetType() const {
    return mCentralWidget->getWidgetType();
}

/**
 * Set type of central widget
 * @param type type of widget (enumerator)
 */
void RB_ToolBar::setWidgetType(int type) {
    mCentralWidget->setWidgetType(type);
}

/**
 * Get central widget parent type. The parent type is used by the MDI windows to
 * identify the corresponding dockwindows for creation and/or activation
 * @return parent widget type
 */
int RB_ToolBar::getParentWidgetType() const {
    return mCentralWidget->getParentWidgetType();
}

/**
 * Set parent widget type, parent widget type is a MDI window widget type
 * @param type parent widget type
 */
void RB_ToolBar::setParentWidgetType(int type) {
    mCentralWidget->setParentWidgetType(type);
}

/**
 * Get perspective type, for example DB and LDT. This is about the same as
 * the engineering departments
 * @return perspective type
 */
RB2::PerspectiveType RB_ToolBar::getPerspectiveType() const {
    if (!mCentralWidget) return RB2::PerspectiveDefault; // TODO: command dockwidget to be with central widget
    return mCentralWidget->getPerspectiveType();
}

/**
 * Get central widget
 * @return central widget
 */
RB_Widget* RB_ToolBar::getWidget() {
    return mCentralWidget;
}

/**
 * Set central widget
 */
void RB_ToolBar::setWidget(RB_Widget* wdgt) {
    mCentralWidget = wdgt;
    QToolBar::addWidget(wdgt);
}

/**
 * Show window. Does nothing, only QDockWidget::show();
 */
void RB_ToolBar::show() {
//    updateWindowTitle(false);
    QToolBar::show();
}

/**
 * Window state change
 * @param oldState
 * @param newState
 */
void RB_ToolBar::slotWindowStateHasChanged(Qt::WindowStates /* oldState */,
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
//void  RB_ToolBar::slotChangedModification(bool changed){
//    updateWindowTitle(changed);
//}

/**
 * Close event of MDI window
 * @param event event
 */
void RB_ToolBar::closeEvent(QCloseEvent* e) {
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
