/*****************************************************************
 * $Id: scan_scanresultwidget.h 1471 2011-10-20 18:42:46Z rutger $
 * Created: 2011-04-28 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_SCANRESULTWIDGET_H
#define SCAN_SCANRESULTWIDGET_H

#include "rb_widget.h"
#include "ui_scan_scanresultwidget.h"

class SCAN_ScanResultWidget : public RB_Widget, private Ui::SCAN_ScanResultWidget {

    Q_OBJECT

public:
    explicit SCAN_ScanResultWidget(QWidget *parent = 0);
    virtual ~SCAN_ScanResultWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveSCAN; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void fileRevert();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);

    virtual bool saveFile(const RB_String &fn);

public slots:
    virtual void on_pbAddScan_clicked();
    virtual void on_pbDeleteScan_clicked();
    virtual void on_pbSaveScan_clicked();
    virtual void on_pbRevertScan_clicked();

    virtual void on_pbSelectScanType_clicked();
    virtual void on_pbSelectPerson_clicked();

    virtual void on_pbAddQuestion_clicked();
    virtual void on_pbDeleteQuestion_clicked();

    virtual void on_pbRefreshScore_clicked();
    virtual void on_pbRefreshReport_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    bool uploadScan(RB_ObjectBase* scan);
    void printWidget(QPrinter* pr);

    RB_MmProxy* mScanModel;
    RB_MmProxy* mQuestionModel;
    RB_MmProxy* mAnswerModel;

    RB_DataWidgetMapper* mScanMapper;
    RB_DataWidgetMapper* mAnswerMapper;

    RB_String mCbStyle; // combobox read only style appearance as a lineEdit
};

#endif // SCAN_SCANRESULTWIDGET_H
