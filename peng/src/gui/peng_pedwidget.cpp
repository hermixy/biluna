/*****************************************************************
 * $Id: peng_pedwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pedwidget.h"

#include <QtSvg>
#include <QtWebKit>
#include "db_dialogfactory.h"
#include "gv_drawing.h"
#include "peng_determinepedclass.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "peng_navigationwidget.h"
#include "peng_objectfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_debug.h"
#include "rb_urlbasedrenderer.h"
#include "rb_webfactory.h"


/**
 * Constructor
 */
PENG_PedWidget::PENG_PedWidget(QWidget* parent) : RB_Widget(parent) {

	setupUi(this);

    mModelEquipment = NULL;
    mMapper = NULL;
    mPedModel = NULL;
    mPedMapper = NULL;
    mPedTable = NULL;

    mModelLine = NULL;
    mMapperLine = NULL;
    mPedModelLine = NULL;
    mPedMapperLine = NULL;
    mPedTableLine = NULL;

    mNavWidget = NULL;
}

/**
 * Destructor
 */
PENG_PedWidget::~PENG_PedWidget() {
    delete mPedModel; // also deletes mPedMapper
    delete mPedModelLine; // also deletes mPedMapperLine

    // otherwise crash (BTW do not use: delete mMapper;)
    if(mModelEquipment) { // equipment
        mModelEquipment->deleteMapper("PENG_PedWidget"); // not the default
    }
    if (mModelLine) {
        mModelLine->deleteMapper("PENG_PedWidget"); // not the default
    }
}

/**
 * Get proposed saveas filename, default returns window title (getName())
 * with space replaced by underscore
 * @returns filename
 */
RB_String PENG_PedWidget::getSaveAsFileName() {
    if (stackedWidget->currentIndex() == 1) {
        // line
        return "peng_pedclassif_" + leTagNumberLine->text();
    }

    return "peng_pedclassif_" + leTagNumber->text();
}

void PENG_PedWidget::init() {
    // Navigation widget already exists
    RB_Widget* wdgt = PENG_DIALOGFACTORY->getWidget(PENG_DialogFactory::WidgetNavigation, NULL);
    mNavWidget = dynamic_cast<PENG_NavigationWidget*>(wdgt);
    connect(mNavWidget, SIGNAL(currentNavigationTab(int)),
            this, SLOT(slotNavigationTabChanged(int)));

    readSettings();
    mNavWidget->emitState(); // will trigger initEquipment() or initPipeLine()
}

void PENG_PedWidget::initEquipment() {
    stackedWidget->setCurrentIndex(0);

    if (mModelEquipment) {
        // already initiated
        return;
    }

//    bool firstTimeWidget = false;

//    if (!mModelEquipment) { // first time model
//        if (!mModelLine) firstTimeWidget = true;

        // Set equipment model
        mModelEquipment = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipment);

        // Mapper for line edits etc.
        mMapper = mModelEquipment->getMapper("PENG_PedWidget");
        mMapper->addMapping(ileEquipmentType, mModelEquipment->fieldIndex("type_idx"));
        mMapper->addMapping(leTagNumber, mModelEquipment->fieldIndex("number"));
        mMapper->addMapping(leDescription, mModelEquipment->fieldIndex("eqname"));
        mMapper->addMapping(lePs, mModelEquipment->fieldIndex("designpress"));
        mMapper->addMapping(leTs, mModelEquipment->fieldIndex("designtemp"));
        mMapper->addMapping(leV, mModelEquipment->fieldIndex("volume"));

        // Required because not setFormatTableView()
        connect(mModelEquipment, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
        connect(mModelEquipment, SIGNAL(modelBeforeSubmitted()),
                this, SLOT(slotDataIsSaved()));
        // Connections for datawidgetmapper and MDI window changed state
        connect(mModelEquipment, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotClearFields(QModelIndex,QModelIndex)));

        // ped_table is a QFrame and only a place holder in peddialog.ui field
        QGridLayout* layout = new QGridLayout(ped_table);
        layout->setObjectName("grid-layout");
        layout->setSpacing(0);
        layout->setMargin(0);
        mPedTable = new PENG_PedTable(ped_table);
        layout->addWidget(mPedTable);

        lePs->setValidator(new QDoubleValidator(0.0, 999.0, 3, lePs));
        leTs->setValidator(new QDoubleValidator(-273.0, 999.0, 3, leTs));
        leV->setValidator(new QDoubleValidator(0.0, 99999.0, 2, leV));
        leDn->setValidator(new QDoubleValidator(0.0, 9999.0, 2, leDn));

        // PED mapper for both equipment and lines
        mPedModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPedEqClassif, false);
        // Next line only for consistency with pipe line below
        // mPedModel->setParentManager(mModelEquipment);

        mPedMapper = mPedModel->getMapper();
        connect(mPedModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));

        // Root changed signal always sends QModelIndex(),
        // setFilter() and select() already done,
        // this signal/slot because pbAdd not part of this widget
        connect(mPedModel, SIGNAL(rootChanged()),
                this, SLOT(slotEnsureRelevantRow()));

        RB_StringList items;
        items << tr("Equipment") << tr("Generator") << tr("Piping")
              << tr("Pressure Accessory") << tr("Safety Accessory");
        cbPedType->setModel(new QStringListModel(items, this));
        mPedMapper->addMapping(cbPedType, mPedModel->fieldIndex("pedtype_id"),
                               "currentIndex");
        items.clear();
        items << tr("Volume (V)") << tr("Nominal size (DN)");
        cbAssessmentBy->setModel(new QStringListModel(items, this));
        mPedMapper->addMapping(cbAssessmentBy, mPedModel->fieldIndex("assessmentby_id"),
                               "currentIndex");
        items.clear();
        items << tr("Gaseous") << tr("Liquid");
        cbFluidState->setModel(new QStringListModel(items, this));
        mPedMapper->addMapping(cbFluidState, mPedModel->fieldIndex("fluidstate_id"),
                               "currentIndex");
        items.clear();
        items << tr("Group 1 (Dangerous)") << tr("Group 2");
        cbFluidGroup->setModel(new QStringListModel(items, this));
        mPedMapper->addMapping(cbFluidGroup, mPedModel->fieldIndex("fluidgroup_id"),
                               "currentIndex");
        items.clear();
        items << tr("(Not a bottle, extinguisher, cooker)")
                << tr("Bottle for breathing equipment")
                << tr("Portable extinguisher") << tr("Pressure cooker");
        cbAdditionalInformation->setModel(new QStringListModel(items, this));
        mPedMapper->addMapping(cbAdditionalInformation, mPedModel->fieldIndex("additionalinfo_id"),
                               "currentIndex");
        mPedMapper->addMapping(chbUnstableGas, mPedModel->fieldIndex("unstablegas"));
        mPedMapper->addMapping(leFluid1, mPedModel->fieldIndex("fluid1"));
        mPedMapper->addMapping(leFluid2, mPedModel->fieldIndex("fluid2"));
        mPedMapper->addMapping(leFluid3, mPedModel->fieldIndex("fluid3"));
        mPedMapper->addMapping(leFluid4, mPedModel->fieldIndex("fluid4"));
//    }

//    if (firstTimeWidget) {
        setDialogFields();
        readSettings();
//    }
}

void PENG_PedWidget::initPipeLine() {
    stackedWidget->setCurrentIndex(1);

    if (mModelLine) {
        // already initiated
        return;
    }

//    bool firstTimeWidget = false;

//    if (!mModelLine) { // first time model
//        if (!mModelEquipment) firstTimeWidget = true;

        // Set line model
        mModelLine = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLine);

        // Mapper for line edits etc.
        mMapperLine = mModelLine->getMapper("PENG_PedWidget"); // not default
        leEquipmentTypeLine->setText(tr("Pipe Line"));
        leEquipmentTypeLine->setEnabled(false);

        mMapperLine->addMapping(leTagNumberLine, mModelLine->fieldIndex("number"));
        leDescriptionLine->setText(tr("Pipe Line"));
        leDescriptionLine->setEnabled(false);
        mMapperLine->addMapping(lePsLine, mModelLine->fieldIndex("designpress"));
        mMapperLine->addMapping(leTsLine, mModelLine->fieldIndex("designtemp"));
        mMapperLine->addMapping(leDnLine, mModelLine->fieldIndex("sizedn"));

        // Required because not setFormatTableView()
        connect(mModelLine, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
        connect(mModelLine, SIGNAL(modelBeforeSubmitted()),
                this, SLOT(slotDataIsSaved()));
        // Connections for datawidgetmapper and MDI window changed state
        connect(mModelLine, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotClearFields(QModelIndex,QModelIndex)));

        // ped_table is a QFrame and only a place holder in peddialog.ui field
        QGridLayout* layout = new QGridLayout(ped_tableLine);
        layout->setObjectName("grid-layoutline");
        layout->setSpacing(0);
        layout->setMargin(0);
        mPedTableLine = new PENG_PedTable(ped_tableLine);
        layout->addWidget(mPedTableLine);

        lePsLine->setValidator(new QDoubleValidator(0.0, 999.0, 3, lePs));
        leTsLine->setValidator(new QDoubleValidator(-273.0, 999.0, 3, leTs));
        leVLine->setValidator(new QDoubleValidator(0.0, 99999.0, 2, leV));
        leVLine->setEnabled(false);
        leDnLine->setValidator(new QDoubleValidator(0.0, 9999.0, 2, leDn));

        // PED mapper for both equipment and lines
        mPedModelLine = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelPedLineClassif, false);
        // Next line because parent model is only PENG_Equipment in modelfactory
        // mPedModelLine->setParentManager(mModelLine);

        mPedMapperLine = mPedModelLine->getMapper();
        connect(mPedModelLine, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));

        // Root changed signal always sends QModelIndex(),
        // setFilter() and select() already done,
        // this signal/slot because pbAdd not part of this widget
        connect(mPedModelLine, SIGNAL(rootChanged()),
                this, SLOT(slotEnsureRelevantRowLine()));

        RB_StringList items;
        items << tr("Equipment") << tr("Generator") << tr("Piping")
              << tr("Pressure Accessory") << tr("Safety Accessory");
        cbPedTypeLine->setModel(new QStringListModel(items, this));
        mPedMapperLine->addMapping(cbPedTypeLine, mPedModelLine->fieldIndex("pedtype_id"),
                                   "currentIndex");
        cbPedTypeLine->setCurrentIndex(2); // Piping
        cbPedTypeLine->setEnabled(false);

        items.clear();
        items << tr("Volume (V)") << tr("Nominal size (DN)");
        cbAssessmentByLine->setModel(new QStringListModel(items, this));
        mPedMapperLine->addMapping(cbAssessmentByLine, mPedModelLine->fieldIndex("assessmentby_id"),
                                   "currentIndex");
        cbAssessmentByLine->setCurrentIndex(1); // DN
        cbAssessmentByLine->setEnabled(false);

        items.clear();
        items << tr("Gaseous") << tr("Liquid");
        cbFluidStateLine->setModel(new QStringListModel(items, this));
        mPedMapperLine->addMapping(cbFluidStateLine, mPedModelLine->fieldIndex("fluidstate_id"),
                                   "currentIndex");
        items.clear();
        items << tr("Group 1 (Dangerous)") << tr("Group 2");
        cbFluidGroupLine->setModel(new QStringListModel(items, this));
        mPedMapperLine->addMapping(cbFluidGroupLine, mPedModelLine->fieldIndex("fluidgroup_id"),
                                   "currentIndex");
        items.clear();
        items << tr("(Not a bottle, extinguisher, cooker)")
                << tr("Bottle for breathing equipment")
                << tr("Portable extinguisher") << tr("Pressure cooker");
        cbAdditionalInformationLine->setModel(new QStringListModel(items, this));
        mPedMapperLine->addMapping(cbAdditionalInformationLine, mPedModelLine->fieldIndex("additionalinfo_id"),
                                   "currentIndex");
        cbAdditionalInformationLine->setCurrentIndex(0);
        cbAdditionalInformationLine->setEnabled(false);

        mPedMapperLine->addMapping(chbUnstableGasLine, mPedModelLine->fieldIndex("unstablegas"));
        mPedMapperLine->addMapping(leFluid1Line, mPedModelLine->fieldIndex("fluid1"));
        mPedMapperLine->addMapping(leFluid2Line, mPedModelLine->fieldIndex("fluid2"));
        mPedMapperLine->addMapping(leFluid3Line, mPedModelLine->fieldIndex("fluid3"));
        mPedMapperLine->addMapping(leFluid4Line, mPedModelLine->fieldIndex("fluid4"));
//    }

//    if (firstTimeWidget) {
        setDialogFieldsLine();
        readSettings();
//    }

}


void PENG_PedWidget::setDialogFields() {
    if (mPedTable) {
        mPedTable->setTable(0);
    }
    leCategory->setText("");
    leModules->setText("");
    teException->setPlainText("Press 'calculate' for details");
}

void PENG_PedWidget::setDialogFieldsLine() {
    if (mPedTableLine) {
        mPedTableLine->setTable(0);
    }
    leCategoryLine->setText("");
    leModulesLine->setText("");
    teExceptionLine->setPlainText("Press 'calculate' for details");
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 */
bool PENG_PedWidget::fileSave(bool /*withSelect*/) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();

    if (mModelEquipment) {
        mModelEquipment->submitAllAndSelect();
        mPedModel->submitAllAndSelect();
    }
    if (mModelLine) {
        mModelLine->submitAllAndSelect();
        mPedModelLine->submitAllAndSelect();
    }

    setWindowModified(false);
    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Reverse all changes
 */
void PENG_PedWidget::fileRevert() {
    if (mModelEquipment) {
        mModelEquipment->revertAll();
        mPedModel->revertAll();
    }
    if (mModelLine) {
        mModelLine->revertAll();
        mPedModelLine->revertAll();
    }
    setWindowModified(false);
}

/**
 * File export PDF.
 */
void PENG_PedWidget::filePdf(QPrinter* pr) {
    printWidget(pr);
}

/**
 * File Print.
 */
void PENG_PedWidget::filePrint(QPrinter* pr) {
    printWidget(pr);
}

/**
 * File Print Preview.
 * To be overriden by actual widget, shows 'not implemented' message
 */
void PENG_PedWidget::filePrintPreview(QPrinter* pr) {
    printWidget(pr);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String PENG_PedWidget::getHelpSubject() const {
    return objectName();
}


/**
 * Close widget, also called by right top corner close icon.
 * Unregisters widget from dialog and via maybeSave calls fileSave.
 */
bool PENG_PedWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Button previous equipment clicked
 */
void PENG_PedWidget::on_pbPreviousEq_clicked() {
    mMapper->toPrevious();
}

/**
 * Button next equipment clicked
 */
void PENG_PedWidget::on_pbNextEq_clicked() {
    mMapper->toNext();
}

/**
 * Button save equipment clicked
 */
void PENG_PedWidget::on_pbSaveEq_clicked() {
    fileSave(true);
}

/**
 * Button revert equipment clicked
 */
void PENG_PedWidget::on_pbRevertEq_clicked() {
    fileRevert();
}

/**
 * Button previous line clicked
 */
void PENG_PedWidget::on_pbPreviousLine_clicked() {
    mMapperLine->toPrevious();
}

/**
 * Button next line clicked
 */
void PENG_PedWidget::on_pbNextLine_clicked() {
    mMapperLine->toNext();
}

/**
 * Button save line clicked
 */
void PENG_PedWidget::on_pbSaveLine_clicked() {
    fileSave(true);
}

/**
 * Button revert line clicked
 */
void PENG_PedWidget::on_pbRevertLine_clicked() {
    fileRevert();
}

/**
 * Slot to clear fields and graph when a new item is selected
 */
void PENG_PedWidget::slotClearFields(const QModelIndex&, const QModelIndex&) {
    setDialogFields();
    setDialogFieldsLine();
}

/**
 * Slot to clear fields and graph when a new item is selected
 */
//void PENG_PedWidget::slotClearFields(const QModelIndex&) {
//    setDialogFields();
//}

/**
 * Slot to ensure a relevant row is present in the PED classification model,
 * adds a row to the model if not yet exists.
 * Required here because the pbAdd function of the equipment model
 * is not part of this widget
 */
void PENG_PedWidget::slotEnsureRelevantRow() {
    if (!mPedModel || !mModelEquipment || !mModelEquipment->getCurrentIndex().isValid()) return;

    if (mPedModel->rowCount() < 1) {
        int row = 0;
        mPedModel->insertRows(row, 1, QModelIndex());

        QModelIndex index;
        index = mPedModel->index(row, mPedModel->fieldIndex("pedtype_id"));
        mPedModel->setData(index, 0, Qt::EditRole);
        index = mPedModel->index(row, mPedModel->fieldIndex("assessmentby_id"));
        mPedModel->setData(index, 0, Qt::EditRole);
        index = mPedModel->index(row, mPedModel->fieldIndex("fluidstate_id"));
        mPedModel->setData(index, 0, Qt::EditRole);
        index = mPedModel->index(row, mPedModel->fieldIndex("fluidgroup_id"));
        mPedModel->setData(index, 0, Qt::EditRole);
        index = mPedModel->index(row, mPedModel->fieldIndex("additionalinfo_id"));
        mPedModel->setData(index, 0, Qt::EditRole);
        index = mPedModel->index(row, mPedModel->fieldIndex("unstablegas"));
        mPedModel->setData(index, "false", Qt::EditRole);
    }

    if (mPedMapper) {
        mPedMapper->toFirst();
    }
}

/**
 * Slot to ensure a relevant row is present in the PED classification model,
 * adds a row to the model if not yet exists.
 * Required here because the pbAdd function of the equipment model
 * is not part of this widget
 */
void PENG_PedWidget::slotEnsureRelevantRowLine() {
    if (!mPedModelLine || !mModelLine || !mModelLine->getCurrentIndex().isValid()) return;

    if (mPedModelLine->rowCount() < 1) {
        int row = 0;
        mPedModelLine->insertRows(row, 1, QModelIndex());

        QModelIndex index;
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("pedtype_id"));
        mPedModelLine->setData(index, 2, Qt::EditRole);
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("assessmentby_id"));
        mPedModelLine->setData(index, 1, Qt::EditRole);
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("fluidstate_id"));
        mPedModelLine->setData(index, 0, Qt::EditRole);
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("fluidgroup_id"));
        mPedModelLine->setData(index, 0, Qt::EditRole);
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("additionalinfo_id"));
        mPedModelLine->setData(index, 0, Qt::EditRole);
        index = mPedModelLine->index(row, mPedModelLine->fieldIndex("unstablegas"));
        mPedModelLine->setData(index, "false", Qt::EditRole);
    }

    if (mPedMapperLine) {
        mPedMapperLine->toFirst();
    }
}

/**
 * Reset widgets to default state, after insertion of row in model
 */
void PENG_PedWidget::slotResetWidgets() {
    // tabWidget->setCurrentIndex(0);
    if (stackedWidget->currentIndex() == 0) {
        ileEquipmentType->setFocus();
    } else {
        leTagNumberLine->setFocus();
        leTagNumberLine->selectAll();
    }
}

/**
 * Button select item account clicked
 */
void PENG_PedWidget::on_ileEquipmentType_clicked() {
    if (!mModelEquipment->getCurrentIndex().isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = PENG_DIALOGFACTORY->getDialog(PENG_DialogFactory::DialogEqTypeSelect);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = mModelEquipment->getCurrentIndex().row();
            QModelIndex index = mModelEquipment->index(row, mModelEquipment->fieldIndex("type_idx"));
            mModelEquipment->setData(index, obj->getId()
                            + obj->getValue("typename").toString(), Qt::EditRole);
        } else {
            PENG_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                        "Data unchanged."));
        }
    }

    dlg->deleteLater();
}

void PENG_PedWidget::on_pbCalculate_clicked() {
    if (cbPedType->currentIndex() == 2) {
        if (mPedTable) {
            mPedTable->setTable(0);
        }
        leCategory->setText("");
        leModules->setText("");
        teException->setPlainText("Use the 'pipe line tab'' in PENG Navigation for pipe lines");
        return;
    }

    RB_String type = "Equipment";

    if (cbPedType->currentIndex() == 1) {
        type = "Generator";
    } else if (cbPedType->currentIndex() == 2) {
        type = "Piping";
    } else if (cbPedType->currentIndex() == 3) {
        type = "Pressure_Accessory";
    } else if (cbPedType->currentIndex() == 4) {
        type = "Safety_Accessory";
    }

    RB_String assess = "AssessVolume";

    if (cbAssessmentBy->currentIndex() == 1) {
        assess = "AssessDn";
    }

    RB_String fluidState = "Gaseous";

    if (cbFluidState->currentIndex() == 1) {
        fluidState = "Liquid";
    }

    RB_String fluidGroup = "Group1";

    if (cbFluidGroup->currentIndex() == 1) {
        fluidGroup = "Group2";
    }

    double maxPress = lePs->text().toDouble() * 10; // bar
    double maxTemp = leTs->text().toDouble();
    double maxVol = leV->text().toDouble();
    double nomSize = leDn->text().toDouble();
    RB_String addInfo = "(none)";

    if (cbAdditionalInformation->currentIndex() == 1) {
        addInfo = "Bottle_Breathing_Eqmt";
    } else if (cbAdditionalInformation->currentIndex() == 2) {
        addInfo = "Portable_Extinguisher";
    } else if (cbAdditionalInformation->currentIndex() == 3) {
        addInfo = "Pressure_Cooker";
    }

    RB_String unstableGas = "Unstable";

    if (!chbUnstableGas->isChecked()) {
        unstableGas = "Stable";
    }

	// create input model, execute action and get output model
    PENG_DeterminePedClass operPed;
    operPed.execute(type, assess, fluidState, fluidGroup, maxPress,
                    maxTemp, maxVol, nomSize, addInfo, unstableGas);
	
    mPedTable->setTable(operPed.getTable());
    mPedTable->setHorizontalLine(maxPress); 	// bar
    if (cbPedType->currentIndex() == 2) {
        mPedTable->setVerticalLine(nomSize);	// mm
    } else {
        mPedTable->setVerticalLine(maxVol);	// mm
    }

    leCategory->setText(operPed.getCategory());
    leModules->setText(operPed.getModules());
    teException->setHtml(operPed.getNote());
}

void PENG_PedWidget::on_pbCalculateLine_clicked() {
    if (cbPedTypeLine->currentIndex() != 2) {
        if (mPedTableLine) {
            mPedTableLine->setTable(0);
        }
        leCategoryLine->setText("");
        leModulesLine->setText("");
        teExceptionLine->setPlainText("Use the 'equipment tab'' in PENG Navigation for equipment");
        return;
    }

    RB_String type = "Piping";

    if (cbPedTypeLine->currentIndex() == 1) {
        type = "Generator";
    } else if (cbPedTypeLine->currentIndex() == 2) {
        type = "Piping";
    } else if (cbPedTypeLine->currentIndex() == 3) {
        type = "Pressure_Accessory";
    } else if (cbPedTypeLine->currentIndex() == 4) {
        type = "Safety_Accessory";
    }

    RB_String assess = "AssessDn";

    if (cbAssessmentByLine->currentIndex() == 1) {
        assess = "AssessDn";
    }

    RB_String fluidState = "Gaseous";

    if (cbFluidStateLine->currentIndex() == 1) {
        fluidState = "Liquid";
    }

    RB_String fluidGroup = "Group1";

    if (cbFluidGroupLine->currentIndex() == 1) {
        fluidGroup = "Group2";
    }

    double maxPress = lePsLine->text().toDouble() * 10; // bar
    double maxTemp = leTsLine->text().toDouble();
    double maxVol = leVLine->text().toDouble();
    double nomSize = leDnLine->text().toDouble();
    RB_String addInfo = "(none)";

    if (cbAdditionalInformationLine->currentIndex() == 1) {
        addInfo = "Bottle_Breathing_Eqmt";
    } else if (cbAdditionalInformationLine->currentIndex() == 2) {
        addInfo = "Portable_Extinguisher";
    } else if (cbAdditionalInformationLine->currentIndex() == 3) {
        addInfo = "Pressure_Cooker";
    }

    RB_String unstableGas = "Unstable";

    if (!chbUnstableGasLine->isChecked()) {
        unstableGas = "Stable";
    }

    // create input model, execute action and get output model
    PENG_DeterminePedClass operPed;
    operPed.execute(type, assess, fluidState, fluidGroup, maxPress,
                    maxTemp, maxVol, nomSize, addInfo, unstableGas);

    mPedTableLine->setTable(operPed.getTable());
    mPedTableLine->setHorizontalLine(maxPress); 	// bar
    if (cbPedTypeLine->currentIndex() == 2) {
        mPedTableLine->setVerticalLine(nomSize);	// mm
    } else {
        mPedTableLine->setVerticalLine(maxVol);	// mm
    }

    leCategoryLine->setText(operPed.getCategory());
    leModulesLine->setText(operPed.getModules());
    teExceptionLine->setHtml(operPed.getNote());
}

/**
 * to capture the close action by user at the right top corner
 */
void PENG_PedWidget::reject() {
    RB_DEBUG->print("PENG_PedWidget::reject()");
	
	if (maybeSave()) {
        // QDialog::reject();
	}
}

void PENG_PedWidget::slotNavigationTabChanged(int tab) {
    if (tab == PENG_NavigationWidget::TabPipeLine) {
        initPipeLine();
    } else {
        initEquipment();
    }
}

/**
 * Print widget
 * @param pr printer
 */
void PENG_PedWidget::printWidget(QPrinter* pr) {
    if(stackedWidget->currentIndex() == 0 && !mModelEquipment->getCurrentIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog("No item selected");
        return;
    }

    if (stackedWidget->currentIndex() == 1 && !mModelLine->getCurrentIndex().isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog("No item selected");
        return;
    }

    if (stackedWidget->currentIndex() == 0) {
        on_pbCalculate_clicked();
    } else {
        on_pbCalculateLine_clicked();
    }

/* Fourth attempt with QTextEdit */
//    pr->setPaperSize(QSizeF(210, 297), QPrinter::Millimeter);
//    pr->setPageMargins(20, 15, 15, 15, QPrinter::Millimeter);
    QTextEdit* te = new QTextEdit();
    te->document()->setDocumentMargin(0);
    te->document()->setUseDesignMetrics(true);

    if (stackedWidget->currentIndex() == 0) {
        widgetToHtml(te);
    } else {
        widgetToHtmlLine(te);
    }

    te->print(pr);
    delete te;
}

/**
 * Transfer this widget current view to HTML on a webview
 * Function NOT USED
 * @param webView webview
 */
void PENG_PedWidget::widgetToHtml(QWebView* webView) {
    if (!mPedTable || !webView) {
        RB_DEBUG->error("PENG_PedWidget::widgetToHtml() NULL ERROR");
        return;
    }

    RB_String strHtml;
    strHtml = "<html xmlns=\"http://www.w3.org/1999/xhtml\">"
              "<head>"
              "<title>PED Equipment Classification by Biluna</title>"
              "</head>"
              "<body leftmargin=\"100\" marginheight=\"50\" rightmargin=\"50\">"
              "<p><strong>PED Equipment Classification</strong></p><p />";
    strHtml += "<table style=\"border:0px;\" width=\"100%\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxGeneral->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblEquipmentType->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + ileEquipmentType->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblTagNumber->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leTagNumber->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblDescription->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leDescription->text() + "</td>";
    strHtml += "</tr></table><p />";

    strHtml += "<table style=\"border:0px;\" width=\"100%\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + gbProperties->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblPedType->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + cbPedType->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblAssessmentBy->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + cbAssessmentBy->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblFluidState->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + cbFluidState->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblFluidGroup->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + cbFluidGroup->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblPs->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + RB_String::number(lePsLine->text().toDouble() * 10.0, 'g', 1) + " bar</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblTs->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leTs->text() + " Celsius</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblV->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leV->text() + " litres</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblDn->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leDn->text() + " mm</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblAdditionalInformation->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + cbAdditionalInformation->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + chbUnstableGas->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + RB_String(chbUnstableGas->isChecked() ? "Yes" : "No") + "</td>";
    strHtml += "</tr></table><p />";

/* First attempt */
    strHtml += "<object border=\"1\" type=\"image/in-memory\" data=\"\" "
               "width=\"400\" height=\"400\">No object</object>";

//    RB_String strSvg = mPedTable->getSvg();
//    strHtml.append(strSvg);
    // RB_DEBUG->print(strHtml);


/* Second attempt
    strHtml += "<IMG src=\"qt-render://button?width=100&height=100&radius=5&color=green&pattern=dense\" />";
*/

    strHtml += "<p /><table style=\"border:0px;\" width=\"100%\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxResult->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblCategory->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leCategory->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblModules->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leModules->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblException->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + teException->toPlainText() + "</td>";
    strHtml += "</tr></table><p />";
    strHtml += "</body></html>";

    webView->setHtml(strHtml);
}

/**
 * Transfer this widget current view to HTML on a graphicview.
 * Function NOT USED
 * @param dwg graphicview
 */
void PENG_PedWidget::widgetToHtml(GV_Drawing* dwg) {
    double scale = 1 / dwg->getScaleFactor() * 0.9;
    QFont font = QFont("Arial", 8, QFont::Normal, false);
    // font.setLetterSpacing(QFont::PercentageSpacing, 110);

    RB_String strHtml;
    strHtml = "<p><strong>PED Equipment Classification</strong></p><p />";
    strHtml += "<table style=\"border:0px;\" width=\"700px\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxGeneral->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblEquipmentType->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + ileEquipmentType->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblTagNumber->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leTagNumber->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblDescription->text() + " :</td>";
    strHtml += "<td width=\"70%\">" + leDescription->text() + "</td>";
    strHtml += "</tr></table><p />";

    QGraphicsTextItem* txtItem = new QGraphicsTextItem();
    txtItem->setHtml(strHtml);
    txtItem->setFont(font);
    txtItem->setPos(20, 15);
    txtItem->setScale(scale);
    dwg->addItem(txtItem);

    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();

    strHtml = "<p>Print Date : ";
    strHtml += QDateTime::currentDateTime().toString(Qt::ISODate) + "</p><p />";
    strHtml += "<table style=\"border:0px;\" width=\"700px\"><tr>";
    strHtml += "<td colspan=\"2\"><u>Project</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Number : </td>";
    strHtml += "<td width=\"70%\">" + root->getValue("number").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Description : </td>";
    strHtml += "<td width=\"70%\">" + root->getValue("description").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Company : </td>";
    strHtml += "<td width=\"70%\">" + root->getValue("company").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Location : </td>";
    strHtml += "<td width=\"70%\">" + root->getValue("location").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Approval Name : </td>";
    strHtml += "<td width=\"70%\">__________________</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">Approval Signature : </td>";
    strHtml += "<td width=\"70%\">__________________</td>";
    strHtml += "</tr></table><p />";

    txtItem = new QGraphicsTextItem();
    txtItem->setHtml(strHtml);
    txtItem->setFont(font);
    txtItem->setPos(110, 15);
    txtItem->setScale(scale);
    dwg->addItem(txtItem);

    strHtml = "<table style=\"border:0px;\" width=\"700px\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + gbProperties->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblPedType->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + cbPedType->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblAssessmentBy->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + cbAssessmentBy->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblFluidState->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + cbFluidState->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblFluidGroup->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + cbFluidGroup->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblPs->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + RB_String::number(lePsLine->text().toDouble() * 10.0, 'g', 1) + " bar</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblTs->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + leTs->text() + " Celsius</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblV->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + leV->text() + " litres</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblDn->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + leDn->text() + " mm</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblAdditionalInformation->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + cbAdditionalInformation->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + chbUnstableGas->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + RB_String(chbUnstableGas->isChecked() ? "Yes" : "No") + "</td>";
    strHtml += "</tr></table><p />";

    txtItem = new QGraphicsTextItem();
    txtItem->setHtml(strHtml);
    txtItem->setFont(font);
    txtItem->setPos(20, 47);
    txtItem->setScale(scale);
    dwg->addItem(txtItem);

    strHtml = "<p /><table style=\"border:0px;\" width=\"350px\"><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxResult->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblCategory->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + leCategory->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblModules->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + leModules->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"30%\">" + lblException->text() + " : </td>";
    strHtml += "<td width=\"70%\">" + teException->toPlainText() + "</td>";
    strHtml += "</tr></table><p />";

    txtItem = new QGraphicsTextItem();
    txtItem->setHtml(strHtml);
    txtItem->setFont(font);
    txtItem->setPos(110, 60);
    txtItem->setScale(scale);
    dwg->addItem(txtItem);

    RB_String strSvg = mPedTable->getSvg();
    QSvgWidget* svgWgt = new QSvgWidget();
    svgWgt->load(strSvg.toUtf8());
    QGraphicsProxyWidget* proxyItem = new QGraphicsProxyWidget();
    proxyItem->setWidget(svgWgt);
    proxyItem->setPos(20, 110);
    proxyItem->setScale(0.78 * scale);
    dwg->addItem(proxyItem);

}

/**
 * Transfer this widget current equipment view to HTML on a textEdit
 * @param te textEdit (view)
 */
void PENG_PedWidget::widgetToHtml(QTextEdit* te) {
    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();

    RB_String strHtml;
    // Note: 1px table border does not work
    // strHtml += "<table border=\"0.5\" cellpadding=\"0\" cellspacing=\"0\" border-color=\"black\" width=\"100%\"><tr>";
    // Alternative:
    // strHtml += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"" + mTextColor.name() + "\" width=\"100%\"><tr>";
    // and set all cell background color to white

    strHtml += "<table border=\"0\" width=\"100%\"><tr>";
    strHtml += "<td colspan=\"2\"><p><strong>PED Equipment Classification</strong></p><p /></td>";
    strHtml += "<td colspan=\"2\"><p>Created : "
               + QDateTime::currentDateTime().toString(Qt::ISODate) + "</p><p /></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxGeneral->title() + "</u></td>";
    strHtml += "<td colspan=\"2\"><u>Project</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"23%\">" + lblEquipmentType->text() + " :</td>";
    strHtml += "<td width=\"30%\">" + ileEquipmentType->text() + "</td>";
    strHtml += "<td width=\"17%\">Number : </td>";
    strHtml += "<td width=\"30%\">" + root->getValue("number").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblTagNumber->text() + " :</td>";
    strHtml += "<td>" + leTagNumber->text() + "</td>";
    strHtml += "<td>Description : </td>";
    strHtml += "<td>" + root->getValue("description").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblDescription->text() + " :</td>";
    strHtml += "<td>" + leDescription->text() + "</td>";
    strHtml += "<td>Company : </td>";
    strHtml += "<td>" + root->getValue("company").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>&nbsp;</td>";
    strHtml += "<td>&nbsp;</td>";
    strHtml += "<td>Location : </td>";
    strHtml += "<td>" + root->getValue("location").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td colspan=\"2\"><u>" + gbProperties->title() + "</u></td>";
    strHtml += "<td>Approval Name : </td>";
    strHtml += "<td>__________________</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblPedType->text() + " : </td>";
    strHtml += "<td>" + cbPedType->currentText() + "</td>";
    strHtml += "<td>Approval Signature : </td>";
    strHtml += "<td>__________________</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblAssessmentBy->text() + " : </td>";
    strHtml += "<td>" + cbAssessmentBy->currentText() + "</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblFluidState->text() + " : </td>";
    strHtml += "<td>" + cbFluidState->currentText() + "</td>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxResult->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblFluidGroup->text() + " : </td>";
    strHtml += "<td>" + cbFluidGroup->currentText() + "</td>";
    strHtml += "<td>" + lblCategory->text() + " : </td>";
    strHtml += "<td>" + leCategory->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblPs->text() + " : </td>";
    double psbar = lePs->text().toDouble() * 10.0;
    strHtml += "<td>" + RB_String::number(psbar) + " bar</td>";
    strHtml += "<td>" + lblModules->text() + " : </td>";
    strHtml += "<td>" + leModules->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblTs->text() + " : </td>";
    strHtml += "<td>" + leTs->text() + " Celsius</td>";
    strHtml += "<td>" + lblException->text() + " : </td>";
    strHtml += "<td>" + teException->toPlainText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblV->text() + " : </td>";
    strHtml += "<td>" + leV->text() + " litres</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblDn->text() + " : </td>";
    strHtml += "<td>" + leDn->text() + " mm</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblAdditionalInformation->text() + " : </td>";
    strHtml += "<td colspan=\"3\">" + cbAdditionalInformation->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + chbUnstableGas->text() + " : </td>";
    strHtml += "<td>" + RB_String(chbUnstableGas->isChecked() ? "Yes" : "No") + "</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + tr("Fluids") + " : </td>";
    strHtml += "<td colspan=\"3\">" + leFluid1->text();
    if (!leFluid2->text().isEmpty()) {
        strHtml += ", " + leFluid2->text();
    }
    if (!leFluid3->text().isEmpty()) {
        strHtml += ", " + leFluid3->text();
    }
    if (!leFluid4->text().isEmpty()) {
        strHtml += ", " + leFluid4->text();
    }
    strHtml += "</td>";
    strHtml += "</tr>";
    strHtml += "</table>";
    strHtml += "<p>&nbsp;</p>";
    strHtml += "<p>&nbsp;</p>";

    te->document()->setHtml(strHtml);

    // Create image
    QImage img(QSize(600,600), QImage::Format_Mono);
    mPedTable->renderImage(&img);

    // Insert image
    QTextCursor cursor = te->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertImage(img);
}

/**
 * Transfer this widget current line view to HTML on a textEdit
 * @param te textEdit (view)
 */
void PENG_PedWidget::widgetToHtmlLine(QTextEdit* te) {
    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();

    RB_String strHtml;
    // Note: 1px table border does not work
    // strHtml += "<table border=\"0.5\" cellpadding=\"0\" cellspacing=\"0\" border-color=\"black\" width=\"100%\"><tr>";
    // Alternative:
    // strHtml += "<table border=\"0\" cellpadding=\"0\" cellspacing=\"1\" bgcolor=\"" + mTextColor.name() + "\" width=\"100%\"><tr>";
    // and set all cell background color to white

    strHtml += "<table border=\"0\" width=\"100%\"><tr>";
    strHtml += "<td colspan=\"2\"><p><strong>PED Pipe Line Classification</strong></p><p /></td>";
    strHtml += "<td colspan=\"2\"><p>Created : "
               + QDateTime::currentDateTime().toString(Qt::ISODate) + "</p><p /></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxGeneralLine->title() + "</u></td>";
    strHtml += "<td colspan=\"2\"><u>Project</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td width=\"23%\">" + lblEquipmentTypeLine->text() + " :</td>";
    strHtml += "<td width=\"30%\">" + leEquipmentTypeLine->text() + "</td>";
    strHtml += "<td width=\"17%\">Number : </td>";
    strHtml += "<td width=\"30%\">" + root->getValue("number").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblTagNumberLine->text() + " :</td>";
    strHtml += "<td>" + leTagNumberLine->text() + "</td>";
    strHtml += "<td>Description : </td>";
    strHtml += "<td>" + root->getValue("description").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblDescriptionLine->text() + " :</td>";
    strHtml += "<td>" + leDescriptionLine->text() + "</td>";
    strHtml += "<td>Company : </td>";
    strHtml += "<td>" + root->getValue("company").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>&nbsp;</td>";
    strHtml += "<td>&nbsp;</td>";
    strHtml += "<td>Location : </td>";
    strHtml += "<td>" + root->getValue("location").toString() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td colspan=\"2\"><u>" + gbPropertiesLine->title() + "</u></td>";
    strHtml += "<td>Approval Name : </td>";
    strHtml += "<td>__________________</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblPedTypeLine->text() + " : </td>";
    strHtml += "<td>" + cbPedTypeLine->currentText() + "</td>";
    strHtml += "<td>Approval Signature : </td>";
    strHtml += "<td>__________________</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblAssessmentByLine->text() + " : </td>";
    strHtml += "<td>" + cbAssessmentByLine->currentText() + "</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblFluidStateLine->text() + " : </td>";
    strHtml += "<td>" + cbFluidStateLine->currentText() + "</td>";
    strHtml += "<td colspan=\"2\"><u>" + groupBoxResultLine->title() + "</u></td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblFluidGroupLine->text() + " : </td>";
    strHtml += "<td>" + cbFluidGroupLine->currentText() + "</td>";
    strHtml += "<td>" + lblCategoryLine->text() + " : </td>";
    strHtml += "<td>" + leCategoryLine->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblPsLine->text() + " : </td>";
    double psbar = lePsLine->text().toDouble() * 10.0;
    strHtml += "<td>" + RB_String::number(psbar) + " bar</td>";
    strHtml += "<td>" + lblModulesLine->text() + " : </td>";
    strHtml += "<td>" + leModulesLine->text() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblTsLine->text() + " : </td>";
    strHtml += "<td>" + leTsLine->text() + " Celsius</td>";
    strHtml += "<td>" + lblExceptionLine->text() + " : </td>";
    strHtml += "<td>" + teExceptionLine->toPlainText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblVLine->text() + " : </td>";
    strHtml += "<td>" + leVLine->text() + " litres</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblDnLine->text() + " : </td>";
    strHtml += "<td>" + leDnLine->text() + " mm</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + lblAdditionalInformationLine->text() + " : </td>";
    strHtml += "<td colspan=\"3\">" + cbAdditionalInformationLine->currentText() + "</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + chbUnstableGasLine->text() + " : </td>";
    strHtml += "<td>" + RB_String(chbUnstableGasLine->isChecked() ? "Yes" : "No") + "</td>";
    strHtml += "<td colspan=\"2\">&nbsp;</td>";
    strHtml += "</tr><tr>";
    strHtml += "<td>" + tr("Fluids") + " : </td>";
    strHtml += "<td colspan=\"3\">" + leFluid1Line->text();
    if (!leFluid2Line->text().isEmpty()) {
        strHtml += ", " + leFluid2Line->text();
    }
    if (!leFluid3Line->text().isEmpty()) {
        strHtml += ", " + leFluid3Line->text();
    }
    if (!leFluid4->text().isEmpty()) {
        strHtml += ", " + leFluid4Line->text();
    }
    strHtml += "</td>";
    strHtml += "</tr>";
    strHtml += "</table>";
    strHtml += "<p>&nbsp;</p>";
    strHtml += "<p>&nbsp;</p>";

    te->document()->setHtml(strHtml);

    // Create image
    QImage img(QSize(600,600), QImage::Format_Mono);
    mPedTableLine->renderImage(&img);

    // Insert image
    QTextCursor cursor = te->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertImage(img);
}
