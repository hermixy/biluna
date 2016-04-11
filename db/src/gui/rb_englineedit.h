/*****************************************************************
 * $Id: rb_englineedit.h 0 2016-04-09 19:13:00Z rutger $
 * Created: Apr 9, 2016 19:13:00 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ENGLINEEDIT_H
#define RB_ENGLINEEDIT_H

#include <QLineEdit>
#include <QString>
#include "db_global.h"

/**
 * @brief The RB_EngLineEdit class for displaying the doubles from the database
 * in engineering/scientific notation as per QString::number
 */
class DB_EXPORT RB_EngLineEdit : public QLineEdit {

    Q_OBJECT

public:
    explicit RB_EngLineEdit(QWidget* parent = 0);

public slots:
    virtual void setText(const QString& txt);
};

#endif // RB_ENGLINEEDIT_H
