/*****************************************************************
 * $Id: acc_selectcontactdialog.h 2153 2014-07-25 16:38:28Z rutger $
 * Created: Feb 21, 2010 11:25:40 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCONTACTDIALOG_H
#define ACC_SELECTCONTACTDIALOG_H

#include <QtWidgets>
#include "db_tabledialog.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "rb_mdiwindow.h"

class ACC_SelectContactDialog : public DB_TableDialog {

    Q_OBJECT

public:
    ACC_SelectContactDialog(QWidget* parent) : DB_TableDialog(parent) {
        this->setObjectName("ACC_SelectContactDialog");
    }

    virtual ~ACC_SelectContactDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Get parent id
        RB_MmProxy* model = NULL;
        RB_MdiWindow* mdiW = ACC_DIALOGFACTORY->getActiveMdiWindow();
        int wgtType = mdiW->getWidgetType();

        if (wgtType == ACC_DialogFactory::WidgetSalesOrder) {
            model = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesOrder);
        } else if (wgtType == ACC_DialogFactory::WidgetPurchaseOrder) {
            model = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPurchaseOrder);
        }

        if (!model) {
            RB_DEBUG->error("ACC_SelectContactDialog.init() model is NULL ERROR");
            return;
        }

        // Set model, root is already set by modelFactory
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSelectContact);
        mModel->setRoot(model->getCurrentValue("parent").toString());
        mModel->select();

        setWindowTitle(tr("ACC Select Contact[*]"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of contacts"));
//        pbAdd->setText(tr("&Add")); //  has icon
//        pbDelete->setText(tr("&Delete")); //  has icon
//        pbUp->setText(tr("&Up")); //  has icon
//        pbDown->setText(tr("D&own")); //  has icon
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

//        this->initEditSort();
//        this->initEditUpDown();
        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("ACC_SelectContactDialog");
        this->readSettings();

        // Hidden columns only for tableviews
        int colCount = mModel->columnCount();
        for (int i = 0; i < colCount; ++i) {
            if (i != mModel->fieldIndex("firstname") && i != mModel->fieldIndex("lastname")
                    && i != mModel->fieldIndex("title") && i != mModel->fieldIndex("phonework")
                    && i != mModel->fieldIndex("email1")) {
                tableView->hideColumn(i);
            } else {
                tableView->showColumn(i);

                if (tableView->columnWidth(i) < 5) {
                    tableView->setColumnWidth(i, 100);
                }
            }
        }

    }
};

#endif // ACC_SELECTCONTACTDIALOG_H
