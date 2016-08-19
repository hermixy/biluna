/****************************************************************************
**
** Copyright (C) 2008 RibbonSoft. All rights reserved.
**
** This file is part of the QCad project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid QCad Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "qc_application.h"

#include <QMessageBox>



/**
 * Mac OS X event handling to open files on double-click.
 */
bool QC_Application::event(QEvent *ev) {
    bool eaten;

    switch (ev->type()) {

    case QEvent::FileOpen: {
            QString file = static_cast<QFileOpenEvent*>(ev)->file();
            if (appWin!=NULL) {
                bool existing = false;

                // switch to existing MDI with file that was openened again:
                QWidgetList list = appWin->getWorkspace()->windowList();
                for (int i=0; i<list.count(); ++i) {
                    QG_MdiWindow* mdiWindow = (QG_MdiWindow*)list.at(i);
                    RS_Document* document = mdiWindow->getDocument();
                    if (document==NULL) {
                        break;
                    }

                    if (document->getFilename()==file) {
                        existing = true;
                        appWin->getWorkspace()->setActiveWindow(mdiWindow);
                        break;
                    }

                    /*
                    QMessageBox::warning(NULL, "Debug", 
                        QString("existing: %1\nnew: %2")
                        .arg(document->getFilename())
                        .arg(file));
                    */
                }

                // open the new file:
                if (!existing) {
                    appWin->slotFileOpen(file, RS2::FormatUnknown);
                }
            }
            eaten = true;
        }
        break;

    default:
        eaten = QApplication::event(ev);
        break;
    }

    return eaten;
}


