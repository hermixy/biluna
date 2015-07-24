/*****************************************************************
 * $Id: peng_paintdialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_paintdialog.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "rb_sqlrelationaldelegate.h"


/**
 * Constructor
 */
PENG_PaintDialog::PENG_PaintDialog(QWidget *parent)
                        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
}

/**
 * Destructor
 */
PENG_PaintDialog::~PENG_PaintDialog() {
    // unique models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("PENG_PaintDialog::~PENG_PaintDialog() OK");
}

/**
 * Initialize widget and models
 */
void PENG_PaintDialog::init() {

    setWindowTitle(tr("PENG Edit Paint Systems[*]"));

    //
    // 1. Set model with ID
    //
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPaint, false);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leNumber, mModel->fieldIndex("number"));
    mMapper->addMapping(leDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(leRevision, mModel->fieldIndex("revision"));
    RB_StringList items;
    items << tr("(none)") << tr("All related lines") << tr("Lines not insulated")
          << tr("Lines insulated") << tr("Carbon steel (all)") << tr("Carbon steel (not insul.)")
          << tr("Stainless steel (all)") << tr("Stainless steel (not insul.)");
    mModel->setTextList(mModel->fieldIndex("applicable_id"), items);
    cbApplicableLines->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbApplicableLines, mModel->fieldIndex("applicable_id"), "currentIndex");
    mMapper->addMapping(leTempLimitHigh, mModel->fieldIndex("temperaturelimithigh"));
    /*
     * VALIDATORS examples
     * integer:
     * 	lined2->setValidator( new QIntValidator( lined2 ) );
     * double: allowable top, bottom, decimals
     * 	lined2->setValidator( new QDoubleValidator( -999.0, 999.0, 2, lined2 ) );
     * date:
     *	lined4->setInputMask( "0000-00-00" );
     *	lined4->setText( "00000000" );
     *	lined4->setCursorPosition( 0 );
     *
     * example:
     * lineEditLineNo->setValidator(new QIntValidator(0, 9999999, lineEditLineNo));
     */
    leTempLimitHigh->setValidator(new QDoubleValidator(-999.0, 999.0, 2, leTempLimitHigh));
    mMapper->addMapping(leTempLimitLow, mModel->fieldIndex("temperaturelimitlow"));
    leTempLimitLow->setValidator(new QDoubleValidator(-999.0, 999.0, 2, leTempLimitLow));
    mMapper->addMapping(leComment, mModel->fieldIndex("comment"));
    mMapper->addMapping(pteSurfacePreparation, mModel->fieldIndex("surfacepreparation"));
    mMapper->addMapping(ptePrimerCoat, mModel->fieldIndex("primercoat"));
    mMapper->addMapping(pteFinishCoat1, mModel->fieldIndex("finishcoat1"));
    mMapper->addMapping(pteFinishCoat2, mModel->fieldIndex("finishcoat2"));
    mMapper->addMapping(pteFinishCoat3, mModel->fieldIndex("finishcoat3"));

    //
    // 3. Execute SELECT on model
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    formatTableView(tvPaint, mModel);

    //
    // 5. Set toolbuttonbar
    //
    tbbPaint->initEdit(false, false, false);
    tvPaint->setToolButtonBar(tbbPaint);
    connect(tbbPaint, SIGNAL(addClicked()),
            this, SLOT(slotPbAdd_clicked()));

    //
    // 6. Set visible columns in tableView
    //
    int colCount = mModel->columnCount(QModelIndex());

    for (int i = 0; i < colCount; ++i) {
        if (i != mModel->fieldIndex("number") && i != mModel->fieldIndex("description")
                && i != mModel->fieldIndex("revision") && i != mModel->fieldIndex("applicable_id")
                && i != mModel->fieldIndex("temperaturelimithigh")
                && i != mModel->fieldIndex("temperaturelimitlow")) {
            tvPaint->hideColumn(i);
        } else {
            tvPaint->showColumn(i);

            if (tvPaint->columnWidth(i) < 5) {
                tvPaint->setColumnWidth(i, 100);
            }
        }
    }

    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool PENG_PaintDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    if (withSelect && mModel) {
        if (mModel->submitAllAndSelect()) {

            setWindowModified(false);
            return true;
        }
    } else if (mModel) {
        if (mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }
    // QApplication::restoreOverrideCursor();
    return false;
}

/**
 * Reverse all changes
 */
void PENG_PaintDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}

void PENG_PaintDialog::slotPbAdd_clicked() {
    if(!mModel)  return;

    // insert already done
//    int row = 0;

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
//    QModelIndex idx;
//    idx = mModel->index(row, mModel->fieldIndex("number"), QModelIndex());
//    mModel->setData(idx, "<NEW>", Qt::EditRole);
//    idx = mModel->index(row, mModel->fieldIndex("applicable_id"), QModelIndex());
//    mModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    // Set current index and scroll to already done

    tabWidget->setCurrentIndex(0);
    leNumber->setFocus();
    leNumber->selectAll();
}

/**
 * Button help clicked
 */
void PENG_PaintDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = PENG_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void PENG_PaintDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button cancel clicked
 */
void PENG_PaintDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void PENG_PaintDialog::changeEvent(QEvent* e) {
    RB_Dialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
