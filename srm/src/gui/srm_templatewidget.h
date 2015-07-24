/*****************************************************************
 * $Id: srm_templatewidget.h 2071 2014-01-22 21:56:32Z rutger $
 * Created: Mar 06, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_TEMPLATEWIDGET_H
#define SRM_TEMPLATEWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_templatewidget.h"

class RB_TextPreview;

/**
 * Widget to create templates for CRM sales activities such as a mailing
 */
class SRM_TemplateWidget : public RB_Widget, private Ui::SRM_TemplateWidget {

    Q_OBJECT
    
public:
    explicit SRM_TemplateWidget(QWidget* parent = 0);
    ~SRM_TemplateWidget();
    
    virtual RB_String getName() const { return "SRM Template"; }
//    virtual RB_String getSaveAsFileName();
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

    virtual RB_String getHelpSubject() const;
    virtual void setPrinterProperties(QPrinter* pr);
    virtual bool closeWidget();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

protected:
    virtual void changeEvent(QEvent* e);

protected slots:
    void slotTabChanged(int index);
    void slotCurrentItemChanged(const QModelIndex& curr, const QModelIndex& prev);

private:
    RB_TextEdit* getTextEdit();

    //! Preview of text document in print layout
    RB_TextPreview* mTextPreview;
    //! Template data model
    RB_MmProxy* mModel;
    //! Template data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // SRM_TEMPLATEWIDGET_H
