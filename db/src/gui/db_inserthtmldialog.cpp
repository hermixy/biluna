/*****************************************************************
 * $Id: db_inserthtmldialog.cpp 1639 2012-05-16 08:30:35Z rutger $
 * Created: Apr 24, 2012 4:25:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_inserthtmldialog.h"

#include "db_dialogfactory.h"

/**
 * Constructor
 */
DB_InsertHtmlDialog::DB_InsertHtmlDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);
}

/**
 * Destructor
 */
DB_InsertHtmlDialog::~DB_InsertHtmlDialog() {
    delete mHighlighter;
    RB_DEBUG->print("DB_InsertHtmlDialog::~DB_InsertHtmlDialog() OK");
}

/**
 * Initialize widget and models
 */
void DB_InsertHtmlDialog::init() {
    setWindowTitle(tr("Insert HTML"));

    plainTextEdit->setPlainText("<table border=0 width=\"100%\">\n"
                                "    <tr><td>cell 11</td><td>cell 12</td></tr>\n"
                                "    <tr><td>cell 21</td><td>cell 22</td></tr>\n"
                                "</table>");
    mHighlighter = new DB_HtmlHighlighter(plainTextEdit->document());
}

/**
 *
 */
RB_String DB_InsertHtmlDialog::getHtml() {
    return plainTextEdit->toPlainText();
}

/**
 * Button Help clicked
 */
void DB_InsertHtmlDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = DB_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void DB_InsertHtmlDialog::on_pbOk_clicked() {
    accept();
}

/**
 * Button Cancel clicked
 */
void DB_InsertHtmlDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Change event, for example translation
 */
void DB_InsertHtmlDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

