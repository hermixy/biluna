/*****************************************************************
 * $Id: rb_plaintextedit.cpp 1964 2013-08-06 17:35:10Z rutger $
 * Created: 2012-12-12 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_plaintextedit.h"

#include <QPrinter>

/**
 * Constructor
 */
RB_PlainTextEdit::RB_PlainTextEdit(QWidget *parent) : QPlainTextEdit(parent) {
    mLeftLimitChar = "";
    mRightLimitChar = "";
}

/**
 * Print this textEdit widget
 * @param printer
 */
void RB_PlainTextEdit::print(QPrinter* printer) {
    document()->setUseDesignMetrics(true);
    document()->print(printer);
}

void RB_PlainTextEdit::keyPressEvent(QKeyEvent* ke) {
    if (ke->key() == Qt::Key_BraceLeft) {
        mLeftLimitChar = "{";
        mRightLimitChar = "}";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_BracketLeft) {
        mLeftLimitChar = "[";
        mRightLimitChar = "]";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_ParenLeft) {
        mLeftLimitChar = "(";
        mRightLimitChar = ")";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_Less) {
        mLeftLimitChar = "<";
        mRightLimitChar = ">";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_QuoteDbl) {
        mLeftLimitChar = "\"";
        mRightLimitChar = "\"";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    } else if (ke->key() == Qt::Key_Apostrophe) {
        mLeftLimitChar = "\'";
        mRightLimitChar = "\'";
        handleLeftLimitCharPressed();
        ke->accept();
        return;
    }

    QPlainTextEdit::keyPressEvent(ke);
}

/**
 * Completion for keyPressedEvent of 'limiting' characters
 */
void RB_PlainTextEdit::handleLeftLimitCharPressed() {
    // Handles keyPressEvent in Editor class if key == Qt::Key_ParenLeft
    // and signal emitted for this widget to catch, and key pad

    // cursor positions
    QTextCursor cursor = textCursor();

    int openCharPos = 0;
    int closeCharPos = 0;

    if (cursor.anchor() < cursor.position()) {
        openCharPos = cursor.anchor();
        closeCharPos = cursor.position();
    } else {
        openCharPos = cursor.position();
        closeCharPos = cursor.anchor();
    }

    // check for number of right parenthese at the right
    cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);
    setTextCursor(cursor);

    QString str = cursor.selectedText();
    int left = 0;
    int right = 0;

    QChar leftChar = mLeftLimitChar.at(0);
    QChar rightChar = mRightLimitChar.at(0);

    for (int i = 0; i < str.size(); ++i) {
        if (str.at(i) == leftChar) {
            left += 1;
        } else if (str.at(i) == rightChar) {
            right += 1;
        }
    }

    cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
    cursor.setPosition(closeCharPos, QTextCursor::KeepAnchor);
    setTextCursor(cursor);

    if (cursor.hasSelection()) {
        // selection of text prevails above left/right mismatch
        cursor.setPosition(closeCharPos, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
        cursor.insertText(mRightLimitChar);
        cursor.setPosition(openCharPos, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
        cursor.insertText(mLeftLimitChar);
        cursor.setPosition(closeCharPos + 2, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    } else if (left < right) {
        // complete the left parenthese only
        cursor.insertText(mLeftLimitChar);
    } else {
        cursor.insertText(mLeftLimitChar + mRightLimitChar);
        cursor.setPosition(cursor.position() - 1);
        setTextCursor(cursor);
    }
}
