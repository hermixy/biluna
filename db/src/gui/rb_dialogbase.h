/*****************************************************************
 * $Id: rb_dialogbase.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 26, 2009 8:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef RB_DIALOGBASE_H
#define RB_DIALOGBASE_H

#include "rb_dialog.h"
#include "rb_mmproxy.h"
#include "rb_mainwindow.h"
#include "ui_rb_dialogbase.h"


/**
 * Abstract base dialog using a model for the editing or selection from a table
 */
class DB_EXPORT RB_DialogBase : public RB_Dialog, protected Ui::RB_DialogBase {

    Q_OBJECT

public:
    RB_DialogBase(QWidget* parent);
    virtual ~RB_DialogBase();

    virtual void init() { }
    virtual bool fileSave(bool withSelect = false);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);
    virtual QModelIndex getCurrentModelIndex() const;
    virtual RB_MmProxy* getModel();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbUp_clicked();
    virtual void on_pbDown_clicked();
    virtual void on_pbFind_clicked();
    virtual void on_pbFilter_clicked();
    virtual void on_pbFilterClear_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();

    virtual void changeEvent(QEvent* e);
    virtual void slotSourceFilterChange(int /*number*/) { }

protected:
    virtual void initDialog();
    virtual void initEditSort();
    virtual void initEditUpDown();
    virtual void initSelectionOnly();
    virtual void initTest();

    RB_MmProxy* mModel;
    RB_MainWindow* mMainWindow;
    RB_String mHelpSubject;

    QModelIndex mCurrentModelIndex;

};

#endif // RB_DIALOGBASE_H
