/*****************************************************************
 * $Id: srm_actionsysseqno.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 26, 2014 10:51:42 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_actionsysseqno.h"

#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "rb_mdiwindow.h"


SRM_ActionSysSeqNo::SRM_ActionSysSeqNo()
        : RB_Action() {
    // nothing
}


RB_GuiAction* SRM_ActionSysSeqNo::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (!ga) {
        ga = new RB_GuiAction(SRM_ActionSysSeqNo::getName(),
                              //QIcon(":/images/icons/star.png"),
                              tr("S&ystem Sequence Numbers"));
        ga->setStatusTip(tr("System sequence number maintenance"));
        ga->setFactory(SRM_ActionSysSeqNo::factory);
    }
    return ga;
}

RB_Action* SRM_ActionSysSeqNo::factory() {
    RB_Action* a = new SRM_ActionSysSeqNo();
    // no graphicsView with eventhandler which deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void SRM_ActionSysSeqNo::trigger() {
    // Check required settings
    if (SRM_MODELFACTORY->getRootId() == ""
            || !SRM_MODELFACTORY->getDatabase().isOpen()) {
        SRM_DIALOGFACTORY->requestWarningDialog(tr("No project selected.\n"
                                                   "Connect first to database\n"
                                                   "and then select project."));
        return;
    }

    // Create dialog
    RB_Dialog* dlg = SRM_DIALOGFACTORY->getDialog(SRM_DialogFactory::DialogSysSeqNo);
    dlg->exec();
    dlg->deleteLater();
}
