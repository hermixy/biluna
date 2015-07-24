/*****************************************************************
 * $Id: peng_pedeqsummarywidget.cpp 1826 2012-12-08 00:31:33Z rutger $
 * Created: Mar 2, 2011 10:00:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pedeqsummarywidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "peng_determinepedclass.h"
#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "peng_navigationwidget.h"
#include "peng_sqlcommonfunctions.h"
#include "rb_stringlist.h"


/**
 * Constructor
 */
PENG_PedEqSummaryWidget::PENG_PedEqSummaryWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mSelected = true;
    mIsEquipmentLine = -1;
    mIsEquipment = true;
}

/**
 * Destructor
 */
PENG_PedEqSummaryWidget::~PENG_PedEqSummaryWidget() {
    // nothing
}

/**
 * Get proposed saveas filename, default returns window title (getName())
 * with space replaced by underscore
 * @returns filename
 */
RB_String PENG_PedEqSummaryWidget::getSaveAsFileName() {
    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();
    return "peng_pedsummary_" + root->getValue("number").toString();
}

/**
 * Initialize and create trial balance
 */
void PENG_PedEqSummaryWidget::init() {
    // Should already exist
    RB_Widget* wdgt = PENG_DIALOGFACTORY->getWidget(PENG_DialogFactory::WidgetNavigation,
                                                    PENG_DIALOGFACTORY->getMainWindow());
    mNavWdgt = dynamic_cast<PENG_NavigationWidget*>(wdgt);
    connect(mNavWdgt, SIGNAL(currentNavigationTab(int)),
            this, SLOT(slotEquipmentNavigated(int)));

    cbSelection->addItem(tr("Selected Items"));
    cbSelection->addItem(tr("All Items"));

    mHtml = "<p>";
    mHtml += tr("Select All- or Selected Items above and click refresh button ...");
    mHtml += "</p>";

    textEdit->setHtml(mHtml);
    readSettings();
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool PENG_PedEqSummaryWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void PENG_PedEqSummaryWidget::filePdf(QPrinter* printer) {
    printer->setOrientation(QPrinter::Landscape);
    textEdit->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void PENG_PedEqSummaryWidget::filePrint(QPrinter* printer) {
    textEdit->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void PENG_PedEqSummaryWidget::filePrintPreview(QPrinter* printer) {
    textEdit->print(printer);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String PENG_PedEqSummaryWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Initialize and create trial balance
 */
void PENG_PedEqSummaryWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mNavWdgt->emitState();  // set mIsEquipmentLine

    if (mIsEquipmentLine == mNavWdgt->TabEquipment
            || mIsEquipmentLine == mNavWdgt->TabPipeLine) {
        createTable();
    } else {
        mHtml = "<p>";
        mHtml += tr("Current navigation selection does not include equipment or pipelines.");
        mHtml += "</p>";
    }

    textEdit->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Slot to receive signal with selected navigation tab
 */
void PENG_PedEqSummaryWidget::slotEquipmentNavigated(int tab) {
    if (tab == mNavWdgt->TabEquipment || tab == mNavWdgt->TabPipeLine) {
        mIsEquipmentLine = tab;
    } else {
        mIsEquipmentLine = -1;
    }
}

/**
 * Create equipment PED classification summary list
 */
void PENG_PedEqSummaryWidget::createTable() {
    if (mIsEquipmentLine == mNavWdgt->TabEquipment) {
        mIsEquipment = true;
    } else {
        mIsEquipment = false;
    }

    RB_String strTitle = mIsEquipment ? tr("PED Equipment Classification - Project ")
                            : tr("PED Pipe Line Classification - Project ");
    RB_String strEq = mIsEquipment ? tr("Equipment") : tr("Pipe Line");
    RB_String strEqData = mIsEquipment ? tr("Equipment Data") : tr("Pipe Line Data");
    RB_String strVolSize = mIsEquipment ? tr("Volume<br>[l]") : tr("Nom.Size<br>[mm]");


    QDateTime dt = QDateTime::currentDateTime();
    RB_ObjectBase* root = PENG_MODELFACTORY->getRoot();
    RB_String projStr = root->getValue("number").toString();

    mHtml = "<table cellpadding=\"3\" rules=\"groups\" frame=\"box\" width=\"100%\" style=\"font-size:11pt;\">"
            "<thead>"
            "<tr><th colspan=\"14\" align=\"center\">" + strTitle + projStr + "</th></tr>"
            "<tr><td colspan=\"14\" align=\"center\" style=\"font-size:8pt;\">" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
            "<tr><th colspan=\"3\">" + strEq + "</th>"
            "<th colspan=\"4\">" + tr("Fluid") + "</th>"
            "<th colspan=\"2\">" + tr("PED Fluid") + "</th>"
            "<th colspan=\"3\">" + strEqData + "</th>"
            "<th colspan=\"2\">" + tr("Classification") + "</th></tr>"
            "<tr style=\"font-size:9pt;\"><td width=\"10%\">" + tr("Tag") + "</td><td width=\"19%\">" + tr("Name") + "</td><td width=\"13%\">" + tr("Type") + "</td>"
            "<td width=\"7%\" align=\"center\">1</td><td width=\"7%\" align=\"center\">2</td><td width=\"7%\" align=\"center\">3</td><td width=\"7%\" align=\"center\">4</td>"
            "<td width=\"5%\" align=\"center\">" + tr("State") + "</td><td width=\"5%\" align=\"center\">" + tr("Group") + "</td>"
            "<td width=\"4%\" align=\"center\">" + tr("Press.<br>[bar]") + "</td><td width=\"4%\" align=\"center\">" + tr("Temp.<br>[C]") + "</td><td width=\"4%\" align=\"center\">" + strVolSize + "</td>"
            "<td width=\"4%\" align=\"center\">" + tr("Table") + "</td><td width=\"4%\" align=\"center\">" + tr("Category") + "</td></tr>"
            "<tr><td colspan=\"14\"><hr style=\"border-style:solid;\" /></td></tr></thead><tbody style=\"font-size:8pt;\">";



    if (mIsEquipmentLine == mNavWdgt->TabEquipment) {
        // Equipment
        RB_StringList eqIdList;
        PENG_SqlCommonFunctions util;
        QSqlQuery q(PENG_MODELFACTORY->getDatabase());

        if (cbSelection->currentIndex() == 1 /* All Items*/) {
            util.getPedEqList(q, eqIdList);
            createTableHelper(q);
        } else {
            RB_MmProxy* m = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipment);
            QItemSelectionModel* selModel = m->getSelectionModel();
            if (!selModel) return;

            QModelIndexList idxList = selModel->selectedIndexes();
            QModelIndex idIdx;
            RB_String id;
            int count = idxList.size();

            for (int i = 0; i < count; ++i) {
                idIdx = m->index(idxList.at(i).row(), 0);
                id = idIdx.data(RB2::RoleOrigData).toString();
                eqIdList.append(id);
            }

            util.getPedEqList(q, eqIdList);
            createTableHelper(q);
        }
    } else {
        // Pipe Lines
        RB_StringList lineIdList;
        PENG_SqlCommonFunctions util;
        QSqlQuery q(PENG_MODELFACTORY->getDatabase());


        if (cbSelection->currentIndex() == 1 /* All Items*/) {
            util.getPedLineList(q, lineIdList);
            createTableHelper(q);
        } else {
            RB_MmProxy* m = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelLine);
            QItemSelectionModel* selModel = m->getSelectionModel();
            if (!selModel) return;

            QModelIndexList idxList = selModel->selectedIndexes();
            QModelIndex idIdx;
            RB_String id;
            int count = idxList.size();

            for (int i = 0; i < count; ++i) {
                idIdx = m->index(idxList.at(i).row(), 0);
                id = idIdx.data(RB2::RoleOrigData).toString();
                lineIdList.append(id);
            }

            util.getPedLineList(q, lineIdList);
            createTableHelper(q);
        }
    }

    mHtml += "</tbody>";
    mHtml += "</table>";
}

void PENG_PedEqSummaryWidget::createTableHelper(QSqlQuery& q) {
    // QSqlQuery q(PENG_MODELFACTORY->getDatabase());
    QSqlRecord rec = q.record();

    int numberCol = rec.indexOf("number");
    int typenameCol = mIsEquipment ? rec.indexOf("typename") : -1;
    int eqnameCol = mIsEquipment ? rec.indexOf("eqname") : -1;
    int designtempCol = rec.indexOf("designtemp");
    int designpressCol = rec.indexOf("designpress");
    int volumeCol = mIsEquipment ? rec.indexOf("volume") : -1;
    int dnCol = mIsEquipment ? -1 : rec.indexOf("sizedn");
    int pedtypeCol = rec.indexOf("pedtype_id");
    int assessmentbyCol = rec.indexOf("assessment");
    int fluidstateCol = rec.indexOf("fluidstate_id");
    int fluidgroupCol = rec.indexOf("fluidgroup_id");
    int additionalinfoCol = rec.indexOf("additionalinfo_id");
    int unstablegasCol = rec.indexOf("unstablegas");
    int fluid1Col = rec.indexOf("fluid1");
    int fluid2Col = rec.indexOf("fluid2");
    int fluid3Col = rec.indexOf("fluid3");
    int fluid4Col = rec.indexOf("fluid4");

    RB_String type = "Equipment";
    RB_String assess = "AssessVolume";
    RB_String fluidState = "Gaseous";
    RB_String fluidGroup = "Group1";
    RB_String addInfo = "(none)";
    RB_String unstableGas = "Unstable";

    while (q.next()) {
        if (q.value(pedtypeCol).toInt() == 1) {
            type = "Generator";
        } else if (q.value(pedtypeCol).toInt() == 2) {
            type = "Piping";
        } else if (q.value(pedtypeCol).toInt() == 3) {
            type = "Pressure_Accessory";
        } else if (q.value(pedtypeCol).toInt() == 4) {
            type = "Safety_Accessory";
        }

        if (q.value(assessmentbyCol).toInt() == 1) {
            assess = "AssessDn";
        }

        if (q.value(fluidstateCol).toInt() == 1) {
            fluidState = "Liquid";
        }

        if (q.value(fluidgroupCol).toInt() == 1) {
            fluidGroup = "Group2";
        }

        if (q.value(additionalinfoCol).toInt() == 1) {
            addInfo = "Bottle_Breathing_Eqmt";
        } else if (q.value(additionalinfoCol).toInt() == 2) {
            addInfo = "Portable_Extinguisher";
        } else if (q.value(additionalinfoCol).toInt() == 3) {
            addInfo = "Pressure_Cooker";
        }

        if (!q.value(unstablegasCol).toBool()) {
            unstableGas = "Stable";
        }

        // create input model, execute action and get output model
        mOperPed.execute(type, assess, fluidState, fluidGroup,
                         q.value(designpressCol).toDouble() * 10,
                         q.value(designtempCol).toDouble(),
                         mIsEquipment ? q.value(volumeCol).toDouble() : 0.0,
                         mIsEquipment ? 0.0 : q.value(dnCol).toDouble(),
                         addInfo, unstableGas);

        addEqDataRow(q.value(numberCol).toString(),
                     mIsEquipment ? q.value(eqnameCol).toString() : "-",
                     mIsEquipment ? q.value(typenameCol).toString() : "Pipeline", // not 'type'
                     q.value(fluid1Col).toString(),
                     q.value(fluid2Col).toString(),
                     q.value(fluid3Col).toString(),
                     q.value(fluid4Col).toString(),
                     fluidState,
                     fluidGroup,
                     q.value(designpressCol).toDouble() * 10,
                     q.value(designtempCol).toDouble(),
                     mIsEquipment ? q.value(volumeCol).toDouble() : q.value(dnCol).toDouble());
    }
}

/**
 * Add a data row with amounts
 */
void PENG_PedEqSummaryWidget::addEqDataRow(const RB_String& number,
                                        const RB_String& eqname,
                                        const RB_String& type,
                                        const RB_String& fluid1,
                                        const RB_String& fluid2,
                                        const RB_String& fluid3,
                                        const RB_String& fluid4,
                                        const RB_String& fluidState,
                                        const RB_String& fluidGroup,
                                        double maxPress,
                                        double maxTemp,
                                        double maxVol_Dn) {
    mHtml += "<tr><td>";
    mHtml += number;
    mHtml += "</td><td>";
    mHtml += eqname;
    mHtml += "</td><td>";
    mHtml += type; //type_id
    mHtml += "</td><td>";
    mHtml += fluid1;
    mHtml += "</td><td>";
    mHtml += fluid2;
    mHtml += "</td><td>";
    mHtml += fluid3;
    mHtml += "</td><td>";
    mHtml += fluid4;
    mHtml += "</td><td>";
    mHtml += fluidState;
    mHtml += "</td><td>";
    mHtml += fluidGroup;
    mHtml += "</td><td>";
    mHtml += RB_String::number(maxPress);
    mHtml += "</td><td>";
    mHtml += RB_String::number(maxTemp);
    mHtml += "</td><td>";
    mHtml += RB_String::number(maxVol_Dn);
    mHtml += "</td><td>";
    mHtml += RB_String::number(mOperPed.getTable());
    mHtml += "</td><td>";
    mHtml += mOperPed.getCategory();
    mHtml += "</td></tr>";
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool PENG_PedEqSummaryWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << textEdit->toHtml();
    QApplication::restoreOverrideCursor();

    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

void PENG_PedEqSummaryWidget::calcPedClass(int row) {
    RB_MmProxy* m = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEquipment);

    RB_String type = "Equipment";
    QModelIndex idx = m->index(row, m->fieldIndex("pedtype_id"));

    if (idx.data().toInt() == 1) {
        type = "Generator";
    } else if (idx.data().toInt() == 2) {
        type = "Piping";
    } else if (idx.data().toInt() == 3) {
        type = "Pressure_Accessory";
    } else if (idx.data().toInt() == 4) {
        type = "Safety_Accessory";
    }

    RB_String assess = "AssessVolume";
    idx = m->index(row, m->fieldIndex("assessmentby_id"));

    if (idx.data().toInt() == 1) {
        assess = "AssessDn";
    }

    RB_String fluidState = "Gaseous";
    idx = m->index(row, m->fieldIndex("fluidstate_id"));

    if (idx.data().toInt() == 1) {
        fluidState = "Liquid";
    }

    RB_String fluidGroup = "Group1";
    idx = m->index(row, m->fieldIndex("fluidgroup_id"));

    if (idx.data().toInt() == 1) {
        fluidGroup = "Group2";
    }

    idx = m->index(row, m->fieldIndex("designpress"));
    double maxPress = idx.data().toDouble() * 10;
    idx = m->index(row, m->fieldIndex("designtemp"));
    double maxTemp = idx.data().toDouble();
    idx = m->index(row, m->fieldIndex("volume"));
    double maxVol = idx.data().toDouble();
    idx = m->index(row, m->fieldIndex("nominalsize"));
    double nomSize = idx.data().toDouble();

    RB_String addInfo = "(none)";
    idx = m->index(row, m->fieldIndex("additionalinfo_id"));

    if (idx.data().toInt() == 1) {
        addInfo = "Bottle_Breathing_Eqmt";
    } else if (idx.data().toInt() == 2) {
        addInfo = "Portable_Extinguisher";
    } else if (idx.data().toInt() == 3) {
        addInfo = "Pressure_Cooker";
    }

    RB_String unstableGas = "Unstable";
    idx = m->index(row, m->fieldIndex("unstablegas"));

    if (idx.data().toInt() == 0) {
        unstableGas = "Stable";
    }

    // create input model, execute action and get output model
    mOperPed.execute(type, assess, fluidState, fluidGroup, maxPress,
                    maxTemp, maxVol, nomSize, addInfo, unstableGas);
}

bool PENG_PedEqSummaryWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
}

/**
 * Change event such as language change
 */
void PENG_PedEqSummaryWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
