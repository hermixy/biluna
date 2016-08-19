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

#include "qg_linebisectoroptions.h"

#include "rs_actiondrawlinebisector.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_LineBisectorOptions::QG_LineBisectorOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leLength, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLength(const QString&)));
    connect(sbNumber, SIGNAL(valueChanged(int)),
            this, SLOT(updateNumber(int)));
}


/**
 * Destructor
 */
QG_LineBisectorOptions::~QG_LineBisectorOptions() {
    RS_DEBUG->print("QG_LineBisectorOptions::~QG_LineBisectorOptions");

    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/LineBisectorLength", leLength->text());
    RS_SETTINGS->writeEntry("/LineBisectorNumber", sbNumber->text());
    RS_SETTINGS->endGroup();
}


void QG_LineBisectorOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawLineBisector*>(a);
    if (action!=NULL) {
        QString sl;
        QString sn;
        if (update) {
            sl = QString("%1").arg(action->getLength());
            sn = QString("%1").arg(action->getNumber());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sl = RS_SETTINGS->readEntry("/LineBisectorLength", "1.0");
            sn = RS_SETTINGS->readEntry("/LineBisectorNumber", "1");
            RS_SETTINGS->endGroup();
        }
        leLength->setText(sl);
        sbNumber->setValue(sn.toInt());
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_LineBisectorOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_LineBisectorOptions::updateLength(const QString& l) {
    if (action!=NULL) {
        action->setLength(RS_Math::eval(l));
    }
}

void QG_LineBisectorOptions::updateNumber(int n) {
    if (action!=NULL) {
        action->setNumber(n);
    }
}
