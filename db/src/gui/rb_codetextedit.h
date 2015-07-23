/*****************************************************************
 * $Id: rb_codetextedit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2013-08-28 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_CODETEXTEDIT_H
#define RB_CODETEXTEDIT_H

#include "db_global.h"
#include "rb_plaintextedit.h"


class DB_EXPORT RB_CodeTextEdit : public RB_PlainTextEdit {

    Q_OBJECT

public:
    explicit RB_CodeTextEdit(QWidget *parent = 0);

    virtual void lineNumberAreaPaintEvent(QPaintEvent *event);
    virtual int lineNumberAreaWidth();

protected:
    void changeEvent(QEvent* e);
    void resizeEvent(QResizeEvent *e);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect&, int dy);

private:
    void setPaletteColors();

    QWidget* mLineNumberArea;
};

/**
 * @brief The LineNumberArea class
 */
class LineNumberArea : public QWidget {

public:
    LineNumberArea(RB_CodeTextEdit* editor) : QWidget(editor) {
        mCodeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(mCodeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        mCodeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    RB_CodeTextEdit* mCodeEditor;
};

#endif // RB_CODETEXTEDIT_H
