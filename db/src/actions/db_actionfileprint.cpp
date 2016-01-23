/*****************************************************************
 * $Id: db_actionfileprint.cpp 2049 2013-12-09 11:54:27Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfileprint.h"

#include <QPrinter>
#include <QPrintDialog>
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"


DB_ActionFilePrint::DB_ActionFilePrint() : RB_Action() {

}


RB_GuiAction* DB_ActionFilePrint::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFilePrint::getName(),
                                  QIcon(":/images/icons/printer.png"),
                                  tr("&Print..."));
        ga->setShortcut("Ctrl+P");
        ga->setStatusTip(tr("Prints the current document"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFilePrint::factory);
    }
    return ga;
}

RB_Action* DB_ActionFilePrint::factory() {
    RB_Action* a = new DB_ActionFilePrint();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFilePrint::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;
    RB_Widget* wdgt = mdiWin->getWidget();
    if (!wdgt) {
        wdgt = mWidget;
    }

    if (wdgt) {

#ifndef QT_NO_PRINTER
        QPrinter* pr = DB_ACTIONFACTORY->getPrinter();
        wdgt->setPrinterProperties(pr);
        pr->setOutputFormat(QPrinter::NativeFormat);

        QPrintDialog* dlg = new QPrintDialog(pr, wdgt);
        dlg->setWindowTitle(tr("Print Document"));
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
        if (dlg->exec() != QDialog::Accepted) return;

        wdgt->filePrint(pr);
 #endif
    }
}

/**
 * @brief Print content of a dialog, for example used by ACC_OrderPreviewDialog
 * @param dialog
 */
void DB_ActionFilePrint::printDialog(RB_Dialog* dialog) {
    if (!dialog) return;

#ifndef QT_NO_PRINTER
    QPrinter* pr = DB_ACTIONFACTORY->getPrinter();
    dialog->setPrinterProperties(pr);
    pr->setOutputFormat(QPrinter::NativeFormat);

    QPrintDialog* dlg = new QPrintDialog(pr, dialog);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() != QDialog::Accepted) return;

    dialog->filePrint(pr);
 #endif
}



