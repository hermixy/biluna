/*****************************************************************
 * $Id: crm_contactwidget.h 1796 2012-10-24 20:35:18Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CONTACTWIDGET_H
#define CRM_CONTACTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_crm_contactwidget.h"

class RB_MmProxy;

class CRM_ContactWidget : public RB_Widget, private Ui::CRM_ContactWidget {

    Q_OBJECT

public:
    CRM_ContactWidget(QWidget *parent = 0);
    virtual ~CRM_ContactWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "CRM Contact edit/view"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddContact_clicked();
    virtual void on_pbDeleteContact_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();
    virtual void slotSelectedByChanged(int index);

    virtual void on_pbSendEmail1_clicked();
    virtual void on_pbSendEmail2_clicked();
    virtual void on_pbCopyParent_clicked();
    virtual void on_pbCopyPrimaryAddress_clicked();

    virtual void on_pbSelectCustomer_clicked();

protected:
    void changeEvent(QEvent* e);

protected slots:
    void slotCustomerRowChanged(const QModelIndex& current,
                                const QModelIndex& previous);
    void slotParentRowChanged(const QModelIndex& curr,
                              const QModelIndex& prev);
    void slotHandleParentRowChanged();

private:
    //! Customer model for synchronization
    RB_MmProxy* mCustomerModel;
    //! Contact data model
    RB_MmProxy* mContactModel;
    //! Contact data widget mapper
    RB_DataWidgetMapper* mContactMapper;
    //! Contact detail (CRM) data model
    RB_MmProxy* mContactDetModel;
    //! Contact detail (CRM) data widget mapper
    RB_DataWidgetMapper* mContactDetMapper;
    //! Last selected ID
    RB_String mCurrentId;
    //! Is widget initialized
    bool mIsInitialized;

};

#endif // CRM_CONTACTWIDGET_H
