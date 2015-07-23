/*****************************************************************
 * $Id: db_xmlviewvalidatorwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 11, 2013 9:08:25 AM - Rutger Botermans
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_XMLVIEWVALIDATORWIDGET_H
#define DB_XMLVIEWVALIDATORWIDGET_H

#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_xmlviewvalidatorwidget.h"

/**
 * View and validate XML files
 * @brief The DB_XmlViewValidatorWidget class
 */
class DB_EXPORT DB_XmlViewValidatorWidget : public RB_Widget, private Ui::DB_XmlViewValidatorWidget {

    Q_OBJECT

public:
    explicit DB_XmlViewValidatorWidget(QWidget *parent = 0);
    ~DB_XmlViewValidatorWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    virtual QWidget* getMainWidget();

    virtual void init();

private:


};

#endif // DB_XMLVIEWVALIDATORWIDGET_H
