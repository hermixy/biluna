/*****************************************************************
 * $Id: db_commandedit.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 11, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_COMMANDEDIT_H
#define DB_COMMANDEDIT_H

#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QStringList>
#include "db_global.h"

/**
 * A command line edit with some typical console features 
 * (uparrow for the history, tab, ..).
 */
class DB_EXPORT DB_CommandEdit: public QLineEdit {
    Q_OBJECT

public:
    DB_CommandEdit(QWidget* parent=0);
    virtual ~DB_CommandEdit();

protected:
    virtual bool event(QEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

signals:
    void tabPressed();
    void escape();
    void focusIn();
    void focusOut();
    void clearAll(); // QLineEdit already as signal clear()

private:
    QStringList historyList;
    QStringList::Iterator it;
    bool acceptCoordinates;
};

#endif

