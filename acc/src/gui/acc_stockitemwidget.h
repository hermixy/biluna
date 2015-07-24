/*****************************************************************
 * $Id: acc_stockitemwidget.h 1843 2012-12-22 12:31:51Z rutger $
 * Created: Jan 16, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKITEMWIDGET_H
#define ACC_STOCKITEMWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_stockitemwidget.h"


class ACC_StockItemWidget : public RB_Widget, private Ui::ACC_StockItemWidget {

    Q_OBJECT

public:
    ACC_StockItemWidget(QWidget *parent = 0);
    virtual ~ACC_StockItemWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Stock Item"; }
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
    virtual void on_pbAddImage_clicked();
    virtual void on_pbDeleteImage_clicked();
    virtual void on_ileCostCenter_clicked();
    virtual void slotResetWidgets();

protected:
    void changeEvent(QEvent *e);

private:
    //! Stockmaster data model
    RB_MmProxy* mModel;
    //! Stockmaster data widget mapper
    RB_DataWidgetMapper* mMapper;

};


#endif // ACC_STOCKITEMWIDGET_H
