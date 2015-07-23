/*****************************************************************
 * $Id: rb_dialog.h 1234 2010-07-05 21:02:33Z rutger $
 * Created: Aug 4, 2010 9:21:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_FILTERDIALOG_H
#define DB_FILTERDIALOG_H

#include <QDialog>
#include "db_global.h"
#include "ui_db_filterdialog.h"

class DB_EXPORT DB_FilterDialog : public QDialog, protected Ui::DB_FilterDialog {

    Q_OBJECT

public:
    DB_FilterDialog(QWidget *parent = 0);
    virtual ~DB_FilterDialog();

    virtual void addFieldName(const QString& fieldName);
    virtual void setFieldName(const QString& fieldName);
    virtual void clearFieldNames();
    virtual QString getFieldName();

    virtual void setFilterText(const QString& text);
    virtual QString getFilterText();

    virtual void setMatchCase(bool matchCase);
    virtual bool getMatchCase();

protected slots:
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

};

#endif // DB_FILTERDIALOG_H
