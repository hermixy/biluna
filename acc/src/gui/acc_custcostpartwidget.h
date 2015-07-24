/*****************************************************************
 * $Id: acc_custcostpartwidget.h 1923 2013-04-08 21:37:40Z rutger $
 * Created: Sept 21, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CUSTCOSTPARTWIDGET_H
#define ACC_CUSTCOSTPARTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_custcostpartwidget.h"

class RB_MmProxy;

class ACC_CustCostPartWidget : public RB_Widget, private Ui::ACC_CustCostPartWidget {

    Q_OBJECT

public:
    ACC_CustCostPartWidget(QWidget *parent = 0);
    virtual ~ACC_CustCostPartWidget();

    virtual RB_String getName() const { return "ACC Customer Cost Part"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

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

    virtual void on_ileCostGroup_clicked();
    virtual void on_ileCostCenter_clicked();

    virtual void slotParentRowChanged(const QModelIndex&, const QModelIndex&);

protected:
    void changeEvent(QEvent* e);

private:
    //! Cost Center Part model
    RB_MmProxy* mModel;
    //! Cost Center Part data widget mapper
    RB_DataWidgetMapper* mMapper;
    //! Customer model
    RB_MmProxy* mCustModel;

};

#endif // ACC_CUSTCOSTPARTWIDGET_H
