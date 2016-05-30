/*****************************************************************
 * $Id: db_versionwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 2, 2013 1:25:40 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_VERSIONWIDGET_H
#define DB_VERSIONWIDGET_H

#include "rb_tablewidget.h"

class DB_EXPORT DB_VersionWidget : public RB_TableWidget {

    Q_OBJECT

public:
    DB_VersionWidget(QWidget* parent);
    virtual ~DB_VersionWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual void init();

};

#endif // DB_VERSIONWIDGET_H
