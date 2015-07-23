/*****************************************************************
 * $Id: rb_textedit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2011-05-02 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TEXTEDIT_H
#define RB_TEXTEDIT_H

#include <vector>
#include <QTextEdit>
#include "db_global.h"
#include "rb_string.h"

class QPrinter;

class DB_EXPORT RB_TextEdit : public QTextEdit {

    Q_OBJECT

public:
    explicit RB_TextEdit(QWidget *parent = 0);
    void setIsDialog();

    virtual void print(QPrinter* printer);

    virtual void insertImage(const RB_String& fn);
    virtual void saveHtmlEmbeddedImage(RB_String& html);
    virtual void showVirtualTableBorders(bool isShowVirtualTableBorders = true);
    virtual void fontZoomIn(int pointSize);
    virtual void fontZoomOut(int pointSize);

    //! HACK: to create horizontal line, one pixel transparent PNG image in <p></p>
    virtual RB_String getOnePixelTransparentImage() const {
        return RB_String("<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; "
                         "margin-right:0px; -qt-block-indent:0; text-indent:0px;'>"
                         "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAY"
                         "AAAAfFcSJAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRDb21tZW50AENyZWF0ZW"
                         "Qgd2l0aCBHSU1QV4EOFwAAAA1JREFUCJljYGBgYAAAAAUAAYehTtQAAAAASUVORK5CYII=' "
                         "height=\"1\" width=\"1\"/></p>");
    }

    QColor getAlternateBaseColor() const { return mAlternateBaseColor; }
    QColor getBaseColor() const { return mBaseColor; }
    QColor getTextColor() const { return mTextColor; }

    enum ListType {
        ListNone = 0,
        ListDisc,
        ListCircle,
        ListSquare,
        ListDecimal,
        ListLowerAlpha,
        ListUpperAlpha,
        ListLowerRoman,
        ListUpperRoman,
        ListDefault
    };

    enum AlignType {
        AlignNone = 0,
        AlignLeft,
        AlignCenter,
        AlignRight,
        AlignJustify,
        AlignDefault
    };

signals:

public slots:
    void append(const QString& text);
    void cut();
    void insertHtml(const QString& text);
    void insertPlainText(const QString& text);
    void paste();
    void setHtml(const QString& text);
    void setPlainText(const QString& text);
    void setText(const QString& text);

    void slotTextBold();
    void slotTextUnderline();
    void slotTextItalic();
    void slotTextStrikeOut();
    void slotTextFamily();
    void slotTextSize();
    void slotTextStyle(int listIndex);
    void slotTextIndent(int incr);
    void slotTextColor();
    void slotBackgroundColor();
    void slotTextAlign(int alignIndex);
    void slotInsertHtml(const RB_String& html);

    void slotUpdateCursorPosition();

    QColor getAlternateBaseColor() { return mAlternateBaseColor; }
    QColor getBaseColor() { return mBaseColor; }
    QColor getTextColor() { return mTextColor; }

protected:
    virtual void changeEvent(QEvent* e);
    virtual bool canInsertFromMimeData(const QMimeData* source) const;
    virtual void insertFromMimeData(const QMimeData* source);

protected slots:
    void slotAdjustActions();

private:
    void unsetBorder(QTextFrame* frame);
    void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
    QTextCursor getTextCursor();

    void dropImage(const QUrl& url, const QImage& image);
    void dropTextFile(const QUrl& url);
    void setDefaultFormat();

    void setPaletteColors();

    bool mIsDialog;
    int mCursorAnchor;
    int mCursorPosition;
    bool mIsContentInserted;
    bool mIsShowVirtualTableBorders;

    QColor mAlternateBaseColor;
    QColor mBaseColor;
    QColor mTextColor;
};

#endif // RB_TEXTEDIT_H
