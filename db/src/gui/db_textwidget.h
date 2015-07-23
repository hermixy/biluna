/*****************************************************************
 * $Id: db_textwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 5, 2010 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TEXTWIDGET_H
#define DB_TEXTWIDGET_H

#include "db_htmlhighlighter.h"
#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_textwidget.h"



class DB_EXPORT DB_TextWidget : public RB_Widget, private Ui::DB_TextWidget {

    Q_OBJECT

public:
    DB_TextWidget(QWidget* parent = 0);

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
//    virtual QWidget* getMainWidget();

    virtual void init();

    virtual void fileNew();
    virtual void fileOpen();
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
    virtual void editZoomIn(double factor = 1.0);
    virtual void editZoomOut(double factor = -1.0);

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

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String &fn);
    virtual bool closeWidget();

    virtual void appendText(const RB_String& txt);
    virtual void insertHtml(const RB_String& html);
    virtual RB_TextEdit* getTextEdit();

public slots:
    virtual void slotClearText();

protected:
    void changeEvent(QEvent* e);

protected slots:
    void slotContentChanged();
    void changeTab(int index);
    void slotVerticalScrollbar();

private:
    DB_HtmlHighlighter* mCodeHighlighter;
    bool mSourceDirty;
    double mVerticalScrollbarPerunage;


};

#endif // DB_TEXTWIDGET_H
