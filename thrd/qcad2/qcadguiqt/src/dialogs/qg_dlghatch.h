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

#ifndef QG_DLGHATCH_H
#define QG_DLGHATCH_H

#include <QDialog>

#include "ui_qg_dlghatch.h"

class RS_Hatch;
class RS_Pattern;
class RS_EntityContainer;

/**
 *
 */
class CAD_EXPORT QG_DlgHatch: public QDialog, private Ui::QG_DlgHatch {
    Q_OBJECT

public:
    QG_DlgHatch(QWidget* parent=0);
    virtual ~QG_DlgHatch();

protected:
    //virtual bool event(QEvent* e);
    //virtual void resizeEvent(QResizeEvent* e);
    //virtual void showEvent(QShowEvent * e);
    
public:
    void setHatch(RS_Hatch& h, bool isNew);
    void setPattern(const QString& p);

public slots:
    //void showEvent(QShowEvent* e);
    void updateHatch();
    void updatePreview();
    void updatePreview(RS_Pattern*);

private:
    RS_EntityContainer* preview;
    bool isNew;
    RS_Pattern* pattern;
    RS_Hatch* hatch;
};

#endif

