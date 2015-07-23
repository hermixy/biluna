/*****************************************************************
 * $Id: db_systemgroupdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 18, 2014 10:24:42 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SYSTEMGROUPDIALOG_H
#define DB_SYSTEMGROUPDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class DB_SystemGroupDialog;
}

/**
 * Dialog to edit or select the system group. The system group
 * will be assigned permissions. System users can be part of
 * a system group.
 * @brief The DB_SystemGroupDialog class
 */
class DB_EXPORT DB_SystemGroupDialog : public RB_Dialog {

    Q_OBJECT

public:
    explicit DB_SystemGroupDialog(QWidget* parent = 0);
    virtual ~DB_SystemGroupDialog();

    virtual void init();
    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB System Group"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual RB_ObjectBase* getCurrentObject();

public slots:
    virtual void slotPbAdd_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    void setCurrentObject();

    Ui::DB_SystemGroupDialog* ui;

    RB_MmProxy* mModel;
    RB_DataWidgetMapper* mMapper;
    RB_ObjectBase* mCurrentObject;
};

#endif // DB_SYSTEMGROUPDIALOG_H
