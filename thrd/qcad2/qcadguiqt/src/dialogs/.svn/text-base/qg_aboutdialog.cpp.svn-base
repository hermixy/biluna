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

#include "qg_aboutdialog.h"
#include <QLabel>

/**
 * Constructor
 */
QG_AboutDialog::QG_AboutDialog(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
#ifdef QC_ABOUT_NO_ICON
    lIcon->hide();
    lText->setFrameShape(QFrame::NoFrame);
    lText->setWordWrap(true);
    hboxLayout1->addItem( new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum) );
    hboxLayout->addSpacing(20);
#else
    setIcon(QPixmap(":/qcad/images/qcad64.png"));
#endif
}


/**
 * Destructor
 */
QG_AboutDialog::~QG_AboutDialog() {}



/**
 * Sets the text of the about dialog
 */
void QG_AboutDialog::setText(const QString& s) {
    lText->setText(s);
}



/**
 * Sets the icon of the about dialog
 */
void QG_AboutDialog::setIcon(const QPixmap& i) {
    lIcon->setPixmap(i);
}
