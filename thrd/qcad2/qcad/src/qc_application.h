/****************************************************************************
** $Id: qc_applicationwindow.h 8340 2008-01-20 17:28:31Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
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

#ifndef QC_APPLICATION_H
#define QC_APPLICATION_H


#include <QApplication>

#include "qc_applicationwindow.h"



/**
 * Main application. Handles mac events for opening files.
 *
 * @author Andrew Mustun
 */
class QC_Application: public QApplication {
    Q_OBJECT

public:
    QC_Application(int &argc, char **argv) 
        : QApplication(argc, argv), appWin(NULL) {
    }
    virtual ~QC_Application() {}

    void setAppWin(QC_ApplicationWindow* aw) {
        appWin = aw;
    }

protected:
    bool event(QEvent *ev);

private:
    QC_ApplicationWindow* appWin;
};



#endif
