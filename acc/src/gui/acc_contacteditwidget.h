/*****************************************************************
 * $Id: acc_contacteditwidget.h 2186 2014-10-16 14:04:01Z rutger $
 * Created: Jan 06, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CONTACTWIDGET_H
#define ACC_CONTACTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_contacteditwidget.h"

class RB_MmProxy;

class ACC_ContactEditWidget : public RB_Widget, private Ui::ACC_ContactEditWidget {

    Q_OBJECT

public:
    ACC_ContactEditWidget(QWidget *parent = 0, bool parentIsCustomer = true);
    virtual ~ACC_ContactEditWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddContact_clicked();
    virtual void on_pbDeleteContact_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFind_clicked();
    virtual void on_pbShowAll_clicked();
    virtual void on_pbSendEmail1_clicked();
    virtual void on_pbSendEmail2_clicked();
    virtual void on_pbCopyParent_clicked();
    virtual void on_pbCopyPrimaryAddress_clicked();

    virtual void slotParentRowChanged(const QModelIndex&, const QModelIndex&);
    virtual void slotParentSubmitted();

protected:
    void changeEvent(QEvent* e);

private:
    //! Customer/supplier data model
    RB_MmProxy* mCustSuppModel;
    //! Contact data model
    RB_MmProxy* mContactModel;
    //! Contact data widget mapper
    RB_DataWidgetMapper* mContactMapper;
    //! Flag setting parent as customer or supplier
    bool mParentIsCustomer; // TODO: not necessary when customer and suppliers are merged
};

#endif // ACC_CONTACTWIDGET_H
