/*****************************************************************
 * $Id: db_permissionwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 2, 2013 8:27 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONWIDGET_H
#define DB_PERMISSIONWIDGET_H

#include "rb_widget.h"

namespace Ui {
class DB_PermissionWidget;
}

/**
 * Set permissions of database users. On one side there is the
 * permission project and permission group, and at the other side
 * there are the system users and the system user groups. The system group
 * is the connecting link between the two. A system user group is member
 * of a system group and a system group is assigned to a permission group.
 * This creates the possibility of a many to many relationship and to
 * switch on/off a group at one location.
 */
class DB_EXPORT DB_PermissionWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit DB_PermissionWidget(QWidget* parent = 0);
    ~DB_PermissionWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB User permission"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:


protected:
    void changeEvent(QEvent *e);

protected slots:
    void slotProjectAdded();
    void slotGroupAdded();
    void slotUserAdded();
    void slotUserGroupAdded();
    void slotUserPassWordChanged();
    void slotUserCurrentRowChanged(QModelIndex current, QModelIndex previous);
    void slotUserRootChanged();

    void slotCheckDuplicateUserName();
    void slotCheckDuplicatePerspectiveProject();
    void slotCheckDuplicateProjectGroup();
    void slotCheckDuplicateUserGroup();

private:
    void setTableColumnHide(RB_TableView* tv, int columnCount);

    RB_MmProxy* mPermissionProjectModel;
    RB_DataWidgetMapper* mPermissionProjectMapper;
    RB_MmProxy* mPermissionGroupModel;
    RB_DataWidgetMapper* mPermissionGroupMapper;
    RB_MmProxy* mSystemUserModel;
    RB_DataWidgetMapper* mSystemUserMapper;
    RB_MmProxy* mSystemUserGroupModel;
    RB_DataWidgetMapper* mSystemUserGroupMapper;

    Ui::DB_PermissionWidget* ui;
};

#endif // DB_PERMISSIONWIDGET_H
