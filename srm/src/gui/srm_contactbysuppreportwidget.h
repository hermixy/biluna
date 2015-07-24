/*****************************************************************
 * $Id: srm_contactbysuppreportwidget.h 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 7, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CONTACTBYSUPPREPORTWIDGET_H
#define SRM_CONTACTBYSUPPREPORTWIDGET_H

#include "srm_reportwidget.h"
#include "ui_srm_contactbysuppreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View contact by customer report widget
 */
class SRM_ContactBySuppReportWidget : public SRM_ReportWidget, private Ui::SRM_ContactBySuppReportWidget {

    Q_OBJECT

public:
    explicit SRM_ContactBySuppReportWidget(QWidget *parent = 0);
    virtual ~SRM_ContactBySuppReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Contact by Supplier Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }
    virtual QWidget* getMainWidget() { return mTeReport; }

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

    virtual RB_String getHelpSubject() const;

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setComboBoxes();

};

#endif // SRM_CONTACTBYSUPPREPORTWIDGET_H
