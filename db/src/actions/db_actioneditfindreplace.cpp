/*****************************************************************
 * $Id: db_actioneditfindreplace.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Feb 22, 2012 10:34:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditfindreplace.h"

#include <QTextEdit>
#include <QWebView>
#include "db_dialogfactory.h"
#include "db_findreplacedialog.h"
#include "rb_mdiwindow.h"



DB_ActionEditFindReplace::DB_ActionEditFindReplace()
        : RB_Action() {
    // nothing
}


RB_GuiAction* DB_ActionEditFindReplace::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(DB_ActionEditFindReplace::getName(),
                              QIcon(":/images/icons/find_replace.png"),
                              tr("Find and &Replace"));
//        ga->setCommand("da, database");
//        ga->setKeycode("daa");
        ga->setShortcut("Ctrl+R");
        ga->setStatusTip(tr("Find and replace"));
        ga->setToolTip("Find and replace");

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        // which in turn executes factory(). In factory the choice is made
        // to execute this action on its own like below, or to add the action
        // to for example a graphicsView's eventHandler which sets the
        // handling of the command-, mouse- and keyEvents at the graphicsView
        // If ga->setFactory is not used the RB_GuiAction will emit only
        // triggered() which can be connected directly to a mainWindow
        ga->setFactory(DB_ActionEditFindReplace::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditFindReplace::factory() {
    // Prerequisites
    RB_MdiWindow* mdiW = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiW) return NULL;
    RB_Widget* wdgt = mdiW->getWidget();
    if (!wdgt) return NULL;

    RB_Dialog* dlg = DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogFindReplace);
    DB_FindReplaceDialog* frDlg = dynamic_cast<DB_FindReplaceDialog*>(dlg);
    frDlg->init();
    frDlg->showReplaceWidgets(); // extra compared with 'find'

    QWidget* focusWgt = wdgt->focusWidget();

    if (QTextEdit* te = dynamic_cast<QTextEdit*>(focusWgt)) {
        frDlg->setTextEdit(te);
    } else if (QPlainTextEdit* pte = dynamic_cast<QPlainTextEdit*>(focusWgt)) {
        frDlg->setPlainTextEdit(pte);
    } else if (QWebView* wv = dynamic_cast<QWebView*>(focusWgt)) {
        frDlg->setWebView(wv);
    } else {
        DB_DIALOGFACTORY->requestWarningDialog("Find/replace not applicable,\n"
                                               "this is not a text window");
        dlg->deleteLater();
        return NULL;
    }

    // Modal
    // dlg->exec();

    // Modeless, delete on close or by deletion of mainwindow (is parent widget)
    dlg->show();
    dlg->raise();
    dlg->activateWindow();

    return NULL;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
//void DB_ActionEditFindReplace::trigger() {
//    //
//    // Prepare models
//    //
//    RB_MmObjectInterface* model = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTest);
//    model->setMainModel(true);
//    model->setRoot(); // equal to mModel->setRoot("");
//
//    // Fill model with some rows
//    model->insertRows(0, 1, QModelIndex());
//    model->insertRows(1, 1, QModelIndex());
//
//    // Child model and connect to child (table view),
// //    RB_MmObjectInterface* childModel = DB_MmTestChild::getInstance();
//    RB_MmObjectInterface* childModel = DB_TESTMODELFACTORY->getModel(DB_TestModelFactory::ModelTestChild);
//    childModel->setModel("DB_TestChildList", model);
//
//    // Activate models
//    mTableWidget->setModel();
//}
