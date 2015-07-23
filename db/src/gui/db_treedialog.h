/*****************************************************************
 * $Id: db_treedialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Aug 10, 2010 8:34:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef DB_TREEDIALOG_H
#define DB_TREEDIALOG_H

#include "rb_dialog.h"
#include "rb_mmproxy.h"
#include "rb_mainwindow.h"
#include "ui_db_treedialog.h"


/**
 * Base dialog using a model for the editing or selection from a tree
 */
class DB_EXPORT DB_TreeDialog : public RB_Dialog, protected Ui::DB_TreeDialog {

    Q_OBJECT

public:
    DB_TreeDialog(QWidget* parent);
    virtual ~DB_TreeDialog();

    virtual void init() { }
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

    virtual void setHelpSubject(const RB_String& subject);

    virtual RB_ObjectBase* getCurrentObject();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbAddChild_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbFind_clicked();
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

#endif // DB_TREEDIALOG_H
