/*****************************************************************
 * $Id: acc_taxconfirmationdialog.h 1798 2012-11-10 12:19:45Z rutger $
 * Created: Mar 28, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXCONFIRMATIONDIALOG_H
#define ACC_TAXCONFIRMATIONDIALOG_H

#include <map>
#include "rb_dialog.h"
#include "rb_objectmember.h"
#include "ui_acc_taxconfirmationdialog.h"


/**
 * Dialog to confirm the tax(es) per salesorder item
 */
class ACC_TaxConfirmationDialog : public RB_Dialog, private Ui::ACC_TaxConfirmationDialog {

    Q_OBJECT

public:
    ACC_TaxConfirmationDialog(QWidget *parent = 0);
    virtual ~ACC_TaxConfirmationDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Confirm Stockmoves Taxes"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual void setTransModel(RB_ObjectBase* transDoc,
                               RB_ObjectContainer* debCredTransList,
                               bool isSalesOrder = true);

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();
    virtual void on_pbRefreshTax_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void updateTax();
    void setFields();

    RB_ObjectBase* mTransDoc;
    RB_ObjectContainer* mDebCredTransList;
    std::map<QTableWidgetItem*, RB_ObjectMember*> mTaxMap;

    bool mIsSalesOrder;
};

#endif // ACC_TAXCONFIRMATIONDIALOG_H
