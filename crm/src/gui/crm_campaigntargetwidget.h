/*****************************************************************
 * $Id: crm_campaigntargetwidget.h 2068 2014-01-21 21:53:40Z rutger $
 * Created: Mar 05, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_CAMPAIGNTARGETWIDGET_H
#define CRM_CAMPAIGNTARGETWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_crm_campaigntargetwidget.h"

class RB_MmProxy;
class RB_PrintTextHeaderFooter;

class CRM_CampaignTargetWidget : public RB_Widget, private Ui::CRM_CampaignTargetWidget {

    Q_OBJECT

public:
    CRM_CampaignTargetWidget(QWidget *parent = 0);
    virtual ~CRM_CampaignTargetWidget();

    virtual RB_String getName() const { return "CRM Campaign Target"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }

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

#endif // CRM_CAMPAIGNTARGETWIDGET_H
