/*****************************************************************
 * $Id: db_actioneditinsertimage.cpp 1809 2012-12-03 12:26:40Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actioneditinsertimage.h"

#include "db_actionfileopen.h"
#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

DB_ActionEditInsertImage::DB_ActionEditInsertImage() : RB_Action() {}


RB_GuiAction* DB_ActionEditInsertImage::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionEditInsertImage::getName(),
                                  QIcon(":/images/icons/picture_add.png"),
                                  tr("Insert &Image"));
        ga->setStatusTip(tr("Insert Image"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionEditInsertImage::factory);
    }
    return ga;
}

RB_Action* DB_ActionEditInsertImage::factory() {
    RB_Action* a = new DB_ActionEditInsertImage();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionEditInsertImage::trigger() {
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
        // wdgt->init(); not required
        QString filters;
        filters += tr("Common Graphics (*.png *.jpg *.jpeg *.gif);;");
        filters += tr("Portable Network Graphics (PNG) (*.png);;");
        filters += tr("JPEG (*.jpg *.jpeg);;");
        filters += tr("Graphics Interchange Format (*.gif);;");
        filters += tr("All Files (*)");

        RB_String fn = "";
        DB_ActionFileOpen a;
        a.getOpenFileName(fn, filters);

        wdgt->editInsertImage(fn);
    }
}
