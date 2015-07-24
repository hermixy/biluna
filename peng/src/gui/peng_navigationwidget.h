/*****************************************************************
 * $Id: peng_navigationwidget.h 2233 2015-04-29 19:10:59Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_NAVIGATIONWIDGET_H
#define PENG_NAVIGATIONWIDGET_H

#include "rb_widget.h"
#include "ui_peng_navigationwidget.h"

class PENG_NavigationWidget : public RB_Widget, private Ui::PENG_NavigationWidget {

    Q_OBJECT

public:
    PENG_NavigationWidget(QWidget *parent = 0);
    virtual ~PENG_NavigationWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Navigation"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectivePENG;
    }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget() { return true; }

    virtual void emitState();


    enum TabType {
        TabPnid,
        TabEquipment,
        TabPipeLine,
        TabInstrument,
        TabElectrical
    };

signals:
    void currentNavigationTab(int);

public slots:
    virtual void slotShowTab(int tabType);

    virtual void on_pbAddEquipment_clicked();
    virtual void on_pbDeleteEquipment_clicked();
    virtual void on_pbSaveEquipment_clicked();

    virtual void on_pbAddLine_clicked();
    virtual void on_pbDeleteLine_clicked();
    virtual void on_pbSaveLine_clicked();

protected:
    void changeEvent(QEvent *e);

private:
//    virtual void setTab(int tabType);

    RB_MmProxy* mModelEquipment;
    RB_MmProxy* mModelLine;

// old
//    //! P&ID master widget
//    RB_Widget* mPnidMasterWidget;
//    //! Equipment master widget
//    RB_Widget* mEquipmentMasterWidget;
//    //! Pipe line master widget
//    RB_Widget* mPipeLineMasterWidget;
//    //! Instrument master widget
//    RB_Widget* mInstrumentMasterWidget;
//    //! Electrical master widget
//    RB_Widget* mElectricalMasterWidget;
//    // TODO: instrument list and electrical list
};

#endif // PENG_NAVIGATIONWIDGET_H
