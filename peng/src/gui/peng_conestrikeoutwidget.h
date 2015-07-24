/*****************************************************************
 * $Id: peng_conestrikeoutwidget.h 1849 2012-12-31 11:25:09Z rutger $
 * Created: Apr 15, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_CONESTRIKEOUTWIDGET_H
#define PENG_CONESTRIKEOUTWIDGET_H

#include <QGraphicsScene>
#include <QtSvg>
#include "peng_vector.h"
#include "rb_widget.h"
#include "ui_peng_conestrikeoutwidget.h"


class PENG_ConeStrikeOutWidget : public RB_Widget,
                                private Ui::PENG_ConeStrikeOutWidget {
    Q_OBJECT

public:
    PENG_ConeStrikeOutWidget(QWidget *parent = 0);
    ~PENG_ConeStrikeOutWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Cone Strikeout"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }
    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    void on_pbAdd_clicked();
    void on_pbDelete_clicked();
    void on_pbSave_clicked();
    void on_pbDesign_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void calculateCone();
    void drawCone();

    void drawBlackArc(double x, double y, double w, double h,
                      int startAngle, int spanAngle);

    void drawArc(double x, double y, double w, double h,
                 int startAngle, int spanAngle, const QPen& p);
    void drawSceneArc(double x, double y, double w, double h,
                      int startAngle, int spanAngle, const QPen& p);

    void drawBlackLine(double x1, double y1, double x2, double y2);
    void drawBlueLine(double x1, double y1, double x2, double y2);
    void drawGreenLine(double x1, double y1, double x2, double y2);
    void drawRedLine(double x1, double y1, double x2, double y2);

    void drawLine(double x1, double y1, double x2, double y2, const QPen& p);
    void drawSceneLine(double x1, double y1, double x2, double y2, const QPen& p);

    void drawText(const QString& str, double x, double y, const QPen& p);
    void drawSceneText(const QString& str, double x, double y, const QPen& p);

    void drawAxes(double f = 1.0);
    void drawParentItemOrigine(double f = 1.0);
    void drawSceneOrigine(double f = 1.0);
    void fitInDrawingArea(const QRectF& dwgArea, QGraphicsItem* item);
    void fitInLegendArea(const QRectF& dwgArea, QGraphicsItem* item);
    void zoom(double factor);
    void resetView();

    //! Weldolet is drawn on this canvas parent item
    QGraphicsRectItem* mParentItem;
    QGraphicsScene* mScene;
    QTransform mMatrix;
    QTransform mTextMatrix;

    double mD0;
    double mD1;
//    double mt0;
//    double mt1;
    double mH0;
    double mArea;
    double mRadius0;
    double mRadius1;
    double mBeta;
    double mOverallWidth;
    double mOverallHeight;
    double mEdgeRadius1;

    //! Number of steps over 360 degree
    int mSteps;
    //! True for intermediate calculation results
    bool mTextOutput;
    //! Current zoom factor
    double mCurrentZoomFactor;

    //! Weldolet model
    RB_MmProxy* mModel;
    //! Data widget mapper for weldolet model
    RB_DataWidgetMapper* mMapper;

    QGraphicsView* mGraphicsView;
    QSvgRenderer* mSvgBorderRend;
    QGraphicsSvgItem* mSvgBorderItem;
    QSvgRenderer* mSvgLegendRend;
    QGraphicsSvgItem* mSvgLegendItem;


};

#endif // PENG_CONESTRIKEOUTWIDGET_H
