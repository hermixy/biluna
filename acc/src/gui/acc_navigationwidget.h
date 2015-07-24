/*****************************************************************
 * $Id: acc_navigationwidget.h 2146 2014-05-23 20:27:06Z rutger $
 * Created: Apr 7, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_NAVIGATIONWIDGET_H
#define ACC_NAVIGATIONWIDGET_H

#include "rb_widget.h"
#include "ui_acc_navigationwidget.h"

class ACC_NavigationWidget : public RB_Widget, private Ui::ACC_NavigationWidget {

    Q_OBJECT

public:
    ACC_NavigationWidget(QWidget *parent = 0);
    virtual ~ACC_NavigationWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Navigation"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool closeWidget() { return true; }

    // TODO: required? can be asked via model
    virtual QModelIndex getCurrentModelIndex(int modelType) const;

    enum TabType {
        TabCustomer,
        TabSupplier,
        TabAsset,
        TabStockItem
    };

public slots:
    void slotShowTab(int tabType);

protected:
    void changeEvent(QEvent *e);

private:
    //! Asset widget
    RB_Widget* mAssetMasterWidget;
    //! Customer and branch widget
    RB_Widget* mCustMasterWidget;
    //! Stock Category and stock item widget
    RB_Widget* mStockCategoryWidget;
    //! Supplier widget
    RB_Widget* mSupplierMasterWidget;

};

#endif // ACC_NAVIGATIONWIDGET_H
