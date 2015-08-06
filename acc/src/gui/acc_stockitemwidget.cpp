/*****************************************************************
 * $Id: acc_stockitemwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 16, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_stockitemwidget.h"

#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfileopen.h"
#include "rb_datawidgetmapper.h"
#include "rb_idxlineedit.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
ACC_StockItemWidget::ACC_StockItemWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
ACC_StockItemWidget::~ACC_StockItemWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mModel; not here deleted by ACC_DebtorsMasterWidget (dockwindow)

    // HACK: otherwise crash (BTW do not use delete mMapper)
    mModel->deleteMapper();
    RB_DEBUG->print("ACC_StockItemWidget::~ACC_StockItemWidget() OK");
}

/**
 * Initialize widget and models
 * TODO: maybeSave() is not handled yet
 */
void ACC_StockItemWidget::init() {
    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelStockMaster);

    // because no setFormatTableView()
    connect(mModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
    connect(mModel, SIGNAL(modelBeforeSubmitted()),
            this, SLOT(slotDataIsSaved()));

    mMapper = mModel->getMapper();
    mMapper->addMapping(this->leItemCode, mModel->fieldIndex("stockcode"));
    mMapper->addMapping(this->leDescriptionShort, mModel->fieldIndex("description"));
    mMapper->addMapping(this->teDescriptionLong,mModel->fieldIndex("longdescription"));

    addComboBoxMapping(mModel, "unitofmeasurement", "ACC_UnitOfMeasure", "id", "unitname",
                       cbUnitOfMeasure, mMapper);
    mMapper->addMapping(lePackagedVolume, mModel->fieldIndex("volume"));
    mMapper->addMapping(lePackagedWeight,  mModel->fieldIndex("kgs"));
    mMapper->addMapping(lblPixmap, mModel->fieldIndex("stockimage"));

    // Current or obsolete
    RB_StringList items;
    items << tr("Current") << tr("Obsolete");
    cbCurrentObsolete->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbCurrentObsolete, mModel->fieldIndex("discontinued"), "currentIndex");
    // Assembly, Kit, Manufactured or Service/Labour (Phantom, Purchased) flag
    items.clear();
    items << tr("Assembly") << tr("Kit") << tr("Manufactured") << tr("Phantom")
             << tr("Purchase") << tr("Service/Labour");
    cbAssembly->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbAssembly, mModel->fieldIndex("mbflag"), "currentIndex");
    // Batch, serial or lot controlled (Control, No control)
    items.clear();
    items << tr("No control") << tr("Controlled");
    cbControl->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbControl, mModel->fieldIndex("controlled"), "currentIndex");
    // Yes, No
    items.clear();
    items << tr("No") << tr("Yes");
    cbSerialized->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbSerialized, mModel->fieldIndex("serialised"), "currentIndex");
    mMapper->addMapping(leSerialNumber, mModel->fieldIndex("serialno"));
    // Yes, No, Perishable = subject to decay, ruin, or destruction: perishable fruits and vegetables.
    cbPerishable->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbPerishable, mModel->fieldIndex("perishable"), "currentIndex");
    // Number of decimal places to display quantity
    mMapper->addMapping(leDecimalPlaces, mModel->fieldIndex("decimalplaces"));
    // Barcode
    mMapper->addMapping(leBarCode, mModel->fieldIndex("barcode"));

    // Accounting
    mMapper->addMapping(leDiscountCategory, mModel->fieldIndex("discountcategory"));
    mMapper->addMapping(leEconomic, mModel->fieldIndex("eoq"));
    // Raw material purchase cost per unit
    mMapper->addMapping(leStandardMaterialCost, mModel->fieldIndex("materialcost"));
    // Standard labour cost per unit
    mMapper->addMapping(leStandardLabourCost, mModel->fieldIndex("labourcost"));
    // Standard overhead cost per unit
    mMapper->addMapping(leStandardOverheadCost, mModel->fieldIndex("overheadcost"));

    // not (yet) used here from ACC_StockMaster
//    addMember("lastcurcostdate", "-", "", RB2::MemberDateTime);
//    addMember("actualcost", "-", 0.0, RB2::MemberDouble);
//    addMember("lastcost", "-", 0.0, RB2::MemberDouble);
//    addMember("lowestlevel", "-", 0, RB2::MemberInteger);
//
//    addMember("nextserialno", "-", 0, RB2::MemberInteger);
//    addMember("pansize", "-", 0.0, RB2::MemberDouble);
//    addMember("shrinkfactor", "-", 0.0, RB2::MemberDouble);
//    addMember("netweight", "-", 0.0, RB2::MemberDouble);

    // Cost Center
    mMapper->addMapping(ileCostCenter, mModel->fieldIndex("costcenter_idx"));

    // emit a invalid QModelIndex to clear mapper
    connect(mModel, SIGNAL(rootChanged()),
            mMapper, SLOT(slotClearWidgets()));

    readSettings();

    // Request modelindex in case of already selected index
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    QModelIndex idx = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockMaster);
    mMapper->setCurrentModelIndex(idx);
}


/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_StockItemWidget::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    bool childSuccess = false;

    if (withSelect) {
        childSuccess = mModel->submitAllAndSelect();

    } else {
        childSuccess = mModel->submitAll();
    }
    // QApplication::restoreOverrideCursor();

    if (!childSuccess) {
        return false;
    }

    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_StockItemWidget::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}


/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_StockItemWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_StockItemWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Button previous clicked
 */
void ACC_StockItemWidget::on_pbPrevious_clicked() {
    mMapper->toPrevious();
}

/**
 * Button next clicked
 */
void ACC_StockItemWidget::on_pbNext_clicked() {
    mMapper->toNext();
}

/**
 * Button save clicked
 */
void ACC_StockItemWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button revert clicked
 */
void ACC_StockItemWidget::on_pbRevert_clicked() {
    fileRevert();
}

/**
 * Add image clicked (will be 100x100 pixels)
 */
void ACC_StockItemWidget::on_pbAddImage_clicked() {
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    if (!wgt) {
        return;
    }
    QModelIndex index = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockMaster);

    if (!index.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No stock item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    DB_ActionFileOpen* action = new DB_ActionFileOpen();
    RB_String fn;
    action->getOpenFileName(fn, "Image files (*.gif *.jpg *.jpeg *.png);;"
                            "All files (*.*);;");
    delete action;

    if (fn.isEmpty()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No image selected."));
        return;
    }

    QPixmap pxm;

    if (!pxm.load(fn)) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not load image."));
        return;
    }

    pxm = pxm.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // save pixmap to byte array
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    pxm.save(&buffer, "PNG");

    int row = index.row();
    index = mModel->index(row, mModel->fieldIndex("stockimage"));
    mModel->setData(index, ba, Qt::EditRole); // bytearray instead of pxm
}

/**
 * Delete image clicked
 */
void ACC_StockItemWidget::on_pbDeleteImage_clicked() {
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    if (!wgt) {
        return;
    }
    QModelIndex index = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockMaster);

    if (!index.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No stock item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    int row = index.row();
    index = mModel->index(row, mModel->fieldIndex("stockimage"));
    mModel->setData(index, QByteArray(), Qt::EditRole);
}

/**
 * Button select cost center clicked
 */
void ACC_StockItemWidget::on_ileCostCenter_clicked() {
    RB_Widget* wgt = ACC_DIALOGFACTORY->getWidget(ACC_DialogFactory::WidgetNavigation, NULL);
    if (!wgt) {
        return;
    }
    QModelIndex index = wgt->getCurrentModelIndex(ACC_ModelFactory::ModelStockMaster);

    if (!index.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No stock item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCostCenter);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mModel->index(row, mModel->fieldIndex("costcenter_idx"));
            mModel->setData(index, obj->getId()
                            + obj->getValue("centercode").toString() + " - "
                            + obj->getValue("centername").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center selected.\n"
                                                   "Cost center is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void ACC_StockItemWidget::slotResetWidgets() {
    tabWidget->setCurrentIndex(0);
    leItemCode->setFocus();
    leItemCode->selectAll();
}

/**
 * Change event, for example translation
 */
void ACC_StockItemWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
