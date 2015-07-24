/*****************************************************************
 * $Id: peng_paintdialog.h 2163 2014-09-04 08:03:35Z rutger $
 *
 * Copyright (C) 2005 Red-Bag. All rights reserved.
 * This file is part of the LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PAINTDIALOG_H
#define PENG_PAINTDIALOG_H

#include "rb_dialog.h"
#include "ui_peng_paintdialog.h"


/**
 * Edit paint systems in paint specification
 */
class PENG_PaintDialog : public RB_Dialog, private Ui::PENG_PaintDialog {

    Q_OBJECT

public:
    explicit PENG_PaintDialog(QWidget *parent = 0);
    virtual ~PENG_PaintDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Paint System"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void slotPbAdd_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    //! Paint model
    RB_MmProxy* mModel;
    //! Data widget mapper for paint model
    RB_DataWidgetMapper* mMapper;

};

#endif // PENG_PAINTDIALOG_H
