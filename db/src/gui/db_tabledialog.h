/*****************************************************************
 * $Id: db_tabledialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Aug 8, 2010 8:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef DB_TABLEDIALOG_H
#define DB_TABLEDIALOG_H

#include "rb_dialog.h"
#include "rb_mmproxy.h"
#include "rb_mainwindow.h"
#include "ui_db_tabledialog.h"


/**
 * Base dialog using a model for the editing or selection from a table
 */
class DB_EXPORT DB_TableDialog : public RB_Dialog, protected Ui::DB_TableDialog {

    Q_OBJECT

public:
    DB_TableDialog(QWidget* parent);
    virtual ~DB_TableDialog();

    virtual void init() { }
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);
    //virtual QModelIndex getCurrentModelIndex() const;
    virtual RB_ObjectBase* getCurrentObject();

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
    virtual void on_pbCancel_clicked();

protected:
    virtual void initEditSort();
    virtual void initEditUpDown();
    virtual void initEditOnly();
    virtual void initSelectionOnly();
    virtual void initTest();

    void setCurrentObject();
    void changeEvent(QEvent* e);

    RB_MmProxy* mModel;
    RB_String mHelpSubject;

    QModelIndex mCurrentModelIndex;
    RB_ObjectBase* mCurrentObject;

};

#endif // DB_TABLEDIALOG_H
