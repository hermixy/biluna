/*****************************************************************
 * $Id: peng_conestrikeoutwidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 15, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_conestrikeoutwidget.h"

#include <cmath>
#include <QTransform>

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "peng_objectfactory.h"
#include "peng_graphicsview.h"


/**
 * Constructor
 */
PENG_ConeStrikeOutWidget::PENG_ConeStrikeOutWidget(QWidget *parent)
                                : RB_Widget(parent) {
    setupUi(this);

    mD0 = 0.0;
    mD1 = 0.0;
//    mt0 = 0.0;
//    mt1 = 0.0;
    mH0 = 0.0;
    mArea = 0.0;
    mRadius0 = 0.0;
    mRadius1 = 0.0;
    mBeta = 0.0;
    mOverallHeight = 0.0;
    mOverallWidth = 0.0;
    mEdgeRadius1 = 0.0;

    mModel = NULL;
    mMapper = NULL;
    mParentItem = NULL;

    // Layout widgets
    //
    QGridLayout* layout = new QGridLayout(frame);
    layout->setMargin(0);
    layout->setSpacing(0);
    mGraphicsView = new PENG_GraphicsView(this);
    layout->addWidget(mGraphicsView, 0, 0, 1, 1);

    mScene = new QGraphicsScene(this);
    mGraphicsView->setRenderHint(QPainter::Antialiasing, true);
    mMatrix.setMatrix(1, 0, 0, 0, -1, 0, 200, 200, 1);
    mTextMatrix.setMatrix(1, 0, 0, 0, -1, 0, 0, 0, 1);
    mArea = 0.0;

    // Example 3: SVG drawing border
    QSvgRenderer* mSvgBorderRend = new QSvgRenderer();
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
        RB_DEBUG->error("PENG_ConeStrikeOutWidget"
                        "::PENG_ConeStrikeOutWidget() 1 ERROR");
    }

    QSvgRenderer* svgLegendRend = new QSvgRenderer();
    success = svgLegendRend->load(QString(":/images/cone_legend_bw.svg"));

    if (success) {
        mSvgLegendItem = new QGraphicsSvgItem();
        mSvgLegendItem->setSharedRenderer(svgLegendRend);
        mScene->addItem(mSvgLegendItem);
        mSvgLegendItem->setPos(QPointF(10.0, -10.0));

        QRectF bRect = mSvgLegendItem->boundingRect();
        QTransform m = mSvgLegendItem->transform();
        m.scale(50/bRect.width(), 80/bRect.height());
        mSvgLegendItem->setTransform(m, false);
    } else {
        RB_DEBUG->error("PENG_ConeStrikeOutWidget"
                        "::PENG_ConeStrikeOutWidget() 2 ERROR");
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
    mD0 = 219.1;
    mD1 = 386.0;
    mH0 = 215.0;
    mTextOutput = true;
    mCurrentZoomFactor = 1.0;

    leD0->setText(QString::number(mD0));
    leD1->setText(QString::number(mD1));
    leH0->setText(QString::number(mH0));
    leTol0->setText(QString::number(mTol0));
    leTol1->setText(QString::number(mTol1));
*/
    mSizeHint = QSize(900, 500);
    QList<int> intList;
    intList << 165 << 735;
    splitter->setSizes(intList);
}

/**
 * Destructor
 */
PENG_ConeStrikeOutWidget::~PENG_ConeStrikeOutWidget() {
    delete mMapper;
    delete mModel;
}

RB_String PENG_ConeStrikeOutWidget::getSaveAsFileName() {
    return "peng_conestrikeout_" + mModel->getCurrentValue("number").toString();
}

void PENG_ConeStrikeOutWidget::init() {
    //
    // 1. Set model with ID (sets root object) and/or query
    //
    mModel = PENG_MODELFACTORY->getModel(PENG_ModelFactory::ModelEngCone);
    mModel->setRoot(PENG_MODELFACTORY->getRootId());

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mMapper = mModel->getMapper();
    mMapper->addMapping(this->leNumber, mModel->fieldIndex("number"));
    mMapper->addMapping(this->leDescription, mModel->fieldIndex("description"));
    mMapper->addMapping(this->leRevision,mModel->fieldIndex("revision"));
    mMapper->addMapping(this->leD0, mModel->fieldIndex("D0"));
    mMapper->addMapping(this->leD1, mModel->fieldIndex("D1"));
    mMapper->addMapping(this->leH0, mModel->fieldIndex("H0"));

    //
    // 3. Select after relations have been set, only for database models
    //
    mModel->select();

    //
    // 4. Connect model to main view
    //
    setFormatTableView(tvCone, mModel);
    readSettings();

    int countCol = mModel->columnCount(QModelIndex());
    for(int i = 0; i < countCol; i++) {
        if (i != RB2::HIDDENCOLUMNS && i != RB2::HIDDENCOLUMNS+1) {
            tvCone->hideColumn(i);
        }
    }

    leProject->setText(PENG_MODELFACTORY->getRoot()->getValue("number").toString());
}

/**
 * Menu file save
 */
bool PENG_ConeStrikeOutWidget::fileSave(bool /*withSelect*/) {
    bool success = mModel->submitAllAndSelect();

    setWindowModified(false);
    return success;
}

/**
 * Menu file save as, not applicable for this widget
 */
bool PENG_ConeStrikeOutWidget::fileSaveAs() {
    PENG_DIALOGFACTORY->requestWarningDialog(tr("Menu File Save As is\n"
                                               "not applicable for this window"));
    return false;
}

/**
 * Menu file PDF
 */
void PENG_ConeStrikeOutWidget::filePdf(QPrinter* pr) {
    pr->setFullPage(true);
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_ConeStrikeOutWidget::filePrint(QPrinter* pr) {
    pr->setFullPage(true);
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_ConeStrikeOutWidget::filePrintPreview(QPrinter* pr) {
    pr->setFullPage(true);
    QGraphicsScene* scene = mGraphicsView->scene();
    QPainter painter(pr);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
}

/**
 * @returns Help subject
 */
RB_String PENG_ConeStrikeOutWidget::getHelpSubject() const {
    return objectName();
}

/**
 * On pushbutton add weldolet clicked.
 */
void PENG_ConeStrikeOutWidget::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    //    QModelIndex index = mModel->index(row, mModel->fieldIndex("shipvia_id"));
    //    mModel->setData(index, "0", Qt::EditRole);
    //    index = mModel->index(row, mModel->fieldIndex("quotation"));
    //    mModel->setData(index, "0", Qt::EditRole);
    //    mModel->submit();
    // end NOTE

    tvCone->setCurrentIndex(mModel->index(row, mModel->fieldIndex("number"),
                                              QModelIndex()));
    tvCone->scrollTo(tvCone->currentIndex());
    leNumber->setFocus();
    leNumber->selectAll();
}

/**
 * On pushbutton delete weldolet clicked.
 */
void PENG_ConeStrikeOutWidget::on_pbDelete_clicked() {
    if (!mModel) return;

    QModelIndex index = tvCone->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button save clicked
 */
void PENG_ConeStrikeOutWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button design clicked
 */
void PENG_ConeStrikeOutWidget::on_pbDesign_clicked() {
    if (!mModel || !mSvgBorderItem) return;

    QModelIndex index = tvCone->currentIndex();
    if (!index.isValid()) {
        PENG_DIALOGFACTORY->requestWarningDialog(tr("No cone selected"));
        return;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

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
    mParentItem->setRect(-5, -5, 10, 10);
    mParentItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    mParentItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    QPen pen(QBrush(Qt::white), 1.0);
    mParentItem->setPen(pen);

    calculateCone(); // will set mArea and others
    drawCone();

    mScene->addItem(mParentItem);
    fitInDrawingArea(QRectF(35.0, 30.0, 150.0, 150.0), mParentItem);

    mScene->addItem(mSvgBorderItem);
    mSvgBorderItem->setPos(QPointF(0.0, 0.0));

    mScene->addItem(mSvgLegendItem);
    fitInLegendArea(QRectF(35.0, 190.0, 100.0, 80.0), mSvgLegendItem);

    // Values
    double lineHeight = 4.0;
    int i = 0;
    QString str = tr("Area = ");
    str += QString::number(mArea, 'E', 5) + " mm2";
    drawSceneText(str, 140.0, 190.0, QPen(QBrush(Qt::black), 1.0));
    str = "Plate length = " + QString::number(mOverallHeight) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "Plate width = " + QString::number(mOverallWidth) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "D0 = " + QString::number(mD0) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "D1 = " + QString::number(mD1) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));
    str = "H0 = " + QString::number(mH0) + " mm";
    drawSceneText(str, 140.0, 190.0 + (++i*lineHeight), QPen(QBrush(Qt::black), 1.0));

    // Title block
    double paper_y = 274.0;
    str = PENG_MODELFACTORY->getRoot()->getValue("company").toString();
    drawSceneText(str, 60.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    QModelIndex idx = tvCone->currentIndex();
    str = mModel->data(mModel->index(idx.row(), mModel->fieldIndex("revision"))).toString();
    drawSceneText(str, 115.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    str = "1 of 1";
    drawSceneText(str, 130.0, paper_y, QPen(QBrush(Qt::black), 1.0));
    str = "PENG-CONE-";
    str += PENG_MODELFACTORY->getRoot()->getValue("number").toString() + "-";
    str += mModel->data(mModel->index(idx.row(), mModel->fieldIndex("number"))).toString();
    drawSceneText(str, 150.0, paper_y, QPen(QBrush(Qt::black), 1.0));

    drawParentItemOrigine();
//    drawSceneOrigine();

    // Fit in viewport
    mGraphicsView->fitInView(mScene->sceneRect(), Qt::KeepAspectRatio);
    QApplication::restoreOverrideCursor();
}

void PENG_ConeStrikeOutWidget::calculateCone() {
    mD0 = leD0->text().toDouble();
    mD1 = leD1->text().toDouble();
    mH0 = leH0->text().toDouble();

    // tangent of cone angle alpha
    double tanAlpha = (mD0 - mD1) / (2 * mH0);
    double alpha = atan(tanAlpha);

    mRadius1 = mD1 / (2 * sin(alpha));
    mRadius0 = mD0 / (2 * sin(alpha));
    mBeta = M_PI * mD1 / mRadius1;

    if(mBeta < M_PI) {
        mOverallHeight = 2 * mRadius0 * sin(mBeta / 2);
    } else {
        mOverallHeight = 2 * mRadius0;
    }

    mEdgeRadius1 = mRadius1 * cos(mBeta / 2);

    if(mBeta < M_PI) {
        mOverallWidth = mRadius0 - mEdgeRadius1;
    } else {
        mOverallWidth = mRadius0 - mRadius0 * cos(mBeta / 2);
    }

    // Calculate the profile surface area
    double area0 = mBeta / 2 * pow(mRadius0, 2);
    double area1 = mBeta / 2 * pow(mRadius1, 2);
    mArea = area0 - area1;
}

void PENG_ConeStrikeOutWidget::drawCone() {
    int start =  (M_PI - mBeta / 2) * 180 / M_PI * 16;
    int span = mBeta * 180 / M_PI * 16;
    drawBlackArc(0, 0, mRadius0 * 2, mRadius0 * 2, start, span);
    drawBlackArc(0, 0, mRadius1 * 2, mRadius1 * 2, start, span);

    // Radii dimensions
    drawBlackLine(-mRadius0, 0.0, -(mRadius0 + mRadius1) / 2, 0.0);
    drawBlackLine(-mRadius0, 0.0, -mRadius0 + 10, 3.0);
    drawBlackLine(-mRadius0, 0.0, -mRadius0 + 10, -3.0);
    drawText("R0 = " + RB_String::number(mRadius0), -mRadius0, 0.0, QPen(QColor(Qt::black)));
    drawBlackLine(-mRadius1, 0.0, -mRadius1 / 2, 0.0);
    drawBlackLine(-mRadius1, 0.0, -mRadius1 + 10, 3.0);
    drawBlackLine(-mRadius1, 0.0, -mRadius1 + 10, -3.0);
    drawText("R1 = " + RB_String::number(mRadius1), -mRadius1, 0.0, QPen(QColor(Qt::black)));

    // Horizontal dimensions (vertical lines first)
    drawBlackLine(-mRadius0, 0.0, -mRadius0, -(mOverallHeight / 2 + 100));
    drawBlackLine(-mRadius0 * cos(mBeta/2), -mRadius0 * sin(mBeta/2),
                  -mRadius0 * cos(mBeta/2), -(mOverallHeight / 2 + 30));
    drawBlackLine(-mRadius0, -(mOverallHeight / 2 + 10),
                  -mRadius0 * cos(mBeta/2), -(mOverallHeight / 2 + 10)); // horiz
    double value = mRadius0 - mRadius0 * cos(mBeta/2);
    drawText(RB_String::number(value), -mRadius0, -(mOverallHeight / 2 + 10),
             QPen(QColor(Qt::black)));
    drawBlackLine(-mRadius1 * cos(mBeta/2), -mRadius1 * sin(mBeta/2),
                  -mRadius1 * cos(mBeta/2), -(mOverallHeight / 2 + 100));
    drawBlackLine(-mRadius0, -(mOverallHeight / 2 + 90),
                  -mRadius1 * cos(mBeta/2), -(mOverallHeight / 2 + 90)); // horiz
    value = mRadius0 - mRadius1 * cos(mBeta/2);
    drawText(RB_String::number(value), -mRadius0, -(mOverallHeight / 2 + 90),
             QPen(QColor(Qt::black)));

    // Vertical dimensions (horizontal lines first)
    double radius = mBeta * 180 / M_PI > 180 ? mRadius0 : mRadius1;
    drawBlackLine(-mRadius0 * cos(mBeta/2), -mRadius0 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 100, -mRadius0 * sin(mBeta/2));
    drawBlackLine(-mRadius0 * cos(mBeta/2), mRadius0 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 100, mRadius0 * sin(mBeta/2));
    drawBlackLine(-radius * cos(mBeta/2) + 90, -mRadius0 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 90, mRadius0 * sin(mBeta/2)); // vert
    value = mRadius0 * sin(mBeta/2) * 2;
    drawText(RB_String::number(value), -radius * cos(mBeta/2) + 90, 10,
             QPen(QColor(Qt::black)));
    drawBlackLine(-mRadius1 * cos(mBeta/2), -mRadius1 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 30, -mRadius1 * sin(mBeta/2));
    drawBlackLine(-mRadius1 * cos(mBeta/2), mRadius1 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 30, mRadius1 * sin(mBeta/2));
    drawBlackLine(-radius * cos(mBeta/2) + 10, -mRadius1 * sin(mBeta/2),
                  -radius * cos(mBeta/2) + 10, mRadius1 * sin(mBeta/2)); // vert
    value = mRadius1 * sin(mBeta/2) * 2;
    drawText(RB_String::number(value), -radius * cos(mBeta/2) + 10, -20,
             QPen(QColor(Qt::black)));
}

void PENG_ConeStrikeOutWidget::drawBlackArc(double x, double y,
                                           double w, double h,
                                           int startAngle, int spanAngle) {
    QPen pen(QBrush(Qt::black), 1.0);
    drawArc(x - w / 2, y - h / 2, w, h, startAngle, spanAngle, pen);
}

void PENG_ConeStrikeOutWidget::drawArc(double x, double y,
                                      double w, double h,
                                      int startAngle, int spanAngle,
                                      const QPen& p) {
    QGraphicsEllipseItem* ell = new QGraphicsEllipseItem(x, y, w, h, mParentItem);
    ell->setPen(p);
    ell->setStartAngle(startAngle);
    ell->setSpanAngle(spanAngle);

    if (mParentItem) {
        mParentItem->setRect(mParentItem->rect().united(ell->boundingRect()));
    } else if (mScene) {
        mScene->addItem(ell);
    }
}

void PENG_ConeStrikeOutWidget::drawSceneArc(double x, double y,
                                           double w, double h,
                                           int startAngle, int spanAngle,
                                           const QPen& p) {
    if (!mScene) return;
    QGraphicsEllipseItem* ell = new QGraphicsEllipseItem(x, y, w, h);
    ell->setPen(p);
    ell->setStartAngle(startAngle);
    ell->setSpanAngle(spanAngle);
    mScene->addItem(ell);
}

void PENG_ConeStrikeOutWidget::drawBlackLine(double x1, double y1,
                                           double x2, double y2) {
    QPen pen(QBrush(Qt::black), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_ConeStrikeOutWidget::drawBlueLine(double x1, double y1,
                                           double x2, double y2) {
    QPen pen(QBrush(Qt::darkBlue), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_ConeStrikeOutWidget::drawGreenLine(double x1, double y1,
                                            double x2, double y2) {
    QPen pen(QBrush(Qt::darkGreen), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_ConeStrikeOutWidget::drawRedLine(double x1, double y1,
                                          double x2, double y2) {
    QPen pen(QBrush(Qt::red), 1.0);
    drawLine(x1, y1, x2, y2, pen);
}

void PENG_ConeStrikeOutWidget::drawLine(double x1, double y1, double x2,
                                       double y2, const QPen& p) {
    QGraphicsLineItem* l = new QGraphicsLineItem(x1, y1, x2, y2, mParentItem);
    l->setPen(p);

    if (mParentItem) {
        mParentItem->setRect(mParentItem->rect().united(l->boundingRect()));
    } else if (mScene) {
        mScene->addItem(l);
    }
}

void PENG_ConeStrikeOutWidget::drawSceneLine(double x1, double y1, double x2,
                                            double y2, const QPen& p) {
    if (!mScene) return;
    QGraphicsLineItem* l = new QGraphicsLineItem(x1, y1, x2, y2);
    l->setPen(p);
    mScene->addItem(l);
}

void PENG_ConeStrikeOutWidget::drawText(const QString& str, double x,
                                            double y, const QPen& /* p */) {
    QGraphicsTextItem* t = new QGraphicsTextItem(str, mParentItem);
    t->setPos(x, y);
    t->setTransform(mTextMatrix);

    if (mParentItem) {
        mParentItem->setRect(mParentItem->rect().united(t->boundingRect()));
    } else if (mScene) {
        mScene->addItem(t);
    }
}

void PENG_ConeStrikeOutWidget::drawSceneText(const QString& str, double x,
                                            double y, const QPen& p) {
    QGraphicsTextItem* t = new QGraphicsTextItem(str);
    t->setPos(x, y);
    t->setScale(0.3);
    t->setDefaultTextColor(p.color());
    mScene->addItem(t);
}

void PENG_ConeStrikeOutWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
 * Draw X, Y and Z axis
 * @param f scaling factor
 */
void PENG_ConeStrikeOutWidget::drawAxes(double f) {
    drawGreenLine(0, 0, 100 * f, 0); // x-axis
    drawGreenLine(90 * f, 4 * f, 100 * f, 0);
    drawGreenLine(90 * f, -4 * f, 100 * f, 0);
    drawGreenLine(90 * f, 4 * f, 90 * f, -4 * f);
    drawText("X", 105 * f, 0, QPen(QBrush(Qt::black), 1.0));
    drawGreenLine(0, 0, 100 * f, 0); // y-axis
    drawGreenLine(4 * f, 90 * f, 100 * f, 0);
    drawGreenLine(-4 * f, 90 * f, 100 * f, 0);
    drawGreenLine(4 * f, 90 * f,  -4 * f, 90 * f);
    drawText("Y", 0, 115 * f, QPen(QBrush(Qt::black), 1.0));
}

/**
 * Draw origine, for testing purposes only
 * @param f scaling factor
 */
void PENG_ConeStrikeOutWidget::drawParentItemOrigine(double f) {
    QPen pen(QBrush(Qt::black), 1.0 * f);
    drawLine(-10.0 * f, 0.0, 10.0 * f, 0.0, pen);
    drawLine(0.0, -10.0 * f, 0.0, 10.0 * f, pen);
}

/**
 * Draw origine, for testing purposes only
 * @param f scaling factor
 */
void PENG_ConeStrikeOutWidget::drawSceneOrigine(double f) {
    QPen pen(QBrush(Qt::blue), 1.0 * f);
    drawSceneLine(-20.0 * f, 0.0, 0.0 * f, 0.0, pen);
    drawSceneLine(0.0, -10.0 * f, 0.0, 10.0 * f, pen);
}

/**
 * Fit item in drawing area
 * @param dwgArea drawing area
 * @param item rectangle item, usualy parent item of group of drawing items
 */
void PENG_ConeStrikeOutWidget::fitInDrawingArea(const QRectF& dwgArea,
                                                    QGraphicsItem* item) {
    // Scale
    double factor = dwgArea.width() / item->boundingRect().width();
    double testFactor = dwgArea.height() / item->boundingRect().height();
    factor = factor < testFactor ? factor : testFactor;

    QTransform matrix(factor, 0, 0, 0, -factor, 0, 0, 0, 1);
    item->setTransform(matrix, false);


    // Position top left corner at 35, 25 mm, due transform bottomLeft
    QPointF itemSceneTopLeft = item->mapToScene(item->boundingRect().bottomLeft());
    QPointF itemScenePos = item->scenePos();
    QPointF deltaTopLeft = itemScenePos - itemSceneTopLeft;
    QPointF newPos = QPointF(35, 25) + deltaTopLeft;
    item->setPos(newPos);

    // Rescale text
    int count = item->childItems().count();
    for (int i = 0; i < count; ++i) {
        QGraphicsTextItem* textItem =
                dynamic_cast<QGraphicsTextItem*>(item->childItems().at(i));
        if (textItem) {
            textItem->setScale(textItem->scale() / factor * 0.3);
        }
    }
}

/**
 * Fit item in drawing area
 * @param dwgArea drawing area
 * @param item rectangle item, usualy parent item of group of drawing items
 */
void PENG_ConeStrikeOutWidget::fitInLegendArea(const QRectF& dwgArea,
                                                    QGraphicsItem* item) {
    // Scale
    double factor = dwgArea.width() / item->boundingRect().width();
    double testFactor = dwgArea.height() / item->boundingRect().height();
    factor = factor < testFactor ? factor : testFactor;
    // Create matrix with position
    QTransform matrix(factor, 0, 0, 0, factor, 0, dwgArea.x(), dwgArea.y(), 1);
    item->setTransform(matrix, false);
}

void PENG_ConeStrikeOutWidget::zoom(double /*factor*/) {
//    qreal scale = mCurrentZoomFactor * factor;
//    // qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));
//
//    QMatrix matrix;
//    matrix.scale(scale, scale);
//    mGraphicsView->setMatrix(matrix);
}

void PENG_ConeStrikeOutWidget::resetView() {
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));
}

bool PENG_ConeStrikeOutWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}


