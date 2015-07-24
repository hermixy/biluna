/*****************************************************************
 * $Id: peng_pedwidget.h 2145 2014-05-18 10:45:32Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PEDWIDGET_H
#define PENG_PEDWIDGET_H

#include "peng_pedtable.h"
#include "rb_widget.h"
#include "ui_peng_pedwidget.h"

class GV_Drawing;
class PENG_NavigationWidget;
class RB_DataWidgetMapper;
class QWebView;

/**
 * Widget for the detail PED classification of equipment and pipe lines
 *
 * TODO: this terrible double coding for equipment and pipe lines was
 * necessary due to undefined and unwanted behavior of the signal/slot
 * mechanism. In a code cleanup project this widget has to be revised
 * 2011-04-11 rutger
 */
class PENG_PedWidget : public RB_Widget, private Ui::PENG_PedWidget {
	
    Q_OBJECT

public:
    PENG_PedWidget(QWidget *parent);
    virtual ~PENG_PedWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "PED Classification"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }
    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbPreviousEq_clicked();
    virtual void on_pbNextEq_clicked();
    virtual void on_pbSaveEq_clicked();
    virtual void on_pbRevertEq_clicked();
    virtual void on_pbPreviousLine_clicked();
    virtual void on_pbNextLine_clicked();
    virtual void on_pbSaveLine_clicked();
    virtual void on_pbRevertLine_clicked();

protected slots:
    virtual void slotClearFields(const QModelIndex&, const QModelIndex&);
//    virtual void slotClearFields(const QModelIndex&);
    virtual void slotEnsureRelevantRow();
    virtual void slotEnsureRelevantRowLine();
    virtual void slotResetWidgets();

    // Format for autoconnection slots:
    //   void on_<widget name>_<signal name>(<signal parameters>);
    void on_ileEquipmentType_clicked();
    void on_pbCalculate_clicked();
    void on_pbCalculateLine_clicked();
//    void on_pbHelp_clicked(); // dummy for code see .cpp file
    void reject();

    void slotNavigationTabChanged(int tab);

private:
    void initEquipment();
    void initPipeLine();
	void setDialogFields();
    void setDialogFieldsLine();
    void printWidget(QPrinter* pr);
    void widgetToHtml(QWebView* webView);
    void widgetToHtml(GV_Drawing* dwg);

    void widgetToHtml(QTextEdit* te);
    void widgetToHtmlLine(QTextEdit* te);

    // Equipment
    PENG_PedTable* mPedTable;
    RB_MmProxy* mModelEquipment;
    RB_DataWidgetMapper* mMapper;
    RB_MmProxy* mPedModel;
    RB_DataWidgetMapper* mPedMapper;
    // Pipe Line
    PENG_PedTable* mPedTableLine;
    RB_MmProxy* mModelLine;
    RB_DataWidgetMapper* mMapperLine;
    RB_MmProxy* mPedModelLine;
    RB_DataWidgetMapper* mPedMapperLine;

    PENG_NavigationWidget* mNavWidget;
};

#endif /*PENG_PEDWIDGET_H*/
