/*****************************************************************
 * $Id: rb_mdiwindow.cpp 2213 2015-01-28 16:45:13Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_mdiwindow.h"

#include <QCloseEvent>
#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_mainwindow.h"


/**
 * Constructor
 * @param mw main window
 * @param mwgt main widget to be shown on MDI window
 */
RB_MdiWindow::RB_MdiWindow(RB_MainWindow* mw, RB_Widget* mwgt)
            : QMdiSubWindow(mw) {
    RB_DEBUG->print("RB_MdiWindow::RB_MdiWindow()");

    if (!mw || !mwgt) return;
    mMainWindow = mw;
    mCentralWidget = mwgt;

    setWidget(mCentralWidget);
    setObjectName("RB_MdiWindow");
    setWindowIcon(QIcon(":/images/biluna16.png"));
    setAttribute(Qt::WA_DeleteOnClose);

    connect(mCentralWidget, SIGNAL(modificationChanged(bool)),
            this, SLOT(slotChangedModification(bool)));
    connect(mCentralWidget, SIGNAL(widgetClose()), this, SLOT(close()));
}

/**
 * Destructor
 */
RB_MdiWindow::~RB_MdiWindow() {
    // Regular delete was not required since mWidget is created
    // with RB_MdiWindow as parent. However crash of DB_InternetBrowserWidget
    // deleteLater() provides object leak in case of application close()
//    setWidget(NULL);
//    mCentralWidget->deleteLater();
    RB_DEBUG->print("RB_MdiWindow::~RB_MdiWindow() OK");
}

/**
 * Get ID of current document, such as database table name,
 * drawing from database.
 * @return ID of document
 */
RB_String RB_MdiWindow::getId() const {
    return mCentralWidget->getId();
}

/**
 * Get name of current document, such as table, drawing from filename or
 * window type in case of unique window
 * @return name of document or window type, shown as window title
 */
RB_String RB_MdiWindow::getName() const {
    return mCentralWidget->getName();
}

/**
 * Get the full file path and file name, instead of name which
 * only gives the file name
 * @return full file path and file name
 */
QString RB_MdiWindow::getSaveAsFileName() const {
    return mCentralWidget->getSaveAsFileName();
}

/**
 * Get type of central widget (enumerator), which is used as identifier
 * when managing the open or active windows in the MDI area.
 * @return type of central widget
 */
int RB_MdiWindow::getWidgetType() const {
    return mCentralWidget->getWidgetType();
}

/**
 * Set type of central widget
 * @param type type of widget (enumerator)
 */
void RB_MdiWindow::setWidgetType(int type) {
    mCentralWidget->setWidgetType(type);
}

/**
 * Get central widget parent type. The parent type is used by the MDI windows to
 * identify the corresponding dockwindows for creation and/or activation
 * @return parent widget type
 */
int RB_MdiWindow::getParentWidgetType() const {
    return mCentralWidget->getParentWidgetType();
}

/**
 * Set parent widget type, parent widget type is a MDI window widget type
 * @param type parent widget type
 */
void RB_MdiWindow::setParentWidgetType(int type) {
    mCentralWidget->setParentWidgetType(type);
}

/**
 * Get perspective type, for example DB and LDT. This is about the same as
 * the engineering departments
 * @return perspective type
 */
RB2::PerspectiveType RB_MdiWindow::getPerspectiveType() const {
    return mCentralWidget->getPerspectiveType();
}

/**
 * Get central widget
 * @return central widget
 */
RB_Widget* RB_MdiWindow::getWidget() {
    return mCentralWidget;
}

/**
 * Show window, overriding non-virtual base function to set correct
 * window title based on widget name first.
 */
void RB_MdiWindow::show() {
    updateWindowTitle(false);
    QMdiSubWindow::show();
}

/**
 * Set window is modified state when model/document is dirty or saved
 * @param dirty true will add asterisk, false will remove asterisk
 */
void  RB_MdiWindow::slotChangedModification(bool dirty){
    updateWindowTitle(dirty);
}

/**
 * Close event of MDI window
 * @param event event
 */
void RB_MdiWindow::closeEvent(QCloseEvent* e) {
    if (isWindowModified()) {
        if (mCentralWidget->closeWidget()) {
            e->accept();
        } else {
            e->ignore();
        }
    } else {
        e->accept();
    }
}

/**
 * Update window title based on the central widget title
 * @param dirty is true if document has been edited and not yet saved
 */
void RB_MdiWindow::updateWindowTitle(bool dirty) {
    if (isWindowModified() && dirty) {
        // Widget already set modified
        return;
    }

    if (mCentralWidget->getName().isEmpty()) {
        RB_DEBUG->warning("RB_MdiWindow::updateWindowTitle() document name empty WARNING");
    } else {
        // placeholder [*] always required
        setWindowTitle(mCentralWidget->getName() + "[*]");
        setWindowModified(dirty);
    }
}
