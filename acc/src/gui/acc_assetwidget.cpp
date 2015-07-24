/*****************************************************************
 * $Id: acc_assetwidget.cpp 2186 2014-10-16 14:04:01Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_assetwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_AssetWidget::ACC_AssetWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mAssetModel = NULL;
    mAssetMapper = NULL;
}

/**
 * Destructor
 */
ACC_AssetWidget::~ACC_AssetWidget() {
    // Do not delete mAssetModel it is shared with the ACC_AssetMasterWidget
    // instead delete only the mapper:
    mAssetModel->deleteMapper();
    RB_DEBUG->print("ACC_AssetWidget::~ACC_AssetWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_AssetWidget::init() {
    //
    // 1. Set model for customer mapper
    //
    mAssetModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAsset);
    // required because not setFormatTableView()
    connect(mAssetModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(slotDataIsChanged(const QModelIndex&, const QModelIndex&)));
    connect(mAssetModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));

    //
    // Get mapper for line edits etc.
    //
    mAssetMapper = mAssetModel->getMapper();
    mAssetMapper->addMapping(this->leAssetCode, mAssetModel->fieldIndex("assetcode"));
    mAssetMapper->addMapping(this->leAssetName, mAssetModel->fieldIndex("mname"));
    addComboBoxMapping(mAssetModel, "type_id", "ACC_AssetType", "id", "mname",
                       cbAssetType, mAssetMapper);
    mAssetMapper->addMapping(this->leData1, mAssetModel->fieldIndex("data1"));
    mAssetMapper->addMapping(this->leData2, mAssetModel->fieldIndex("data2"));
    mAssetMapper->addMapping(this->leData3, mAssetModel->fieldIndex("data3"));
    mAssetMapper->addMapping(this->leData4, mAssetModel->fieldIndex("data4"));
    mAssetMapper->addMapping(this->leData5, mAssetModel->fieldIndex("data5"));
    mAssetMapper->addMapping(this->leData6, mAssetModel->fieldIndex("data6"));

    mAssetMapper->addMapping(this->leLatitude, mAssetModel->fieldIndex("lat"));
    mAssetMapper->addMapping(this->leLongitude, mAssetModel->fieldIndex("lng"));
    mAssetMapper->addMapping(this->leSalesMan, mAssetModel->fieldIndex("salesman_id"));
    mAssetMapper->addMapping(this->dteSince, mAssetModel->fieldIndex("assetsince"));

    mAssetMapper->addMapping(this->leContact, mAssetModel->fieldIndex("contactname"));
    mAssetMapper->addMapping(this->leEmail, mAssetModel->fieldIndex("email"));
    mAssetMapper->addMapping(this->lePhoneNumber, mAssetModel->fieldIndex("phoneno"));
    mAssetMapper->addMapping(this->leFaxNumber, mAssetModel->fieldIndex("faxno"));

    mAssetMapper->addMapping(this->teNote, mAssetModel->fieldIndex("note"));

    readSettings();

    // Navigation widget has been set to the correct tab
    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelAsset);
    mAssetMapper->setCurrentModelIndex(idx);
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_AssetWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
    if (withSelect && mAssetModel) {
        if (mAssetModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mAssetModel) {
        if (mAssetModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Revert changes
 */
void ACC_AssetWidget::fileRevert() {
    mAssetModel->revertAll();
    setWindowModified(false);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_AssetWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_AssetWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Button previous clicked
 */
void ACC_AssetWidget::on_pbPrevious_clicked() {
    mAssetMapper->toPrevious();
}

/**
 * Button next clicked
 */
void ACC_AssetWidget::on_pbNext_clicked() {
    mAssetMapper->toNext();
}

/**
 * Button save clicked
 */
void ACC_AssetWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button revert clicked
 */
void ACC_AssetWidget::on_pbRevert_clicked() {
    fileRevert();
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void ACC_AssetWidget::slotResetWidgets() {
    tabWidget->setCurrentIndex(0);
    leAssetCode->setFocus();
    leAssetCode->selectAll();
}

/**
 * Change event, for example translation
 */
void ACC_AssetWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
