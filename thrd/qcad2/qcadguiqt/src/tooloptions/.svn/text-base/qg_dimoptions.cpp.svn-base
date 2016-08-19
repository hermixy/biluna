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

#include "qg_dimoptions.h"

#include "rs_actiondimension.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_DimOptions::QG_DimOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);
    
    cbSymbol->addItem(QString("%1").arg(QChar(248)));
    cbSymbol->addItem(QString("%1").arg(QChar(176)));
    cbSymbol->addItem(QString("%1").arg(QChar(177)));
    cbSymbol->addItem(QString("%1").arg(QChar(960)));
    cbSymbol->addItem(QString("%1").arg(QChar(215)));
    cbSymbol->addItem(QString("%1").arg(QChar(247)));
    cbSymbol->addItem(QString("%1").arg(QChar(0x25FB)));

    connect(leLabel, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLabel()));
    connect(bDiameter, SIGNAL(toggled(bool)),
            this, SLOT(updateLabel()));
    connect(leTol1, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLabel()));
    connect(leTol2, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateLabel()));
    connect(cbSymbol, SIGNAL(activated(const QString&)),
            this, SLOT(insertSign(const QString&)));
}


/**
 * Destructor
 */
QG_DimOptions::~QG_DimOptions() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/DimLabel", leLabel->text());
    RS_SETTINGS->writeEntry("/DimTol1", leTol1->text());
    RS_SETTINGS->writeEntry("/DimTol2", leTol2->text());
    RS_SETTINGS->endGroup();
}


void QG_DimOptions::setAction(RS_ActionInterface* a, bool update) {
    action = dynamic_cast<RS_ActionDimension*>(a);
    if (action!=NULL) {
        QString st;
        QString stol1;
        QString stol2;
        bool diam;
        if (update) {
            st = action->getLabel();
            stol1 = action->getTol1();
            stol2 = action->getTol2();
            diam = action->getDiameter();
        } else {
            //st = "";
            RS_SETTINGS->beginGroup("/Draw");
            st = RS_SETTINGS->readEntry("/DimLabel", "");
            stol1 = RS_SETTINGS->readEntry("/DimTol1", "");
            stol2 = RS_SETTINGS->readEntry("/DimTol2", "");
            diam = (bool)RS_SETTINGS->readNumEntry("/DimDiameter", 0);
            RS_SETTINGS->endGroup();
        }
        leLabel->setText(st);
        leTol1->setText(stol1);
        leTol2->setText(stol2);
        bDiameter->setChecked(diam);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_DimensionOptions::setAction: wrong action type");
        action = NULL;
    }
}


void QG_DimOptions::updateLabel() {
    if (action!=NULL) {
        action->setText("");
        action->setLabel(leLabel->text());
        action->setDiameter(bDiameter->isChecked());
        action->setTol1(leTol1->text());
        action->setTol2(leTol2->text());
        
        action->setText(action->getText());
  }
}

void QG_DimOptions::insertSign(const QString& c) {
    leLabel->insert(c);
}
