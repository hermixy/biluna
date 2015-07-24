/*****************************************************************
 * $Id: acc_stockmasterwidget.h 1556 2012-03-01 20:39:26Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKMASTERWIDGET_H
#define ACC_STOCKMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_acc_stockmasterwidget.h"


/**
 * Stock category/master widget, part of the navigation dockwidget
 */
class ACC_StockMasterWidget : public RB_Widget, private Ui::ACC_StockMasterWidget {

    Q_OBJECT

public:
    ACC_StockMasterWidget(QWidget *parent = 0);
    virtual ~ACC_StockMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Stock Master"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modelType) const;

public slots:
    virtual void on_pbAddCategory_clicked();
    virtual void on_pbSubCategory_clicked();
    virtual void on_pbDeleteCategory_clicked();
    virtual void on_pbSave_clicked();

    virtual void on_pbPromote_clicked();
    virtual void on_pbDemote_clicked();

    virtual void on_pbAddItem_clicked();
    virtual void on_pbDeleteItem_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Child data model
    RB_MmProxy* mChildModel;

};

#endif // ACC_STOCKMASTERWIDGET_H
