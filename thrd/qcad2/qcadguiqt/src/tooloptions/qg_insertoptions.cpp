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

#include "qg_insertoptions.h"

#include "rs_settings.h"
#include "rs_actionblocksinsert.h"

/**
 * Constructor
 */
QG_InsertOptions::QG_InsertOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(leAngle, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(leFactor, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(sbColumns, SIGNAL(valueChanged(int)),
        this, SLOT(updateData()));
    connect(sbRows, SIGNAL(valueChanged(int)),
        this, SLOT(updateData()));
    connect(leColumnSpacing, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
    connect(leRowSpacing, SIGNAL(textChanged(const QString&)),
        this, SLOT(updateData()));
}


/**
 * Destructor
 */
QG_InsertOptions::~QG_InsertOptions() {
    RS_DEBUG->print("QG_InsertOptions::~QG_InsertOptions");
    
    RS_SETTINGS->beginGroup("/Insert");
    RS_SETTINGS->writeEntry("/InsertAngle", leAngle->text());
    RS_SETTINGS->writeEntry("/InsertFactor", leFactor->text());
    RS_SETTINGS->writeEntry("/InsertColumns", sbColumns->text());
    RS_SETTINGS->writeEntry("/InsertRows", sbRows->text());
    RS_SETTINGS->writeEntry("/InsertColumnSpacing", leColumnSpacing->text());
    RS_SETTINGS->writeEntry("/InsertRowSpacing", leRowSpacing->text());
    RS_SETTINGS->endGroup();
}


void QG_InsertOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionBlocksInsert*>(a);
    if (action!=NULL) {
        QString sAngle;
        QString sFactor;
        QString sColumns;
        QString sRows;
        QString sColumnSpacing;
        QString sRowSpacing;
        if (update) {
            sAngle = QString("%1").arg(RS_Math::rad2deg(action->getAngle()));
            sFactor = QString("%1").arg(action->getFactor());
            sColumns = QString("%1").arg(action->getColumns());
            sRows = QString("%1").arg(action->getRows());
            sColumnSpacing = QString("%1").arg(action->getColumnSpacing());
            sRowSpacing = QString("%1").arg(action->getRowSpacing());
        } else {
            RS_SETTINGS->beginGroup("/Insert");
            sAngle = RS_SETTINGS->readEntry("/InsertAngle", "0.0");
            sFactor = RS_SETTINGS->readEntry("/InsertFactor", "1.0");
            sColumns = RS_SETTINGS->readEntry("/InsertColumns", "1");
            sRows = RS_SETTINGS->readEntry("/InsertRows", "1");
            sColumnSpacing = RS_SETTINGS->readEntry("/InsertColumnSpacing", "1.0");
            sRowSpacing = RS_SETTINGS->readEntry("/InsertRowSpacing", "1.0");
            RS_SETTINGS->endGroup();
        }
        leAngle->setText(sAngle);
        leFactor->setText(sFactor);
        sbColumns->setValue(sColumns.toInt());
        sbRows->setValue(sRows.toInt());
        leColumnSpacing->setText(sColumnSpacing);
        leRowSpacing->setText(sRowSpacing);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_InsertOptions::setAction: wrong action type");
        action = NULL;
    }
}



void QG_InsertOptions::updateData() {
    if (action!=NULL) {
        action->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
        action->setFactor(RS_Math::eval(leFactor->text()));
        action->setColumns(sbColumns->value());
        action->setRows(sbRows->value());
        action->setColumnSpacing(RS_Math::eval(leColumnSpacing->text()));
        action->setRowSpacing(RS_Math::eval(leRowSpacing->text()));
    }
}
