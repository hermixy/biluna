/*****************************************************************
 * $Id: db_projecteditdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
#ifndef DB_PROJECTEDITDIALOG_H
#define DB_PROJECTEDITDIALOG_H

#include "rb_projecteditdialog.h"


/**
 * Edit DB project dialog
 */
class DB_EXPORT DB_ProjectEditDialog : public RB_ProjectEditDialog {

    Q_OBJECT

public:
    DB_ProjectEditDialog(QWidget* parent);
    virtual ~DB_ProjectEditDialog();

    virtual void init();

public slots:
    virtual void on_pbAdd_clicked();

};

#endif // DB_PROJECTEDITDIALOG_H
