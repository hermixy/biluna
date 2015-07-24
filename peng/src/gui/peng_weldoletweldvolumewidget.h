/*****************************************************************
 * $Id: peng_weldoletweldvolumewidget.h 1849 2012-12-31 11:25:09Z rutger $
 * Created: Apr 15, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_WELDOLETWELDVOLUMEWIDGET_H
#define PENG_WELDOLETWELDVOLUMEWIDGET_H

#include <QGraphicsScene>
#include <QtSvg>
#include "peng_vector.h"
#include "rb_widget.h"
#include "ui_peng_weldoletweldvolumewidget.h"


class PENG_WeldoletWeldVolumeWidget : public RB_Widget,
                                private Ui::PENG_WeldoletWeldVolumeWidget {
    Q_OBJECT

public:
    PENG_WeldoletWeldVolumeWidget(QWidget *parent = 0);
    ~PENG_WeldoletWeldVolumeWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Weldolet on Pipe Weld Volume"; }
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
    void on_pbCalcVol_clicked();
    void slotUpdateH3();
    void slotUpdateH3(const QModelIndex&);
    void slotClearVolume(const QModelIndex&);

protected:
    void changeEvent(QEvent *e);

private:
    void calculateVolume();
    double calcCubeVolume(bool textOutput = false);

    void drawBlueLine3D(double x1, double y1, double z1,
                        double x2, double y2, double z2);
    void drawGreenLine3D(double x1, double y1, double z1,
                         double x2, double y2, double z2);
    void drawRedLine3D(double x1, double y1, double z1,
                       double x2, double y2, double z2);
    void drawBlueLine(double x1, double y1, double x2, double y2);
    void drawGreenLine(double x1, double y1, double x2, double y2);
    void drawRedLine(double x1, double y1, double x2, double y2);
    void drawLine(double x1, double y1, double x2, double y2, const QPen& p);
    void drawSceneLine(double x1, double y1, double x2, double y2, const QPen& p);
    void drawText3D(const QString& str, double x1, double y1, double z1);
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

    PENG_Vector mP1;
    PENG_Vector mP2;
    PENG_Vector mP3;
    PENG_Vector mP4;
    PENG_Vector mP5;
    PENG_Vector mP6;
    PENG_Vector mP7;
    PENG_Vector mP8;

    double mCos30;
    double mSin30;

    double mAlpha1;
    double mAlpha2;
    double mAlpha3;
    double mD0;
    double mD2;
    double mD21;
    double mD3;
    double mGap;
    double mGap1;
    double mH3;

    double mVolume;

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

#endif // PENG_WELDOLETWELDVOLUMEWIDGET_H
