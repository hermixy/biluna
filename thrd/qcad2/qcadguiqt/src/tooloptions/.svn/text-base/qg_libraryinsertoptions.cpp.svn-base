/****************************************************************************
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
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

#include "qg_libraryinsertoptions.h"

#include "rs_actionlibraryinsert.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LibraryInsertOptions::QG_LibraryInsertOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(leFactor, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_LibraryInsertOptions::~QG_LibraryInsertOptions() {
    RS_SETTINGS->beginGroup("/LibraryInsert");
    RS_SETTINGS->writeEntry("/LibraryInsertAngle", leAngle->text());
    RS_SETTINGS->writeEntry("/LibraryInsertFactor", leFactor->text());
    RS_SETTINGS->endGroup();
}


void QG_LibraryInsertOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionLibraryInsert*>(a);
    if (action!=NULL) {
        QString sAngle;
        QString sFactor;
        if (update) {
            sAngle = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            sFactor = QString("%1").arg(action->getFactor());
        } else {
            RS_SETTINGS->beginGroup("/LibraryInsert");
            sAngle = RS_SETTINGS->readEntry("/LibraryInsertAngle", "0.0");
            sFactor = RS_SETTINGS->readEntry("/LibraryInsertFactor", "1.0");
            RS_SETTINGS->endGroup();
        }
        leAngle->setText(sAngle);
        leFactor->setText(sFactor);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_LibraryInsertOptions::setAction: wrong action type");
        action = NULL;
    }
}



void QG_LibraryInsertOptions::updateData() {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
        action->setFactor(RS_Math::eval(leFactor->text()));
    }
}
