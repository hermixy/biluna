/*****************************************************************
 * $Id: db_prepostfixtextdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jul 13, 2011 15:25:40 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PREPOSTFIXTEXT_H
#define DB_PREPOSTFIXTEXT_H

#include <QtWidgets>
#include "rb_dialog.h"
#include "ui_db_prepostfixtextdialog.h"

/**
 * Dialog to set prefix and postfix for textlines, such as file names
 */
class DB_EXPORT DB_PrePostfixTextDialog : public RB_Dialog,
        private Ui::DB_PrePostfixTextDialog {

    Q_OBJECT

public:
    DB_PrePostfixTextDialog(QWidget* parent);
    virtual ~DB_PrePostfixTextDialog();

    void init() { }
    RB_String getPrefix();
    RB_String getPostfix();

protected slots:
    void on_pbHelp_clicked();
    void on_pbOk_clicked();
    void on_pbCancel_clicked();
};

#endif // DB_PREPOSTFIXTEXT_H
