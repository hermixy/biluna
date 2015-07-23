/*****************************************************************
 * $Id: rb_projecteditdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef RB_EDITPROJECTDIALOG_H
#define RB_EDITPROJECTDIALOG_H

#include "rb_dialog.h"
#include "rb_mmproxy.h"
#include "rb_mainwindow.h"
#include "ui_rb_projecteditdialog.h"


/**
 * Base edit project dialog using a model for the editing only
 */
class DB_EXPORT RB_ProjectEditDialog : public RB_Dialog, protected Ui::RB_ProjectEditDialog {

    Q_OBJECT

public:
    RB_ProjectEditDialog(QWidget* parent);
    virtual ~RB_ProjectEditDialog();

    virtual void init() = 0;
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);

public slots:
    virtual void on_pbAdd_clicked() = 0;
    virtual void on_pbDelete_clicked();
    virtual void on_pbFilter_clicked();
    virtual void on_pbFilterClear_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent* e);

    //! Company (project) model
    RB_MmProxy* mModel;
    //! Data widget mapper for company model
    RB_DataWidgetMapper* mMapper;
    //! Set help subject otherwise base class name will be used
    RB_String mHelpSubject;

};

#endif // RB_EDITPROJECTDIALOG_H
