/*****************************************************************
 * $Id: crm_customerwidget.h 1559 2012-03-03 11:10:33Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CUSTOMERWIDGET_H
#define CRM_CUSTOMERWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_crm_customerwidget.h"

class RB_MmProxy;

class CRM_CustomerWidget : public RB_Widget, private Ui::CRM_CustomerWidget {

    Q_OBJECT

public:
    CRM_CustomerWidget(QWidget *parent = 0);
    virtual ~CRM_CustomerWidget();

    virtual RB_String getName() const { return "CRM Customer"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

    virtual void on_ileCustomerParent_clicked();
    virtual void on_pbCopyParent_clicked();
    virtual void on_pbCopyPostAddress_clicked();
    virtual void on_pbMapLocation_clicked();
    virtual void on_pbSendEmail_clicked();

    virtual void on_pbSelectCompany_clicked();
    virtual void on_ileInternalAccountHolder_clicked();

protected:
    void changeEvent(QEvent* e);

protected slots:
    void slotParentRowChanged(const QModelIndex& curr,
                              const QModelIndex& prev);
    void slotHandleParentRowChanged();

private:
    //! Customer data model
    RB_MmProxy* mCustModel;
    //! Customer data widget mapper
    RB_DataWidgetMapper* mCustMapper;
    //! Customer detail (CRM) data model
    RB_MmProxy* mCustDetModel;
    //! Customer detail (CRM) data widget mapper
    RB_DataWidgetMapper* mCustDetMapper;
    //! Last selected ID
    RB_String mCurrentId;

};

#endif // CRM_CUSTOMERWIDGET_H
