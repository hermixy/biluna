/*****************************************************************
 * $Id: rb_tabledialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jan 31, 2016 8:34:42 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_tablewidget.h"

#include <QtWidgets>
#include "db_dialogfactory.h"
#include "db_filterdialog.h"
#include "rb_mmproxy.h"

/**
 * Constructor
 * @param parent parent widget such as a mainwindow
 */
RB_TableWidget::RB_TableWidget(QWidget *parent)
        : RB_Widget(parent) {
    RB_DEBUG->debug("RB_TableWidget::RB_TableWidget()");

    setupUi(this);

    mCurrentModelIndex = QModelIndex();
    mCurrentObject = NULL;

    mModel = NULL;
}

/**
 * Destructor
 */
RB_TableWidget::~RB_TableWidget() {
    RB_DEBUG->debug("RB_TableWidget::~RB_TableWidget()");
}

/**
 * File Save, save model changes
 */
bool RB_TableWidget::fileSave(bool withSelect) {
    if (!mModel) return false;
    beforeFileSave();

    mCurrentModelIndex = tableView->currentIndex();

    if (!mCurrentModelIndex.isValid()) {
        mCurrentObject = NULL;
    } else {
        // for selection windows
        setCurrentObject();
    }

    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            
            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->print("RB_TableWidget::fileSave() ERROR");
    return false;
}

/**
 * File Revert, reverse modelchanges
 */
void RB_TableWidget::fileRevert() {
    if (mModel) {
        mModel->revertAll();
        setWindowModified(false);
    }
}

/**
 * Set help subject for 'Help' button. For a dialog this is usually
 * the dialog class name
 * @param subject help subject
 */
void RB_TableWidget::setHelpSubject(const RB_String& subject) {
    mHelpSubject = subject;
}

/**
 * Get current object based on selected model index.
 * @return object with data from model
 */
RB_ObjectBase* RB_TableWidget::getCurrentObject() {
    return mCurrentObject;
}

void RB_TableWidget::initEditSort(bool isFindFilter) {
    formatTableView(tableView, mModel);
    tbb->initEdit(false, true, isFindFilter, false);
    tableView->setToolButtonBar(tbb);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void RB_TableWidget::initEditUpDown() {
    formatTableView(tableView, mModel);
    tbb->initEdit(false, false, true, false);
    tableView->setToolButtonBar(tbb);
    tableView->setSortingEnabled(false);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void RB_TableWidget::initEditOnly() {
    formatTableView(tableView, mModel);
    tbb->initDetailEdit(true);
    tableView->setToolButtonBar(tbb);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

void RB_TableWidget::initSelectionOnly() {
    formatTableView(tableView, mModel);
    tbb->initSelect(true, false);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leRoot->setReadOnly(true);
    cbSourceModelFilter->hide();
}

/**
 * Initialize base dialog for testing only (both sorting and up/down buttons)
 * @param model table view model
 */
void RB_TableWidget::initTest() {
    formatTableView(tableView, mModel);
    tbb->initEdit(false, true, true, true);
    tableView->setToolButtonBar(tbb);
    leRoot->setReadOnly(true);
}

/**
 * Set current object with data from current selected index
 */
void RB_TableWidget::setCurrentObject() {
    if (tableView->currentIndex().isValid()) {
        mCurrentObject = mModel->getCurrentObject();
    } else {
        mCurrentObject = NULL;
    }
}

/**
 * Event handler to handle state changes, such as language change
 * @param e event
 */
void RB_TableWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
