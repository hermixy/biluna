/*****************************************************************
 * $Id: sail_mapnavigationwidget.cpp 1729 2012-08-08 20:48:52Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_navigationtrackwidget.h"

#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Constructor
 */
SAIL_NavigationTrackWidget::SAIL_NavigationTrackWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

    mTrackModel = NULL;
    mTrackMapper = NULL;

}

/**
 * Destructor
 */
SAIL_NavigationTrackWidget::~SAIL_NavigationTrackWidget() {
    delete mTrackModel;
    RB_DEBUG->print("SAIL_NavigationTrackWidget::~SAIL_NavigationTrackWidget() OK");
}

/**
 * Get proposed saveas filename
 * @returns filename
 */
RB_String SAIL_NavigationTrackWidget::getSaveAsFileName() {
//    if (!mModel->getCurrentIndex().isValid()) {
        return QString();
//    }

//    RB_String str = mModel->getCurrentValue("reference").toString();
//    return str;
}

/**
 * Initialize widget and models
 */
void SAIL_NavigationTrackWidget::init() {
    //
    // 1. Set model and mapper
    //
    mTrackModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelTrack);
    mTrackModel->setRoot(SAIL_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mTrackMapper = mTrackModel->getMapper();
    mTrackMapper->addMapping(leTrackName, mTrackModel->fieldIndex("mname"));
    mTrackMapper->addMapping(leTrackComment, mTrackModel->fieldIndex("comment"));
    mTrackMapper->addMapping(ileTrackCoordinateFrom, mTrackModel->fieldIndex("from_idx"));
    mTrackMapper->addMapping(ileTrackCoordinateTo, mTrackModel->fieldIndex("to_idx"));
    mTrackMapper->addMapping(leWeightFactor, mTrackModel->fieldIndex("weightfactor"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mTrackModel->select();

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvTrack, mTrackModel);

    int count = mTrackModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvTrack->hideColumn(i);
        } else {
            tvTrack->showColumn(i);

            if (tvTrack->columnWidth(i) < 5) {
                tvTrack->setColumnWidth(i, 100);
            }
        }
    }

}

/**
 * Save data to database
 */
bool SAIL_NavigationTrackWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mTrackModel) {
        mTrackModel->submitAllAndSelect();
        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    } else if (mTrackModel) {
        mTrackModel->submitAll();
        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    }
    QApplication::restoreOverrideCursor();
    return false;
}

/**
 * File menu PDF clicked
 * @param printer
 */
void SAIL_NavigationTrackWidget::filePdf(QPrinter* printer) {
    // later
}

/**
 * File menu PDF clicked
 * @param printer
 */
void SAIL_NavigationTrackWidget::filePrint(QPrinter* printer) {
    // later
}

/**
 * File menu PDF clicked
 * @param printer
 */
void SAIL_NavigationTrackWidget::filePrintPreview(QPrinter* printer) {
    // later
}

/**
 * Button add track clicked
 */
void SAIL_NavigationTrackWidget::on_pbAddTrack_clicked() {
    if (!mTrackModel) return;
    mTrackModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mTrackModel->rowCount();
    mTrackModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mTrackModel->index(row, mTrackModel->fieldIndex("mname"));
    mTrackModel->setData(idx, "<NEW>", Qt::EditRole);
    // end NOTE

    tvTrack->setCurrentIndex(mTrackModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTrack->scrollTo(tvTrack->currentIndex());
    setWindowModified(true);
    leTrackName->setFocus();
    leTrackName->selectAll();
}

/**
 * Button delete track clicked
 */
void SAIL_NavigationTrackWidget::on_pbDeleteTrack_clicked() {
    int ret = SAIL_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        SAIL_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvTrack->currentIndex();
    if (!index.isValid()) return;

    mTrackModel->removeRows(index.row(), 1, QModelIndex());
    setWindowModified(true);
}

/**
 * Push button save clicked
 */
void SAIL_NavigationTrackWidget::on_pbSaveTrack_clicked() {
    fileSave(true);
}

/**
 * Button select 'From' coordinate clicked
 */
void SAIL_NavigationTrackWidget::on_ileTrackCoordinateFrom_clicked() {
    QModelIndex index = tvTrack->currentIndex();

    if (!index.isValid()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No track item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = SAIL_DIALOGFACTORY->getDialog(SAIL_DialogFactory::DialogSelectCoordinate);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mTrackModel->index(row, mTrackModel->fieldIndex("from_idx"));
            mTrackModel->setData(index, obj->getId()
                            + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No coordinate selected.\n"
                                                   "Coordinate is unchanged."));
    }

    delete dlg;
}

/**
 * Button select 'To' coordinate clicked
 */
void SAIL_NavigationTrackWidget::on_ileTrackCoordinateTo_clicked() {
    QModelIndex index = tvTrack->currentIndex();

    if (!index.isValid()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No track item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = SAIL_DIALOGFACTORY->getDialog(SAIL_DialogFactory::DialogSelectCoordinate);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mTrackModel->index(row, mTrackModel->fieldIndex("to_idx"));
            mTrackModel->setData(index, obj->getId()
                            + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No coordinate selected.\n"
                                                   "Coordinate is unchanged."));
    }

    delete dlg;
}

void SAIL_NavigationTrackWidget::on_pbTest_clicked() {
    mTrackModel->select();
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String SAIL_NavigationTrackWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Close widget
 */
bool SAIL_NavigationTrackWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void SAIL_NavigationTrackWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

