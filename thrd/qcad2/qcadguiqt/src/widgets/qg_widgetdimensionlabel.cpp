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

#include "qg_widgetdimensionlabel.h"

/**
 * Constructor
 */
QG_WidgetDimensionLabel::QG_WidgetDimensionLabel(QWidget* parent) : QWidget(parent) {
    setupUi(this);
        
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(248)).arg(tr("Diameter")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(176)).arg(tr("Degree")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(177)).arg(tr("Plus / Minus")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(960)).arg(tr("Pi")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(215)).arg(tr("Times")));
    cbSymbol->addItem(QString("%1 (%2)").arg(QChar(247)).arg(tr("Division")));

    bDiameter->setText(QString("%1").arg(QChar(248)));
    
    connect(cbSymbol, SIGNAL(activated(const QString&)),
            this, SLOT(insertSymbol(const QString&)));
}


/**
 * Destructor
 */
QG_WidgetDimensionLabel::~QG_WidgetDimensionLabel() {
}


void QG_WidgetDimensionLabel::setLabel(const QString& l) {
    int i0, i1a, i1b, i2;
    QString label, tol1, tol2;
    bool hasDiameter = false;
    
    label = l;
    
    if (label.at(0)==QChar(0x2205) || label.at(0)==QChar(0xF8)) {
        hasDiameter = true;
        bDiameter->setChecked(true);
    }
    
    i0 = l.indexOf("\\S");
    if (i0>=0) {
        i1a = l.indexOf("^ ", i0);
        i1b = i1a+1;
        if (i1a<0) {
            i1a = i1b = l.indexOf('^', i0);
        }
        if (i1a>=0) {
            i2 = l.indexOf(';', i1b);
            label = l.mid(0, i0);
            tol1 = l.mid(i0+2, i1a-i0-2);
            tol2 = l.mid(i1b+1, i2-i1b-1);
        }
    }
    
    leLabel->setText(label.mid(hasDiameter));
    leTol1->setText(tol1);
    leTol2->setText(tol2);
}

QString QG_WidgetDimensionLabel::getLabel() {
    QString l = leLabel->text();
    
    // diameter:
    if (bDiameter->isChecked()) {
        if (l.isEmpty()) {
            l = QString("%1<>").arg(QChar(0x2205));
        }
        else {
            l = QChar(0x2205) + l;
        }
    }
    
    if (leTol1->text().isEmpty() && leTol2->text().isEmpty()) {
        return l;
    }
    else {
        return l + "\\S" + leTol1->text() + 
            "^ " + leTol2->text() + ";";
    }
}

void QG_WidgetDimensionLabel::insertSymbol(const QString& s) {
    leLabel->insert(s.left(1));
}
