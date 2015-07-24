/*****************************************************************
 * $Id: acc_supplcostpartwidget.h 1783 2012-09-25 20:25:28Z rutger $
 * Created: Sept 25, 2012 11:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SUPPLCOSTPARTWIDGET_H
#define ACC_SUPPLCOSTPARTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_supplcostpartwidget.h"

class RB_MmProxy;

class ACC_SupplCostPartWidget : public RB_Widget, private Ui::ACC_SupplCostPartWidget {

    Q_OBJECT

public:
    ACC_SupplCostPartWidget(QWidget *parent = 0);
    virtual ~ACC_SupplCostPartWidget();

    virtual RB_String getName() const { return "ACC Supplier Cost Part"; }
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
    //! Supplier model
    RB_MmProxy* mSupplModel;

};

#endif // ACC_SUPPLCOSTPARTWIDGET_H
