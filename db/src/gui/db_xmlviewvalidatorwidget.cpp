/*****************************************************************
 * $Id: db_xmlviewvalidatorwidget.cpp 2032 2013-11-11 20:01:57Z rutger $
 * Created: Nov 11, 2013 9:08:25 AM - Rutger Botermans
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_xmlviewvalidatorwidget.h"

#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"


DB_XmlViewValidatorWidget::DB_XmlViewValidatorWidget(QWidget *parent) :
                                                    RB_Widget(parent) {
    setupUi(this);
}

DB_XmlViewValidatorWidget::~DB_XmlViewValidatorWidget() {
    // nothing
}

/**
 * Get name for MDI window title
 * @return file name (or type of window in case of unique window),
 * the file name is empty at opening a new window and replaced
 * by the file name when the text file is saved. When opening an
 * existing file the file name will be returned.
 */
RB_String DB_XmlViewValidatorWidget::getName() const {
    if (mFullFileName.isEmpty()) {
        return getId();
    }
    return QFileInfo(mFullFileName).fileName();
}

/**
 * @returns text edit widget
 */
QWidget* DB_XmlViewValidatorWidget::getMainWidget() {
    // return richTextEdit;
    return NULL;
}

/**
 * Initialize this widget
 */
void DB_XmlViewValidatorWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());
    readSettings();
    // richTextEdit->setFocus();
}

