/*****************************************************************
 * $Id: db_testrelationwidget.h 1435 2011-07-14 17:15:58Z rutger $
 * Created: Aug 9, 2010 3:04:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTRELATIONWIDGET_H
#define DB_TESTRELATIONWIDGET_H

#include <QtGui>
#include "rb_tablewidget.h"


class DB_TestRelationWidget : public RB_TableWidget {

    Q_OBJECT

public:
    DB_TestRelationWidget(QWidget* parent);
    virtual ~DB_TestRelationWidget();

    RB_String getName() const { return "DB_TestReleationWidget"; }
    RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    void init();

};
#endif // DB_TESTRELATIONWIDGET_H
