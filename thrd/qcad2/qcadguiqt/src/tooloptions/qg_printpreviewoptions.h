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

#ifndef QG_PRINTPREVIEWOPTIONS_H
#define QG_PRINTPREVIEWOPTIONS_H

#include "qg_optionsbase.h"

#include "ui_qg_printpreviewoptions.h"

class RS_ActionFilePrintPreview;

/**
 *
 */
class CAD_EXPORT QG_PrintPreviewOptions: public QG_OptionsBase, 
                              private Ui::QG_PrintPreviewOptions {
    Q_OBJECT

public:
    QG_PrintPreviewOptions(QWidget* parent=0);
    virtual ~QG_PrintPreviewOptions();
    
    void setAction(RS_ActionInterface* a, bool update);

public slots:
    void updateScale();
    void center();
    void setBlackWhite(bool on);
    void fit();
    void scale(const QString& s);

private:
    RS_ActionFilePrintPreview* action;
    QStringList imperialScales;
    QStringList metricScales;
    bool updateDisabled;
};

#endif

