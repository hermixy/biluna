/*****************************************************************
 * $Id: acc_actionmaintexportsettings.cpp 1864 2013-01-10 17:45:55Z rutger $
 * Created: Jan 16, 2012 11:54:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionmaintexportsettings.h"

#include "acc_dbvisitor.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_syssetxmlwriter.h"
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_objectfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_objectbase.h"
#include "rb_htmlwriter.h"



ACC_ActionMaintExportSettings::ACC_ActionMaintExportSettings() : RB_Action() {

}


RB_GuiAction* ACC_ActionMaintExportSettings::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(ACC_ActionMaintExportSettings::getName(),
                                  // "fileclose",
                                  tr("E&xport Settings"));
        action->setStatusTip(tr("Export the current model settings to file"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(ACC_ActionMaintExportSettings::factory);
    }
    return action;
}

RB_Action* ACC_ActionMaintExportSettings::factory() {
    RB_Action* a = new ACC_ActionMaintExportSettings();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionMaintExportSettings::trigger() {
    RB_String fileName = "acc_syssetting.xml";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fileName);

    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Cannot write file %1:\n%2 (1).")
                    .arg(fileName)
                    .arg(file.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Database visitor for settings
    ACC_DbVisitor* vis = new ACC_DbVisitor();
    vis->setDatabase(ACC_MODELFACTORY->getDatabase());
    vis->setResolveLevel(RB2::ResolveAll);
    vis->setDbRead();

    // Read settings from database
    RB_String rootId = ACC_MODELFACTORY->getRootId();
    RB_ObjectBase* root = ACC_OBJECTFACTORY->newSingleObject("ACC_Project");
    root->setId(rootId);
    root->acceptVisitor(*vis);
    delete vis;

    // Write settings to file
    ACC_SysSetXmlWriter* xw = new ACC_SysSetXmlWriter(&file);
    root->acceptVisitor(*xw);
    delete xw;
    delete root;

    QApplication::restoreOverrideCursor();
}

