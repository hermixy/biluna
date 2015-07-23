/*****************************************************************
 * $Id: db_inserthtmldialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Apr 24, 2012 4:25:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_INSERTHTMLDIALOG_H
#define DB_INSERTHTMLDIALOG_H

#include "db_htmlhighlighter.h"
#include "rb_dialog.h"
#include "ui_db_inserthtmldialog.h"



/**
 * Insert HTML dialog
 */
class DB_EXPORT DB_InsertHtmlDialog : public RB_Dialog, private Ui::DB_InsertHtmlDialog {

    Q_OBJECT

public:
    DB_InsertHtmlDialog(QWidget *parent = 0);
    virtual ~DB_InsertHtmlDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Insert HTML"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();
    virtual RB_String getHtml();

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

protected slots:

private:
    DB_HtmlHighlighter* mHighlighter;
};

#endif // DB_INSERTHTMLDIALOG_H
