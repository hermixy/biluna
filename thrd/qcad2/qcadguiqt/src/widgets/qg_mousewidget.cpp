/****************************************************************************
** $Id: qg_colorbox.cpp 2510 2005-07-26 20:28:53Z andrew $
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

#include "qg_mousewidget.h"

#include "rs_graphic.h"
#include "rs_settings.h"
#include "rs_vector.h"

/**
 * Constructor
 */
QG_MouseWidget::QG_MouseWidget(QWidget* parent)
        : QWidget(parent) {
    setupUi(this);
    
    lLeftButton->setText("");
    lRightButton->setText("");
    
    int fsize;
#ifdef __APPLE__
    fsize = 9;
#else
    fsize = 9;
#endif
    
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();
    
    QFont font = lLeftButton->font();
    font.setPixelSize(fsize);
    
    lLeftButton->setFont(font);
    lRightButton->setFont(font);
}


/**
 * Destructor
 */
QG_MouseWidget::~QG_MouseWidget() {}


void QG_MouseWidget::setHelp(const QString& left, const QString& right) {
    lLeftButton->setText(left);
    lRightButton->setText(right);
}
