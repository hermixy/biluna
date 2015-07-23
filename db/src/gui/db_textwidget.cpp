/*****************************************************************
 * $Id: db_textwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jan 5, 2010 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_textwidget.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "rb_textedit.h"

/**
 * Constructor
 */
DB_TextWidget::DB_TextWidget(QWidget* parent) : RB_Widget(parent){

    setupUi(this);

    mFullFileName = "";
    mSourceDirty = false;

    tabWidget->setTabText(0, "Rich Text");
    tabWidget->setTabText(1, "Source");
    plainTextEdit->setTabStopWidth(20); // in pixels, default is 80

    // richTextEdit is a RB_TextEdit
    connect(richTextEdit, SIGNAL(textChanged()),
            this, SLOT(slotContentChanged()));
    connect(plainTextEdit, SIGNAL(textChanged()),
            this, SLOT(slotContentChanged()));
    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(changeTab(int)));

    mCodeHighlighter = new DB_HtmlHighlighter(plainTextEdit->document());
    mVerticalScrollbarPerunage = 0.0;
    richTextEdit->showVirtualTableBorders();
}

/**
 * Get name for MDI window title
 * @return file name (or type of window in case of unique window),
 * the file name is empty at opening a new window and replaced
 * by the file name when the text file is saved. When opening an
 * existing file the file name will be returned.
 */
RB_String DB_TextWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }
    return QFileInfo(mFullFileName).fileName();
}

/**
 * @returns text edit widget
 */
//QWidget* DB_TextWidget::getMainWidget() {
//    if (tabWidget->currentIndex() == 0) {
//        return richTextEdit;
//    }

//    return plainTextEdit;
//}

/**
 * Initialize this widget
 */
void DB_TextWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());
    readSettings();
    richTextEdit->setFocus();

    // Widget font
    QFont f = richTextEdit->font();
    int newSize = 11;
    f.setPointSize(newSize);
    richTextEdit->setFont(f);

    // Alternative use document font
    //    QFont font = richTextEdit->document()->defaultFont();
    //    int fntSize = font.pointSize();
    //    font.setPointSize(fntSize + factor);
    //    richTextEdit->document()->setDefaultFont(fnt);
    //    return;
}

/**
 * File New, does nothing
 */
void DB_TextWidget::fileNew() {
    setIsNewWidget(true);
}

/**
 * File Open, does nothing
 */
void DB_TextWidget::fileOpen() {
    // does nothing
}

/**
 * File Save, can be called by closeWidget()
 */
bool DB_TextWidget::fileSave(bool /*withSelect*/) {
    if (isNewWidget()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool DB_TextWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * File export PDF.
 */
void DB_TextWidget::filePdf(QPrinter* printer) {
    filePrint(printer);
}

/**
 * File Print.
 */
void DB_TextWidget::filePrint(QPrinter* printer) {
    richTextEdit->print(printer);
}

/**
 * File Print Preview.
 */
void DB_TextWidget::filePrintPreview(QPrinter* printer) {
    filePrint(printer);
}

/**
 * Edit Cut.
 */
void DB_TextWidget::editCut() {
    richTextEdit->cut();
}

/**
 * Edit Copy.
 */
void DB_TextWidget::editCopy() {
    richTextEdit->copy();
}

/**
 * Edit Paste.
 */
void DB_TextWidget::editPaste() {
    richTextEdit->paste();
}

/**
 * Edit Undo.
 */
void DB_TextWidget::editUndo() {
    richTextEdit->undo();
}

/**
 * Edit Redo.
 */
void DB_TextWidget::editRedo() {
    richTextEdit->redo();
}

/**
 * Select All.
 */
void DB_TextWidget::editSelectAll() {
    richTextEdit->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void DB_TextWidget::editInsertImage(const QString& fn) {
    richTextEdit->insertImage(fn);
}

/**
 * Zoom out is done by increasing the font points by factor,
 * for large documents this can take some time
 * @param factor to zoom in not used
 */
void DB_TextWidget::editZoomIn(double /*factor*/) {
    richTextEdit->fontZoomIn(1);
}

/**
 * Zoom out is done by decreasing the font points by factor
 * @param factor to zoom out not used
 */
void DB_TextWidget::editZoomOut(double /*factor*/) {
    richTextEdit->fontZoomOut(1);
}

/**
 * Format Bold
 */
void DB_TextWidget::formatBold() {
    richTextEdit->slotTextBold();
}

/**
 * Format Italic
 */
void DB_TextWidget::formatItalic() {
    richTextEdit->slotTextItalic();
}

/**
 * Format Underline
 */
void DB_TextWidget::formatUnderline() {
    richTextEdit->slotTextUnderline();
}

/**
 * Format Strikethrough
 */
void DB_TextWidget::formatStrikethrough() {
    richTextEdit->slotTextStrikeOut();
}

/**
 * Format Align Left
 */
void DB_TextWidget::formatAlignLeft() {
    richTextEdit->slotTextAlign(RB_TextEdit::AlignLeft);
}

/**
 * Format Align Center
 */
void DB_TextWidget::formatAlignCenter() {
    richTextEdit->slotTextAlign(RB_TextEdit::AlignCenter);
}

/**
 * Format Align Right
 */
void DB_TextWidget::formatAlignRight() {
    richTextEdit->slotTextAlign(RB_TextEdit::AlignRight);
}

/**
 * Format Align Justify
 */
void DB_TextWidget::formatAlignJustify() {
    richTextEdit->slotTextAlign(RB_TextEdit::AlignJustify);
}

/**
 * Format Increase Indent
 */
void DB_TextWidget::formatIncreaseIndent() {
    richTextEdit->slotTextIndent(1);
}

/**
 * Format Decrease Indent
 */
void DB_TextWidget::formatDecreaseIndent() {
    richTextEdit->slotTextIndent(-1);
}

/**
 * Format numbered list
 */
void DB_TextWidget::formatNumberedList() {
    richTextEdit->slotTextStyle(RB_TextEdit::ListDecimal);
}

/**
 * Format bulleted list
 */
void DB_TextWidget::formatBulletedList() {
    richTextEdit->slotTextStyle(RB_TextEdit::ListDisc);
}

/**
 * Format font family
 */
void DB_TextWidget::formatFontName() {
    richTextEdit->slotTextFamily();
}

/**
 * Format font size
 */
void DB_TextWidget::formatFontSize() {
    richTextEdit->slotTextSize();
}

/**
 * Format text color
 */
void DB_TextWidget::formatTextColor() {
    richTextEdit->slotTextColor();
}

/**
 * Format background color
 */
void DB_TextWidget::formatBackgroundColor() {
    richTextEdit->slotBackgroundColor();
}

/**
 * Remove formatting and style
 */
void DB_TextWidget::formatRemove() {
//    webView->triggerPageAction(QWebPage::RemoveFormat);
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_TextWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Load file
 * @param fn filename
 * @return true at success
 */
bool DB_TextWidget::loadFile(const QString& fn) {
    // Check if file can be read
    QFile file(fn);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot read file %1:\n%2.").arg(fn).arg(file.errorString()));
        return false;
    }

    setCurrentFileName(fn);

    // Read file
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (fn.endsWith(".htm", Qt::CaseInsensitive)
        || fn.endsWith(".html", Qt::CaseInsensitive)
        || fn.endsWith(".xml", Qt::CaseInsensitive)) {
        richTextEdit->setHtml(in.readAll());
    } else {
        richTextEdit->setPlainText(in.readAll());
    }

    richTextEdit->document()->setModified(false);
    QApplication::restoreOverrideCursor();
    return true;
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool DB_TextWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (fn.endsWith(".htm", Qt::CaseInsensitive)
            || fn.endsWith(".html", Qt::CaseInsensitive)
            || fn.endsWith(".xhtml", Qt::CaseInsensitive)) {
        RB_String html = richTextEdit->toHtml();
        richTextEdit->saveHtmlEmbeddedImage(html);
        out << html;
    } else {
        out << richTextEdit->toPlainText();
    }

    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    setIsNewWidget(false);
    richTextEdit->document()->setModified(false);
    emit modificationChanged(false);
    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

bool DB_TextWidget::closeWidget() {
    if (maybeSave()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Append text to the document
 * @param txt text to be added
 */
void DB_TextWidget::appendText(const RB_String& txt) {
    richTextEdit->append(txt);
}

/**
 * Insert HTML at the cursor position
 * @param html HTML to be added
 */
void DB_TextWidget::insertHtml(const RB_String& html) {
    // equal to richTextEdit->textCursor()->insertHtml(html);
    richTextEdit->insertHtml(html);
}

/**
 * @returns RB_TextEdit
 */
RB_TextEdit* DB_TextWidget::getTextEdit() {
    return richTextEdit;
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 */
//bool DB_TextWidget::maybeSave() {
//    // RB_DEBUG->print("DB_TextWidget::maybeSave()");

//    if (richTextEdit->document()->isModified()) {
//        int ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(
//                tr("Document changed ..."),
//                tr("The document has been modified.\n"
//                   "Do you want to keep your changes?"));
//        if (ret == QMessageBox::Yes) {
//            if (!fileSave()) { // if saved message by action
//                DB_DIALOGFACTORY->statusBarMessage(tr("File not saved"), 2000);
//            }
//        } else if (ret == QMessageBox::No) {
//            DB_DIALOGFACTORY->statusBarMessage(tr("File not saved"), 2000);
//        } else if (ret == QMessageBox::Cancel) {
//            return false;
//        }
//    }

//    return true;
//}


/**
 * Slot: clear text widget
 */
void DB_TextWidget::slotClearText() {
    this->richTextEdit->clear();
}

void DB_TextWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void DB_TextWidget::slotContentChanged() {
    mSourceDirty = true;
    emit modificationChanged(true);
}

void DB_TextWidget::changeTab(int index) {
    if (mSourceDirty && (index == 1)) {
        RB_String content = richTextEdit->toHtml();
        plainTextEdit->setPlainText(content);
        mSourceDirty = false;

        int min = richTextEdit->verticalScrollBar()->minimum();
        int max = richTextEdit->verticalScrollBar()->maximum();
        int value = richTextEdit->verticalScrollBar()->value();
        mVerticalScrollbarPerunage = (double)(value - min) / (max - min);

        // Max is still zero at plainTextEdit, alternative:
        QTimer::singleShot(200, this, SLOT(slotVerticalScrollbar()));
    } else if (mSourceDirty && (index == 0)) {
        RB_String content = plainTextEdit->toPlainText();
        richTextEdit->setHtml(content);
        mSourceDirty = false;

        int min = plainTextEdit->verticalScrollBar()->minimum();
        int max = plainTextEdit->verticalScrollBar()->maximum();
        int value = plainTextEdit->verticalScrollBar()->value();
        mVerticalScrollbarPerunage = (double)(value - min) / (max - min);

        // Max is still zero at richTextEdit, alternative:
        QTimer::singleShot(200, this, SLOT(slotVerticalScrollbar()));
    }
}

void DB_TextWidget::slotVerticalScrollbar() {
    if (tabWidget->currentIndex() == 1) {
        QScrollBar* vsb = plainTextEdit->verticalScrollBar();
        int min = vsb->minimum();
        int max = vsb->maximum();
        int value = (int)((mVerticalScrollbarPerunage) * (max - min) + min);
        vsb->setValue(value);
    } else {
        QScrollBar* vsb = richTextEdit->verticalScrollBar();
        int min = vsb->minimum();
        int max = vsb->maximum();
        int value = (int)((mVerticalScrollbarPerunage) * (max - min) + min);
        vsb->setValue(value);
    }
}

