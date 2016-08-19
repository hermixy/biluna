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

#include "qg_snapdistoptions.h"

#include "rs_settings.h"
#include "rs_snapper.h"
#include "rs_math.h"

/**
 * Constructor
 */
QG_SnapDistOptions::QG_SnapDistOptions(QWidget* parent)
        : QG_SnapOptionsBase(parent) {
    setupUi(this);

    connect(leDist, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateDist()));
    
    RS_SETTINGS->beginGroup("/Snap");
    QString r = RS_SETTINGS->readEntry("/Distance", "1.0");
    RS_SETTINGS->endGroup();

    leDist->setText(r);
}


/**
 * Destructor
 */
QG_SnapDistOptions::~QG_SnapDistOptions() {
    RS_SETTINGS->beginGroup("/Snap");
    RS_SETTINGS->writeEntry("/Distance", leDist->text());
    RS_SETTINGS->endGroup();
}



void QG_SnapDistOptions::setSnapper(RS_Snapper* s) {
    QG_SnapOptionsBase::setSnapper(s);

    updateDist();
}


/*void QG_SnapDistOptions::setDist(double* d) {
    dist = d;

    RS_SETTINGS->beginGroup("/Snap");
    QString r = RS_SETTINGS->readEntry("/Distance", "1.0");
    RS_SETTINGS->endGroup();

    leDist->setText(r);
}
*/

void QG_SnapDistOptions::updateDist() {
    if (snapper!=NULL) {
        QString d = leDist->text();
        snapper->setDistance(RS_Math::eval(d, 1.0));
    }
}
