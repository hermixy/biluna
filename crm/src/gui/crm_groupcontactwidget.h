/*****************************************************************
 * $Id: crm_groupcontactwidget.h 1561 2012-03-05 22:18:15Z rutger $
 * Created: Mar 05, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_GROUPCONTACTWIDGET_H
#define CRM_GROUPCONTACTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_crm_groupcontactwidget.h"

class RB_MmProxy;

class CRM_GroupContactWidget : public RB_Widget, private Ui::CRM_GroupContactWidget {

    Q_OBJECT

public:
    CRM_GroupContactWidget(QWidget *parent = 0);
    virtual ~CRM_GroupContactWidget();

    virtual RB_String getName() const { return "CRM Group Contact"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddGroup_clicked();
    virtual void on_pbDeleteGroup_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOnGroup_clicked();
    virtual void on_pbFilterOffGroup_clicked();

    virtual void on_pbAddContact_clicked();
    virtual void on_pbDeleteContact_clicked();
    virtual void on_pbFilterOnContact_clicked();
    virtual void on_pbFilterOffContact_clicked();

    virtual void on_ileContact_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    //! Group data model
    RB_MmProxy* mGroupModel;
    //! Group data widget mapper
    RB_DataWidgetMapper* mGroupMapper;
    //! Contact data model
    RB_MmProxy* mContactModel;
    //! Contact data widget mapper
    RB_DataWidgetMapper* mContactMapper;

};

#endif // CRM_GROUPCONTACTWIDGET_H
