/*****************************************************************
 * $Id: peng_pipeclassdialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pipeclassdialog.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_sqlrelationaldelegate.h"


PENG_PipeClassDialog::PENG_PipeClassDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModelPipeClass = NULL;
    mModelLineSize = NULL;
    mModelDesignPT = NULL;

    setMinimumSize(420, 320);
}

PENG_PipeClassDialog::~PENG_PipeClassDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModelDesignPT;
    delete mModelLineSize;
    delete mModelPipeClass;
    RB_DEBUG->print("PENG_PipeClassDialog::~PENG_PipeClassDialog() OK");
}

void PENG_PipeClassDialog::init() {
    setWindowTitle(tr("PENG Edit Pipe Class Specification[*]"));

    // 1. Set model with ID
    mModelPipeClass = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPipeClass, false);
    mModelLineSize = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPipeClassSize, false);
    mModelDesignPT = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPipeClassPT, false);

    mModelPipeClass->setRoot(PENG_MODELFACTORY->getRootId());

    // 2. No mappers

    // 3. Model
    mModelPipeClass->select();

    // 4. Connect model to main view
    formatTableView(tvPipeClass, mModelPipeClass);
    formatTableView(tvLineSize, mModelLineSize);
    formatTableView(tvDesignPT, mModelDesignPT);

    // 5. Set toolbuttonbar
    tbbPipeClass->initEdit(false, false, false);
    tvPipeClass->setToolButtonBar(tbbPipeClass);
//    connect(tbbPaint, SIGNAL(addClicked()),
//            this, SLOT(slotPbAdd_clicked()));
    tbbLineSize->initEdit(false, false, false);
    tvLineSize->setToolButtonBar(tbbLineSize);
    tbbDesignPT->initEdit(false, false, false);
    tvDesignPT->setToolButtonBar(tbbDesignPT);

    // 5. Set visible columns in tableView
    int colCount = mModelPipeClass->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvPipeClass->hideColumn(i);
        } else {
            tvPipeClass->showColumn(i);

            if (tvPipeClass->columnWidth(i) < 5) {
                tvPipeClass->setColumnWidth(i, 100);
            }
        }
    }

    colCount = mModelLineSize->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvLineSize->hideColumn(i);
        } else {
            tvLineSize->showColumn(i);

            if (tvLineSize->columnWidth(i) < 5) {
                tvLineSize->setColumnWidth(i, 100);
            }
        }
    }

    colCount = mModelDesignPT->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvDesignPT->hideColumn(i);
        } else {
            tvDesignPT->showColumn(i);

            if (tvDesignPT->columnWidth(i) < 5) {
                tvDesignPT->setColumnWidth(i, 100);
            }
        }
    }

    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool PENG_PipeClassDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect) {
        mModelDesignPT->submitAllAndSelect();
        mModelLineSize->submitAllAndSelect();
        mModelPipeClass->submitAllAndSelect();
        setWindowModified(false);
        return true;
    } else {
        mModelDesignPT->submitAll();
        mModelLineSize->submitAll();
        mModelPipeClass->submitAll();
        setWindowModified(false);
        return true;
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void PENG_PipeClassDialog::fileRevert() {
    mModelDesignPT->revert();
    mModelLineSize->revert();
    mModelPipeClass->revert();
    setWindowModified(false);
}

/**
 * Button help clicked
 */
void PENG_PipeClassDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = PENG_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void PENG_PipeClassDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void PENG_PipeClassDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void PENG_PipeClassDialog::changeEvent(QEvent* e) {
    RB_Dialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
