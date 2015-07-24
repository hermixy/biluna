/*****************************************************************
 * $Id: acc_invoiceagedreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Dec 8, 2010 12:29:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_INVOICEAGEDREPORTWIDGET_H
#define ACC_INVOICEAGEDREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_invoiceagedreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View invoice aged report widget, used for customer and supplier
 */
class ACC_InvoiceAgedReportWidget : public RB_Widget, private Ui::ACC_InvoiceAgedReportWidget {

    Q_OBJECT

public:
    explicit ACC_InvoiceAgedReportWidget(QWidget *parent = 0);
    virtual ~ACC_InvoiceAgedReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return te; }

    virtual void init();

    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editSelectAll();
    virtual void editInsertImage(const RB_String& fn);
    virtual void editZoomIn(double factor);
    virtual void editZoomOut(double factor);

    virtual bool saveFile(const RB_String &fn);

    virtual void setDocType(int docType) { mDocType = docType; }

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setComboBoxes();

    void createTable(RB_ObjectContainer* invoiceAgedList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();
    void addFooter();

    //! Report HTML
    RB_String mHtml;
    //! Customer or supplier name
    RB_String mCustSuppName;


    // Summary amounts
    double mSumAmount;
    double mSumAlloc;
    double mSumDue;
    double mSumOverDue1;
    double mSumOverDue2;

    // Totals
    double mTotalAmount;
    double mTotalAlloc;
    double mTotalDue;
    double mTotalOverDue1;
    double mTotalOverDue2;

    //! Document type: 10 = Debtor, 20 = Creditor
    int mDocType;
    int mOverDueDays1;
    int mOverDueDays2;
    RB_String mReportTitle;
};

#endif // ACC_INVOICEAGEDREPORTWIDGET_H
