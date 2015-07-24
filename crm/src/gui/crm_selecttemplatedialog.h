/*****************************************************************
 * $Id: crm_selecttemplatedialog.h 2068 2014-01-21 21:53:40Z rutger $
 * Created: Mar 19, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_SELECTTEMPLATEDIALOG_H
#define CRM_SELECTTEMPLATEDIALOG_H

#include <QtWidgets>
#include "crm_modelfactory.h"
#include "db_tabledialog.h"
#include "rb_sqlrelation.h"

class CRM_SelectTemplateDialog : public DB_TableDialog {

    Q_OBJECT

public:
    CRM_SelectTemplateDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("CRM_SelectTemplateDialog");
    }

    virtual ~CRM_SelectTemplateDialog() {
        // Non unique models are created and deleted by the dialog
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root is already set by modelFactory
        mModel = CRM_MODELFACTORY->getModel(CRM_ModelFactory::ModelTemplate, false);
//        // Relation
//        mModel->setRelation(mModel->fieldIndex("accountgroup_id"),
//               RB_SqlRelation("ACC_AccountGroups", "id", "groupname"));
        mModel->setRoot(CRM_MODELFACTORY->getRootId());
        mModel->select();



        setWindowTitle(tr("CRM Select template"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Groups"));
//        pbAdd->setText(tr("&Add"));  //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&Select"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("CRM_SelectTemplateDialog");
        this->readSettings();

        int colCount = mModel->columnCount();

        for (int i = 0; i < colCount; i++) {
            if (i == RB2::HIDDENCOLUMNS + 1 || i == RB2::HIDDENCOLUMNS + 2) {
                tableView->showColumn(i);
            } else {
                tableView->hideColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }
    }
};

#endif // CRM_SELECTTEMPLATEDIALOG_H
