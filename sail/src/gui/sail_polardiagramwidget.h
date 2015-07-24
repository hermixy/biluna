/*****************************************************************
 * $Id: sail_polardiagramwidget.h 2166 2014-09-25 13:23:18Z rutger $
 * Created: Aug 22, 2013 13:00:25 AM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_POLARDIAGRAMWIDGET_H
#define SAIL_POLARDIAGRAMWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"

//#include <QChartView>
//#include <QPolarChart>
//QTCOMMERCIALCHART_USE_NAMESPACE // is QtCommercialChart:: namespace


/**
 * Polar speed diagram of a sailing boat
 *@brief The SAIL_PolarDiagramWidget class
 */
class SAIL_PolarDiagramWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit SAIL_PolarDiagramWidget(QWidget* parent = 0);
    virtual ~SAIL_PolarDiagramWidget();

    virtual RB_String getName() const { return "SAIL Polar Diagram"; }
    virtual RB_String getSaveAsFileName();
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSAIL; }

    virtual void init();
//    virtual bool fileSave(bool withSelect);
//    virtual void filePdf(QPrinter* printer);
//    virtual void filePrint(QPrinter* printer);
//    virtual void filePrintPreview(QPrinter* printer);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

// public slots:

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void switchChartType();

//    QChartView* mChartView;
//    QPolarChart* mChart;
};

#endif // SAIL_POLARDIAGRAMWIDGET_H
