/*****************************************************************
 * $Id: db_modelobjectdialog.h 2210 2015-12-28 14:59:25Z rutger $
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MODELOBJECTDIALOG_H
#define DB_MODELOBJECTDIALOG_H

#include "rb_dialog.h"

namespace Ui {
class DB_ModelObjectDialog;
}
class RB_CmbDelegate;

class DB_ModelObjectDialog : public RB_Dialog {

    Q_OBJECT

public:
    explicit DB_ModelObjectDialog(QWidget* parent = 0);
    ~DB_ModelObjectDialog();

    virtual void init();

public slots:
    void addClicked();
    void deleteClicked();
    void on_pbHelp_clicked();
    void on_pbSave_clicked();
    void on_pbCancel_clicked();

private:
    void createMemberList(RB_ObjectContainer* memberList);
    RB_CmbDelegate* getNewMemberTypeDelegate();

    Ui::DB_ModelObjectDialog* ui;
};

#endif // DB_MODELOBJECTDIALOG_H
