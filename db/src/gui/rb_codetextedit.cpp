/*****************************************************************
 * $Id: rb_codetextedit.cpp 2198 2014-11-17 21:34:16Z rutger $
 * Created: 2013-08-28 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_codetextedit.h"

#include <QtWidgets>


/**
 * Constructor
 */
RB_CodeTextEdit::RB_CodeTextEdit(QWidget *parent) : RB_PlainTextEdit(parent) {
    // Only for line number area
    mLineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

/**
 * @brief RB_CodeTextEdit::lineNumberAreaPaintEvent
 * @param event
 */
void RB_CodeTextEdit::lineNumberAreaPaintEvent(QPaintEvent* event) {
    QPainter painter(mLineNumberArea);
    painter.fillRect(event->rect(),
                     QApplication::palette().color(QPalette::Active, QPalette::Button)
                     /*Qt::lightGray*/);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QApplication::palette().color(QPalette::Active, QPalette::ButtonText)/*Qt::black*/);
            painter.drawText(0, top, mLineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

/**
 * @brief RB_CodeTextEdit::resizeEvent
 * @param e event
 */
void RB_CodeTextEdit::resizeEvent(QResizeEvent* e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    mLineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                       lineNumberAreaWidth(), cr.height()));
}

/**
 * @brief RB_CodeTextEdit::lineNumberAreaWidth
 * @return space for line number area
 */
int RB_CodeTextEdit::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 4 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

/**
 * Change event, for example translation
 */
void RB_CodeTextEdit::changeEvent(QEvent *e) {
    RB_PlainTextEdit::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
//        retranslateUi(this);
        break;
    case QEvent::PaletteChange:
        setPaletteColors();
        break;
    default:
        break;
    }
}

/**
 * @brief RB_CodeTextEdit::updateLineNumberAreaWidth
 * @param newBlockCount
 */
void RB_CodeTextEdit::updateLineNumberAreaWidth(int /*newBlockCount*/) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/**
 * @brief RB_CodeTextEdit::highlightCurrentLine
 */
void RB_CodeTextEdit::highlightCurrentLine() {
    setPaletteColors();
}

/**
 * @brief RB_CodeTextEdit::updateLineNumberArea
 * @param rect rectangle
 * @param dy delta y
 */
void RB_CodeTextEdit::updateLineNumberArea(const QRect& rect, int dy) {
    if (dy)
        mLineNumberArea->scroll(0, dy);
    else
        mLineNumberArea->update(0, rect.y(), mLineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void RB_CodeTextEdit::setPaletteColors() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QApplication::palette().color(QPalette::Active, QPalette::AlternateBase); // QColor(Qt::yellow).lighter(160);
        QColor textColor = QApplication::palette().color(QPalette::Active, QPalette::Text);
        selection.format.setBackground(lineColor);
        selection.format.setForeground(QBrush(textColor));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
