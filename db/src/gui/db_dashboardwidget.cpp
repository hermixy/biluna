/*****************************************************************
 * $Id: db_dashboardwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Mar 31, 2011 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_dashboardwidget.h"

#include "rb_settings.h"



/**
 * Constructor
 */
DB_DashboardWidget::DB_DashboardWidget(QWidget *parent)
                        : RB_Widget(parent){

    setupUi(this);

    mWidgetName = tr("Biluna Dashboard");
}

/**
 * Destructor
 */
DB_DashboardWidget::~DB_DashboardWidget(){
//    RB_SETTINGS->beginGroup(objectName());
//    if (chbDoNotShowAgain->isChecked()) {
//        RB_SETTINGS->setValue("donotshowagain", 1);
//    } else {
//        RB_SETTINGS->setValue("donotshowagain", 0);
//    }
//    RB_SETTINGS->endGroup();
}

/**
 * Get name for MDI window title
 * @return file name or type of window (in case of unique window)
 */
RB_String DB_DashboardWidget::getName() const {
    return mWidgetName;
}

void DB_DashboardWidget::init() {
    // Set links are not to be opened but send via anchorClicked(url) signal
    textBrowser->setOpenLinks(false);
    readSettings();
}

/**
 * File Print.
 */
void DB_DashboardWidget::filePrint(QPrinter* printer) {
    textBrowser->print(printer);
}

/**
 * File Print Preview.
 */
void DB_DashboardWidget::filePrintPreview(QPrinter* printer) {
    textBrowser->print(printer);
}

/**
 * Append text to the document
 * @param txt text to be added
 */
void DB_DashboardWidget::setHtml(const RB_String& txt) {
    textBrowser->setHtml(txt);
}

/**
 * @returns QTextBrowser
 */
QTextBrowser* DB_DashboardWidget::getTextBrowser() {
    return textBrowser;
}

void DB_DashboardWidget::changeEvent(QEvent* e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
