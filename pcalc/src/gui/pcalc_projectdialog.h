/*****************************************************************
 * $Id: peng_projectdialog.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 29, 2015 21:25:40 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_PROJECTDIALOG_H
#define PCALC_PROJECTDIALOG_H

#include <QtWidgets>
#include "pcalc_modelfactory.h"
#include "db_tabledialog.h"
#include "db_modelfactory.h"
#include "db_permissionhandler.h"
#include "rb_dialogbase.h"

class PCALC_ProjectDialog : public DB_TableDialog {

    Q_OBJECT

public:
    PCALC_ProjectDialog(QWidget* parent) : DB_TableDialog(parent) {
        // HACK: for write settings otherwise RB_DialogBase
        this->setObjectName("PCALC_ProjectDialog");
    }

    virtual ~PCALC_ProjectDialog() {
        // Dialogs create and delete models by themselves
        if (mModel) {
            delete mModel;
            mModel = NULL;
        }
    }

    void init() {
        // Set model, root="" is already set by modelFactory, however change to system root
        mModel = PCALC_MODELFACTORY->getModel(PCALC_ModelFactory::ModelProject, false);

        if (!DB_PERMISSIONHANDLER->isAdmin()) {
            // id set to "" will remove statement: parent='XXXX'
            mModel->setRoot("");

            QStringList projectIdList;
            DB_PERMISSIONHANDLER->getProjectIdList("PCALC", projectIdList);
            QString whereStatement = "INVALID_USER";

            int idCount = projectIdList.size();

            for (int i = 0; i < idCount; ++i) {
                if (i > 0) {
                    whereStatement += " OR ";
                } else {
                    whereStatement.clear();
                }

                whereStatement += "id='" + projectIdList.at(i) + "'";
            }

            mModel->setWhere(whereStatement);
        } else {
            // Single user
            mModel->setRoot(DB_MODELFACTORY->getRootId());
        }

        mModel->select();

//        for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
//            if (i != mModel->fieldIndex("coyname") && i != mModel->fieldIndex("regoffice1")
//                    && i != mModel->fieldIndex("regoffice2") && i != mModel->fieldIndex("regoffice3")
//                    && i != mModel->fieldIndex("regoffice4") && i != mModel->fieldIndex("regoffice5")
//                    && i != mModel->fieldIndex("regoffice6")) {
//                tableView->hideColumn(i);
//            }
//        }

        for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
            tableView->hideColumn(i);
        }

        setWindowTitle(tr("Select project[*]"));
        // lblRoot->setText(tr("Plant Engineering Project"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("List of projects"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("PCALC_ProjectDialog");

        readSettings();
        int count = PCALC_MODELFACTORY->hiddenColumnCount();

        for (int i = 0; i < count; i++) {
            tableView->hideColumn(i);
        }

        // Make sure there is one ACC_Project
        if (mModel->rowCount() < 1) {
            on_pbAdd_clicked();
        }
    }

    void on_pbAdd_clicked() {
        mModel->undoFilterSort();
        int row = 0; // mModel->rowCount();
        mModel->insertRows(row, 1, QModelIndex());

        // NOTE: do not forget to set the default column values, specially for the
        //       relational table otherwise new row will not show!
        QModelIndex idx = mModel->index(row, mModel->fieldIndex("number"), QModelIndex());
        mModel->setData(idx, tr("<NEW>"), Qt::EditRole);
        // end NOTE

        tableView->setCurrentIndex(mModel->index(row, mModel->fieldIndex("number"), QModelIndex()));
        tableView->scrollTo(mModel->index(row, mModel->fieldIndex("number")));
    }

};

#endif // PCALC_PROJECTDIALOG_H
