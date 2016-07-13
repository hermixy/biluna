/*****************************************************************
 * $Id: db_actionsystemtoolsimportmodel.cpp 1494 2011-12-15 19:53:41Z rutger $
 * Created: Nov 29, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsimportmodel.h"

#include <QInputDialog>
#include "db_actionfileopen.h"
#include "db_dialogfactory.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "db_xmlreader.h"
#include "rb_mdiwindow.h"
#include "rb_objectbase.h"

#include "rb_xmlreader.h"
#include "rb_xmlwriter.h"
#include "db_textwidget.h"


DB_ActionSystemToolsImportModel::DB_ActionSystemToolsImportModel() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsImportModel::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionSystemToolsImportModel::getName(),
                                  // "fileclose",
                                  tr("&Import Model Data"));
        action->setStatusTip(tr("Import model data to the database"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(DB_ActionSystemToolsImportModel::factory);
    }
    return action;
}

RB_Action* DB_ActionSystemToolsImportModel::factory() {
    RB_Action* a = new DB_ActionSystemToolsImportModel();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsImportModel::trigger() {
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

    bool useNewId = false;
    int result = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("New ID"), tr("Do you want to use new ID's?"));
    if (result == QMessageBox::Yes) {
        useNewId = true;
    }

    bool saveToDatabase = false;
    result = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Save to database"), tr("Do you want to save XML to database (Yes),\n"
                                           "or show on a text window (No)?"));
    if (result == QMessageBox::Yes) {
        saveToDatabase = true;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_ObjectContainer* root = new RB_ObjectContainer("", NULL, "DB");
    RB_XmlReader* xr = new RB_XmlReader(root);
    xr->setNewId(useNewId);

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

    if (saveToDatabase) {
        // Save to database
        root->setValue("parent", "none"); // otherwise root object is not stored
        success = root->dbUpdate(DB_MODELFACTORY->getDatabase(), RB2::ResolveAll);
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
    } else {
        // Write to widget
        RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getMdiWindow(
                DB_DialogFactory::WidgetTextDocument);
        if (!mdiWin) return;

        RB_Widget* wdgt = mdiWin->getWidget();

        if (!wdgt) {
            // widget set from outside
            wdgt = mWidget;
        }

        if (wdgt) {
            // wdgt->init(); not required
            DB_TextWidget* tw = dynamic_cast<DB_TextWidget*>(wdgt);
            if (tw) {
                RB_String str;
                RB_XmlWriter* xw = new RB_XmlWriter(&str);

                root->acceptVisitor(*xw);

                QTextEdit* te = tw->getTextEdit();
                te->setText(str);

                delete xw;
            }
        }
        QApplication::restoreOverrideCursor();
        mdiWin->show();
    }
}

