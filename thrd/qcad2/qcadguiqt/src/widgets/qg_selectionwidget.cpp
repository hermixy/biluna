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

#include "qg_selectionwidget.h"

#include "rs_settings.h"

/**
 * Constructor
 */
QG_SelectionWidget::QG_SelectionWidget(QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    
    lEntities->setText("0");
    
    int fsize;
#ifdef __APPLE__
    fsize = 9;
#else
    fsize = 9;
#endif
    
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();
    
    QFont font = lLabel->font();
    font.setPixelSize(fsize);
    
    lEntities->setFont(font);
    lLabel->setFont(font);
}


/**
 * Destructor
 */
QG_SelectionWidget::~QG_SelectionWidget() {}


void QG_SelectionWidget::setNumber(int n) {
    QString str;
    str.setNum(n);
    lEntities->setText(str);
}
