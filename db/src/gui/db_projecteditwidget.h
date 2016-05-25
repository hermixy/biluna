/*****************************************************************
 * $Id: db_projecteditdialog.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 14, 2012 8:34:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PROJECTEDITWIDGET_H
#define DB_PROJECTEDITWIDGET_H

#include "rb_projecteditwidget.h"

class DB_EXPORT DB_ProjectEditWidget : public RB_ProjectEditWidget {

    Q_OBJECT

public:
    DB_ProjectEditWidget(QWidget* parent);

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual void init();

};

#endif // DB_PROJECTEDITWIDGET_H
