/*****************************************************************
 * $Id: db_permissionperspectiveprojectdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 2, 2015 17:15:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONPERSPECTIVEPROJECTDIALOG_H
#define DB_PERMISSIONPERSPECTIVEPROJECTDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class DB_PermissionPerspectiveProjectDialog;
}

/**
 * Dialog to select the perspective project for a permission setting.
 * @brief The DB_PermissionPerspectiveProjectDialog class
 */
class DB_EXPORT DB_PermissionPerspectiveProjectDialog : public RB_Dialog {

    Q_OBJECT

public:
    explicit DB_PermissionPerspectiveProjectDialog(QWidget* parent = 0);
    ~DB_PermissionPerspectiveProjectDialog();

    virtual void init();
    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Permission Perspective Projects"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual RB_ObjectBase* getCurrentObject();

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    void setCurrentObject();

    Ui::DB_PermissionPerspectiveProjectDialog* ui;
    RB_MmProxy* mModel;
    RB_ObjectBase* mCurrentObject;
};

#endif // DB_PERMISSIONPERSPECTIVEPROJECTDIALOG_H
