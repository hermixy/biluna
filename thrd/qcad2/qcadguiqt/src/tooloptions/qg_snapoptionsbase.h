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

#ifndef QG_SNAPOPTIONSBASE_H
#define QG_SNAPOPTIONSBASE_H

#include <QWidget>
#include "rs_debug.h"

class RS_ActionInterface;
class RS_Snapper;

/**
 * Base class for action option widgets.
 */
class QG_SnapOptionsBase: public QWidget {
    Q_OBJECT
    
public:
    QG_SnapOptionsBase(QWidget* parent=0) : QWidget(parent) {
        snapper = NULL;
    }
    virtual ~QG_SnapOptionsBase() {
        RS_DEBUG->print("QG_SnapOptionsBase::~QG_SnapOptionsBase");
    }

public:
    virtual void setSnapper(RS_Snapper* s) {
        snapper = s;
    }

protected:
    RS_Snapper* snapper;
};

#endif

