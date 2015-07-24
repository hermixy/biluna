/*****************************************************************
 * $Id: acc_trialbalancewidget.h 2132 2014-03-29 09:59:36Z rutger $
 * Created: Oct 13, 2010 9:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TRIALBALANCEWIDGET_H
#define ACC_TRIALBALANCEWIDGET_H

#include "rb_widget.h"
#include "ui_acc_trialbalancewidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View trial balance widget
 */
class ACC_TrialBalanceWidget : public RB_Widget, private Ui::ACC_TrialBalanceWidget {

    Q_OBJECT

public:
    explicit ACC_TrialBalanceWidget(QWidget *parent = 0);
    virtual ~ACC_TrialBalanceWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Trial Balance"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teTrialBalance; }

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

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void createTable(RB_ObjectContainer* trialBalanceList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();
    void addResultRow();
    void addFooter();

    RB_String mHtml;
    int mFromPrd;
    int mToPrd;
    RB_String mGroupName;

    double mGroupSumDebit;
    double mGroupSumCredit;
    double mGroupTrialDebit;
    double mGroupTrialCredit;
    double mGroupPandlDebit;
    double mGroupPandlCredit;
    double mGroupBalDebit;
    double mGroupBalCredit;

    double mTotalSumDebit;
    double mTotalSumCredit;
    double mTotalTrialDebit;
    double mTotalTrialCredit;
    double mTotalPandlDebit;
    double mTotalPandlCredit;
    double mTotalBalDebit;
    double mTotalBalCredit;

};

#endif // ACC_TRIALBALANCEWIDGET_H
