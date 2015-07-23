/*****************************************************************
 * $Id: db_actionfileprintpreview.cpp 2218 2015-02-20 16:03:26Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_actionfileprintpreview.h"

#include <QPrinter>
#include <QPrintPreviewDialog>
#include "db_actionfactory.h"
#include "db_dialogfactory.h"
#include "db_htmleditorwidget.h"
#include "rb_mdiwindow.h"
#include "rb_widget.h"

/**
 * Constructor.
 */
DB_ActionFilePrintPreview::DB_ActionFilePrintPreview() : RB_Action() {
//    showOptions();
}

/**
 * Destructor
 */
DB_ActionFilePrintPreview::~DB_ActionFilePrintPreview() {}


RB_GuiAction* DB_ActionFilePrintPreview::createGuiAction() {
    static RB_GuiAction* ga = NULL;

    if (ga==NULL) {
        ga = new RB_GuiAction(DB_ActionFilePrintPreview::getName(),
                                  QIcon(":/images/icons/printer_preview.png"),
                                  tr("Print Pre&view"));
        ga->setStatusTip(tr("Shows a preview of a print"));

        // The setFactory() connects ga->triggered() to ga->slotTriggered()
        ga->setFactory(DB_ActionFilePrintPreview::factory);
    }
    return ga;
}

RB_Action* DB_ActionFilePrintPreview::factory() {
    RB_Action* a = new DB_ActionFilePrintPreview();
    // no graphicsView with eventhandler which triggers and deletes the action
    a->trigger();
    delete a;
    a = NULL;
    return a;
}

/**
 * Trigger this action, which is done after all data and objects are set
 */
void DB_ActionFilePrintPreview::trigger() {
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
        mWidget = wdgt;

#ifndef QT_NO_PRINTER
        QPrinter* pr = DB_ACTIONFACTORY->getPrinter();
        wdgt->setPrinterProperties(pr);
        pr->setOutputFormat(QPrinter::NativeFormat);

        QPrintPreviewDialog* dlg =
                new QPrintPreviewDialog(pr, wdgt);

//#ifdef Q_OS_MAC
//        // Required for MacOSX otherwise no close button present
//        QPushButton* pbClose = new QPushButton(QIcon(":/images/icons/cross.png"),
//                                               tr("Close"), dlg);
//        QHBoxLayout* hLayout = new QHBoxLayout();
//        hLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding,
//                                               QSizePolicy::Minimum));
//        hLayout->addWidget(pbClose);
//        QWidget* wdgt = new QWidget();
//        wdgt->setLayout(hLayout);
//        dlg->layout()->addWidget(wdgt);
//        connect(pbClose, SIGNAL(clicked()), dlg, SLOT(close()));
//#endif

        // Use the paintRequested signal otherwise printing will start immediately
        connect(dlg, SIGNAL(paintRequested(QPrinter*)),
                this, SLOT(slotFilePrintPreview(QPrinter*)));
        dlg->exec();
        dlg->deleteLater();
#endif
    }
}

/**
 * Slot for print preview signal
 */
void DB_ActionFilePrintPreview::slotFilePrintPreview(QPrinter* pr) {
    if (!mWidget) return;
    mWidget->filePrintPreview(pr);
}

