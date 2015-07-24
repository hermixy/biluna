/*****************************************************************
 * $Id: peng_pipeclassdialog.h 2185 2014-10-09 15:09:35Z rutger $
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PIPECLASSDIALOG_H
#define PENG_PIPECLASSDIALOG_H

#include "rb_dialog.h"
#include "ui_peng_pipeclassdialog.h"


/**
 * Edit pipe class specification
 */
class PENG_PipeClassDialog : public RB_Dialog, private Ui::PENG_PipeClassDialog {

    Q_OBJECT

public:
    explicit PENG_PipeClassDialog(QWidget *parent = 0);
    ~PENG_PipeClassDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PENG Pipe Class Specification"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    RB_MmProxy* mModelPipeClass;
    RB_MmProxy* mModelLineSize;
    RB_MmProxy* mModelDesignPT;
    // no mappers required

};

#endif // PENG_PIPECLASSDIALOG_H
