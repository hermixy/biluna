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

#include "qg_polylineoptions.h"

#include "rs_actiondrawpolyline.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_PolylineOptions::QG_PolylineOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(bClose, SIGNAL(clicked()),
        this, SLOT(close()));
    connect(bUndo, SIGNAL(clicked()),
        this, SLOT(undo()));
    connect(leRadius, SIGNAL(textChanged(const QString&)),
        this, SLOT(setRadius(const QString&)));
    connect(cbArc, SIGNAL(toggled(bool)),
        leRadius, SLOT(setEnabled(bool)));
    connect(cbArc, SIGNAL(toggled(bool)),
        lRadius, SLOT(setEnabled(bool)));
    connect(cbArc, SIGNAL(toggled(bool)),
        this, SLOT(setArc(bool)));
}


/**
 * Destructor
 */
QG_PolylineOptions::~QG_PolylineOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    //RS_SETTINGS->writeEntry("/PolylineArc", (int)cbArc->isChecked());
    RS_SETTINGS->writeEntry("/PolylineArcRadius", leRadius->text());
    RS_SETTINGS->endGroup();
}


void QG_PolylineOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDrawPolyline*>(a);
    if (action!=NULL) {
        QString sa;
        QString sar;
        if (update) {
            sa = QString("%1").arg(action->isArc());
            sar = QString("%1").arg(action->getRadius());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            //sa = RS_SETTINGS->readEntry("/PolylineArc", "0");
            sa = "0";
            sar = RS_SETTINGS->readEntry("/PolylineArcRadius", "1.0");
            RS_SETTINGS->endGroup();
            setRadius(sar);
            setArc(false);
            RS_DEBUG->print("QG_PolylineOptions::setAction: radius and arc set");
        }
        cbArc->setChecked(sa=="1");
        lRadius->setEnabled(sa=="1");
        leRadius->setEnabled(sa=="1");
        leRadius->setText(sar);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_PolylineOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_PolylineOptions::close() {
    if (action!=NULL) {
        action->close();
    }
}

void QG_PolylineOptions::undo() {
    if (action!=NULL) {
        action->undo();
    }
}

void QG_PolylineOptions::setRadius(const QString& r) {
    if (action!=NULL) {
        action->setRadius(r.toDouble());
    }
}

void QG_PolylineOptions::setArc(bool on) {
    if (action!=NULL) {
        action->setArc(on);
        setRadius(leRadius->text());
    }
}
