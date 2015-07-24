/*****************************************************************
 * $Id: sail_mapnavigationwidget.h 1746 2012-08-23 21:01:10Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_MAPNAVIGATIONWIDGET_H
#define SAIL_MAPNAVIGATIONWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "sail_coordcalc.h"
#include "ui_sail_mapnavigationwidget.h"


/**
 * Widget to navigate the course on a map
 */
class SAIL_MapNavigationWidget : public RB_Widget, private Ui::SAIL_MapNavigationWidget {

    Q_OBJECT
    
public:
    explicit SAIL_MapNavigationWidget(QWidget* parent = 0);
    virtual ~SAIL_MapNavigationWidget();
    
    virtual RB_String getName() const { return "SAIL Map Navigation"; }
    virtual RB_String getSaveAsFileName();
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSAIL; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddCoordinate_clicked();
    virtual void on_pbDeleteCoordinate_clicked();
    virtual void on_pbSaveCoordinate_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();
    virtual void on_pbAddRoute_clicked();
    virtual void on_pbDeleteRoute_clicked();
    virtual void on_pbSaveRoute_clicked();
    virtual void on_pbAddRouteCoordinate_clicked();
    virtual void on_pbDeleteRouteCoordinate_clicked();
    virtual void on_pbCalcRoute_clicked();
    virtual void on_pbAddTrack_clicked();
    virtual void on_pbDeleteTrack_clicked();
    virtual void on_pbSaveTrack_clicked();

    virtual void on_ileRouteCoordinate_clicked();
    virtual void on_ileTrackCoordinateFrom_clicked();
    virtual void on_ileTrackCoordinateTo_clicked();

    virtual void slot_latitude_update();
    virtual void slot_longitude_update();
    virtual void slot_latDecimal_update();
    virtual void slot_longDecimal_update();

    virtual void slotDrawScene(int tabIndex);

protected:
    virtual void changeEvent(QEvent* e);

protected slots:
    void slotCurrentItemChanged(const QModelIndex& curr, const QModelIndex& prev);

private:
    //! Coordinate data model
    RB_MmProxy* mModel;
    //! Coordinate data widget mapper
    RB_DataWidgetMapper* mMapper;

    //! Route data model
    RB_MmProxy* mRouteModel;
    //! Route data widget mapper
    RB_DataWidgetMapper* mRouteMapper;
    //! Route coordinate data model
    RB_MmProxy* mRouteCoordinateModel;
    //! Route coordinate data widget mapper
    RB_DataWidgetMapper* mRouteCoordinateMapper;
    //! Track data model
    RB_MmProxy* mTrackModel;
    //! Track data widget mapper
    RB_DataWidgetMapper* mTrackMapper;

    SAIL_CoordCalc mCalc;

    bool mAddCoordinateInProgress;

    //! Graphics scene
    QGraphicsScene mScene;
};

#endif // SAIL_MAPNAVIGATIONWIDGET_H
