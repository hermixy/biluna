/*****************************************************************
 * $Id: acc_pricewidget.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Jan 20, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PRICEWIDGET_H
#define ACC_PRICEWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_pricewidget.h"

class RB_MmObjectInterface;

class ACC_PriceWidget : public RB_Widget, private Ui::ACC_PriceWidget {

    Q_OBJECT

public:
    ACC_PriceWidget(QWidget *parent = 0);
    virtual ~ACC_PriceWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Price"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();

    virtual void slotParentSelectionChanged(const QModelIndex& current,
                                            const QModelIndex& previous);

protected:
    void changeEvent(QEvent *e);

private:
    //! Stock parent model, ACC_StockMaster
    RB_MmProxy* mParentModel;
    //! Price model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_PRICEWIDGET_H
