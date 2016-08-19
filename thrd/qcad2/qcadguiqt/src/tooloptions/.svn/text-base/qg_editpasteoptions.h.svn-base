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

#ifndef QG_EDITPASTEOPTIONS_H
#define QG_EDITPASTEOPTIONS_H

#include "qg_optionsbase.h"

#include "ui_qg_editpasteoptions.h"

class RS_ActionEditPaste;

/**
 *
 */
class QG_EditPasteOptions: public QG_OptionsBase, private Ui::QG_EditPasteOptions {
    Q_OBJECT

public:
    QG_EditPasteOptions(QWidget* parent=0);
    virtual ~QG_EditPasteOptions();
    
    void setAction(RS_ActionInterface* a, bool update);

public slots:
    void updateData();

private:
    RS_ActionEditPaste* action;
};

#endif

