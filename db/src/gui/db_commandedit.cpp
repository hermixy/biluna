/*****************************************************************
 * $Id: db_commandedit.cpp 1807 2012-11-30 20:01:45Z rutger $
 * Created: Nov 11, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_commandedit.h"
#include "rb_debug.h"


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
DB_CommandEdit::DB_CommandEdit(QWidget* parent) : QLineEdit(parent) {
    it = historyList.end();    
}



/**
 * Destructor
 */
DB_CommandEdit::~DB_CommandEdit() {
    //it = 0;
}


/**
 * Bypass for key press events from the tab key.
 */
bool DB_CommandEdit::event(QEvent* e) {
    if (e->type()==QEvent::KeyPress) {
        QKeyEvent* k = (QKeyEvent*)e;
        if (k->key()==Qt::Key_Tab) {
            emit tabPressed();
            return true;
        }
    }
    
    return QLineEdit::event(e);
}



/**
 * History (arrow key up/down) support, tab.
 * Mac: There is a problem with the ESCAPE key on a Mac OS X. It does not leave command line.
 */
void DB_CommandEdit::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_Up:
        if (it!=historyList.begin() /*&& it!=0*/) {
            it--;
            setText(*it);
        }
        break;
        
    case Qt::Key_Down:
        if (it!=historyList.end() /*&& it!=0*/) {
            it++;
            if (it!=historyList.end()) {
                setText(*it);
            }
            else {
                setText("");
            }
        }
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        historyList.append(text());
        it = historyList.end();
        QLineEdit::keyPressEvent(e);
        break;

    case Qt::Key_Escape:
        // -- command line is empty and ESCAPE pressed so leave command line mode
        // -- hit only ENTER while text is in command line will remove the text only
        if (text().isEmpty()) {
            emit escape();  // mac: The code is executed here, but emit has no effect, dont know why.
        }
        else {
            setText("");
        }
        break;

    case Qt::Key_L:
        // -- Clear command line screen
        if (e->modifiers()==Qt::ControlModifier) {
            emit clearAll();
        }
        else {
            QLineEdit::keyPressEvent(e);
        }
        break;
        
    default:
        QLineEdit::keyPressEvent(e);
        break;
    }
}


void DB_CommandEdit::focusInEvent(QFocusEvent *e) {
    emit focusIn();
    QLineEdit::focusInEvent(e);
}


void DB_CommandEdit::focusOutEvent(QFocusEvent *e) {
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}


