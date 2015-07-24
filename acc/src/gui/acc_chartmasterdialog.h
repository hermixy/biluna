/*****************************************************************
 * $Id: acc_chartmasterdialog.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Feb 25, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CHARTMASTERDIALOG_H
#define ACC_CHARTMASTERDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_chartmasterdialog.h"

/**
 * Edit dialog for the chart of accounts
 */
class ACC_ChartMasterDialog : public RB_Dialog, private Ui::ACC_ChartMasterDialog {

    Q_OBJECT

public:
    ACC_ChartMasterDialog(QWidget *parent = 0);
    virtual ~ACC_ChartMasterDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Chart of Accounts"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

protected slots:
    virtual void slotAccountGroupChanged(int row);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_CHARTMASTERDIALOG_H
