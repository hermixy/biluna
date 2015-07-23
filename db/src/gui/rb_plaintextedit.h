/*****************************************************************
 * $Id: rb_plaintextedit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2012-12-12 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PLAINTEXTEDIT_H
#define RB_PLAINTEXTEDIT_H

#include <vector>
#include <QPlainTextEdit>
#include "db_global.h"
#include "rb_string.h"

class QPrinter;

class DB_EXPORT RB_PlainTextEdit : public QPlainTextEdit {

    Q_OBJECT

public:
    explicit RB_PlainTextEdit(QWidget *parent = 0);

    virtual void print(QPrinter* printer);

};

#endif // RB_PLAINTEXTEDIT_H
