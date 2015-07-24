/*****************************************************************
 * $Id: acc_orderreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ORDERREPORTWIDGET_H
#define ACC_ORDERREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_orderreportwidget.h"

class RB_ObjectBase;
class RB_ObjectContainer;

/**
 * Sales and purchase order report widget
 */
class ACC_OrderReportWidget : public RB_Widget, private Ui::ACC_OrderReportWidget {

    Q_OBJECT

public:
    explicit ACC_OrderReportWidget(QWidget *parent = 0);
    virtual ~ACC_OrderReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teOrder; }

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

    virtual void setPurchaseOrder();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void createTable(RB_ObjectContainer* orderList);
    void addHeaderRow(RB_ObjectBase* obj);
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();
    void addFooter();

    RB_String mHtml;
    RB_String mFromPrd;
    RB_String mToPrd;
    RB_String mOrderNumber;
    double mOrderTotal;
    double mOrderGrandTotal;
    double mInvoicedTotal;
    double mInvoicedGrandTotal;
    bool mIsPurchaseOrder;

};

#endif // ACC_ORDERREPORTWIDGET_H
