/*****************************************************************
 * $Id: sail_mapnavigationwidget.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_mapnavigationwidget.h"

#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_sqlrelationaldelegate.h"
#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "sail_routeoperation.h"
#include "sail_sqlcommonfunctions.h"


/**
 * Constructor
 */
SAIL_MapNavigationWidget::SAIL_MapNavigationWidget(QWidget* parent)
                                    : RB_Widget(parent) {
    setupUi(this);

//    mTextPreview = new RB_TextPreview(this);
//    scrollArea->setBackgroundRole(QPalette::Dark);
//    scrollArea->setWidgetResizable(true);
//    scrollArea->setWidget(mTextPreview);

    mModel = NULL;
    mMapper = NULL;
    mRouteModel = NULL;
    mRouteMapper = NULL;
    mRouteCoordinateModel = NULL;
    mRouteCoordinateMapper = NULL;
    mTrackModel = NULL;
    mTrackMapper = NULL;

    mAddCoordinateInProgress = false;
}

/**
 * Destructor
 */
SAIL_MapNavigationWidget::~SAIL_MapNavigationWidget() {
    delete mModel;
    delete mRouteModel;
    delete mRouteCoordinateModel;
    delete mTrackModel;
    RB_DEBUG->print("SAIL_MapNavigationWidget::~SAIL_MapNavigationWidget() OK");
}

/**
 * Get proposed saveas filename
 * @returns filename
 */
RB_String SAIL_MapNavigationWidget::getSaveAsFileName() {
//    if (!mModel->getCurrentIndex().isValid()) {
        return QString();
//    }

//    RB_String str = mModel->getCurrentValue("reference").toString();
//    return str;
}

/**
 * Initialize widget and models
 */
void SAIL_MapNavigationWidget::init() {
    //
    // 1. Set model and mapper
    //
    mModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelCoordinate);
    mModel->setRoot(SAIL_MODELFACTORY->getRootId());
    mRouteModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelRoute);
    mRouteModel->setRoot(SAIL_MODELFACTORY->getRootId());
    mRouteCoordinateModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelRouteCoordinate);
    mRouteCoordinateModel->setRoot("invalid"); // test, needs to stay?
    mTrackModel = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelTrack);
    mTrackModel->setRoot(SAIL_MODELFACTORY->getRootId());

    //
    // 2. Get mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(leCoordinateName, mModel->fieldIndex("mname"));
    mMapper->addMapping(leCoordinateComment, mModel->fieldIndex("comment"));
    mMapper->addMapping(leLatDecimal, mModel->fieldIndex("latitude"));
    mMapper->addMapping(leLongDecimal, mModel->fieldIndex("longitude"));

    mRouteMapper = mRouteModel->getMapper();
    mRouteMapper->addMapping(leRouteName, mRouteModel->fieldIndex("mname"));
    mRouteMapper->addMapping(leRouteComment, mRouteModel->fieldIndex("comment"));

    mRouteCoordinateMapper = mRouteCoordinateModel->getMapper();
    mRouteCoordinateMapper->addMapping(leSequenceNumber, mRouteCoordinateModel->fieldIndex("seqno"));
    mRouteCoordinateMapper->addMapping(leRouteCoordinateComment, mRouteCoordinateModel->fieldIndex("comment"));
    mRouteCoordinateMapper->addMapping(ileRouteCoordinate, mRouteCoordinateModel->fieldIndex("coordinate_idx"));

    mTrackMapper = mTrackModel->getMapper();
    mTrackMapper->addMapping(leTrackName, mTrackModel->fieldIndex("mname"));
    mTrackMapper->addMapping(leTrackComment, mTrackModel->fieldIndex("comment"));
    mTrackMapper->addMapping(ileTrackCoordinateFrom, mTrackModel->fieldIndex("from_idx"));
    mTrackMapper->addMapping(ileTrackCoordinateTo, mTrackModel->fieldIndex("to_idx"));
    mTrackMapper->addMapping(leWeightFactor, mTrackModel->fieldIndex("weightfactor"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();
    mRouteModel->select();
    mRouteCoordinateModel->select(); // test, needs to stay?
    mTrackModel->select();

    //
    // 4. Connect model to main view
    //
    readSettings();
    setFormatTableView(tvCoordinate, mModel);
    setFormatTableView(tvRoute, mRouteModel);
    setFormatTableView(tvRouteCoordinate, mRouteCoordinateModel);
    setFormatTableView(tvTrack, mTrackModel);

    int count;
    count = mModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvCoordinate->hideColumn(i);
        } else {
            tvCoordinate->showColumn(i);

            if (tvCoordinate->columnWidth(i) < 5) {
                tvCoordinate->setColumnWidth(i, 100);
            }
        }
    }

    count = mRouteModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvRoute->hideColumn(i);
        } else {
            tvRoute->showColumn(i);

            if (tvRoute->columnWidth(i) < 5) {
                tvRoute->setColumnWidth(i, 100);
            }
        }
    }

    count = mRouteCoordinateModel->columnCount(QModelIndex());

    for (int i = 0; i < count; ++i) {
        if (i < RB2::HIDDENCOLUMNS) {
            tvRouteCoordinate->hideColumn(i);
        } else {
            tvRouteCoordinate->showColumn(i);

            if (tvRouteCoordinate->columnWidth(i) < 5) {
                tvRouteCoordinate->setColumnWidth(i, 100);
            }
        }
    }

    count = mTrackModel->columnCount(QModelIndex());

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

    leLatDecimal->setValidator(new QDoubleValidator(-90.0, 90.0, 8, this));
    leLongDecimal->setValidator(new QDoubleValidator(-180.0, 180.0, 8, this));

    connect(mModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentItemChanged(QModelIndex,QModelIndex)));

    connect(leLatitude, SIGNAL(editingFinished()),
            this, SLOT(slot_latDecimal_update()));
    connect(leLongitude, SIGNAL(editingFinished()),
            this, SLOT(slot_longDecimal_update()));
    connect(leLatDecimal, SIGNAL(editingFinished()),
            this, SLOT(slot_latitude_update()));
    connect(leLongDecimal, SIGNAL(editingFinished()),
            this, SLOT(slot_longitude_update()));

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotDrawScene(int)));

    gv->setScene(&mScene);
    gv->setRenderHints(QPainter::Antialiasing);
    slotDrawScene(0);

}

/**
 * Save data to database
 */
bool SAIL_MapNavigationWidget::fileSave(bool withSelect) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // HACK to emit editingFinished() for current QLineEdit:
    beforeFileSave();

    if (withSelect && mModel && mRouteModel && mRouteCoordinateModel && mTrackModel) {
        mModel->submitAllAndSelect();
        mRouteCoordinateModel->submitAllAndSelect();
        mRouteModel->submitAllAndSelect();
        mTrackModel->submitAllAndSelect();

        setWindowModified(false);
        QApplication::restoreOverrideCursor();
        return true;
    } else if (mModel && mRouteModel && mRouteCoordinateModel && mTrackModel) {
        mModel->submitAll();
        mRouteCoordinateModel->submitAll();
        mRouteModel->submitAll();
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
void SAIL_MapNavigationWidget::filePdf(QPrinter* printer) {
    // later
}

/**
 * File menu PDF clicked
 * @param printer
 */
void SAIL_MapNavigationWidget::filePrint(QPrinter* printer) {
    // later
}

/**
 * File menu PDF clicked
 * @param printer
 */
void SAIL_MapNavigationWidget::filePrintPreview(QPrinter* printer) {
    // later
}

/**
 * Button add clicked
 */
void SAIL_MapNavigationWidget::on_pbAddCoordinate_clicked() {
    if (!mModel) return;
    mAddCoordinateInProgress = true;
    mModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mModel->index(row, mModel->fieldIndex("mname"));
    mModel->setData(idx, "<NEW>", Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("latitude"));
    mModel->setData(idx, 0.0, Qt::EditRole);
    idx = mModel->index(row, mModel->fieldIndex("longitude"));
    mModel->setData(idx, 0.0, Qt::EditRole);
    // end NOTE

    tvCoordinate->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvCoordinate->scrollTo(tvCoordinate->currentIndex());
    leCoordinateName->setFocus();
    leCoordinateName->selectAll();
    mAddCoordinateInProgress = false;

    leLatitude->setText("00 00.000N");
    leLongitude->setText("000 00.000E");
}

/**
 * Button delete clicked
 */
void SAIL_MapNavigationWidget::on_pbDeleteCoordinate_clicked() {
    int ret = SAIL_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        SAIL_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvCoordinate->currentIndex();
    if (!index.isValid()) return;

    mModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void SAIL_MapNavigationWidget::on_pbSaveCoordinate_clicked() {
    fileSave(true);
}

void SAIL_MapNavigationWidget::on_pbFilterOn_clicked() {
    RB_String str = leFilter->text();
    if (str.isEmpty()) {
        return;
    }
    if (!str.endsWith("%")) {
        str += "%";
    }
    RB_String filter = "`sail_coordinate`.`mname` LIKE '";
    filter += str + "'";
    mModel->setWhere(filter);
}

void SAIL_MapNavigationWidget::on_pbFilterOff_clicked() {
    mModel->setWhere("");
    leFilter->setText("");
}

/**
 * Button add route clicked
 */
void SAIL_MapNavigationWidget::on_pbAddRoute_clicked() {
    if (!mRouteModel) return;
    mRouteModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mRouteModel->rowCount();
    mRouteModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mRouteModel->index(row, mRouteModel->fieldIndex("mname"));
    mRouteModel->setData(idx, "<NEW>", Qt::EditRole);
    // end NOTE

    tvRoute->setCurrentIndex(mRouteModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvRoute->scrollTo(tvRoute->currentIndex());
    leRouteName->setFocus();
    leRouteName->selectAll();
}

/**
 * Button delete route clicked
 */
void SAIL_MapNavigationWidget::on_pbDeleteRoute_clicked() {
    int ret = SAIL_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        SAIL_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvRoute->currentIndex();
    if (!index.isValid()) return;

    mRouteModel->removeRows(index.row(), 1, QModelIndex());
    fileSave(true);
}

/**
 * Push button save clicked
 */
void SAIL_MapNavigationWidget::on_pbSaveRoute_clicked() {
    fileSave(true);
}

/**
 * Button add route coordinate clicked
 */
void SAIL_MapNavigationWidget::on_pbAddRouteCoordinate_clicked() {
    if (!tvRoute->currentIndex().isValid()) {
        SAIL_DIALOGFACTORY->requestWarningDialog("Cannot add a coordinate,\n"
                                                 "no route selected.");
    }

    if (!mRouteCoordinateModel) return;
    mRouteCoordinateModel->undoFilterSort();

    // always insert at the begin
    int row = 0; // mRouteCoordinateModel->rowCount();
    mRouteCoordinateModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mRouteCoordinateModel->index(row, mRouteCoordinateModel->fieldIndex("seqno"));
    mRouteCoordinateModel->setData(idx, 0, Qt::EditRole);
    // end NOTE

    tvRouteCoordinate->setCurrentIndex(mRouteCoordinateModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvRouteCoordinate->scrollTo(tvRouteCoordinate->currentIndex());
    leSequenceNumber->setFocus();
    leSequenceNumber->selectAll();
}

/**
 * Button delete route coordinate clicked
 */
void SAIL_MapNavigationWidget::on_pbDeleteRouteCoordinate_clicked() {
    int ret = SAIL_DIALOGFACTORY->requestYesNoDialog(
            tr("Delete item ..."),
            tr("This action cannot be undone.\n"
               "Do you want to delete this item?"));
    if (ret == QMessageBox::No) {
        SAIL_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    QModelIndex index = tvRouteCoordinate->currentIndex();
    if (!index.isValid()) return;

    mRouteCoordinateModel->removeRows(index.row(), 1, QModelIndex());
    mRouteModel->submitAllAndSelect();
    // setWindowModified(false); parent could have changed
}

/**
 * Button calculate distance and duration of route
 */
void SAIL_MapNavigationWidget::on_pbCalcRoute_clicked() {
    SAIL_RouteOperation oper;
    QModelIndexList idxList = mRouteCoordinateModel->getSelectionModel()->selectedIndexes();
    int numberOfSelectedRows = idxList.size(); // TODO not correct

    if (numberOfSelectedRows <= 1) {
        // if zero or one is selected calculate the whole route
        RB_String id = mRouteCoordinateModel->getRoot()->getId();
        RB_StringList idList;
        idList.append(id);

        oper.setWindFromDirection(leWindDirectionFrom->text().toDouble());
        oper.setCoordinateIdList(idList);
        oper.execute();

        leCalculatedDistance->setText(RB_String::number(oper.calculatedDistance()));
        leCalculatedDuration->setText(RB_String::number(oper.calculatedDuration()));
    } else {
        // if two or more are selected, calculate the route of the selected coordinates.
        int rowCount = idxList.size();
        RB_StringList idList;
        RB_String id;
        QModelIndex selIdx;
        QModelIndex idx;

        for (int i = 0; i < rowCount; ++i) {
            selIdx = idxList.at(i);
            idx = mRouteCoordinateModel->index(selIdx.row(), 0);
            id = mRouteCoordinateModel->data(idx).toString();
            idList.append(id);
        }

        oper.setWindFromDirection(leWindDirectionFrom->text().toDouble());
        oper.setCoordinateIdList(idList);
        oper.execute();

        leCalculatedDistance->setText(RB_String::number(oper.calculatedDistance()));
        leCalculatedDuration->setText(RB_String::number(oper.calculatedDuration()));
    }

}

/**
 * Button add track clicked
 */
void SAIL_MapNavigationWidget::on_pbAddTrack_clicked() {
    if (!mTrackModel) return;
    mTrackModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mTrackModel->rowCount(); // 0;
    mTrackModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex idx;
    idx = mTrackModel->index(row, mTrackModel->fieldIndex("mname"));
    mTrackModel->setData(idx, "<NEW>", Qt::EditRole);
    // HACK:
    idx = mTrackModel->index(row, mTrackModel->fieldIndex("comment"));
    mTrackModel->setData(idx, "new comment", Qt::EditRole);
    idx = mTrackModel->index(row, mTrackModel->fieldIndex("weightfactor"));
    mTrackModel->setData(idx, 0, Qt::EditRole);
    // HACK:
    leWeightFactor->clear();
    // end NOTE

    tvTrack->setCurrentIndex(mTrackModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvTrack->scrollTo(tvTrack->currentIndex());
    leTrackName->setFocus();
    leTrackName->selectAll();
}

/**
 * Button delete track clicked
 */
void SAIL_MapNavigationWidget::on_pbDeleteTrack_clicked() {
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
    fileSave(true);
}

/**
 * Push button save clicked
 */
void SAIL_MapNavigationWidget::on_pbSaveTrack_clicked() {
    fileSave(true);
}

/**
 * Button select 'route coordinate' clicked
 */
void SAIL_MapNavigationWidget::on_ileRouteCoordinate_clicked() {
    QModelIndex index = tvRouteCoordinate->currentIndex();

    if (!index.isValid()) {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No rout coordinate selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = SAIL_DIALOGFACTORY->getDialog(SAIL_DialogFactory::DialogSelectCoordinate);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = index.row();
            index = mRouteCoordinateModel->index(row, mRouteCoordinateModel->fieldIndex("coordinate_idx"));
            mRouteCoordinateModel->setData(index, obj->getId()
                                           + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        SAIL_DIALOGFACTORY->requestWarningDialog(tr("No coordinate selected.\n"
                                                   "Coordinate is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button select 'From' coordinate clicked
 */
void SAIL_MapNavigationWidget::on_ileTrackCoordinateFrom_clicked() {
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

    dlg->deleteLater();
}

/**
 * Button select 'To' coordinate clicked
 */
void SAIL_MapNavigationWidget::on_ileTrackCoordinateTo_clicked() {
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

    dlg->deleteLater();
}

/**
 * Slot updates the line edit latitude based on the decimal value
 */
void SAIL_MapNavigationWidget::slot_latitude_update() {
    double lat = leLatDecimal->text().toDouble();
    leLatitude->setText(mCalc.latLongToStrMin(lat, true));
}

/**
 * Slot updates the line edit longitude based on the decimal value
 */
void SAIL_MapNavigationWidget::slot_longitude_update() {
    double lon = leLongDecimal->text().toDouble();
    leLongitude->setText(mCalc.latLongToStrMin(lon, false));
}

/**
 * Slot updates the line edit decimal latitude based on the string value
 */
void SAIL_MapNavigationWidget::slot_latDecimal_update() {
    if (!tvCoordinate->currentIndex().isValid()) {
        return;
    }

    std::string stdStr = leLatitude->text().toStdString();
    char* charStr = new char[stdStr.size() + 1];
    strcpy(charStr, stdStr.c_str());

    double lat = mCalc.strToLatLong(charStr);
    QModelIndex idx = mModel->index(tvCoordinate->currentIndex().row(),
                                    mModel->fieldIndex("latitude"));
    mModel->setData(idx, lat);
    leLatitude->setText(mCalc.latLongToStrMin(lat, true));
}

/**
 * Slot updates the line edit decimal longitude based on the string value
 */
void SAIL_MapNavigationWidget::slot_longDecimal_update() {
    if (!tvCoordinate->currentIndex().isValid()) {
        return;
    }

    std::string stdStr = leLongitude->text().toStdString();
    char* charStr = new char[stdStr.size() + 1];
    strcpy(charStr, stdStr.c_str());

    double lon = mCalc.strToLatLong(charStr);
    QModelIndex idx = mModel->index(tvCoordinate->currentIndex().row(),
                                    mModel->fieldIndex("longitude"));
    mModel->setData(idx, lon);
    leLongitude->setText(mCalc.latLongToStrMin(lon, false));
}

/**
 * Temporary slot to draw scene(model)
 */
void SAIL_MapNavigationWidget::slotDrawScene(int tabIndex) {
    if (tabIndex != 0) {
        return;
    }

    mScene.clear();
    QPen pen;
    pen.setColor(QColor("#3333cc"));

    int nameIdx = mModel->fieldIndex("mname");
    int latIdx = mModel->fieldIndex("latitude"); // y
    int lonIdx = mModel->fieldIndex("longitude"); // x
    double x0, y0;
    RB_String str;
    int rowCount = mModel->rowCount(); // coordinate model

    // Draw coordinates with text
    for (int i = 0; i < rowCount; ++i) {
        x0 = 1000 * mModel->data(mModel->index(i, lonIdx)).toDouble();
        y0 = -1000 * mModel->data(mModel->index(i, latIdx)).toDouble();

        mScene.addEllipse(x0, y0, 2, 2, pen);

        str = mModel->data(mModel->index(i, nameIdx)).toString();

        QGraphicsTextItem* textItem = new QGraphicsTextItem(str);
        mScene.addItem(textItem);
        textItem->setPos(x0, y0);
    }

    // Draw route lines
    RB_String strId = mRouteModel->getCurrentId();
    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());
    SAIL_SqlCommonFunctions sqlFunc;
    sqlFunc.selectRouteCoordinates(query, strId);

    pen.setColor(QColor("#cc3333"));
    double x1, y1;
    bool isFirstCoordinate = true;

    while (query.next()) {
        x1 = 1000 * query.value(4).toDouble();
        y1 = -1000 * query.value(3).toDouble();

        if (!isFirstCoordinate) {
            QGraphicsLineItem* lineItem = new QGraphicsLineItem(x0, y0, x1, y1);
            lineItem->setPen(pen);
            mScene.addItem(lineItem);
        }

        x0 = x1;
        y0 = y1;
        isFirstCoordinate = false;
    }
}

/**
 * Slot current item in coordinate table changed, to update the latitude
 * and longitude lineEdits.
 * @param curr current index
 * @param prev previous index
 */
void SAIL_MapNavigationWidget::slotCurrentItemChanged(const QModelIndex& curr,
                                                const QModelIndex& /*prev*/) {
    if (!curr.isValid() || mAddCoordinateInProgress) {
        leLatitude->setText("");
        leLongitude->setText("");
        leLatDecimal->setText("");
        leLongDecimal->setText("");
    } else {
        slot_latitude_update();
        slot_longitude_update();
    }
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
RB_String SAIL_MapNavigationWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Close widget
 */
bool SAIL_MapNavigationWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}

/**
 * Change event, for example translation
 */
void SAIL_MapNavigationWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

