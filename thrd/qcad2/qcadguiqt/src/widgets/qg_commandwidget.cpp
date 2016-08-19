/****************************************************************************
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

#include "qg_commandwidget.h"

#include "rs_dialogfactory.h"
#include "rs_actioninterface.h"
#include "rs_guiaction.h"

/**
 * Constructor
 */
QG_CommandWidget::QG_CommandWidget(QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    
    leCommand->setFrame(false);
    leCommand->setFocusPolicy(Qt::StrongFocus);
    
    connect(leCommand, SIGNAL(returnPressed()),
        this, SLOT(trigger()));
    connect(leCommand, SIGNAL(tabPressed()),
        this, SLOT(tabPressed()));
    connect(leCommand, SIGNAL(escape()),
        this, SLOT(slotEscape()));
    connect(leCommand, SIGNAL(focusIn()),
        this, SLOT(setCommandMode()));
    connect(leCommand, SIGNAL(focusOut()),
        this, SLOT(setNormalMode()));
    connect(leCommand, SIGNAL(clear()),
        teHistory, SLOT(clear()));
}


/**
 * Destructor
 */
QG_CommandWidget::~QG_CommandWidget() {}


bool QG_CommandWidget::checkFocus() {
    return leCommand->hasFocus();
}

void QG_CommandWidget::setFocus() {
    leCommand->setFocus();
}


void QG_CommandWidget::setCommand(const QString& cmd) {
    if (cmd!=lCommand->text()) {
        if (cmd!="") {
            lCommand->setText(cmd);
        } else {
            lCommand->setText(tr("Command:"));
        }
        leCommand->setText("");
    }
}


void QG_CommandWidget::appendHistory(const QString& msg) {
    teHistory->append(msg);
}


void QG_CommandWidget::trigger() {
    QString cmd = leCommand->text();

    if (cmd=="") {
        cmd="\n";
    } else {
        // 20080202: command appended in RS_GuiAction
        //appendHistory(cmd);
    }

    RS_CommandEvent e(cmd);

    // try to dispatch command to running action:
    if (RS_DIALOGFACTORY->getGraphicView()!=NULL) {
        RS_DIALOGFACTORY->getGraphicView()->commandEvent(&e);
    }

    // trigger new action:
    if (!e.isAccepted()) {
        bool success = RS_GuiAction::triggerByCommand(cmd);

        if (!success) {
            if (!cmd.trimmed().isEmpty()) {
                appendHistory(tr("Command '%1' not found").arg(cmd));
            }
        }
    }

    leCommand->setText("");
}



void QG_CommandWidget::tabPressed() {
    QStringList reducedChoice;
    QString typed = leCommand->text();
    QStringList choice;
    
    // try to list commands of current action:
    if (RS_DIALOGFACTORY->getGraphicView()!=NULL) {
        RS_ActionInterface* currentAction = 
            RS_DIALOGFACTORY->getGraphicView()->getCurrentAction();
        if (currentAction!=NULL) {
            choice = currentAction->getAvailableCommands(typed);
        }
        else {
            choice = RS_GuiAction::getAvailableCommands(typed, true);
        }
    }

    // command found:
    if (choice.count()==1) {
        leCommand->setText(choice.first());
    }
    else if (choice.count()>0) {
        // suggest available commands to user:
        appendHistory(choice.join(", "));
        // find maximum common start of commands to complete
        QString complete = getCommonStart(choice);
        leCommand->setText(complete);
    }
}



void QG_CommandWidget::slotEscape() {
    emit escape();
}

void QG_CommandWidget::setCommandMode() {
    QPalette p = lCommand->palette();
    p.setColor(QPalette::Foreground, Qt::blue);
    lCommand->setPalette(p);
}

void QG_CommandWidget::setNormalMode() {
    QPalette p = lCommand->palette();
    p.setColor(QPalette::Foreground, Qt::black);
    lCommand->setPalette(p);
}

void QG_CommandWidget::redirectStderr() {
    //fclose(stderr);
    //ferr = new QFile();
    //ferr->open(IO_ReadWrite, stderr);
    //std::streambuf buf;
    //errStream = new std::ostream(&errBuf);
    //std::cerr.rdbuf(errStream->rdbuf());
}

void QG_CommandWidget::processStderr() {
    /*
    if (errStream==NULL) {
        return;
    }
    
    std::string s = errBuf.str();
    if (s.length()!=0) {
        appendHistory(QString("%1").arg(s.c_str()));
    }
    //char c;
    / *while ((c=ferr->getch())!=-1) {
        appendHistory(QString("%1").arg(c));
    }
    ferr->close();* /
    */
}
        

QString QG_CommandWidget::getCommonStart(const QStringList& sl) {
    QString found;
    if (sl.count()>0) {
        for (int i=0; i<sl.first().length(); i++) {
            QString test = sl.first().left(i+1);
            for (QStringList::ConstIterator it2 = sl.begin(); 
                it2 != sl.end(); ++it2) {
                
                if (!(*it2).startsWith(test)) {
                    return found;
                    break;
                }
            }
            found =  test;
        }
    }
    return found;
}
