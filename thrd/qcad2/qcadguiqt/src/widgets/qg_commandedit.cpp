/****************************************************************************
** $Id: qg_commandedit.cpp 2495 2005-07-24 17:46:28Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qg_commandedit.h"
//Added by qt3to4:
#include <QFocusEvent>
#include <QKeyEvent>
#include <QEvent>


#include "rs_debug.h"


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_CommandEdit::QG_CommandEdit(QWidget* parent)
        : QLineEdit(parent) {
    it = historyList.end();    
}



/**
 * Destructor
 */
QG_CommandEdit::~QG_CommandEdit() {
    //it = 0;
}


/**
 * Bypass for key press events from the tab key.
 */
bool QG_CommandEdit::event(QEvent* e) {
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
void QG_CommandEdit::keyPressEvent(QKeyEvent* e) {
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
            emit clear();
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


void QG_CommandEdit::focusInEvent(QFocusEvent *e) {
    emit focusIn();
    QLineEdit::focusInEvent(e);
}


void QG_CommandEdit::focusOutEvent(QFocusEvent *e) {
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}


