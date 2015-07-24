/*****************************************************************
 * $Id: acc_reportpreviewdialog.h 1744 2012-08-22 17:47:26Z rutger $
 * Created: Mar 20, 2010 12:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_REPORTPREVIEWDIALOG_H
#define ACC_REPORTPREVIEWDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_reportpreviewdialog.h"

class DB_HtmlViewWidget;

class ACC_ReportPreviewDialog : public RB_Dialog,
        private Ui::ACC_ReportPreviewDialog {

    Q_OBJECT

public:
    ACC_ReportPreviewDialog(QWidget *parent = 0);
    virtual ~ACC_ReportPreviewDialog();

    virtual void init();
    virtual void setHtml(const RB_String& content);

public slots:
    virtual void on_pbPrint_clicked();
    virtual void on_pbSavePdf_clicked();
    virtual void on_pbSaveHtml_clicked();

protected:
    void changeEvent(QEvent *e);

};

#endif // ACC_REPORTPREVIEWDIALOG_H
