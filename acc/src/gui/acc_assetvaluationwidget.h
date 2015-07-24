/*****************************************************************
 * $Id: acc_assetvaluationwidget.h 1616 2012-04-18 20:14:54Z rutger $
 * Created: Nov 7, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ASSETVALUATIONWIDGET_H
#define ACC_ASSETVALUATIONWIDGET_H

#include "rb_widget.h"
#include "ui_acc_assetvaluationwidget.h"

/**
 * Widget (on MDI window) to edit asset customer and supplier attributes.
 * These attributes in combination with the asset attributes are
 * used to make the property or asset valuation report
 */
class ACC_AssetValuationWidget : public RB_Widget, private Ui::ACC_AssetValuationWidget {

    Q_OBJECT

public:
    ACC_AssetValuationWidget(QWidget *parent = 0);
    virtual ~ACC_AssetValuationWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Asset Valuation"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual bool closeWidget();

    enum ChartType {
        ChartCashFlow,
        ChartCumulativeCF,
        ChartNettPresentValue,
        ChartCumulativeNPV
    };

public slots:
    virtual void on_pbSave_clicked();
    virtual void on_pbAddAttrCust_clicked();
    virtual void on_pbDeleteAttrCust_clicked();
    virtual void on_ileCustName_clicked();
    virtual void on_pbAddAttrSuppl_clicked();
    virtual void on_pbDeleteAttrSuppl_clicked();
    virtual void on_ileSupplName_clicked();
    virtual void on_pbRefresh_clicked();

protected:
    virtual void changeEvent(QEvent* e);

protected slots:
    virtual void slotAssetRowChanged(const QModelIndex& current,
                                     const QModelIndex& previous);

private:
    void setBarChart(RB_String& content, RB_ObjectBase* outputList, ChartType type);
    void setValueTable(RB_String& content, const RB_String& heading);

    //! Asset attribute customer model
    RB_MmProxy* mAttrCustModel;
    //! Data widget mapper for asset customer attribute model
    RB_DataWidgetMapper* mAttrCustMapper;
    //! Asset attribute supplier model
    RB_MmProxy* mAttrSupplModel;
    //! Data widget mapper for asset supplier attribute model
    RB_DataWidgetMapper* mAttrSupplMapper;
    //! Asset parent model
    RB_MmProxy* mAssetModel;
    //! Result values for chart
    std::map<int, double> mResultMap;

};

#endif // ACC_ASSETVALUATIONWIDGET_H
