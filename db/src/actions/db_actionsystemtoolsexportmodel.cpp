/*****************************************************************
 * $Id: db_actionsystemtoolsexportmodel.cpp 1494 2011-12-15 19:53:41Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsexportmodel.h"

#include <QInputDialog>
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_objectfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_objectbase.h"
#include "rb_htmlwriter.h"
#include "rb_xmlwriter.h"



DB_ActionSystemToolsExportModel::DB_ActionSystemToolsExportModel() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsExportModel::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionSystemToolsExportModel::getName(),
                                  // "fileclose",
                                  tr("E&xport Model Structure"));
        action->setStatusTip(tr("Export the current model structure to file"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(DB_ActionSystemToolsExportModel::factory);
    }
    return action;
}

RB_Action* DB_ActionSystemToolsExportModel::factory() {
    RB_Action* a = new DB_ActionSystemToolsExportModel();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsExportModel::trigger() {
    // Get model
    RB_ObjectFactory* f = NULL;
    QStringList items;
    items << tr("None");

    std::vector<RB_ObjectFactory*> fl = DB_OBJECTFACTORY->getFactoryList();
    std::vector<RB_ObjectFactory*>::iterator iter;
    iter = fl.begin();
    while (iter != fl.end()) {
        items.append((*iter)->getName());
        ++iter;
    }

    bool ok = false;

    QString factoryName = QInputDialog::getItem(DB_DIALOGFACTORY->getMainWindow(),
                                      tr("Select model ..."),
                                      tr("Model"), items, 0, false, &ok);
    if (ok && !factoryName.isEmpty()) {
        iter = fl.begin();
        while (iter != fl.end()) {
            if (factoryName == (*iter)->getName()) {
                f = (*iter);
            }
            ++iter;
        }
    }

    if (!f) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("Model not found."));
        return;
    }


    RB_String fileName = factoryName.toLower() + ".xml";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fileName);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text)) {
        DB_DIALOGFACTORY->requestWarningDialog(
                    tr("Cannot write file %1:\n%2 (1).")
                    .arg(fileName)
                    .arg(file.errorString()));
        return;
    }

    RB_XmlWriter* xw = new RB_XmlWriter(&file);

    RB_ObjectBase* root = f->getModelStructure("XX_Project");
    root->acceptVisitor(*xw);
    delete xw;

    QApplication::restoreOverrideCursor();

/*
    // Test HTML writing only
    // Create widget
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
            RB_HtmlWriter* xw = new RB_HtmlWriter(&str);

            RB_ObjectBase* root = f->getModelStructure("XX_Project");
            root->acceptVisitor(*xw);

            QTextEdit* te = tw->getTextEdit();
            te->setPlainText(str);

            delete xw;
        }
    }

    QApplication::restoreOverrideCursor();
    mdiWin->show();
*/
}

