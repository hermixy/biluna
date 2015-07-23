/*****************************************************************
 * $Id: db_aboutdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 26, 2009 4:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ABOUTDIALOG_H
#define DB_ABOUTDIALOG_H

#include "rb_dialog.h"

#include "ui_db_aboutdialog.h"

/**
 * About this application dialog in Help menu
 */
class DB_EXPORT DB_AboutDialog: public RB_Dialog, private Ui::DB_AboutDialog {

    Q_OBJECT

public:
    DB_AboutDialog(QWidget* parent = NULL);
    virtual ~DB_AboutDialog();

    void setText(const QString& s);
    void setIcon(const QPixmap& i);
};

#endif

