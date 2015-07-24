/*****************************************************************
 * $Id: srm_groupcontactwidget.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 05, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_GROUPCONTACTWIDGET_H
#define SRM_GROUPCONTACTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_groupcontactwidget.h"

class RB_MmProxy;

class SRM_GroupContactWidget : public RB_Widget, private Ui::SRM_GroupContactWidget {

    Q_OBJECT

public:
    SRM_GroupContactWidget(QWidget *parent = 0);
    virtual ~SRM_GroupContactWidget();

    virtual RB_String getName() const { return "SRM Group Contact"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

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

#endif // SRM_GROUPCONTACTWIDGET_H
