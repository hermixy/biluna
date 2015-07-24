/*****************************************************************
 * $Id: peng_equipmentmasterwidget.h 1759 2012-09-01 20:29:36Z rutger $
 * Created: Feb 16, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_EQUIPMENTMASTERWIDGET_H
#define PENG_EQUIPMENTMASTERWIDGET_H

#include "rb_widget.h"
#include "ui_peng_equipmentmasterwidget.h"

/**
 * Equipment master widget, part of the navigation dockwidget
 */
class PENG_EquipmentMasterWidget : public RB_Widget,
                                    private Ui::PENG_EquipmentMasterWidget {
    Q_OBJECT

public:
    PENG_EquipmentMasterWidget(QWidget *parent = 0);
    virtual ~PENG_EquipmentMasterWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Equipment Master[*]"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Equipment master model
    RB_MmProxy* mModel;

};

#endif // PENG_EQUIPMENTMASTERWIDGET_H
