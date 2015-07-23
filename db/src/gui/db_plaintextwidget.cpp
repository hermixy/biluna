/*****************************************************************
 * $Id: db_plaintextwidget.cpp 2197 2014-11-15 18:59:07Z rutger $
 * Created: Jan 23, 2012 9:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_plaintextwidget.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "rb_codetextedit.h"

/**
 * Constructor
 */
DB_PlainTextWidget::DB_PlainTextWidget(QWidget* parent) : RB_Widget(parent){

    setupUi(this);

    pte->setTabStopWidth(20); // in pixels, default is 80
    mFullFileName = "";

    // pte is a RB_CodeTextEdit
    connect(pte->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
}

/**
 * Get name for MDI window title
 * @return file name (or type of window in case of unique window),
 * the file name is empty at opening a new window and replaced
 * by the file name when the text file is saved. When opening an
 * existing file the file name will be returned.
 */
RB_String DB_PlainTextWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }
    return QFileInfo(mFullFileName).fileName();
}

/**
 * @returns text edit widget
 */
QWidget* DB_PlainTextWidget::getMainWidget() {
    return pte;
}

/**
 * Initialize this widget
 */
void DB_PlainTextWidget::init() {
    // this->setObjectName("DB_PlainTextWidget"); only required in header file
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    readSettings();
}

/**
 * File New, does nothing
 */
void DB_PlainTextWidget::fileNew() {
    setIsNewWidget(true);
}

/**
 * File Open, does nothing
 */
void DB_PlainTextWidget::fileOpen() {
    // does nothing
}

/**
 * File Save, can be called by closeWidget()
 */
bool DB_PlainTextWidget::fileSave(bool /*withSelect*/) {
    if (isNewWidget()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool DB_PlainTextWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * File export PDF.
 */
void DB_PlainTextWidget::filePdf(QPrinter* printer) {
    pte->print(printer);
}

/**
 * File Print.
 */
void DB_PlainTextWidget::filePrint(QPrinter* printer) {
    pte->print(printer);
}

/**
 * File Print Preview.
 */
void DB_PlainTextWidget::filePrintPreview(QPrinter* printer) {
    pte->print(printer);
}

/**
 * Edit Cut.
 */
void DB_PlainTextWidget::editCut() {
    pte->cut();
}

/**
 * Edit Copy.
 */
void DB_PlainTextWidget::editCopy() {
    pte->copy();
}

/**
 * Edit Paste.
 */
void DB_PlainTextWidget::editPaste() {
    pte->paste();
}

/**
 * Edit Undo.
 */
void DB_PlainTextWidget::editUndo() {
    pte->undo();
}

/**
 * Edit Redo.
 */
void DB_PlainTextWidget::editRedo() {
    pte->redo();
}

/**
 * Edit Redo.
 */
void DB_PlainTextWidget::editSelectAll() {
    pte->selectAll();
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String DB_PlainTextWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Load file
 * @param fn filename
 * @return true at success
 */
bool DB_PlainTextWidget::loadFile(const QString& fn) {
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

    pte->setPlainText(in.readAll());
    pte->document()->setModified(false);
    QApplication::restoreOverrideCursor();
    return true;
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool DB_PlainTextWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        DB_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    out << pte->toPlainText();

    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    setIsNewWidget(false);
    pte->document()->setModified(false);
    emit modificationChanged(false);
    DB_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

bool DB_PlainTextWidget::closeWidget() {
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
void DB_PlainTextWidget::appendText(const RB_String& txt) {
    pte->appendPlainText(txt);
}

/**
 * @returns QTextEdit
 */
RB_PlainTextEdit* DB_PlainTextWidget::getPlainTextEdit() {
    return pte;
}

/**
 * Slot: clear text widget
 */
void DB_PlainTextWidget::slotClearText() {
    this->pte->clear();
}

void DB_PlainTextWidget::changeEvent(QEvent* e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

