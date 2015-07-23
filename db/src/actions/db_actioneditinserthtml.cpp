/*****************************************************************
 * $Id: db_actioneditinserthtml.cpp 1639 2012-05-16 08:30:35Z rutger $
 * Created: Apr 24, 2012 4:25:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditinserthtml.h"

#include "db_dialogfactory.h"
#include "db_inserthtmldialog.h"
#include "db_htmleditorwidget.h"
#include "db_textwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditInsertHtml::DB_ActionEditInsertHtml() : RB_Action() {}


RB_GuiAction* DB_ActionEditInsertHtml::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditInsertHtml::getName(),
                                  QIcon(":/images/icons/tag_add.png"),
                                  tr("Insert &HTML"));
        ga->setStatusTip(tr("Insert Image"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionEditInsertHtml::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditInsertHtml::factory() {
    RB_Action* a = new DB_ActionEditInsertHtml();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditInsertHtml::trigger() {
    // No preliminary checks required

    // No models to prepare

    // Get active widget, html editor
    RB_MdiWindow* mdiWin = DB_DIALOGFACTORY->getActiveMdiWindow();
    if (!mdiWin) return;
    DB_HtmlEditorWidget* hwdgt = dynamic_cast<DB_HtmlEditorWidget*>(mdiWin->getWidget());
    if (!hwdgt) {
        hwdgt = dynamic_cast<DB_HtmlEditorWidget*>(mWidget);
    }
    if (hwdgt) {
        DB_InsertHtmlDialog* dlg =
                dynamic_cast<DB_InsertHtmlDialog*>
                (DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogInsertHTML));

        if (dlg->exec() == QDialog::Accepted) {
            hwdgt->insertHtml(dlg->getHtml());
            return;
        }
    }

    // Rich text editor
    DB_TextWidget* twdgt = dynamic_cast<DB_TextWidget*>(mdiWin->getWidget());
    if (!twdgt) {
        twdgt = dynamic_cast<DB_TextWidget*>(mWidget);
    }
    if (twdgt) {
        DB_InsertHtmlDialog* dlg =
                dynamic_cast<DB_InsertHtmlDialog*>
                (DB_DIALOGFACTORY->getDialog(DB_DialogFactory::DialogInsertHTML));

        if (dlg->exec() == QDialog::Accepted) {
            twdgt->insertHtml(dlg->getHtml());
        }
    }
}
