/*****************************************************************
 * $Id: peng_weldoletweldvolumewidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 15, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_weldoletweldvolumewidget.h"

#include <cmath>
#include <QTransform>

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "peng_objectfactory.h"
#include "peng_graphicsview.h"


/**
 * Constructor
 */
PENG_WeldoletWeldVolumeWidget::PENG_WeldoletWeldVolumeWidget(QWidget *parent)
                                : RB_Widget(parent) {
    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
    mParentItem = NULL;

    // Layout widgets
    //
    QGridLayout* layout = new QGridLayout(frame); //frame->setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    mGraphicsView = new PENG_GraphicsView(this);
    layout->addWidget(mGraphicsView, 0, 0, 1, 1);

    mScene = new QGraphicsScene(this);
    mGraphicsView->setRenderHint(QPainter::Antialiasing, true);
    mMatrix.setMatrix(1, 0, 0, 0, -1, 0, 200, 200, 1);
    mTextMatrix.setMatrix(1, 0, 0, 0, -1, 0, 0, 0, 1);
    mCos30 = cos(M_PI/180*30);
    mSin30 = sin(M_PI/180*30);
    mVolume = 0.0;

    // Example 3: SVG drawing border
    QSvgRenderer* mSvgBorderRend = new QSvgRenderer();
    // bool success = mSvgBorderRend->load(QString("doc/rb_a4follow-0.svg"));
    bool success = mSvgBorderRend->load(QString(":/images/rb_a4follow-0.svg"));

    if (success) {
        mSvgBorderItem = new QGraphicsSvgItem();
        mSvgBorderItem->setSharedRenderer(mSvgBorderRend);
        mScene->addItem(mSvgBorderItem);
        mSvgBorderItem->setPos(QPointF(0.0, 0.0));

        QRectF bRect = mSvgBorderItem->boundingRect();
        QTransform m = mSvgBorderItem->transform();
        m.scale(210/bRect.width(), 297/bRect.height());
        mSvgBorderItem->setTransform(m, false);
    } else {
        RB_DEBUG->error("PENG_WeldoletWeldVolumeWidget"
                        "::PENG_WeldoletWeldVolumeWidget() 1 ERROR");
    }

    QSvgRenderer* mSvgLegendRend = new QSvgRenderer();
    // success = mSvgLegendRend->load(QString("doc/weldolet_legend_bw.svg"));
    success = mSvgLegendRend->load(QString(":/images/weldolet_legend_bw.svg"));

    if (success) {
        mSvgLegendItem = new QGraphicsSvgItem();
        mSvgLegendItem->setSharedRenderer(mSvgLegendRend);
        mScene->addItem(mSvgLegendItem);
        mSvgLegendItem->setPos(QPointF(0.0, 0.0));

        QRectF bRect = mSvgLegendItem->boundingRect();
        QTransform m = mSvgLegendItem->transform();
        m.scale(50/bRect.width(), 80/bRect.height());
        mSvgLegendItem->setTransform(m, false);
    } else {
        RB_DEBUG->error("PENG_WeldoletWeldVolumeWidget"
                        "::PENG_WeldoletWeldVolumeWidget() 2 ERROR");
    }

//    drawParentItemOrigine();
//    drawSceneOrigine();
    mGraphicsView->setScene(mScene);
    mGraphicsView->fitInView(mScene->sceneRect(), Qt::KeepAspectRatio);

    // Example 2: drawing background, green error and size
//    bool success = mSvgBorderRend.load(QString("doc/rb_a4cover-0.svg"));
//    if (success) {
//        QPixmap bgPixmap(210, 297);
//        QPainter painter(&bgPixmap);
//        mSvgBorderRend.render(&painter);
//        mScene->setBackgroundBrush(QBrush(bgPixmap));
//    }

    // Example 1: prepare background check-board pattern
//    QPixmap tilePixmap(64, 64);
//    tilePixmap.fill(Qt::white);
//    QPainter tilePainter(&tilePixmap);
//    QColor color(220, 220, 220);
//    tilePainter.fillRect(0, 0, 32, 32, color);
//    tilePainter.fillRect(32, 32, 32, 32, color);
//    tilePainter.end();
//    mGraphicsView->setBackgroundBrush(tilePixmap);



/* Example
    mAlpha1 = 9.0;
    mAlpha2 = 50.0;
    mAlpha3 = 32.0; // degree
    mD0 = 1016.0;
    mD2 = 400.0;
    mD21 = 391.02;
    mD3 = 311.3;
    mGap = 1.6;
    mGap1 = 3.0;
    mH3 = (mD2/2 - mD3/2 - mGap) * tan(M_PI / 180 * mAlpha2);
    mSteps = 32;
    mTextOutput = true;
    mCurrentZoomFactor = 1.0;

    leD0->setText(QString::number(mD0));
    leD2->setText(QString::number(mD2));
    leD21->setText(QString::number(mD21));
    leD3->setText(QString::number(mD3));
    leAlpha1->setText(QString::number(mAlpha1));
    leAlpha3->setText(QString::number(mAlpha3));
    leGap->setText(QString::number(mGap));
    leGap1->setText(QString::number(mGap1));
    leAlpha2->setText(QString::number(mAlpha2));
    leH3->setText(QString::number(mH3));
    // leSteps->setText(QString::number(mSteps));
    QStringList lst;
    lst << "32" << "64" << "128";
    cbSteps->addItems(lst);
*/
    mSizeHint = QSize(900, 500);
    QList<int> intList;
    intList << 165 << 735;
    splitter->setSizes(intList);
}

/**
 * Destructor
 */
PENG_WeldoletWeldVolumeWidget::~PENG_WeldoletWeldVolumeWidget() {
    // delete mMapper;
    delete mModel;
}

RB_String PENG_WeldoletWeldVolumeWidget::getSaveAsFileName() {
    return "peng_weldoletweldvolume_" + mModel->getCurrentValue("number").toString();
}

void PENG_WeldoletWeldVolumeWidget::init() {
    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEngWeldolet);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(this->leNumber, mModel->fieldIndex("number"));
    mMapper->addMapping(this->leDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(this->leRevision,mModel->fieldIndex("revision"));
    mMapper->addMapping(this->leAlpha1, mModel->fieldIndex("alpha1"));
    mMapper->addMapping(this->leAlpha2, mModel->fieldIndex("alpha2"));
    mMapper->addMapping(this->leAlpha3, mModel->fieldIndex("alpha3"));
    mMapper->addMapping(this->leD0, mModel->fieldIndex("D0"));
    mMapper->addMapping(this->leD2, mModel->fieldIndex("D2"));
    mMapper->addMapping(this->leD21, mModel->fieldIndex("D21"));
    mMapper->addMapping(this->leD3, mModel->fieldIndex("D3"));
    mMapper->addMapping(this->leGap, mModel->fieldIndex("gap"));
    mMapper->addMapping(this->leGap1, mModel->fieldIndex("gap1"));
//    mMapper->addMapping(this->leH3, mModel->fieldIndex("H3"));
    RB_StringList items;
    items << "32" << "64" << "128";
    cbSteps->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbSteps, mModel->fieldIndex("steps"),
                            "currentIndex");

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvWeldolet, mModel);
    readSettings();

    int countCol = mModel->columnCount(QModelIndex());
    for(int i = 0; i < countCol; i++) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvWeldolet->hideColumn(i);
        }
    }

    leProject->setText(PENG_MODELFACTORY->getRoot()->getValue("number").toString());

    connect(leAlpha2, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateH3()));
    connect(leGap, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateH3()));
    connect(leD2, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateH3()));
    connect(leD3, SIGNAL(editingFinished()),
            this, SLOT(slotUpdateH3()));
    connect(tvWeldolet, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotUpdateH3(QModelIndex)));
    connect(mModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotClearVolume(QModelIndex)));

}

/**
 * Menu file save
 */
bool PENG_WeldoletWeldVolumeWidget::fileSave(bool /*withSelect*/) {
    bool success = mModel->submitAllAndSelect();

    setWindowModified(false);
    return success;
}

/**
 * Menu file save as, not applicable for this widget
 */
bool PENG_WeldoletWeldVolumeWidget::fileSaveAs() {
    PENG_DIALOGFACTORY->requestWarningDialog(tr("Menu File Save As is\n"
                                               "not applicable for this window"));
    return false;
}

/**
 * Menu file PDF
 */
void PENG_WeldoletWeldVolumeWidget::filePdf(QPrinter* pr) {
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_WeldoletWeldVolumeWidget::filePrint(QPrinter* pr) {
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_WeldoletWeldVolumeWidget::filePrintPreview(QPrinter* pr) {
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * @returns Help subject
 */
RB_String PENG_WeldoletWeldVolumeWidget::getHelpSubject() const {
    return objectName();
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_WeldoletWeldVolumeWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the end
    int row = mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("number"));
    mModel->setData(index, "<NEW>", Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("D2"));
//    mModel->setData(index, 0.0, Qt::EditRole);  // works only if value is not 0 or 0.0
//    index = mModel->index(row, mModel->fieldIndex("D21"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("D3"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("gap"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("gap1"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("H3"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("alpha1"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("alpha2"));
//    mModel->setData(index, 0.0, Qt::EditRole);
//    index = mModel->index(row, mModel->fieldIndex("alpha3"));
//    mModel->setData(index, 0.0, Qt::EditRole);
    // end NOTE

    tvWeldolet->setCurrentIndex(mModel->index(row, mModel->fieldIndex("number"),
                                              QModelIndex()));
    tvWeldolet->scrollTo(tvWeldolet->currentIndex());
    leNumber->setFocus();
    leNumber->selectAll();
    leVolume->clear();

    // HACK: do not know why numeric fields do not clear
    leRevision->setText("0");
    leAlpha1->setText("0");
    leAlpha2->setText("0");
    leAlpha3->setText("0");
    leD0->setText("0");
    leD2->setText("0");
    leD21->setText("0");
    leD3->setText("0");
    leGap->setText("0");
    leGap1->setText("0");
    leH3->setText("0");
}

/**
 * On pushbutton delete weldolet clicked.
 */
void PENG_WeldoletWeldVolumeWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    QModelIndex index = tvWeldolet->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button save clicked
 */
void PENG_WeldoletWeldVolumeWidget::on_pbSave_clicked() {
    fileSave(true);
}

void PENG_WeldoletWeldVolumeWidget::on_pbCalcVol_clicked() {
    if (!mModel || !mSvgBorderItem) return;

    QModelIndex index = tvWeldolet->currentIndex();
    if (!index.isValid()) {
        leVolume->setText(tr("No weldolet selected"));
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    slotUpdateH3();

    // to reset the scenerectangle
    if (mScene) {
        if (mParentItem) {
            mScene->removeItem(mParentItem);
            delete mParentItem;
            mParentItem = NULL;
        }

        if (mSvgBorderItem) {
            mScene->removeItem(mSvgBorderItem);
        }

        if (mSvgLegendItem) {
            mScene->removeItem(mSvgLegendItem);
        }
        delete mScene;
    }
    mScene = new QGraphicsScene(this);
    mGraphicsView->setScene(mScene);
    mParentItem = new QGraphicsRectItem();
    mParentItem->setRect(0, 0, 100, 100);
    mParentItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    mParentItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    QPen pen(QBrush(Qt::white), 1.0);
    mParentItem->setPen(pen);
    mVolume = 0.0;

    calculateVolume(); // will set mVolume

    drawText3D(QString::number(mVolume) + " mm3", 0, 0, 0);

    mScene->addItem(mParentItem);
    fitInDrawingArea(QRectF(35.0, 30.0, 150.0, 150.0), mParentItem);

    mScene->addItem(mSvgBorderItem);
    mSvgBorderItem->setPos(QPointF(0.0, 0.0));

    mScene->addItem(mSvgLegendItem);
    fitInLegendArea(QRectF(35.0, 190.0, 100.0, 80.0), mSvgLegendItem);

    // Values
    double lineHeight = 4.0;
    int i = 0;
    QString str = tr("Weld Volume = ");
    str += QString::number(mVolume, 'E', 5) + " mm3";
    drawSceneText(str, 120.0, 190.0, QPen(QBrush(Qt::black), 1.0));
    str = "D0 = " + QString::number(mD0) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "D2 = " + QString::number(mD2) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "D21 = " + QString::number(mD21) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "D3 = " + QString::number(mD3) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "H3 = " + QString::number(mH3) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Alpha1 = " + QString::number(mAlpha1) + " degree";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Alpha2 = " + QString::number(mAlpha2) + " degree";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Alpha3 = " + QString::number(mAlpha3) + " degree";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Gap = " + QString::number(mGap) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Gap1 = " + QString::number(mGap1) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));

    // Title block
    double paper_y = 274.0;
    str = PENG_MODELFACTORY->getRoot()->getValue("company").toString();
    drawSceneText(str, 60.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    QModelIndex idx = tvWeldolet->currentIndex();
    str = mModel->data(mModel->index(idx.row(), mModel->fieldIndex("revision"))).toString();
    drawSceneText(str, 115.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    str = "1 of 1";
    drawSceneText(str, 130.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    str = "PENG-WWV-";
    str += PENG_MODELFACTORY->getRoot()->getValue("number").toString() + "-";
    str += mModel->data(mModel->index(idx.row(), mModel->fieldIndex("number"))).toString();
    drawSceneText(str, 150.0, paper_y, QPen(QBrush(Qt::black), 1.0));


//    drawParentItemOrigine();
//    drawSceneOrigine();

    // Fit in viewport
    mGraphicsView->fitInView(mScene->sceneRect(), Qt::KeepAspectRatio);
    leVolume->setText(QString::number(mVolume, 'E', 6));
    QApplication::restoreOverrideCursor();
}

/**
 * Update value H3, triggered by leAlpha2
 */
void PENG_WeldoletWeldVolumeWidget::slotUpdateH3() {
    mSteps = cbSteps->currentText().toDouble();
    mD0 = leD0->text().toDouble();
    mD2 = leD2->text().toDouble();
    mD3 = leD3->text().toDouble();
    mGap = leGap->text().toDouble();
    mGap1 = leGap1->text().toDouble();
    mAlpha1 = leAlpha1->text().toDouble(); // degree
    mAlpha2 = leAlpha2->text().toDouble(); // degree
    mAlpha3 = leAlpha3->text().toDouble(); // degree

    mH3 = (mD2/2 - mD3/2 - mGap) * tan(M_PI / 180 * mAlpha2);
    leH3->setText(QString::number(mH3));

    mD21 = leD21->text().toDouble();
}

/**
 * Wrapper
 */
void PENG_WeldoletWeldVolumeWidget::slotUpdateH3(const QModelIndex&) {
    // collect data of mAlpha2 and other and calculate here as per slotUpdateH3()
    slotUpdateH3();
}

void PENG_WeldoletWeldVolumeWidget::slotClearVolume(const QModelIndex &) {
    leVolume->clear();
}

void PENG_WeldoletWeldVolumeWidget::drawBlueLine3D(double x1, double y1, double z1,
                                double x2, double y2, double z2) {
    drawBlueLine(mCos30 * x1 - mCos30 * y1, mSin30 * x1 + mSin30 * y1 + z1,
                 mCos30 * x2 - mCos30 * y2, mSin30 * x2 + mSin30 * y2 + z2);
}

void PENG_WeldoletWeldVolumeWidget::drawGreenLine3D(double x1, double y1, double z1,
                                 double x2, double y2, double z2) {
    drawGreenLine(mCos30 * x1 - mCos30 * y1, mSin30 * x1 + mSin30 * y1 + z1,
                 mCos30 * x2 - mCos30 * y2, mSin30 * x2 + mSin30 * y2 + z2);
}

void PENG_WeldoletWeldVolumeWidget::drawRedLine3D(double x1, double y1, double z1,
                               double x2, double y2, double z2) {
    drawRedLine(mCos30 * x1 - mCos30 * y1, mSin30 * x1 + mSin30 * y1 + z1,
                 mCos30 * x2 - mCos30 * y2, mSin30 * x2 + mSin30 * y2 + z2);
}

void PENG_WeldoletWeldVolumeWidget::drawBlueLine(double x1, double y1,
                                                double x2, double y2) {
    QPen pen(QBrush(Qt::darkBlue), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_WeldoletWeldVolumeWidget::drawGreenLine(double x1, double y1,
                                                 double x2, double y2) {
    QPen pen(QBrush(Qt::darkGreen), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_WeldoletWeldVolumeWidget::drawRedLine(double x1, double y1,
                                               double x2, double y2) {
    QPen pen(QBrush(Qt::red), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_WeldoletWeldVolumeWidget::drawLine(double x1, double y1, double x2,
                                            double y2, const QPen& p) {
    QGraphicsLineItem* l = new QGraphicsLineItem(x1, y1, x2, y2, mParentItem);
    l->setPen(p);
//    l->setTransform(mMatrix);
//    l->setParentItem(mParentItem);
//    mScene->addItem(l);
    if (mParentItem) {
        mParentItem->setRect(mParentItem->rect().united(l->boundingRect()));
    } else if (mScene) {
        mScene->addItem(l);
    }
}

void PENG_WeldoletWeldVolumeWidget::drawSceneLine(double x1, double y1, double x2,
                                            double y2, const QPen& p) {
    if (!mScene) return;
    QGraphicsLineItem* l = new QGraphicsLineItem(x1, y1, x2, y2);
    l->setPen(p);
    mScene->addItem(l);
}

void PENG_WeldoletWeldVolumeWidget::drawText3D(const QString& str, double x1,
                                              double y1, double z1) {
    drawText(str, mCos30 * x1 - mCos30 * y1, mSin30 * x1 + mSin30 * y1 + z1,
             QPen(QBrush(Qt::red), 1.0));
}

void PENG_WeldoletWeldVolumeWidget::drawText(const QString& str, double x,
                                            double y, const QPen& /* p */) {
    QGraphicsTextItem* t = new QGraphicsTextItem(str, mParentItem);
//    t->setPos(x, -y);
//    t->setTransform(mTextMatrix);
//    t->setParentItem(mParentItem);
//    mScene->addItem(t);
    t->setPos(x, y);
    t->setTransform(mTextMatrix);
    if (mParentItem) {
        mParentItem->setRect(mParentItem->rect().united(t->boundingRect()));
    } else if (mScene) {
        mScene->addItem(t);
    }
}

void PENG_WeldoletWeldVolumeWidget::drawSceneText(const QString& str, double x,
                                            double y, const QPen& p) {
    QGraphicsTextItem* t = new QGraphicsTextItem(str);
    t->setPos(x, y);
    t->setScale(0.3);
    t->setDefaultTextColor(p.color());
    mScene->addItem(t);
}

void PENG_WeldoletWeldVolumeWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void PENG_WeldoletWeldVolumeWidget::calculateVolume() {
    if (!mModel) return;

    QModelIndex index = tvWeldolet->currentIndex();
    if (!index.isValid()) {
        return;
    }

    double volume = 0.0;

    // 3D Axis
    drawAxes(1.0);

    // Get input data
    mSteps = cbSteps->currentText().toDouble();
    double D0 = leD0->text().toDouble();
    double D2 = leD2->text().toDouble();
    double D3 = leD3->text().toDouble();
    double gap = leGap->text().toDouble();
    double gap1 = leGap1->text().toDouble();
    double alpha1 = leAlpha1->text().toDouble(); // degree
    double alpha2 = leAlpha2->text().toDouble(); // degree
    double alpha3 = leAlpha3->text().toDouble(); // degree
    double H3 = (D2/2 - D3/2 - gap) * tan(M_PI / 180 * alpha2);
    double D21 = leD21->text().toDouble();


    // Curve
    // mSteps is over 360 degrees
    int steps34 = mSteps * 3 / 4;
    double deltaAngle = 2 * M_PI / mSteps;
    double currentAngle = 0.0;

    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    double x5, y5, z5, x6, y6, z6, x7, y7, z7, x8, y8, z8;
    x1 = y1 = z1 = x2 = y2 = z2 = x3 = y3 = z3 = x4 = y4 = z4 = 0.0;
    x5 = y5 = z5 = x6 = y6 = z6 = x7 = y7 = z7 = x8 = y8 = z8 = 0.0;

    // Validate input 1
    QString isInputError = "";

    if (D0 <=0) isInputError += tr("\nD0 should be larger than zero");
    if (D3 <= 0) isInputError += tr("\nD3 should be larger than zero");
    if (D21 < D3) isInputError += tr("\nD21 should be larger or equal D3");
    if (D2 < D21) isInputError += tr("\nD2 should be larger or equal D21");
    if (gap < 0) isInputError += tr("\ngap should be larger than zero");
    if (gap1 < 0) isInputError += tr("\ngap1 should be larger than zero");
    if (alpha1 < 0 || alpha1 > 50) isInputError += tr("\nalpha1 should be between 0 and 50");
    if (alpha2 < 0 || alpha2 > 70) isInputError += tr("\nalpha2 should be between 0 and 70");
    if (alpha3 < 0 || alpha3 > 70) isInputError += tr("\nalpha3 should be between 0 and 70");
    if ((D0/2 + gap1) <=0) isInputError += tr("\nD0/2+gap1 should be larger than zero");

    if (isInputError != "") {
        QApplication::restoreOverrideCursor();
        QString errorStr = tr("Input error:");
        errorStr += isInputError;
        PENG_DIALOGFACTORY->requestWarningDialog(errorStr);
        return;
    }

    double R0 = D0 / 2;
    double R1 = D3 / 2;
    double R1gap = R1 + gap;
    double R1gapBev = D2 / 2;
    double L1 = R1gapBev + H3 * tan(M_PI / 180 * alpha1);
    double H3deg = H3 + gap1;
    double H4minusH3 = (D21/2 - D3/2 - gap) * tan(M_PI / 180 * alpha3)
                       + ((D0/2 + gap1) * cos(asin((D3/2+gap)/(D0/2 + gap1)))
                          - (D0/2)*cos(asin((D21/2)/(D0/2))))
                       - H3 - gap1;

    double corr = 1.0; // correction from 1 to 0 in 270 to 360 degrees

    // Validate input 2
    if (L1 > R0) {
        QApplication::restoreOverrideCursor();
        PENG_DIALOGFACTORY->requestWarningDialog(tr("Input error:\n"
                                                   "Weld footprint is outside D0.\n"
                                                   "Check D0, D3, D2, alpha1 and 2."));
        return;
    }

    // Start drawing outlines
    //

    // Performance only
    double sinCurrentAngle = 0.0;
    double cosCurrentAngle = 0.0;

    // D3 curve on run pipe and D3 + gap1
    for (int i = 0; i < steps34; ++i) {
        sinCurrentAngle = sin(currentAngle);
        cosCurrentAngle = cos(currentAngle);

        // D3 on run pipe
        //
        x1 = R1 * cosCurrentAngle;
        y1 = R1 * sinCurrentAngle;
        z1 = R0 * cos(asin((R1 * sinCurrentAngle) / R0));
        x5 = x1;
        y5 = y1;
        z5 = z1 + gap1;

        currentAngle += deltaAngle;
        sinCurrentAngle = sin(currentAngle);
        cosCurrentAngle = cos(currentAngle);

        // D3 on pipe
        //
        x4 = R1 * cosCurrentAngle;
        y4 = R1 * sinCurrentAngle;
        z4 = R0 * cos(asin((R1 * sinCurrentAngle) / R0));
        x8 = x4;
        y8 = y4;
        z8 = z4 + gap1;

        // tangential lines
        //
        drawBlueLine3D(x1, y1, z1, x4, y4, z4);
        drawBlueLine3D(x5, y5, z5, x8, y8, z8);
    }

    currentAngle = 0.0;

    // corrections for D2 -> D21, H3 -> H4' and L1 -> L2
    //
    corr = (1 - cos(2*currentAngle))/2;
    R1gapBev = D2/2 - (D2 - D21) * corr/2;
    H3deg = H3 + gap1 + H4minusH3 * corr;
    L1 = R1gapBev + H3deg * tan(M_PI / 180 * alpha1);

    // D3 + gap + bevel on run pipe and D3 + gap + bevel at weldolet
    for (int i = 0; i < steps34; ++i) {
        sinCurrentAngle = sin(currentAngle);
        cosCurrentAngle = cos(currentAngle);

        // D3 + gap + bevel on run pipe
        //
        x2 = L1 * cosCurrentAngle;
        y2 = L1 * sinCurrentAngle;
        z2 = R0 * cos(asin((L1 * sinCurrentAngle) / R0));
        x6 = R1gapBev * cosCurrentAngle;
        y6 = R1gapBev * sinCurrentAngle;
        z6 = R0 * cos(asin((R1gapBev * sinCurrentAngle) / R0)) + H3deg;

        currentAngle += deltaAngle;
        sinCurrentAngle = sin(currentAngle);
        cosCurrentAngle = cos(currentAngle);

        // corrections for D2 -> D21, H3 -> H4' and L1 -> L2
        //
        corr = (1 - cos(2*currentAngle))/2;
        R1gapBev = D2/2 - (D2 - D21) * corr/2;
        H3deg = H3 + gap1 + H4minusH3 * corr;
        L1 = R1gapBev + H3deg * tan(M_PI / 180 * alpha1);

        // D3 + gap + bevel on run pipe
        //
        x3 = L1 * cosCurrentAngle;
        y3 = L1 * sinCurrentAngle;
        z3 = R0 * cos(asin((L1 * sinCurrentAngle) / R0));
        x7 = R1gapBev * cosCurrentAngle;
        y7 = R1gapBev * sinCurrentAngle;
        z7 = R0 * cos(asin((R1gapBev * sinCurrentAngle) / R0)) + H3deg;

        // tangential lines
        //
        drawRedLine3D(x2, y2, z2, x3, y3, z3);
        drawBlueLine3D(x6, y6, z6, x7, y7, z7);
    }

    //
    // End drawing outlines

    currentAngle = M_PI * 1.5;

    // gap part calculation and drawing
    //
    for (int i = 0; i < mSteps/4; ++i) {

        // D3 on run pipe
        //
        x1 = R1 * cos(currentAngle);
        y1 = R1 * sin(currentAngle);
        z1 = R0 * cos(asin((R1 * sin(currentAngle)) / R0));
        x5 = x1;
        y5 = y1;
        z5 = z1 + gap1;

        // D3 + gap on run pipe
        //
        x2 = R1gap * cos(currentAngle);
        y2 = R1gap * sin(currentAngle);
        z2 = R0 * cos(asin((R1gap * sin(currentAngle)) / R0));
        x6 = x2;
        y6 = y2;
        z6 = z2 + gap1;

        currentAngle += deltaAngle;

        // D3 on pipe
        //
        x4 = R1 * cos(currentAngle);
        y4 = R1 * sin(currentAngle);
        z4 = R0 * cos(asin((R1 * sin(currentAngle)) / R0));
        x8 = x4;
        y8 = y4;
        z8 = z4 + gap1;

        // D3 + gap on run pipe
        //
        x3 = R1gap * cos(currentAngle);
        y3 = R1gap * sin(currentAngle);
        z3 = R0 * cos(asin((R1gap * sin(currentAngle)) / R0));
        x7 = x3;
        y7 = y3;
        z7 = z3 + gap1;

        // tangential lines
        //
        drawBlueLine3D(x1, y1, z1, x4, y4, z4);
        drawBlueLine3D(x5, y5, z5, x8, y8, z8);
        drawBlueLine3D(x2, y2, z2, x3, y3, z3);
        drawBlueLine3D(x6, y6, z6, x7, y7, z7);

        // z direction lines
        //
        if (i == 0) {
            drawBlueLine3D(x1, y1, z1, x5, y5, z5);
            drawBlueLine3D(x2, y2, z2, x6, y6, z6);
        }
        drawBlueLine3D(x4, y4, z4, x8, y8, z8);
        drawBlueLine3D(x3, y3, z3, x7, y7, z7);

        // radial lines
        //
        if (i == 0) {
            drawBlueLine3D(x1, y1, z1, x2, y2, z2);
            drawBlueLine3D(x5, y5, z5, x6, y6, z6);
        }
        drawBlueLine3D(x7, y7, z7, x8, y8, z8);
        drawBlueLine3D(x3, y3, z3, x4, y4, z4);

        // Calculate volume
        //
        mP1.set(x1, y1, z1);
        mP2.set(x2, y2, z2);
        mP3.set(x3, y3, z3);
        mP4.set(x4, y4, z4);
        mP5.set(x5, y5, z5);
        mP6.set(x6, y6, z6);
        mP7.set(x7, y7, z7);
        mP8.set(x8, y8, z8);
        volume += calcCubeVolume(mTextOutput);
    }

    currentAngle = M_PI * 1.5;

    // corrections for D2 -> D21, H3 -> H4' and L1 -> L2
    //
    corr = (1 - cos(2*currentAngle))/2;
    R1gapBev = D2/2 - (D2 - D21) * corr/2;
    H3deg = H3 + gap1 + H4minusH3 * corr;
    L1 = R1gapBev + H3deg * tan(M_PI / 180 * alpha1);

    // beveled part calculation and drawing
    //
    for (int i = 0; i < mSteps/4; ++i) {
        // D3 + gap on run pipe
        //
        x1 = R1gap * cos(currentAngle);
        y1 = R1gap * sin(currentAngle);
        z1 = R0 * cos(asin((R1gap * sin(currentAngle)) / R0));
        x5 = x1;
        y5 = y1;
        z5 = z1 + gap1;

        // D3 + gap + bevel on run pipe
        //
        x2 = L1 * cos(currentAngle);
        y2 = L1 * sin(currentAngle);
        z2 = R0 * cos(asin((L1 * sin(currentAngle)) / R0));
        x6 = R1gapBev * cos(currentAngle);
        y6 = R1gapBev * sin(currentAngle);
        z6 = R0 * cos(asin((R1gapBev * sin(currentAngle)) / R0)) + H3deg;

        currentAngle += deltaAngle;

        // corrections for D2 -> D21, H3 -> H4' and L1 -> L2
        //
        corr = (1 - cos(2*currentAngle))/2;
        R1gapBev = D2/2 - (D2 - D21) * corr/2;
        H3deg = H3 + gap1 + H4minusH3 * corr;
        L1 = R1gapBev + H3deg * tan(M_PI / 180 * alpha1);

        // D3 + gap on run pipe
        //
        x4 = R1gap * cos(currentAngle);
        y4 = R1gap * sin(currentAngle);
        z4 = R0 * cos(asin((R1gap * sin(currentAngle)) / R0));
        x8 = x4;
        y8 = y4;
        z8 = z4 + gap1;

        // D3 + gap + bevel on run pipe
        //
        x3 = L1 * cos(currentAngle);
        y3 = L1 * sin(currentAngle);
        z3 = R0 * cos(asin((L1 * sin(currentAngle)) / R0));
        x7 = R1gapBev * cos(currentAngle);
        y7 = R1gapBev * sin(currentAngle);
        z7 = R0 * cos(asin((R1gapBev * sin(currentAngle)) / R0)) + H3deg;

        // tangential lines
        //
        // drawBlueLine3D(x1, y1, z1, x4, y4, z4); // otherwise drawn twice
        // drawBlueLine3D(x5, y5, z5, x8, y8, z8); // otherwise drawn twice
        drawRedLine3D(x2, y2, z2, x3, y3, z3);
        drawBlueLine3D(x6, y6, z6, x7, y7, z7);

        // z direction lines
        //
        if (i == 0) {
            // drawBlueLine3D(x1, y1, z1, x5, y5, z5); otherwise drawn twice
            drawBlueLine3D(x2, y2, z2, x6, y6, z6);
        }
        // drawBlueLine3D(x4, y4, z4, x8, y8, z8); otherwise drawn twice
        drawGreenLine3D(x3, y3, z3, x7, y7, z7);

        // radial lines
        //
        if (i == 0) {
            drawBlueLine3D(x1, y1, z1, x2, y2, z2);
            drawBlueLine3D(x5, y5, z5, x6, y6, z6);
        }
        drawBlueLine3D(x7, y7, z7, x8, y8, z8);
        drawGreenLine3D(x3, y3, z3, x4, y4, z4);

        // Calculate volume
        //
        mP1.set(x1, y1, z1);
        mP2.set(x2, y2, z2);
        mP3.set(x3, y3, z3);
        mP4.set(x4, y4, z4);
        mP5.set(x5, y5, z5);
        mP6.set(x6, y6, z6);
        mP7.set(x7, y7, z7);
        mP8.set(x8, y8, z8);
        volume += calcCubeVolume(mTextOutput);
    }

    mVolume = volume * 4.0; // times 4 because only a quarter is calculated
}

/**
 * Calculate the volume of one cube volume
 */
double PENG_WeldoletWeldVolumeWidget::calcCubeVolume(bool /* textOutput */) {
    // Cumulative volume
    double cumVolume = 0.0;

    // 1236
    PENG_VectorSolutions vs(mP1, mP2, mP3, mP6);
    cumVolume += vs.getTetrahedronVolume();

    //1367
    vs.set(1, mP3);
    vs.set(2, mP6);
    vs.set(3, mP7);
    cumVolume += vs.getTetrahedronVolume();

    //1347
    vs.set(2, mP4);
    cumVolume += vs.getTetrahedronVolume();

    //1348
    vs.set(3, mP8);
    cumVolume += vs.getTetrahedronVolume();

    //1567
    vs.set(1, mP5);
    vs.set(2, mP6);
    vs.set(3, mP7);
    cumVolume += vs.getTetrahedronVolume();

    //1578
    vs.set(2, mP7);
    vs.set(3, mP8);
    cumVolume += vs.getTetrahedronVolume();
    return cumVolume;
}

/**
 * Draw X, Y and Z axis
 * @param f scaling factor
 */
void PENG_WeldoletWeldVolumeWidget::drawAxes(double f) {
    drawGreenLine3D(0, 0, 0, 100 * f, 0, 0); // x-axis
    drawGreenLine3D(90 * f, 4 * f, 0, 100 * f, 0, 0);
    drawGreenLine3D(90 * f, -4 * f, 0, 100 * f, 0, 0);
    drawGreenLine3D(90 * f, 4 * f, 0, 90 * f, -4 * f, 0);
    drawText3D("X", 105 * f, 0, 10 * f);
    drawGreenLine3D(0, 0, 0, 0, 100 * f, 0); // y-axis
    drawGreenLine3D(4 * f, 90 * f, 0, 0, 100 * f, 0);
    drawGreenLine3D(-4 * f, 90 * f, 0, 0, 100 * f, 0);
    drawGreenLine3D(4 * f, 90 * f, 0,  -4 * f, 90 * f, 0);
    drawText3D("Y", 0, 115 * f, 5 * f);
    drawGreenLine3D(0, 0, 0, 0, 0, 100 * f); // z-axis
    drawGreenLine3D(4 * f, 0, 90 * f, 0, 0, 100 * f);
    drawGreenLine3D(-4 * f, 0, 90 * f, 0, 0, 100 * f);
    drawGreenLine3D(4 * f, 0, 90 * f, -4 * f, 0, 90 * f);
    drawText3D("Z", 0, 7 * f, 115 * f);
}

/**
 * Draw origine, for testing purposes only
 * @param f scaling factor
 */
void PENG_WeldoletWeldVolumeWidget::drawParentItemOrigine(double f) {
    QPen pen(QBrush(Qt::black), 1.0 * f);
    drawLine(-10.0 * f, 0.0, 10.0 * f, 0.0, pen);
    drawLine(0.0, -10.0 * f, 0.0, 10.0 * f, pen);
}

/**
 * Draw origine, for testing purposes only
 * @param f scaling factor
 */
void PENG_WeldoletWeldVolumeWidget::drawSceneOrigine(double f) {
    QPen pen(QBrush(Qt::blue), 1.0 * f);
    drawSceneLine(-20.0 * f, 0.0, 0.0 * f, 0.0, pen);
    drawSceneLine(0.0, -10.0 * f, 0.0, 10.0 * f, pen);
}

/**
 * Fit item in drawing area
 * @param dwgArea drawing area
 * @param item rectangle item, usualy parent item of group of drawing items
 */
void PENG_WeldoletWeldVolumeWidget::fitInDrawingArea(const QRectF& dwgArea,
                                                    QGraphicsItem* item) {
    // Scale
    double factor = dwgArea.width() / item->boundingRect().width();
    double testFactor = dwgArea.height() / item->boundingRect().height();
    factor = factor < testFactor ? factor : testFactor;
//    item->setScale(factor);
    // Mirror y coordinates
//    QTransform matrix(1, 0, 0, 0, -1, 0, 0, 0, 1);
//    item->setTransform(matrix, false);
    // Position
//    item->setPos(110.0, 180.0);

    QTransform matrix(factor, 0, 0, 0, -factor, 0, 110, 180, 1);
    item->setTransform(matrix, false);

}

/**
 * Fit item in drawing area
 * @param dwgArea drawing area
 * @param item rectangle item, usualy parent item of group of drawing items
 */
void PENG_WeldoletWeldVolumeWidget::fitInLegendArea(const QRectF& dwgArea,
                                                    QGraphicsItem* item) {
    // Scale
    double factor = dwgArea.width() / item->boundingRect().width();
    double testFactor = dwgArea.height() / item->boundingRect().height();
    factor = factor < testFactor ? factor : testFactor;
    // Create matrix with position
    QTransform matrix(factor, 0, 0, 0, factor, 0, dwgArea.x(), dwgArea.y(), 1);
    item->setTransform(matrix, false);
}

void PENG_WeldoletWeldVolumeWidget::zoom(double /*factor*/) {
//    qreal scale = mCurrentZoomFactor * factor;
//    // qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));
//
//    QMatrix matrix;
//    matrix.scale(scale, scale);
//    mGraphicsView->setMatrix(matrix);
}

void PENG_WeldoletWeldVolumeWidget::resetView() {
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));
}

bool PENG_WeldoletWeldVolumeWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}


