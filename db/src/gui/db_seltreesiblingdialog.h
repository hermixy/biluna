/*****************************************************************
 * $Id: db_seltreesiblingdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 30, 2011 2:34:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef DB_SELTREESIBLINGDIALOG_H
#define DB_SELTREESIBLINGDIALOG_H

#include "rb_dialog.h"
#include "ui_db_seltreesiblingdialog.h"

class RB_MmProxy;

/**
 * Dialog to select a sibling from the tree of items.
 * Used when a tree item is being demoted.
 */
class DB_EXPORT DB_SelTreeSiblingDialog : public RB_Dialog, protected Ui::DB_SelTreeSiblingDialog {

    Q_OBJECT

public:
    DB_SelTreeSiblingDialog(QWidget* parent);
    virtual ~DB_SelTreeSiblingDialog();

    virtual void init();
    virtual void setSiblingList(RB_ObjectContainer* sList);
    virtual void setHideId(const RB_String& id);
    virtual RB_String getCurrentId() const;

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();
    virtual void reject();

protected:
    void changeEvent(QEvent* e);

    RB_ObjectContainer* mSiblingList;
    RB_MmProxy* mModel;

    RB_String mCurrentId;
    RB_String mHideId;

};

#endif // DB_SELTREESIBLINGDIALOG_H
