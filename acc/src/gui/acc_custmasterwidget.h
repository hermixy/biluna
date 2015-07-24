/*****************************************************************
 * $Id: acc_custmasterwidget.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Dec 20, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTMASTERWIDGET_H
#define ACC_CUSTMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_acc_custmasterwidget.h"


/**
 * Customer and branch widget, part of the navigation dockwidget
 */
class ACC_CustMasterWidget : public RB_Widget, private Ui::ACC_CustMasterWidget {

    Q_OBJECT

public:
    ACC_CustMasterWidget(QWidget *parent = 0);
    virtual ~ACC_CustMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Customer Master"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modelType) const;

public slots:
    virtual void on_pbAddCustomer_clicked();
    virtual void on_pbDeleteCustomer_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setDefaultValues(int row, const QModelIndex& idxParent);

    //! Data model
    RB_MmProxy* mModel;

};

#endif // ACC_CUSTMASTERWIDGET_H
