/*****************************************************************
 * $Id: db_actionsystemtoolsviewmodel.cpp 1489 2011-12-07 18:21:38Z rutger $
 * Created: Nov 24, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionsystemtoolsviewmodel.h"

#include <QInputDialog>
#include "db_actionfilesaveas.h"
#include "db_dialogfactory.h"
#include "db_objectfactory.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_objectbase.h"
#include "rb_xmlwriter.h"



DB_ActionSystemToolsViewModel::DB_ActionSystemToolsViewModel() : RB_Action() {

}


RB_GuiAction* DB_ActionSystemToolsViewModel::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(DB_ActionSystemToolsViewModel::getName(),
                                  // "fileclose",
                                  tr("&View Model"));
        action->setStatusTip(tr("View the current model in a text window"));
//        action->setShortcut("Ctrl+W");
        action->setFactory(DB_ActionSystemToolsViewModel::factory);
    }
    return action;
}

RB_Action* DB_ActionSystemToolsViewModel::factory() {
    RB_Action* a = new DB_ActionSystemToolsViewModel();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionSystemToolsViewModel::trigger() {
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

    /* In export model
    RB_String fileName = factoryName.toLower() + ".xml";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fileName);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    DB_FilterXml filter;
    filter.fileExport(f->getModelStructure("XX_Project"), fileName);
    QApplication::restoreOverrideCursor();
    */


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
            RB_XmlWriter* xtw = new RB_XmlWriter(&str);

            RB_ObjectBase* root = f->getModelStructure("XX_Project");
            root->acceptVisitor(*xtw);

            QTextEdit* te = tw->getTextEdit();
            te->setText(str);

            delete xtw;
        }
    }
    mdiWin->show();

}

