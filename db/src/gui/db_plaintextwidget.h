/*****************************************************************
 * $Id: db_plaintextwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 23, 2012 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PLAINTEXTWIDGET_H
#define DB_PLAINTEXTWIDGET_H

#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_plaintextwidget.h"

class RB_PlainTextEdit;


class DB_EXPORT DB_PlainTextWidget : public RB_Widget, private Ui::DB_PlainTextWidget {

    Q_OBJECT

public:
    DB_PlainTextWidget(QWidget* parent = 0);

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    virtual QWidget* getMainWidget();

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

    virtual void formatBold() { return; }
    virtual void formatItalic() { return; }
    virtual void formatUnderline() { return; }
    virtual void formatStrikethrough() { return; }
    virtual void formatAlignLeft() { return; }
    virtual void formatAlignCenter() { return; }
    virtual void formatAlignRight() { return; }
    virtual void formatAlignJustify() { return; }
    virtual void formatIncreaseIndent() { return; }
    virtual void formatDecreaseIndent() { return; }
    virtual void formatNumberedList() { return; }
    virtual void formatBulletedList() { return; }
    virtual void formatFontAll() { return; }
    virtual void formatFontName() { return; }
    virtual void formatFontSize() { return; }
    virtual void formatTextColor() { return; }
    virtual void formatBackgroundColor() { return; }
    virtual void formatRemove() { return; }

    virtual RB_String getHelpSubject() const;

    virtual bool loadFile(const RB_String& fn);
    virtual bool saveFile(const RB_String &fn);
    virtual bool closeWidget();

    virtual void appendText(const RB_String& txt);
    virtual RB_PlainTextEdit *getPlainTextEdit();

public slots:
    virtual void slotClearText();

protected:
    void changeEvent(QEvent* e);


};
#endif // DB_PLAINTEXTWIDGET_H
