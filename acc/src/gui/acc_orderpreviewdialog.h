/*****************************************************************
 * $Id: acc_orderpreviewdialog.h 2061 2014-01-12 16:34:14Z rutger $
 * Created: Jul 14, 2011 12:00:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ORDERPREVIEWDIALOG_H
#define ACC_ORDERPREVIEWDIALOG_H

#include "rb_dialog.h"
#include "rb_printtextheaderfooter.h"
#include "ui_acc_orderpreviewdialog.h"


/**
 * Dialog for preview/print sales order and invoice, purchase order
 */
class ACC_OrderPreviewDialog : public RB_Dialog,
        private Ui::ACC_OrderPreviewDialog {

    Q_OBJECT

public:
    ACC_OrderPreviewDialog(QWidget *parent = 0);
    virtual ~ACC_OrderPreviewDialog();

    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void setHtml(const RB_String& content);

    virtual void setPrinterProperties(QPrinter* pr);

public slots:
    virtual void on_pbPrint_clicked();
    virtual void on_pbSavePdf_clicked();
    virtual void on_pbSaveHtml_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void printContent(QPrinter* pr);
    void prepareContent(QPrinter* pr);

    RB_String mHtml;

    QTextDocument* mTdReportHeader;
    QTextDocument* mTdReportFooter;
    QTextDocument* mTdContent;

    int mHeaderHeight;
    int mFooterHeight;

    RB_PrintTextHeaderFooter mUtilPrint;
};

#endif // ACC_ORDERPREVIEWDIALOG_H
