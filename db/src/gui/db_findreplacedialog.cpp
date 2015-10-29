/*****************************************************************
 * $Id: db_findreplacedialog.cpp 2206 2015-01-16 19:32:14Z rutger $
 * Created: Feb 22, 2012 11:34:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project, only changed to Biluna style
 *
 * Copyright (C) 2009  Lorenzo Bettini <http://www.lorenzobettini.it>
 * See COPYING file that comes with this distribution
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_findreplacedialog.h"

#include <QtWidgets>
#include <QTextEdit>
#include <QRegExp>
#include <QWebEngineView>
#include "rb_settings.h"


#define TEXT_TO_FIND "textToFind"
#define TEXT_TO_REPLACE "textToReplace"
#define DOWN_RADIO "downRadio"
#define UP_RADIO "upRadio"
#define CASE_CHECK "caseCheck"
#define WHOLE_CHECK "wholeCheck"
#define REGEXP_CHECK "regexpCheck"

DB_FindReplaceDialog::DB_FindReplaceDialog(QWidget *parent) : RB_Dialog(parent) {

    setupUi(this);

    mPlainTextEdit = NULL;
    mTextEdit = NULL;
    mIsFindDialog = false;
    mWebViewFound = false;
}

DB_FindReplaceDialog::~DB_FindReplaceDialog() {
    close();
}

void DB_FindReplaceDialog::init() {
    setWindowTitle(tr("Find and replace"));
    errorLabel->setText("");

    connect(leSearchFor, SIGNAL(textChanged(QString)), this, SLOT(textToFindChanged()));
    connect(leSearchFor, SIGNAL(textChanged(QString)), this, SLOT(validateRegExp(QString)));
    connect(chbRegularExpression, SIGNAL(toggled(bool)), this, SLOT(regexpSelected(bool)));
    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));

    readSettings();

    leSearchFor->setFocus();
    leSearchFor->selectAll();
}

void DB_FindReplaceDialog::hideReplaceWidgets() {
    setWindowTitle(tr("Find"));
    mIsFindDialog = true;
    replaceLabel->setVisible(false);
    leReplaceWith->setVisible(false);
    pbReplace->setVisible(false);
    pbReplaceAll->setVisible(false);
}

void DB_FindReplaceDialog::showReplaceWidgets() {
    setWindowTitle(tr("Find and replace"));
    mIsFindDialog = false;
    replaceLabel->setVisible(true);
    leReplaceWith->setVisible(true);
    pbReplace->setVisible(true);
    pbReplaceAll->setVisible(true);
}

void DB_FindReplaceDialog::setPlainTextEdit(QPlainTextEdit* plainTextEdit) {
    mPlainTextEdit = plainTextEdit;
    mTextEdit = NULL;
    mWebView = NULL;
    connect(mPlainTextEdit, SIGNAL(copyAvailable(bool)), pbReplace, SLOT(setEnabled(bool)));
    connect(mPlainTextEdit, SIGNAL(copyAvailable(bool)), pbReplaceAll, SLOT(setEnabled(bool)));

    if (mPlainTextEdit->textCursor().hasSelection()) {
        leSearchFor->setText(mPlainTextEdit->textCursor().selectedText());
        pbReplace->setEnabled(true);
        pbReplaceAll->setEnabled(true);
    }

    chbWholeWordsOnly->setEnabled(true);
    chbRegularExpression->setEnabled(true);
}

void DB_FindReplaceDialog::setTextEdit(QTextEdit* textEdit) {
    mPlainTextEdit = NULL;
    mTextEdit = textEdit;
    mWebView = NULL;
    connect(mTextEdit, SIGNAL(copyAvailable(bool)), pbReplace, SLOT(setEnabled(bool)));
    connect(mTextEdit, SIGNAL(copyAvailable(bool)), pbReplaceAll, SLOT(setEnabled(bool)));

    if (mTextEdit->textCursor().hasSelection()) {
        leSearchFor->setText(mTextEdit->textCursor().selectedText());
        pbReplace->setEnabled(true);
        pbReplaceAll->setEnabled(true);
    }

    chbWholeWordsOnly->setEnabled(true);
    chbRegularExpression->setEnabled(true);
}

void DB_FindReplaceDialog::setWebView(QWebEngineView *webView) {
    mPlainTextEdit = NULL;
    mTextEdit = NULL;
    mWebView = webView;
    connect(mWebView, SIGNAL(selectionChanged()), this, SLOT(slotButtonsEnabled()));

    if (mWebView->hasSelection()) {
        leSearchFor->setText(mWebView->selectedText());
        pbReplace->setEnabled(true);
        pbReplaceAll->setEnabled(true);
    }

    chbWholeWordsOnly->setEnabled(false);
    chbRegularExpression->setEnabled(false);
}

void DB_FindReplaceDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void DB_FindReplaceDialog::textToFindChanged() {
    pbFind->setEnabled(leSearchFor->text().size() > 0);
}

void DB_FindReplaceDialog::regexpSelected(bool sel) {
    if (sel)
        validateRegExp(leSearchFor->text());
    else
        validateRegExp("");
}

void DB_FindReplaceDialog::slotButtonsEnabled() {
    if (mWebView->hasSelection()) {
        pbReplace->setEnabled(true);
        pbReplaceAll->setEnabled(true);
    } else {
        pbReplace->setEnabled(false);
        pbReplaceAll->setEnabled(false);
    }
}

void DB_FindReplaceDialog::validateRegExp(const QString &text) {
    if (!chbRegularExpression->isChecked() || text.size() == 0) {
        errorLabel->setText("");
        return; // nothing to validate
    }

    QRegExp reg(text,
                (chbMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));

    if (reg.isValid()) {
        showError("");
    } else {
        showError(reg.errorString());
    }
}

void DB_FindReplaceDialog::showError(const QString &error) {
    if (error == "") {
        errorLabel->setText("");
    } else {
        errorLabel->setText("<span style=\" font-weight:600; color:#ff0000;\">" +
                                error +
                                "</span>");
    }
}

void DB_FindReplaceDialog::showMessage(const QString &message) {
    if (message == "") {
        errorLabel->setText("");
    } else {
        errorLabel->setText("<span style=\" font-weight:600; color:blue;\">" +
                                message +
                                "</span>");
    }
}

void DB_FindReplaceDialog::on_pbFind_clicked() {
    find(rbDown->isChecked());
}

void DB_FindReplaceDialog::find(bool down) {
    if (!mTextEdit && !mPlainTextEdit && !mWebView) {
        RB_DEBUG->error("DB_FindReplaceDialog::find() no textEdit ERROR");
        return;
    }

    if (mWebView) {
        findWebView(down);
        return;
    }

    // backward search
    bool back = !down;

    const QString& toSearch = leSearchFor->text();

    bool result = false;

    QTextDocument::FindFlags flags;

    if (back)
        flags |= QTextDocument::FindBackward;
    if (chbMatchCase->isChecked())
        flags |= QTextDocument::FindCaseSensitively;
    if (chbWholeWordsOnly->isChecked())
        flags |= QTextDocument::FindWholeWords;

    if (chbRegularExpression->isChecked()) {
        QRegExp reg(toSearch,
                    (chbMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive));

        qDebug() << "searching for regexp: " << reg.pattern();

        if (mTextEdit) {
            mTextCursor = mTextEdit->document()->find(reg, mTextCursor, flags);
            mTextEdit->setTextCursor(mTextCursor);
            result = (!mTextCursor.isNull());
        } else {
            mTextCursor = mPlainTextEdit->document()->find(reg, mTextCursor, flags);
            mPlainTextEdit->setTextCursor(mTextCursor);
            result = (!mTextCursor.isNull());
        }
    } else {
        qDebug() << "searching for: " << toSearch;

        if (mTextEdit) {
            result = mTextEdit->find(toSearch, flags);
        } else {
            result = mPlainTextEdit->find(toSearch, flags);
        }
    }


    if (result) {
        showError("");
    } else {
        showError(tr("no match found"));

        // move to the beginning or end of the document for the next find
        if (mTextEdit) {
            // mTextCursor could be a NULL cursor
            mTextCursor = mTextEdit->textCursor();

            if (back) {
                mTextCursor.movePosition(QTextCursor::End);
            } else {
                mTextCursor.setPosition(0);
            }

            mTextEdit->setTextCursor(mTextCursor);
        } else {
            // mTextCursor could be a NULL cursor
            mTextCursor = mPlainTextEdit->textCursor();

            if (back) {
                mTextCursor.movePosition(QTextCursor::End);
            } else {
                mTextCursor.setPosition(0);
            }

            mPlainTextEdit->setTextCursor(mTextCursor);
        }
    }
}

void DB_FindReplaceDialog::findWebView(bool down) {
    // backward search
    bool back = !down;

    const QString& searchTxt = leSearchFor->text();
    QWebEnginePage::FindFlags flags;
    // flags |= QWebPage::FindWrapsAroundDocument;
    // flags |= QWebPage::HighlightAllOccurrences;

    if (back)
        flags |= QWebEnginePage::FindBackward;
    if (chbMatchCase->isChecked())
        flags |= QWebEnginePage::FindCaseSensitively;

    if (mWebView->hasSelection() && !back) {
//        mWebView->pageAction(QWebEnginePage::MoveToNextChar)->trigger();
//        mWebView->page()->runJavaScript();
    } else {
//        mWebView->pageAction(QWebEnginePage::MoveToPreviousChar)->trigger();
    }

    mWebView->findText(searchTxt, QWebEnginePage::FindFlags(),
                       [this](bool found) {
        if (!found) showError(tr("no match found"));});
}



/**
 * Close the dialog and emit signal closed.
 * @brief DB_FindReplaceDialog::close
 * @return
 */
bool DB_FindReplaceDialog::close() {
    // signal used to set pointer to NULL in DB_DialogFactory
    emit closed();
    return RB_Dialog::close();
}

void DB_FindReplaceDialog::on_pbReplace_clicked() {
    if (mTextEdit) {
        if (mTextEdit->textCursor().selection().toPlainText() != leSearchFor->text()) {
            on_pbFind_clicked();
        } else {
            mTextEdit->textCursor().insertText(leReplaceWith->text());
            on_pbFind_clicked();
        }
    } else if (mPlainTextEdit) {
        if (mPlainTextEdit->textCursor().selection().toPlainText()
                != leSearchFor->text()) {
            on_pbFind_clicked();
        } else {
            mPlainTextEdit->textCursor().insertText(leReplaceWith->text());
            on_pbFind_clicked();
        }
    } else if (mWebView) {
        if (mWebView->selectedText() != leSearchFor->text()) {
            on_pbFind_clicked();
        } else {
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setText(leReplaceWith->text());
            mWebView->pageAction(QWebEnginePage::PasteAndMatchStyle)->trigger();
            on_pbFind_clicked();
        }
    }
}

void DB_FindReplaceDialog::on_pbReplaceAll_clicked() {
    int i = 0;

    if (mTextEdit) {
        if (mTextEdit->textCursor().selection().toPlainText() != leSearchFor->text()) {
            on_pbFind_clicked();
        }

        while (mTextEdit->textCursor().hasSelection()){
            mTextEdit->textCursor().insertText(leReplaceWith->text());
            on_pbFind_clicked();
            i++;
        }

        showMessage(tr("Replaced %1 occurrence(s)").arg(i));
        mTextEdit->setFocus();
    } else if (mPlainTextEdit) {
        if (mPlainTextEdit->textCursor().selection().toPlainText()
                != leSearchFor->text()) {
            on_pbFind_clicked();
        }

        while (mPlainTextEdit->textCursor().hasSelection()){
            mPlainTextEdit->textCursor().insertText(leReplaceWith->text());
            on_pbFind_clicked();
            i++;
        }

        showMessage(tr("Replaced %1 occurrence(s)").arg(i));
        mPlainTextEdit->setFocus();
    } else if (mWebView) {
        if (mWebView->selectedText() != leSearchFor->text()) {
            on_pbFind_clicked();
        }

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(leReplaceWith->text());
        mWebViewFound = true;

        while (mWebView->hasSelection() && mWebViewFound){
            mWebView->pageAction(QWebEnginePage::PasteAndMatchStyle)->trigger();
            on_pbFind_clicked();
            i++;
        }

        showMessage(tr("Replaced %1 occurrence(s)").arg(i));
        mWebView->setFocus();
    }
}

/**
 * Button close clicked
 */
void DB_FindReplaceDialog::on_pbClose_clicked() {
    writeSettings();
}

void DB_FindReplaceDialog::writeSettings() {
    RB_Dialog::writeSettings(); // size and position

    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue(TEXT_TO_FIND, leSearchFor->text());
    RB_SETTINGS->setValue(TEXT_TO_REPLACE, leReplaceWith->text());
    RB_SETTINGS->setValue(DOWN_RADIO, rbDown->isChecked());
    RB_SETTINGS->setValue(UP_RADIO, rbUp->isChecked());
    RB_SETTINGS->setValue(CASE_CHECK, chbMatchCase->isChecked());
    RB_SETTINGS->setValue(WHOLE_CHECK, chbWholeWordsOnly->isChecked());
    RB_SETTINGS->setValue(REGEXP_CHECK, chbRegularExpression->isChecked());
    RB_SETTINGS->endGroup();
}

void DB_FindReplaceDialog::readSettings() {
    RB_Dialog::readSettings(); // size and position

    RB_SETTINGS->beginGroup(objectName());
    leSearchFor->setText(RB_SETTINGS->value(TEXT_TO_FIND, "").toString());
    leReplaceWith->setText(RB_SETTINGS->value(TEXT_TO_REPLACE, "").toString());
    rbDown->setChecked(RB_SETTINGS->value(DOWN_RADIO, true).toBool());
    rbUp->setChecked(RB_SETTINGS->value(UP_RADIO, false).toBool());
    chbMatchCase->setChecked(RB_SETTINGS->value(CASE_CHECK, false).toBool());
    chbWholeWordsOnly->setChecked(RB_SETTINGS->value(WHOLE_CHECK, false).toBool());
    chbRegularExpression->setChecked(RB_SETTINGS->value(REGEXP_CHECK, false).toBool());
    RB_SETTINGS->endGroup();
}
