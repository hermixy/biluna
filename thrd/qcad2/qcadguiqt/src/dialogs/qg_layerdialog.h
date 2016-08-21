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

#ifndef QG_LAYERDIALOG_H
#define QG_LAYERDIALOG_H

#include <QDialog>

#include "ui_qg_layerdialog.h"
    
#include "qg_widgetpen.h"

/**
 *
 */
class CAD_EXPORT QG_LayerDialog: public QDialog, private Ui::QG_LayerDialog {
    Q_OBJECT

public:
    QG_LayerDialog(QWidget* parent=0);
    virtual ~QG_LayerDialog();

public slots:
    void setLayer(RS_Layer* l);
    void updateLayer();
    void validate();
    void setLayerList(RS_LayerList* ll);
    void setEditLayer(bool el);

private:
    RS_Layer* layer;
    RS_LayerList* layerList;
    QString layerName;
    bool editLayer;
};

#endif

