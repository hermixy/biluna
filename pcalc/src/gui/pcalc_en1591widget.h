/*****************************************************************
 * $Id: pcalc_en1591widget.h 2241 2015-05-22 10:22:19Z rutger $
 * Created: Apr 7, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_EN1591WIDGET_H
#define PCALC_EN1591WIDGET_H

#include "rb_widget.h"
#include "ui_pcalc_en1591widget.h"


class PCALC_EN1591Widget : public RB_Widget,
                        private Ui::PCALC_EN1591Widget {
    Q_OBJECT

public:
    PCALC_EN1591Widget(QWidget *parent = 0);
    ~PCALC_EN1591Widget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "EN1591 Flange Calculation"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectivePCALC;
    }
    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void fileRevert();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);

    virtual RB_String getHelpSubject() const;
    QTextEdit* getTextEdit();

public slots:
    virtual void slotDataIsChanged(const QModelIndex& topLeft,
                                   const QModelIndex& bottomRight);
    virtual void slotAssemblyAdded();
    virtual void on_pbCalculate_clicked();

protected slots:
    void slotParentRowChanged(const QModelIndex& curr,
                              const QModelIndex& prev);
    void slotHandleParentRowChanged();
    void slotDisableFormulaWidgets(int index);

protected:
    void changeEvent(QEvent *e);

private:
    void initMapping();
    void initConnections();
    void setInput();
    void setSettings();
    void createSummaryReport();
    void createDetailReport();
    void createReport(const QString& reportTemplate);
    void insertReportInputData(QString& report, RB_ObjectBase* obj);
    void insertReportLoadCaseData(QString& report, RB_ObjectBase* obj);
    void insertReportCalculationData(QString& report, RB_ObjectBase* obj);
    void createValidationReport();
    void createUnitTestSummary();
    void createUnitTestDetail();
    void addObjectMemberVariable(RB_ObjectBase* obj,
                                 const QString& variableName,
                                 const QString& unit, RB_MmProxy* model);
    void addLoadCaseVariable(RB_ObjectBase* loadCase,
                             const QString& variableName,
                             const QString& unit,
                             RB_MmProxy* model,
                             int currentRow);
    void updateAllowStress(const QString& materialId,
                           const QString& temperatureField,
                           const QString& allowStressField);
// continue here
//    void updateElastModulus();
//    void updateThermExp();

    RB_MmProxy* mAssemblyModel;
    RB_DataWidgetMapper* mAssemblyMapper;
    RB_MmProxy* mBoltNutWasherModel;
    RB_DataWidgetMapper* mBoltNutWasherMapper;
    RB_MmProxy* mFlangeModel;
    RB_DataWidgetMapper* mFlangeMapper;
    RB_MmProxy* mGasketModel;
    RB_DataWidgetMapper* mGasketMapper;
    RB_MmProxy* mLoadCaseModel;
    RB_DataWidgetMapper* mLoadCaseMapper;
    RB_MmProxy* mShellModel;
    RB_DataWidgetMapper* mShellMapper;

    //! Last selected assembly ID
    RB_String mCurrentId;

};

#endif // PCALC_EN1591WIDGET_H
