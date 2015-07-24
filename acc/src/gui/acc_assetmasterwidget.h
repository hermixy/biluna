/*****************************************************************
 * $Id: acc_assetmasterwidget.h 2010 2013-10-04 20:58:40Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETMASTERWIDGET_H
#define ACC_ASSETMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_acc_assetmasterwidget.h"


/**
 * Asset widget, part of the navigation dockwidget
 */
class ACC_AssetMasterWidget : public RB_Widget, private Ui::ACC_AssetMasterWidget {

    Q_OBJECT

public:
    ACC_AssetMasterWidget(QWidget *parent = 0);
    virtual ~ACC_AssetMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Asset Master"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modelType) const;

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setDefaultValues(int row, const QModelIndex& idxParent);

    //! Data model
    RB_MmProxy* mModel;

};

#endif // ACC_ASSETMASTERWIDGET_H
