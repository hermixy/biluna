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

#ifndef QG_DLGONETIME_H
#define QG_DLGONETIME_H

#include <QDialog>

#include "ui_qg_dlgonetime.h"

#include "rs_modification.h"

/**
 * Warning dialog that offers the user a check box to never show 
 * the warning again.
 */
class QG_DlgOneTime: public QDialog, private Ui::QG_DlgOneTime {
    Q_OBJECT

public:
    QG_DlgOneTime(QWidget* parent=0);
    virtual ~QG_DlgOneTime();

    bool getDontShowAgain();

public slots:
    void setText(const RS_String& text);

};

#endif

