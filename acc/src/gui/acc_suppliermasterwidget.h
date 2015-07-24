/*****************************************************************
 * $Id: acc_suppliermasterwidget.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Feb 10, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SUPPLIERMASTERWIDGET_H
#define ACC_SUPPLIERMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_acc_suppliermasterwidget.h"

/**
 * Supplier widget, part of the navigation dockwidget
 */
class ACC_SupplierMasterWidget : public RB_Widget,
                                    private Ui::ACC_SupplierMasterWidget {
    Q_OBJECT

public:
    ACC_SupplierMasterWidget(QWidget *parent = 0);
    virtual ~ACC_SupplierMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Supplier Master"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modeltype) const;

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();


protected:
    void changeEvent(QEvent *e);

private:
    //! Supplier data model
    RB_MmProxy* mModel;

};

#endif // ACC_SUPPLIERMASTERWIDGET_H
