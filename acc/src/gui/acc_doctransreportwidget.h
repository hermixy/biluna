/*****************************************************************
 * $Id: acc_doctransreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 10, 2010 9:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_DOCTRANSREPORTWIDGET_H
#define ACC_DOCTRANSREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_doctransreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View document transaction report widget
 */
class ACC_DocTransReportWidget : public RB_Widget, private Ui::ACC_DocTransReportWidget {

    Q_OBJECT

public:
    explicit ACC_DocTransReportWidget(QWidget *parent = 0);
    virtual ~ACC_DocTransReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Document Transactions Report"; }
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

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void createTable(RB_ObjectContainer* trialBalanceList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addErrorRow();

    //! Report HTML
    RB_String mHtml;
    //! Document id, to remember when document is changing
    RB_String mDocId;
    //! Document date and number
    RB_String mDocInfo;
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

    QColor mAlternateBaseColor;
    QColor mBaseColor;
    QColor mTextColor;
};

#endif // ACC_DOCTRANSREPORTWIDGET_H
