/*****************************************************************
 * $Id: db_selectmappingdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 8, 2009 18:25:40 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SELECTMAPPINGDIALOG_H
#define DB_SELECTMAPPINGDIALOG_H

#include <QtWidgets>
#include "db_modelfactory.h"
#include "rb_dialogbase.h"

class DB_EXPORT DB_SelectMappingDialog : public RB_DialogBase {

    Q_OBJECT

public:
    DB_SelectMappingDialog(QWidget* parent) : RB_DialogBase(parent) {
        // nothing
    }

    virtual ~DB_SelectMappingDialog() {
        // Dialogs create and delete models by themselves
        delete mModel;
    }

    void init() {
//        RB_ModelFactoryInterface* mf = mMainWindow->getModelFactory();
//
//        // Get parent object data
//        RB_MmObjectInterface* pMm = mf->getParentModel(modelType);
//        if (!pMm) {
//            RB_DEBUG->print(RB_Debug::D_ERROR,
//                            "Dialog::init() no parent model ERROR");
//            return;
//        }
//
//        QModelIndex index = pMm->getCurrentIndex(); // gives actual index
//        if (!index.isValid()) {
//            RB_DEBUG->print(RB_Debug::D_ERROR,
//                            "Dialog::init() no valid index ERROR");
//            return;
//        }
//        const RB_MmObject* mM = dynamic_cast<const RB_MmObject*>(index.model());
//        leRoot->setText(mM->data(mM->index(index.row(), 3, index.parent()),
//                                           Qt::DisplayRole).toString());

        // Set model, root is already set by modelFactory
        mModel = DB_MODELFACTORY->getModel(DB_MODELFACTORY->ModelSelectMapping);
        mModel->setRoot(DB_MODELFACTORY->getRootId());

        setWindowTitle(tr("Select Mapping"));
        // lblRoot->setText(tr("Root"));
        lblRoot->hide();
        leRoot->hide();

        gbTable->setTitle(tr("Available mappings"));
        pbAdd->setText(tr("&Add"));
        pbDelete->setText(tr("&Delete"));
        pbUp->setText(tr("&Up"));
        pbDown->setText(tr("D&own"));
        pbHelp->setText(tr("&Help"));
        pbOk->setText(tr("&OK"));
        pbCancel->setText(tr("&Cancel"));
        lblTvInfo->setText(tr("Click item to select or [F2] to edit"));

        this->initEditSort();
//        this->initEditUpDown();
//        this->initSelectionOnly();
//        this->initTest();

        this->setHelpSubject("DB_SelectMappingDialog");
    }
};

#endif // DB_SELECTMAPPINGDIALOG_H
