/*****************************************************************
 * $Id: db_commandwidget.cpp 2233 2015-04-29 19:10:59Z rutger $
 * Created: Nov 11, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_commandwidget.h"

#include "rb_action.h"
#include "db_dialogfactory.h"
#include "rb_guiaction.h"
#include "rb_mdiwindow.h"

/**
 * Constructor
 */
DB_CommandWidget::DB_CommandWidget(QWidget* parent) : RB_Widget(parent) {

    setupUi(this);
    
    leCommand->setFrame(false);
    leCommand->setFocusPolicy(Qt::StrongFocus);
    teHistory->document()->setMaximumBlockCount(1000);
    
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
    connect(leCommand, SIGNAL(clearAll()),
        teHistory, SLOT(clear()));
}


/**
 * Destructor, invalidates widget at dialog factory
 */
DB_CommandWidget::~DB_CommandWidget() {
    // nothing yet
}


bool DB_CommandWidget::checkFocus() {
    return leCommand->hasFocus();
}

void DB_CommandWidget::setFocus() {
    leCommand->setFocus();
}


void DB_CommandWidget::setCommand(const QString& cmd) {
    if (cmd!=lCommand->text()) {
        if (cmd!="") {
            lCommand->setText(cmd);
        } else {
            lCommand->setText(tr("Command:"));
        }
        leCommand->setText("");
    }
}


void DB_CommandWidget::appendHistory(const QString& msg) {
    teHistory->append(msg);
}


void DB_CommandWidget::trigger() {
    QString cmd = leCommand->text();

    if (cmd=="") {
        cmd="\n";
    } else {
        // 20080202: command appended in RB_GuiAction
        //appendHistory(cmd);
    }

    DB_CommandEvent e(cmd);

    // try to dispatch command to running action:
    if (DB_DIALOGFACTORY->getActiveMdiWindow()) {
        DB_DIALOGFACTORY->getActiveMdiWindow()->commandEvent(&e);
    }

    // trigger new action:
    if (!e.isAccepted()) {
        bool success = RB_GuiAction::triggerByCommand(cmd);

        if (!success) {
            if (!cmd.trimmed().isEmpty()) {
                appendHistory(tr("Command '%1' not found").arg(cmd));
            }
        }
    }

    leCommand->setText("");
}



void DB_CommandWidget::tabPressed() {
    QStringList reducedChoice;
    QString typed = leCommand->text();
    QStringList choice;
    
    // try to list commands of current action:
    if (DB_DIALOGFACTORY->getActiveMdiWindow()) {
        RB_Action* currentAction =
            DB_DIALOGFACTORY->getActiveMdiWindow()->getCurrentAction();
        if (currentAction!=NULL) {
            choice = currentAction->getAvailableCommands(typed);
        } else {
            choice = RB_GuiAction::getAvailableCommands(typed, true);
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

void DB_CommandWidget::slotEscape() {
    emit escape();
}

void DB_CommandWidget::setCommandMode() {
    QPalette p = lCommand->palette();
    p.setColor(QPalette::Foreground, Qt::blue);
    lCommand->setPalette(p);
}

void DB_CommandWidget::setNormalMode() {
    QPalette p = lCommand->palette();
    p.setColor(QPalette::Foreground, Qt::black);
    lCommand->setPalette(p);
}

void DB_CommandWidget::redirectStderr() {
    //fclose(stderr);
    //ferr = new QFile();
    //ferr->open(IO_ReadWrite, stderr);
    //std::streambuf buf;
    //errStream = new std::ostream(&errBuf);
    //std::cerr.rdbuf(errStream->rdbuf());
}

void DB_CommandWidget::processStderr() {
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
        

QString DB_CommandWidget::getCommonStart(const QStringList& sl) {
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
