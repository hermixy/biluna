/*****************************************************************
 * $Id: rb_mdiwindow.cpp 2213 2015-01-28 16:45:13Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_dialogwindow.h"

#include <QCloseEvent>
#include "rb_debug.h"
#include "rb_dialogfactory.h"
#include "rb_mainwindow.h"


/**
 * Constructor
 * @param mw main window
 * @param mwgt main widget to be shown on MDI window
 */
RB_DialogWindow::RB_DialogWindow(RB_MainWindow* mw, RB_Widget* mwgt)
            : QDialog(mw) {
    RB_DEBUG->print("RB_DialogWindow::RB_DialogWindow()");

    if (!mw || !mwgt) return;
    mMainWindow = mw;
    mCentralWidget = mwgt;

    setObjectName("RB_DialogWindow");
    setWindowIcon(QIcon(":/images/biluna16.png"));
    // setAttribute(Qt::WA_DeleteOnClose); not for dialog because
    // getCurrentObject() method can be called after close.

    // create layout and bottom buttons, dw is dialog window
    gridLayout_dw = new QGridLayout(this);
    gridLayout_dw->setObjectName(QStringLiteral("gridLayout_dw"));
    gridLayout_dw->setHorizontalSpacing(0);
    gridLayout_dw->setVerticalSpacing(0);
    gridLayout_dw->setContentsMargins(0, 0, 0, 0);
    gridLayout_dw->addWidget(mCentralWidget, 0, 0, 1, 1);

    horizontalLayout_dw = new QHBoxLayout();
    horizontalLayout_dw->setContentsMargins(3, 3, 3, 3);
    horizontalLayout_dw->setObjectName(QStringLiteral("horizontalLayout_dw"));
    pbHelp = new QPushButton(this);
    pbHelp->setObjectName(QStringLiteral("pbHelp"));
    pbHelp->setText(tr("&Help"));
    horizontalLayout_dw->addWidget(pbHelp);

    horizontalSpacer_dw = new QSpacerItem(40, 20, QSizePolicy::Expanding,
                                       QSizePolicy::Minimum);
    horizontalLayout_dw->addItem(horizontalSpacer_dw);

    pbOk = new QPushButton(this);
    pbOk->setObjectName(QStringLiteral("pbOk"));
    pbOk->setText(tr("&OK"));
    pbOk->setDefault(true);
    horizontalLayout_dw->addWidget(pbOk);

    pbCancel = new QPushButton(this);
    pbCancel->setObjectName(QStringLiteral("pbCancel"));
    pbCancel->setText(tr("&Cancel"));
    horizontalLayout_dw->addWidget(pbCancel);

    gridLayout_dw->addLayout(horizontalLayout_dw, 1, 0, 1, 1);

    // connection
    connect(mCentralWidget, SIGNAL(modificationChanged(bool)),
            this, SLOT(slotChangedModification(bool)));
    connect(mCentralWidget, SIGNAL(widgetClose()), this, SLOT(close()));
    connect(pbHelp, SIGNAL(clicked()), this, SLOT(slotHelp()));
    connect(pbOk, SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

/**
 * Destructor
 */
RB_DialogWindow::~RB_DialogWindow() {
    // Regular delete was not required since mWidget is created
    // with RB_DialogWindow as parent. However crash of DB_InternetBrowserWidget
    // deleteLater() provides object leak in case of application close()
    //    setWidget(NULL);
    //    mCentralWidget->deleteLater();
    RB_DEBUG->print("RB_DialogWindow::~RB_DialogWindow() OK");
}

/**
 * Get ID of current document, such as database table name,
 * drawing from database.
 * @return ID of document
 */
RB_String RB_DialogWindow::getId() const {
    return mCentralWidget->getId();
}

/**
 * Get name of current document, such as table, drawing from filename or
 * window type in case of unique window
 * @return name of document or window type, shown as window title
 */
RB_String RB_DialogWindow::getName() const {
    return mCentralWidget->getName();
}

/**
 * Get the full file path and file name, instead of name which
 * only gives the file name
 * @return full file path and file name
 */
QString RB_DialogWindow::getSaveAsFileName() const {
    return mCentralWidget->getSaveAsFileName();
}

/**
 * Get type of central widget (enumerator), which is used as identifier
 * when managing the open or active windows in the MDI area.
 * @return type of central widget
 */
int RB_DialogWindow::getWidgetType() const {
    return mCentralWidget->getWidgetType();
}

/**
 * Set type of central widget
 * @param type type of widget (enumerator)
 */
void RB_DialogWindow::setWidgetType(int type) {
    mCentralWidget->setWidgetType(type);
}

/**
 * Get central widget parent type. The parent type is used by the MDI windows to
 * identify the corresponding dockwindows for creation and/or activation
 * @return parent widget type
 */
int RB_DialogWindow::getParentWidgetType() const {
    return mCentralWidget->getParentWidgetType();
}

/**
 * Set parent widget type, parent widget type is a MDI window widget type
 * @param type parent widget type
 */
void RB_DialogWindow::setParentWidgetType(int type) {
    mCentralWidget->setParentWidgetType(type);
}

/**
 * Get perspective type, for example DB and LDT. This is about the same as
 * the engineering departments
 * @return perspective type
 */
RB2::PerspectiveType RB_DialogWindow::getPerspectiveType() const {
    return mCentralWidget->getPerspectiveType();
}

/**
 * Get central widget
 * @return central widget
 */
RB_Widget* RB_DialogWindow::getCentralWidget() {
    return mCentralWidget;
}

/**
 * Get current data object from main model
 * @return RB_ObjectBase data object
 */
RB_ObjectBase* RB_DialogWindow::getCurrentObject() {
    return mCentralWidget->getCurrentObject();
}

/**
 * Get current child data object from submodel 1
 * @return RB_ObjectBase data object
 */
RB_ObjectBase* RB_DialogWindow::getCurrentChild1Object() {
    return mCentralWidget->getCurrentChild1Object();
}

/**
 * Get current child data object from submodel 2
 * @return RB_ObjectBase data object
 */
RB_ObjectBase* RB_DialogWindow::getCurrentChild2Object() {
    return mCentralWidget->getCurrentChild2Object();
}

void RB_DialogWindow::initHelpClose() {
    pbOk->hide();
    pbCancel->setText(tr("Close"));
}

/**
 * Show window, overriding non-virtual base function to set correct
 * window title based on widget name first.
 */
void RB_DialogWindow::show() {
    updateWindowTitle(false);
    QDialog::show();
}

/**
 * Show window, dialog modal
 * @return result
 */
int RB_DialogWindow::exec() {
    updateWindowTitle(false);
    return QDialog::exec();
}

/**
 * Set window is modified state when model/document is dirty or saved
 * @param dirty true will add asterisk, false will remove asterisk
 */
void  RB_DialogWindow::slotChangedModification(bool dirty){
    updateWindowTitle(dirty);
}

/**
 * Slot handling Help button clicked
 */
void RB_DialogWindow::slotHelp() {
    if (mCentralWidget) {
        mMainWindow->slotHelpSubject(mCentralWidget->getHelpSubject());
    }
}

/**
 * Slot handling Ok button clicked
 */
void RB_DialogWindow::slotOk() {
    if (mCentralWidget) {
        // database select not required, dialog is closed
        bool withSelect = false;
        mCentralWidget->fileSave(withSelect);
    }

    QDialog::accept();
}

/**
 * Slot handling Cancel button clicked
 */
void RB_DialogWindow::slotCancel() {
    if (mCentralWidget) {
        // revert changes otherwise saved to database
        mCentralWidget->fileRevert();
    }

    QDialog::reject();
}

/**
 * Close event of MDI window
 * @param event event
 */
void RB_DialogWindow::closeEvent(QCloseEvent* e) {
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
void RB_DialogWindow::updateWindowTitle(bool dirty) {
    if (isWindowModified() && dirty) {
        // Widget already set modified
        return;
    }

    if (mCentralWidget->getName().isEmpty()) {
        RB_DEBUG->warning("RB_DialogWindow::updateWindowTitle() "
                          "document name empty WARNING");
    } else {
        // placeholder [*] always required
        setWindowTitle(mCentralWidget->getName() + "[*]");
        setWindowModified(dirty);
    }
}
