/*****************************************************************
 * $Id: scan_scaneditdialog.h 1470 2011-10-19 21:06:10Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_SCANEDITDIALOG_H
#define SCAN_SCANEDITDIALOG_H

#include "db_codehighlighter.h"
#include "rb_dialog.h"
#include "ui_scan_scaneditdialog.h"

class SCAN_ScanEditDialog : public RB_Dialog, private Ui::SCAN_ScanEditDialog {

    Q_OBJECT

public:
    explicit SCAN_ScanEditDialog(QWidget *parent = 0);
    virtual ~SCAN_ScanEditDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Scan Edit[*]"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveSCAN; }

    virtual void init();
    virtual bool fileSave(bool withSelect = false);
    virtual void fileRevert();

public slots:
    virtual void on_pbAddScan_clicked();
    virtual void on_pbDeleteScan_clicked();
    virtual void on_pbSaveScan_clicked();
    virtual void on_pbRevertScan_clicked();

    virtual void on_pbAddQuestion_clicked();
    virtual void on_pbDeleteQuestion_clicked();
    virtual void on_pbAddAnswer_clicked();
    virtual void on_pbDeleteAnswer_clicked();

    virtual void on_pbAddBlock_clicked();
    virtual void on_pbAddSubBlock_clicked();
    virtual void on_pbDeleteBlock_clicked();
    virtual void on_pbPromoteBlock_clicked();
    virtual void on_pbDemoteBlock_clicked();

    virtual void on_pbAlignLeft_clicked();
    virtual void on_pbAlignRight_clicked();
    virtual void on_pbAlignCenter_clicked();
    virtual void on_pbAlignJustify_clicked();
    virtual void on_pbBold_clicked();
    virtual void on_pbItalic_clicked();
    virtual void on_pbUnderline_clicked();
    virtual void on_pbStrikethrough_clicked();
    virtual void on_pbIndent_clicked();
    virtual void on_pbOutdent_clicked();
    virtual void on_pbNumberList_clicked();
    virtual void on_pbBulletList_clicked();
    virtual void on_pbFontName_clicked();
    virtual void on_pbFontSize_clicked();
    virtual void on_pbColor_clicked();
    virtual void on_pbBgColor_clicked();

    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

    bool queryCommandState(const QString&);
//    bool pageActionChecked(QWebPage::WebAction wa);
//    bool pageActionEnabled(QWebPage::WebAction wa);

protected slots:
    void slotContentChanged();
    void slotUpdateWebView(const QModelIndex& current,
                           const QModelIndex& previous);

protected:
    void changeEvent(QEvent* e);
    bool eventFilter(QObject* obj, QEvent* e);

private slots:
    void adjustActions();
    void changeTab(int);

private:
    void execCommand(const QString&);
    void execCommand(const QString &cmd, const QString &arg);

    RB_MmProxy* mScanModel;
    RB_MmProxy* mQuestionModel;
    RB_MmProxy* mAnswerModel;
    RB_MmProxy* mBlockModel;

    RB_DataWidgetMapper* mQuestionMapper;
    RB_DataWidgetMapper* mAnswerMapper;
    RB_DataWidgetMapper* mBlockMapper;

    DB_CodeHighlighter* mCodeHighlighter;
    bool mSourceDirty;

};

#endif // SCAN_SCANEDITDIALOG_H
