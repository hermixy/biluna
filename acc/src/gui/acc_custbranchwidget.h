/*****************************************************************
 * $Id: acc_custbranchwidget.h 1843 2012-12-22 12:31:51Z rutger $
 * Created: Dec 24, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTBRANCHWIDGET_H
#define ACC_CUSTBRANCHWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_custbranchwidget.h"

class RB_MmProxy;

class ACC_CustBranchWidget : public RB_Widget, private Ui::ACC_CustBranchWidget {

    Q_OBJECT

public:
    ACC_CustBranchWidget(QWidget *parent = 0);
    virtual ~ACC_CustBranchWidget();

    virtual RB_String getName() const { return "ACC Customer Head Office or Branch"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbPrevious_clicked();
    virtual void on_pbNext_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbRevert_clicked();
    virtual void on_ileCustomerParent_clicked();
    virtual void on_pbCopyParent_clicked();
    virtual void on_pbCopyPostAddress_clicked();
    virtual void on_pbMapLocation_clicked();
    virtual void on_pbSendEmail_clicked();
    virtual void on_pbAddNote_clicked();
    virtual void on_pbDeleteNote_clicked();
    virtual void slotResetWidgets();

protected:
    void changeEvent(QEvent* e);

private:
    //! Customer data model
    RB_MmProxy* mCustModel;
    //! Customer data widget mapper
    RB_DataWidgetMapper* mCustMapper;
    //! Note data model
    RB_MmProxy* mNoteModel;
    //! Note data widget mapper
    RB_DataWidgetMapper* mNoteMapper;


};

#endif // ACC_CUSTBRANCHWIDGET_H
