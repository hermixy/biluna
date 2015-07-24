/*****************************************************************
 * $Id: srm_documentwidget.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 25, 2014 3:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_DOCUMENTWIDGET_H
#define SRM_DOCUMENTWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_documentwidget.h"

class RB_PrintTextHeaderFooter;

/**
 * Widget to create documents for SRM sales activities such as a mailing
 */
class SRM_DocumentWidget : public RB_Widget, private Ui::SRM_DocumentWidget {

    Q_OBJECT
    
public:
    explicit SRM_DocumentWidget(QWidget* parent = 0);
    ~SRM_DocumentWidget();
    
    virtual RB_String getName() const { return "SRM Document"; }
    virtual RB_String getSaveAsFileName();
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editSelectAll();

    virtual void formatBold();
    virtual void formatItalic();
    virtual void formatUnderline();
    virtual void formatStrikethrough();
    virtual void formatAlignLeft();
    virtual void formatAlignCenter();
    virtual void formatAlignRight();
    virtual void formatAlignJustify();
    virtual void formatIncreaseIndent();
    virtual void formatDecreaseIndent();
    virtual void formatNumberedList();
    virtual void formatBulletedList();
    virtual void formatFontName();
    virtual void formatFontSize();
    virtual void formatTextColor();
    virtual void formatBackgroundColor();
    virtual void formatRemove();

    virtual void print(QPrinter* pr, bool isPreview = false);
    virtual void addLogActivity(RB_ObjectBase* docObj);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();
    virtual void on_ileSupplier_clicked();
    virtual void on_ileContact_clicked();
    virtual void on_ileTemplate_clicked();

protected:
    virtual void changeEvent(QEvent* e);

private:
    void preparePrintText(RB_PrintTextHeaderFooter* util,
                          RB_ObjectBase* docObj);

    //! Document data model
    RB_MmProxy* mModel;
    //! Document data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // SRM_DOCUMENTWIDGET_H
