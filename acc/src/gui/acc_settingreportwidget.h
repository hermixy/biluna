/*****************************************************************
 * $Id: acc_settingreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 20, 2011 12:29:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SETTINGREPORTWIDGET_H
#define ACC_SETTINGREPORTWIDGET_H

#include "rb_widget.h"
#include "ui_acc_settingreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * Settings report
 * TODO: create the reports based on the tableView and treeView of the
 * corresponding widgets. The report will then include the fixed text
 * of the comboboxes
 */
class ACC_SettingReportWidget : public RB_Widget, private Ui::ACC_SettingReportWidget {

    Q_OBJECT

public:
    explicit ACC_SettingReportWidget(QWidget *parent = 0);
    virtual ~ACC_SettingReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Settings Report"; }
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

    virtual bool saveFile(const RB_String& fn);

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setComboBox();

    void createTable();
    void createHeader();
    void createSql(int reportIdx);
    void createSqlHeaderColumns(const QSqlRecord& rec);
    void createChildTable();

    void createAccountGroupTable();
    void createChartMasterTable();
    void createPriceTable();
    void createCostCenterGroupTable();

    void addHeaderRow();
    void addDataRow(int row);
    void addSqlDataRow(const QSqlRecord& rec);
    void addFooterRow();
    void addFooter();

    RB_MmProxy* mModel;
    RB_MmProxy* mChildModel;

    int mColCount;
    int mStartColumn;

    //! Report HTML
    RB_String mHtml;
    RB_String mReportTitle;
};

#endif // ACC_SETTINGREPORTWIDGET_H
