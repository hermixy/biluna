/*****************************************************************
 * $Id: db_findreplacedialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 22, 2012 11:34:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project, only changed to Biluna style
 *
 * Copyright (C) 2009  Lorenzo Bettini <http://www.lorenzobettini.it>
 * See COPYING file that comes with this distribution
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_FINDREPLACEWIDGET_H
#define DB_FINDREPLACEWIDGET_H

#include "rb_dialog.h"
#include "ui_db_findreplacedialog.h"
#include <QTextCursor>

class QPlainTextEdit;
class QSettings;
class QTextEdit;
class QWebView;

/**
 * The dialog presents the typical widgets you find
 * in standard find/replace dialogs, and it acts on a QTextEdit.
 */
class DB_EXPORT DB_FindReplaceDialog : public RB_Dialog, private Ui::DB_FindReplaceDialog {

    Q_OBJECT

public:
    DB_FindReplaceDialog(QWidget *parent = 0);
    virtual ~DB_FindReplaceDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Find and replace"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    void init();

    /**
      * Associates the plain text editor where to perform the search
      * @param plainTextEdit
      */
    void setPlainTextEdit(QPlainTextEdit* plainTextEdit);

    /**
      * Associates the text editor where to perform the search
      * @param textEdit
      */
    void setTextEdit(QTextEdit* textEdit);

    /**
      * Associates the HTML editor where to perform the search
      * @param webView
      */
    void setWebView(QWebView* webView);

    /// hides replace widgets from the form
    void hideReplaceWidgets();
    /// shows replace widgets from the form
    void showReplaceWidgets();

public slots:
    /**
     * Finds the next occurrence
     */
    void on_pbFind_clicked();
    /**
      * Replaces the found occurrences and goes to the next occurrence
      */
    void on_pbReplace_clicked();
    /**
      * Replaces all the found occurrences
      */
    void on_pbReplaceAll_clicked();
    void on_pbClose_clicked();

    /**
     * performs the find task
     * @param down whether to find the next or the previous
     * occurrence
     */
    void find(bool down);

    /**
     * Finds the next occurrence
     */
    void findNext() { find(true); }

    /**
     * Finds the previous occurrence
     */
    void findPrev() { find(false); }

    bool close();

signals:
    void closed();

protected:
    /**
      * Writes the state of the form to the passed settings.
      * @param settings
      * @param prefix the prefix to insert in the settings
      */
    virtual void writeSettings();

    /**
      * Reads the state of the form from the passed settings.
      * @param settings
      * @param prefix the prefix to look for in the settings
      */
    virtual void readSettings();

    void changeEvent(QEvent *e);

    /// shows an error in the dialog
    void showError(const QString &error);

    /// shows a message in the dialog
    void showMessage(const QString &message);

    void findWebView(bool down);

protected slots:
    /// when the text edit contents changed
    void textToFindChanged();

    /// checks whether the passed text is a valid regexp
    void validateRegExp(const QString &text);

    /// the regexp checkbox was selected
    void regexpSelected(bool sel);

    void slotButtonsEnabled();

protected:
    /// for searching into the text
    QTextCursor mTextCursor;

    /// the plain text editor (possibly) associated with this form
    QPlainTextEdit* mPlainTextEdit;

    /// the text editor (possibly) associated with this form
    QTextEdit* mTextEdit;

    /// the web view/HTML editor (possibly) associated with this form
    QWebView* mWebView;

    bool mIsFindDialog;
    bool mWebViewFound;
};

#endif // DB_FINDREPLACEWIDGET_H
