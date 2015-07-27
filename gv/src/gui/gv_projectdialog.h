/*****************************************************************
 * $Id: gv_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 19, 2010 21:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PROJECTDIALOG_H
#define GV_PROJECTDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "gv_modelfactory.h"

/**
 * Basic project edit/selection dialog
 */
class GV_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    GV_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("GV_ProjectDialog");
    }

    virtual ~GV_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = GV_MODELFACTORY->getModel(GV_ModelFactory::ModelProject);
        mModel->setRoot("");
        mModel->select();

        setWindowTitle(tr("Select project"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of projects"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("GV_ProjectDialog");
        this->readSettings();

        for (int i = 0; i < RB2::HIDDENCOLUMNS ; i++) {
            tableView->hideColumn(i);
        }
    }
};

#endif // GV_PROJECTDIALOG_H
