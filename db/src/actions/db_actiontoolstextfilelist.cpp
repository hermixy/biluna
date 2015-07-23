/*****************************************************************
 * $Id: db_actiontoolstextfilelist.cpp 2105 2014-02-19 21:18:40Z rutger $
 * Created: Jul 13, 2011 3:14:42 PM - rutger
 *
 * Copyright (C) 201! Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actiontoolstextfilelist.h"

#include "db_actionfileopenmultiple.h"
#include "db_actionfilenew.h"
#include "db_dialogfactory.h"
#include "db_prepostfixtextdialog.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "db_textwidget.h"


DB_ActionToolsTextFileList::DB_ActionToolsTextFileList() : RB_Action() {}


RB_GuiAction* DB_ActionToolsTextFileList::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionToolsTextFileList::getName(),
                                  QIcon(":/images/icons/application_view_list.png"),
                                  tr("&Text File List"));
        action->setStatusTip(tr("Creates text of a file list"));
        action->setFactory(DB_ActionToolsTextFileList::factory);
    }                         
    return action;
}

RB_Action* DB_ActionToolsTextFileList::factory() {
    RB_Action* a = new DB_ActionToolsTextFileList();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionToolsTextFileList::trigger() {
    DB_ActionFileNew fileNewAction;
    fileNewAction.trigger();

    // Get active MDI widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMainWindow()->getActiveMdiWindow();
    if (!mdiWin) return;
    DB_TextWidget* wdgt = dynamic_cast<DB_TextWidget*>(mdiWin->getWidget());
    if (!wdgt) return;

    // Set prefix or postfix
    DB_PrePostfixTextDialog dlgPrePost(DB_DIALOGFACTORY->getMainWindow());
    dlgPrePost.setWindowTitle(tr("Set file pre- and postfix"));
    if (dlgPrePost.exec() == QDialog::Rejected) return;

    RB_String prefixText = dlgPrePost.getPrefix();
    RB_String postfixText = dlgPrePost.getPostfix();

    RB_StringList fileList;

    DB_ActionFileOpenMultiple a;
    a.getOpenFileNames(fileList, "All files (*.*);;");

    if (fileList.count() < 1) {
        DB_DIALOGFACTORY->commandMessage(tr("File list empty, no action."));
        DB_DIALOGFACTORY->statusBarMessage(tr("File list empty, no action."), 4000);
        return;
    }

    RB_String str;

    for (int i = 0; i < fileList.count(); ++i) {
        str += prefixText
                + QFileInfo(fileList.at(i)).fileName()
                + postfixText + "\n";
    }

    wdgt->appendText(str);
}

