/*****************************************************************
 * $Id: acc_assetwidget.h 1843 2012-12-22 12:31:51Z rutger $
 * Created: Oct 24, 2011 1:24:25 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETWIDGET_H
#define ACC_ASSETWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_assetwidget.h"

class RB_MmProxy;

class ACC_AssetWidget : public RB_Widget, private Ui::ACC_AssetWidget {

    Q_OBJECT

public:
    ACC_AssetWidget(QWidget *parent = 0);
    virtual ~ACC_AssetWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Asset"; }
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
    virtual void slotResetWidgets();

protected:
    void changeEvent(QEvent* e);

private:
    //! Asset data model
    RB_MmProxy* mAssetModel;
    //! Asset data widget mapper
    RB_DataWidgetMapper* mAssetMapper;

};

#endif // ACC_ASSETWIDGET_H
