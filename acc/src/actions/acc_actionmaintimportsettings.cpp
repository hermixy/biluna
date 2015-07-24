/*****************************************************************
 * $Id: acc_actionmaintimportsettings.cpp 1932 2013-05-02 12:18:08Z rutger $
 * Created: Jan 16, 2012 19:54:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionmaintimportsettings.h"

#include "acc_dbvisitor.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_settingidvisitor.h"
#include "acc_syssetxmlwriter.h"

#include "db_actionfileopen.h"
#include "db_dialogfactory.h"
#include "db_xmlreader.h"

// DEBUG ONLY
//#include "db_textwidget.h"
//#include "rb_mdiwindow.h"

ACC_ActionMaintImportSettings::ACC_ActionMaintImportSettings() : RB_Action() {

}


RB_GuiAction* ACC_ActionMaintImportSettings::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(ACC_ActionMaintImportSettings::getName(),
                                  // "fileclose",
                                  tr("&Import Settings"));
        action->setStatusTip(tr("Import the settings into the current project of the database"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(ACC_ActionMaintImportSettings::factory);
    }
    return action;
}

RB_Action* ACC_ActionMaintImportSettings::factory() {
    RB_Action* a = new ACC_ActionMaintImportSettings();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionMaintImportSettings::trigger() {
    RB_String fileName;
    DB_ActionFileOpen a;
    a.getOpenFileName(fileName,
                      "eXtensible Markup Language files (*.xml);;"
                      "All files (*.*)");
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Cannot read file %1:\n%2 (1).")
                    .arg(fileName)
                    .arg(f.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // read settings into object model
    RB_ObjectBase* root = ACC_MODELFACTORY->getRoot()->clone();
    DB_XmlReader* xr = new DB_XmlReader(root);
    bool useNewId = false;
    xr->setNewId(useNewId); // ID's are handled in ACC_DbVisitor

    bool success = xr->read(&f);

    if (!success) {
        QApplication::restoreOverrideCursor();
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Reading error %1:\n%2 (2)")
                    .arg(fileName)
                    .arg(xr->errorString()));
        delete xr;
        delete root;
        f.close();
        return;
    }

    // Update ID for ACC_AccountGroup objects and store old and new IDs
    // resolve level is already set to RB2::ResolveAll
    ACC_SettingIdVisitor* idVis = new ACC_SettingIdVisitor();
    idVis->setRelatedObjectId();
    root->acceptVisitor(*idVis);

    // Update all other IDs except Acc_AccountGroup and set accountgroup_id
    // in ACC_ChartMaster
    idVis->setObjectWithRelatedId();
    root->acceptVisitor(*idVis);
    delete idVis;

    // Database visitor for settings
    ACC_DbVisitor* vis = new ACC_DbVisitor();
    vis->setDatabase(ACC_MODELFACTORY->getDatabase());
    vis->init();

    if (vis->isSystemSettingExisting() && !vis->isProjectSettingExisting()) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("System settings already existing,\n"
                       "only project settings will be saved."));
    } else if (vis->isSystemSettingExisting()
               && vis->isProjectSettingExisting()) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("System and project settings already existing,\n"
                       "the operation will be canceled."));
        delete vis;
        delete xr;
        delete root;
        f.close();
        QApplication::restoreOverrideCursor();
        return;
    }

    vis->setResolveLevel(RB2::ResolveAll);
    vis->setDbUpdate();
    vis->setUpdateNewId(false); // already done

    // Write settings to database
    root->acceptVisitor(*vis);
    delete vis;

    QApplication::restoreOverrideCursor();

    if (success) {
        DB_DIALOGFACTORY->requestInformationDialog(
                    tr("Model saved to database."));
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Saving model to database error."));
    }

    delete xr;
    delete root;
    f.close();
}

