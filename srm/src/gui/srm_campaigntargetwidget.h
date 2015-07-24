/*****************************************************************
 * $Id: srm_campaigntargetwidget.h 2071 2014-01-22 21:56:32Z rutger $
 * Created: Mar 05, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_CAMPAIGNTARGETWIDGET_H
#define SRM_CAMPAIGNTARGETWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_campaigntargetwidget.h"

class RB_MmProxy;
class RB_PrintTextHeaderFooter;

class SRM_CampaignTargetWidget : public RB_Widget, private Ui::SRM_CampaignTargetWidget {

    Q_OBJECT

public:
    SRM_CampaignTargetWidget(QWidget *parent = 0);
    virtual ~SRM_CampaignTargetWidget();

    virtual RB_String getName() const { return "SRM Campaign Target"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void filePdf(QPrinter* pr);
    virtual void filePrint(QPrinter* pr);
    virtual void filePrintPreview(QPrinter* pr);
    virtual void print(QPrinter* pr, bool isPreview);
    virtual void addLogActivity(RB_ObjectBase* doc, RB_ObjectBase* target);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddCampaign_clicked();
    virtual void on_pbDeleteCampaign_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOnCampaign_clicked();
    virtual void on_pbFilterOffCampaign_clicked();
    virtual void on_ileTemplate_clicked();

    virtual void slotUpdateEndDateTime();

    virtual void on_pbAddTarget_clicked();
    virtual void on_pbDeleteTarget_clicked();
    virtual void on_pbFilterOnTarget_clicked();
    virtual void on_pbFilterOffTarget_clicked();

    virtual void on_ileContactGroup_clicked();
    virtual void on_pbPreview_clicked();
    virtual void on_pbPrint_clicked();
    virtual void on_pbPdf_clicked();

protected:
    void changeEvent(QEvent* e);

private:
    void setTargetList(RB_ObjectContainer* targetList, QSqlQuery& q);
    void preparePrintText(RB_PrintTextHeaderFooter* util,
                          RB_ObjectBase *docObj, RB_ObjectBase *targetObj);

    //! Campaign data model
    RB_MmProxy* mCampaignModel;
    //! Campaign data widget mapper
    RB_DataWidgetMapper* mCampaignMapper;
    //! Target data model
    RB_MmProxy* mTargetModel;
    //! Target data widget mapper
    RB_DataWidgetMapper* mTargetMapper;

    //! Activity model for logging document generation/printing
    RB_MmProxy* mActivityModel;

    QTextDocument* mDocHeader;
    QTextDocument* mDocContent;
    QTextDocument* mDocFooter;
};

#endif // SRM_CAMPAIGNTARGETWIDGET_H
