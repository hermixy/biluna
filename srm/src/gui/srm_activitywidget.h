/*****************************************************************
 * $Id: srm_activitywidget.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 20, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIVITYWIDGET_H
#define SRM_ACTIVITYWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_activitywidget.h"

class RB_MmProxy;

class SRM_ActivityWidget : public RB_Widget, private Ui::SRM_ActivityWidget {

    Q_OBJECT

public:
    SRM_ActivityWidget(QWidget *parent = 0);
    virtual ~SRM_ActivityWidget();

    virtual RB_String getName() const { return "SRM Activity"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void slotSelectStatusChanged(int index);
    virtual void slotSelectedByChanged(int index);

    virtual void slotSupplierRowChanged(const QModelIndex&, const QModelIndex&);
    virtual void slotContactRowChanged(const QModelIndex&, const QModelIndex&);
    virtual void slotUpdateEndDateTime();
    virtual void slotChangedCurrentRow(const QModelIndex&, const QModelIndex&);

protected:
    void changeEvent(QEvent* e);

private:
    //! Activity data model
    RB_MmProxy* mActivityModel;
    //! Activity data widget mapper
    RB_DataWidgetMapper* mActivityMapper;
    //! Supplier data model
    RB_MmProxy* mSupplierModel;
    //! Contact data model
    RB_MmProxy* mContactModel;

    bool mIsOpenContactDialogShowed;
    bool mIsOpenSupplierDialogShowed;
};

#endif // SRM_ACTIVITYWIDGET_H
