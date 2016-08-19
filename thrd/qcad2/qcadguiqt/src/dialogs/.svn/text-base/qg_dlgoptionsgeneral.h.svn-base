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

#ifndef QG_DLGOPTIONSGENERAL_H
#define QG_DLGOPTIONSGENERAL_H

#include <QDialog>

#include "ui_qg_dlgoptionsgeneral.h"
    
/**
 *
 */
class QG_DlgOptionsGeneral: public QDialog, private Ui::QG_DlgOptionsGeneral {
    Q_OBJECT

public:
    QG_DlgOptionsGeneral(QWidget* parent=0);
    virtual ~QG_DlgOptionsGeneral();

public slots:
    void setRestartNeeded();
    void chooseColor(QLabel& l);
    void chooseBackgroundColor();
    void chooseGridColor();
    void chooseMetaGridColor();
    void chooseSelectedColor();
    void chooseHighlightedColor();
    void choosePreviewColor();
    void ok();
    
protected:
    void setColorPreview(QLabel& l, const QString& col);
    QString getColorPreview(const QLabel& l);

private:
    bool restartNeeded;
};

#endif

