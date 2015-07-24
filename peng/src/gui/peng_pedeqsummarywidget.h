/*****************************************************************
 * $Id: peng_pedeqsummarywidget.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: Mar 2, 2011 10:00:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PEDEQSUMMARYWIDGET_H
#define PENG_PEDEQSUMMARYWIDGET_H

#include "peng_determinepedclass.h"
#include "rb_widget.h"
#include "ui_peng_pedeqsummarywidget.h"

class PENG_NavigationWidget;

/**
 * View PED equipment summary
 * TODO: implement setModel(RB_MmProxy* m) before init() f
 * or Line Designation Table use
 */
class PENG_PedEqSummaryWidget : public RB_Widget, private Ui::PENG_PedEqSummaryWidget {

    Q_OBJECT

public:
    explicit PENG_PedEqSummaryWidget(QWidget *parent = 0);
    virtual ~PENG_PedEqSummaryWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return tr("PED Equipment and Pipe Line Summary"); }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectivePENG; }
    virtual RB_String getSaveAsFileName();

    virtual void init();
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);
    virtual RB_String getHelpSubject() const;
    virtual bool saveFile(const RB_String& fn);
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();
    virtual void slotEquipmentNavigated(int);

protected:
    void changeEvent(QEvent *e);

private:
    void createTable();
    void createTableHelper(QSqlQuery& q);
    void addEqDataRow(const RB_String& number,
                    const RB_String& eqname,
                    const RB_String& type,
                    const RB_String& fluid1,
                    const RB_String& fluid2,
                    const RB_String& fluid3,
                    const RB_String& fluid4,
                    const RB_String& fluidState,
                    const RB_String& fluidGroup,
                    double maxPress,
                    double maxTemp,
                    double maxVol);
    void calcPedClass(int row);

    RB_String mHtml;
    bool mSelected;
    int mIsEquipmentLine;
    bool mIsEquipment;
    PENG_NavigationWidget* mNavWdgt;
    PENG_DeterminePedClass mOperPed;
};

#endif // PENG_PEDEQSUMMARYWIDGET_H
