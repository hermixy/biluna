/*****************************************************************
 * $Id: acc_salestaxreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 20, 2010 9:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESTAXREPORTWIDGET_H
#define ACC_SALESTAXREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_salestaxreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View sales tax report widget
 */
class ACC_SalesTaxReportWidget : public RB_Widget, private Ui::ACC_SalesTaxReportWidget {

    Q_OBJECT

public:
    explicit ACC_SalesTaxReportWidget(QWidget *parent = 0);
    virtual ~ACC_SalesTaxReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Sales Tax Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teTax; }

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

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void createTable(RB_ObjectContainer* taxList);
    void addDataRow(RB_ObjectBase* obj);
    void addSumRow(RB_ObjectBase* obj);
    void addErrorRow(const RB_String& errorMessage);
    void dutchSalesTaxItem(int acc, RB_String& itemNr, RB_String& subject);

    //! Report HTML
    RB_String mHtml;
    //! Document id, to remember when document is changing
//    RB_String mDocId;
    //! Document date and number
//    RB_String mDocInfo;
    //! Debit summary for GL account
//    double mSumDebit;
    //! Credit summary for GL account
//    double mSumCredit;
    //! Map of chartmaster (GL accounts), for translation id to code
//    std::map<RB_String, RB_String> mChartMasterMap;
    //! Previous object for the generation of summary report
//    RB_ObjectBase* mPrevObj;
    //! Summary object for the generation of summary report
    RB_ObjectBase* mSumObj;
    //! Summary amounts excluding tax
    double mSumAmount;
    //! Summary current tax amounts
    double mSumTaxAmount;
    //! Total tax payable
    double mSumTaxTotal;
    //! Tax to be reversed in 5b due to 4a, 4b or function 45
    double mSumTaxRev;
};

#endif // ACC_SALESTAXREPORTWIDGET_H
