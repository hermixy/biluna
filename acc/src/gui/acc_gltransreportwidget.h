/*****************************************************************
 * $Id: acc_gltransreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 9, 2010 9:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_GLTRANSREPORTWIDGET_H
#define ACC_GLTRANSREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_gltransreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View GL transaction report widget
 */
class ACC_GlTransReportWidget : public RB_Widget, private Ui::ACC_GlTransReportWidget {

    Q_OBJECT

public:
    explicit ACC_GlTransReportWidget(QWidget *parent = 0);
    virtual ~ACC_GlTransReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC GL Transactions Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teGlTrans; }

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
    void createTable(RB_ObjectContainer* glTransactionList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();

    //! Report HTML
    RB_String mHtml;
    //! GL account code number, to remember when account is changing
    RB_String mAccountCode;
    RB_String mAccountName;
    //! Debit summary for GL account
    double mSumDebit;
    //! Credit summary for GL account
    double mSumCredit;
    //! Map of chartmaster (GL accounts), for translation id to code
    std::map<RB_String, RB_String> mChartMasterMap;
    //! Start date of period
    QDate mStartDate;
    //! End date of period
    QDate mEndDate;
};

#endif // ACC_GLTRANSREPORTWIDGET_H
