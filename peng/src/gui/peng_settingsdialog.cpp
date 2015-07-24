/*****************************************************************
 * $Id: peng_settingsdialog.cpp 2155 2014-08-04 16:50:28Z rutger $
 * Created: Aug 4, 2014 12:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_settingsdialog.h"

#include <QStringListModel>
#include "peng.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_sqlrelationaldelegate.h"


PENG_SettingsDialog::PENG_SettingsDialog(QWidget *parent)
                : RB_Dialog(parent) {
    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

PENG_SettingsDialog::~PENG_SettingsDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    // delete mMapper; mappers are deleted when model is deleted
    delete mModel; // the unique model is used, therefore delete here
    RB_DEBUG->print("PENG_SettingsDialog::~PENG_SettingsDialog() OK");
}

/**
 * Initialize widget and models
 */
void PENG_SettingsDialog::init() {

    setWindowTitle(tr("Edit Main Settings[*]"));

    cbSettingsCategory->insertItem(PENG2::SettingCatNone, tr("(none)"));
    cbSettingsCategory->insertItem(PENG2::SettingCatCustom, tr("Custom"));
    cbSettingsCategory->setCurrentIndex(PENG2::SettingCatNone);

    connect(cbSettingsCategory, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotIndexChanged(int)));

    //
    // 1. Set model with ID (sets root object) and/or query
    // Shared is false because ChartMaster is unfiltered compared to other
    //
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelSetting, false);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());
    mModel->setWhere("category_id=0");

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tableView, mModel);

    //
    // 3. Set toolbuttonbar
    //
    tbbSettings->initDetailEdit();
    tableView->setToolButtonBar(tbbSettings);

    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS + 1) {
            tableView->hideColumn(i);
        } else {
            tableView->showColumn(i);

            if (tableView->columnWidth(i) < 5) {
                tableView->setColumnWidth(i, 100);
            }
        }
    }

}

bool PENG_SettingsDialog::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (!mModel->submitAllAndSelect()) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    } else if (mModel) {
        if (!mModel->submitAll()) {
            QApplication::restoreOverrideCursor();
            return false;
        }
    }

    setWindowModified(false);

    QApplication::restoreOverrideCursor();
    return true;
}

void PENG_SettingsDialog::fileRevert() {
    if (mModel) {
        mModel->revertAll();
    }
}

void PENG_SettingsDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = PENG_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

void PENG_SettingsDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
    PENG_DIALOGFACTORY->slotSettingChanged(PENG_DialogFactory::DialogSettings);
}

void PENG_SettingsDialog::on_pbCancel_clicked() {
    reject();
}

void PENG_SettingsDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void PENG_SettingsDialog::slotIndexChanged(int index) {
    maybeSave();
    mModel->setWhere("category_id=" + RB_String::number(index));
    QModelIndex idx;

    if (index == (int) PENG2::SettingCatCustom) {
        RB_StringList items;
        items << "(none)" << "Custom Label 1" << "Custom Label 2" << "Custom Label 3"
                  << "Custom Label 4" << "Custom Label 5" << "Custom Label 6"
                      << "Custom Label 7" << "Custom Label 8" << "Custom Label 9"
                          << "Custom Label 10";
        int colSet = mModel->fieldIndex("setting_id");
        mModel->setTextList(colSet, items);

        if (mModel->rowCount() <= 0) {
            // always insert at the begin
            int row = 0;
            int colCat = mModel->fieldIndex("category_id");
            int colVal = mModel->fieldIndex("value");
            mModel->insertRows(row, 10, QModelIndex());
            row = 0;

            // NOTE: do not forget to set the default column values, specially for the
            //       relational table otherwise new row will not show!
            idx = mModel->index(row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel1, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel2, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel3, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel4, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel5, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel6, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel7, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel8, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel9, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);

            idx = mModel->index(++row, colCat);
            mModel->setData(idx, (int) PENG2::SettingCatCustom, Qt::EditRole);
            idx = mModel->index(row, colSet);
            mModel->setData(idx, (int) PENG2::SettingCustomLabel10, Qt::EditRole);
            idx = mModel->index(row, colVal);
            mModel->setData(idx, tr("(none)"), Qt::EditRole);
            // end NOTE
        }

    }
}

