/*****************************************************************
 * $Id: acc_actionsettingsdefault.cpp 2113 2014-02-24 16:56:52Z rutger $
 * Created: Feb 24, 2014 15:45:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_actionsettingsdefault.h"

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

ACC_ActionSettingsDefault::ACC_ActionSettingsDefault() : RB_Action() {

}


RB_GuiAction* ACC_ActionSettingsDefault::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(ACC_ActionSettingsDefault::getName(),
                                  // "fileclose",
                                  tr("Set &defaults"));
        action->setStatusTip(tr("Set the default settings for the current project in the database"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(ACC_ActionSettingsDefault::factory);
    }
    return action;
}

RB_Action* ACC_ActionSettingsDefault::factory() {
    RB_Action* a = new ACC_ActionSettingsDefault();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void ACC_ActionSettingsDefault::trigger() {
//    RB_String fileName;
//    DB_ActionFileOpen a;
//    a.getOpenFileName(fileName,
//                      "eXtensible Markup Language files (*.xml);;"
//                      "All files (*.*)");
//    QFile f(fileName);
//    if (!f.open(QFile::ReadOnly | QFile::Text)) {
//        DB_DIALOGFACTORY->requestWarningDialog(
//                    tr("Cannot read file %1:\n%2 (1).")
//                    .arg(fileName)
//                    .arg(f.errorString()));
//        return;
//    }

    // Select English or Dutch chart of accounts
    bool ok = false;
    QStringList items;
    items << tr("English (EN)") << tr("Dutch (NL)");

    QString item = QInputDialog::getItem(DB_DIALOGFACTORY->getMainWindow(), tr("Select settings"),
                                         tr("Settings:"), items, 0, false, &ok);
    QFile f;

    if (!ok || item.isEmpty()) {
        DB_DIALOGFACTORY->requestInformationDialog(tr("Action canceled."));
        return;
    } else if (ok && item == tr("English (EN)")) {
        f.setFileName(":/resources/acc_chartmaster_syssetting_en.xml");
    } else if (ok && item == tr("Dutch (NL)")) {
        f.setFileName(":/resources/acc_chartmaster_syssetting_nl.xml");
    }

    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Cannot read file %1:\n%2 (1).")
                    .arg(f.fileName())
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
                    .arg(f.fileName())
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
                    tr("Default settings saved to database."));
    } else {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Saving default settings to database error."));
    }

    delete xr;
    delete root;
    f.close();
}

