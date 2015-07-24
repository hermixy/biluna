/*****************************************************************
 * $Id: crm_activitywidget.h 1744 2012-08-22 17:47:26Z rutger $
 * Created: Feb 20, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIVITYWIDGET_H
#define CRM_ACTIVITYWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_crm_activitywidget.h"

class RB_MmProxy;

class CRM_ActivityWidget : public RB_Widget, private Ui::CRM_ActivityWidget {

    Q_OBJECT

public:
    CRM_ActivityWidget(QWidget *parent = 0);
    virtual ~CRM_ActivityWidget();

    virtual RB_String getName() const { return "CRM Activity"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }

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

    virtual void slotCustomerRowChanged(const QModelIndex&, const QModelIndex&);
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
    //! Customer data model
    RB_MmProxy* mCustomerModel;
    //! Contact data model
    RB_MmProxy* mContactModel;

    bool mIsOpenContactDialogShowed;
    bool mIsOpenCustomerDialogShowed;
};

#endif // CRM_ACTIVITYWIDGET_H
