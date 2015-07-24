/*****************************************************************
 * $Id: sail_mapnavigationwidget.h 1729 2012-08-08 20:48:52Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_NAVIGATIONTRACKWIDGET_H
#define SAIL_NAVIGATIONTRACKWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_sail_navigationtrackwidget.h"


/**
 * Widget to set tracks based on coordinates
 */
class SAIL_NavigationTrackWidget : public RB_Widget, private Ui::SAIL_NavigationTrackWidget {

    Q_OBJECT
    
public:
    explicit SAIL_NavigationTrackWidget(QWidget* parent = 0);
    virtual ~SAIL_NavigationTrackWidget();
    
    virtual RB_String getName() const { return "SAIL Navigation Track"; }
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
    virtual void on_pbAddTrack_clicked();
    virtual void on_pbDeleteTrack_clicked();
    virtual void on_pbSaveTrack_clicked();

    virtual void on_ileTrackCoordinateFrom_clicked();
    virtual void on_ileTrackCoordinateTo_clicked();

    virtual void on_pbTest_clicked();

protected:
    virtual void changeEvent(QEvent* e);

private:
    //! Track data model
    RB_MmProxy* mTrackModel;
    //! Track data widget mapper
    RB_DataWidgetMapper* mTrackMapper;

};

#endif // SAIL_NAVIGATIONTRACKWIDGET_H
