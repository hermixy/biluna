/*****************************************************************
 * $Id: acc_costcenterreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jul 9, 2012 6:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTCENTERREPORTWIDGET_H
#define ACC_COSTCENTERREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_costcenterreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View cost center report widget
 */
class ACC_CostCenterReportWidget : public RB_Widget, private Ui::ACC_CostCenterReportWidget {

    Q_OBJECT

public:
    explicit ACC_CostCenterReportWidget(QWidget *parent = 0);
    virtual ~ACC_CostCenterReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Cost Center Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teCostCenter; }

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
    virtual void slotPeriodSelectionChanged(int index);

protected:
    void changeEvent(QEvent *e);

private:
    void createTable(RB_ObjectContainer* trialBalanceList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();
    void addFooter();

    void setHtmlTableCell(const RB_String& cellTag, double amt);

    RB_String mHtml;
    int mFromPrd;
    int mToPrd;
    int mPrd3;
    RB_String mGroupName;

    double mGroupBudget1;
    double mGroupForecast1;
    double mGroupActual1;
    double mGroupBudget2;
    double mGroupForecast2;
    double mGroupActual2;
    double mGroupBudget3;
    double mGroupForecast3;
    double mGroupActual3;

    double mTotalBudget1;
    double mTotalForecast1;
    double mTotalActual1;
    double mTotalBudget2;
    double mTotalForecast2;
    double mTotalActual2;
    double mTotalBudget3;
    double mTotalForecast3;
    double mTotalActual3;
};

#endif // ACC_COSTCENTERREPORTWIDGET_H
