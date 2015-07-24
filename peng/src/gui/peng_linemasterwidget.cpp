/*****************************************************************
 * $Id: peng_linemasterwidget.cpp 1925 2013-04-22 12:31:48Z rutger $
 * Created: Apr 09, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_linemasterwidget.h"

#include <QStringListModel>
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"

/**
 * Constructor
 */
PENG_LineMasterWidget::PENG_LineMasterWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mModel = NULL;
    mSizeHint = QSize(190, 600);
}

/**
 * Destructor
 */
PENG_LineMasterWidget::~PENG_LineMasterWidget() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mModel; // shared model
    RB_DEBUG->print("PENG_LineMasterWidget::~PENG_LineMasterWidget() OK");
}

/**
 * Initialize widget and models
 */
void PENG_LineMasterWidget::init() {
    // TODO: implement filter
    cbLineType->hide();

    // Set model and connect to table view
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLine);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());

    // Relations, because of separate dockwidget and mdiwidget
    //    mModel->setRelation(mModel->fieldIndex(field),
    //           QSqlRelation(relationTable, relationId, relationField));
    // mModel->setRelation(mModel->fieldIndex("type_id"),
    //       QSqlRelation("PENG_Type", "id", "typename"));

    // Set standard format and connection for model and view
    setFormatTableView(tvLine, mModel);

    // Hide columns
    for (int i = 0; i < mModel->columnCount(QModelIndex()); ++i) {
        if (i != RB2::HIDDENCOLUMNS) {
            tvLine->hideColumn(i);
        }
    }

    mModel->select();
    readSettings();
}

/**
 * Save data to disk
 * @param withSelect true if select() is to be done after submitAll()
 */
bool PENG_LineMasterWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            QApplication::restoreOverrideCursor();
            return true;
        }
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Button add (customer) clicked
 */
void PENG_LineMasterWidget::on_pbAdd_clicked() {
    if (!mModel) return;

    // Make sure corresponding detail MDI window is open for maybeSave()
// TODO:
//    RB_MdiWindow* mdiWin = PENG_DIALOGFACTORY->getMdiWindow(
//            PENG_DialogFactory::WidgetLineDetail);
//    mdiWin->show();
//    PENG_DIALOGFACTORY->getMainWindow()->setActiveMdiWindow(mdiWin);

    if (mModel->isModelModified()) {
        fileSave(true);
    }

    // Clear sorting and filtering of proxyModel
    mModel->undoFilterSort();
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    // QModelIndex index = mModel->index(row, mModel->fieldIndex("number"));
    // mModel->setData(index, 989100 + i, Qt::EditRole);

    // mModel->submitAll(); // not send directly to database because will reload table
    tvLine->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvLine->scrollTo(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
//    mdiWin->getWidget()->slotResetWidgets();
}

/**
 * Button delete (customer) clicked
 */
void PENG_LineMasterWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = PENG_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvLine->currentIndex();

    if (index.isValid()) {
        int row = index.row();
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);

        if (row > 0) {
            tvLine->setCurrentIndex(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
            tvLine->scrollTo(mModel->index(row-1, RB2::HIDDENCOLUMNS, QModelIndex()));
        }
    }
}

/**
 * Button save clicked
 */
void PENG_LineMasterWidget::on_pbSave_clicked() {
    fileSave(true);
}

bool PENG_LineMasterWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return RB_Widget::closeWidget();
//    }
//    return false;
}

/**
 * Change event, for example translation
 */
void PENG_LineMasterWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
