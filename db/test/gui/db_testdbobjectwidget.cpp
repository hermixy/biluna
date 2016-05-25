/*****************************************************************
 * $Id: db_testdbobjectwidget.cpp 1092 2010-02-10 09:52:33Z rutger $
 * Created: Dec 15, 2009 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_testdbobjectwidget.h"

#include "db_test.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "db_testtextwidget.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"


DB_TestDbObjectWidget::DB_TestDbObjectWidget(QWidget *parent) : RB_Widget(parent) {
    RB_DEBUG->print("DB_TestDbObjectWidget::DB_TestDbObjectWidget()");
    setupUi(this);

    mModel = NULL;
    mChildModel = NULL;
    mObject = NULL;
}

DB_TestDbObjectWidget::~DB_TestDbObjectWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mChildModel;
    delete mModel;
    delete mObject;
    RB_DEBUG->print("DB_TestDbObjectWidget::~DB_TestDbObjectWidget() OK");
}

void DB_TestDbObjectWidget::init() {
    //
    // Get ID of parent model
    //
    RB_MmProxy* parentModel =
            DB_MODELFACTORY->getParentModel(DB_ModelFactory::ModelTestDialog);
    QModelIndex parentIndex = parentModel->getCurrentIndex();
    if (!parentIndex.isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected, \n"
                                                      "could not open dialog."));
    }

    //
    // Set model and connect to table view
    //
    mModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestDialog);
    mModel->setRoot(parentIndex);

    // attach model to main view
    tvParent->setModel(mModel);
    tvParent->setSelectionModel(new QItemSelectionModel(mModel));
    tvParent->setAlternatingRowColors(true);
    tvParent->setEditTriggers(QAbstractItemView::EditKeyPressed |
                                QAbstractItemView::DoubleClicked);
    tvParent->setSortingEnabled(true);
    tvParent->verticalHeader()->setDefaultSectionSize(20); // row height
    tvParent->setItemDelegate(new RB_SqlRelationalDelegate(mModel, this));

    connect(tvParent->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            mModel, SLOT(changeCurrentRow(const QModelIndex&,const QModelIndex&)));
    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));

    //
    // No mapper
    //

    //
    // Set child model and connect to child (table view),
    // root will be set by selecting row in parent model
    //
    mChildModel = DB_MODELFACTORY->getModel(DB_ModelFactory::ModelTestChild);

    tvChild->setModel(mChildModel);
    tvChild->setAlternatingRowColors(true);
    tvChild->setEditTriggers(QAbstractItemView::EditKeyPressed |
                                QAbstractItemView::DoubleClicked);
    tvChild->setSortingEnabled(true);
    tvChild->verticalHeader()->setDefaultSectionSize(20); // row height

    connect(mModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mChildModel, SLOT(parentCurrentRowChanged(QModelIndex,QModelIndex)));
    connect(mChildModel, SIGNAL(rootChanged(const QModelIndex&)),
            tvChild, SLOT(setRootIndex(const QModelIndex&)));
    connect(mChildModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
}

void DB_TestDbObjectWidget::on_pbRead_clicked() {

//    QModelIndex index = mModel->getCurrentIndex(); // actual index of source model
//    const RB_MmObject* m = dynamic_cast<const RB_MmObject*>(index.model());
//    RB_String id = m->data(m->index(index.row(), 0, index.parent()),
//                                        Qt::DisplayRole).toString();
    RB_String id = mModel->getCurrentId();

    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
            DB_DialogFactory::WidgetTestText);
    DB_TestTextWidget* tw = dynamic_cast<DB_TestTextWidget*>(mdiWin->getWidget());

    RB_String str = "ID: " + id + "\n";
    if (!mObject) {
        mObject = DB_OBJECTFACTORY->newSingleObject("DB_Test");
    }
    mObject->setId(id);

    if (this->rbResolveNone->isChecked()) {
        mObject->dbRead(mModel->database(), RB2::ResolveNone);
    } else if (this->rbResolveOne->isChecked()) {
        mObject->dbRead(mModel->database(), RB2::ResolveOne);
    } else if (this->rbResolveAll->isChecked()) {
        mObject->dbRead(mModel->database(), RB2::ResolveAll);
    }

    str += mObject->toString();
    tw->appendText(str);
    mdiWin->show();
}

void DB_TestDbObjectWidget::on_pbUpdate_clicked() {
    if (!mObject) return;

    mObject->setValue("value1", "Rutger");
    mObject->setValue("value2", 98.76);
    mObject->setValue("spinbox", 101);
    if (this->rbResolveNone->isChecked()) {
        mObject->dbUpdate(mModel->database(), RB2::ResolveNone);
    } else if (this->rbResolveOne->isChecked()) {
        mObject->dbUpdate(mModel->database(), RB2::ResolveOne);
    } else if (this->rbResolveAll->isChecked()) {
        mObject->dbUpdate(mModel->database(), RB2::ResolveAll);
    }
}

void DB_TestDbObjectWidget::on_pbDelete_clicked() {
    if (!mObject) return;
    mObject->dbDelete(mModel->database());
}

void DB_TestDbObjectWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
