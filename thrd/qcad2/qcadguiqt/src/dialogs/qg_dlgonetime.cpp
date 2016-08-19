/****************************************************************************
**
** Copyright (C) 2006 RibbonSoft. All rights reserved.
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

#include "qg_dlgonetime.h"

/**
 * Constructor
 */
QG_DlgOneTime::QG_DlgOneTime(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);

}


/**
 * Destructor
 */
QG_DlgOneTime::~QG_DlgOneTime() {
}


void QG_DlgOneTime::setText(const RS_String& text) {
    //lText->resize(lText->width(), text.count(RS_String("\n"))*12);
    lText->setText(text);
}

bool QG_DlgOneTime::getDontShowAgain() {
    return cbDontShowAgain->isChecked();
}

