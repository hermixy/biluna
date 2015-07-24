/*****************************************************************
 * $Id: acc_stockcategorywidget.h 2183 2014-10-02 14:33:40Z rutger $
 * Created: Jan 16, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_STOCKCATEGORYWIDGET_H
#define ACC_STOCKCATEGORYWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_stockcategorywidget.h"


class ACC_StockCategoryWidget : public RB_Widget, private Ui::ACC_StockCategoryWidget {

    Q_OBJECT

public:
    ACC_StockCategoryWidget(QWidget *parent = 0);
    virtual ~ACC_StockCategoryWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Stock Category"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbSave_clicked();
    virtual void on_pbRevert_clicked();
    virtual void on_ileStockGlCode_clicked();
    virtual void on_ileWipGlCode_clicked();
    virtual void on_ileStockAdjustmentGlCode_clicked();
    virtual void on_ilePriceVarianceGlCode_clicked();
    virtual void on_ileUsageVarianceGlCode_clicked();
    virtual void slotResetWidgets();

protected:
    void changeEvent(QEvent *e);

private:
    void setAccount(const RB_String& fieldName);

    //! Stockcategory data model
    RB_MmProxy* mModel;
    //! Stockcategory data widget mapper
    RB_DataWidgetMapper* mMapper;

};


#endif // ACC_STOCKCATEGORYWIDGET_H
