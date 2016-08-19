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

#include "qg_layerdialog.h"

#include <QMessageBox>

/**
 * Constructor
 */
QG_LayerDialog::QG_LayerDialog(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);
    
    layer = NULL;
    layerList = NULL;
    layerName = "";
    editLayer = false;

    connect(bOk, SIGNAL(clicked()),
            this, SLOT(validate()));
}


/**
 * Destructor
 */
QG_LayerDialog::~QG_LayerDialog() {
}

void QG_LayerDialog::setLayer(RS_Layer* l) {
    layer = l;    
    layerName = layer->getName();
    leName->setText(layerName);
    leName->selectAll();
    wPen->setPen(layer->getPen(), false, false, tr("Default Pen"));

    if (layer->getName()=="0") {
        leName->setEnabled(false);
    }
}

void QG_LayerDialog::updateLayer() {
    layer->setName(leName->text().toLatin1());
    layer->setPen(wPen->getPen());
}

void QG_LayerDialog::validate() {
    if (layerList != NULL && 
        (editLayer == false || layerName != leName->text())) {
        RS_Layer* l = layerList->find(leName->text());
        if (l != NULL) {
            QMessageBox::information(parentWidget(),
                                     QMessageBox::tr("Layer Properties"),
                                     QMessageBox::tr("Layer with a name \"%1\" "
                                                     "already exists. Please specify "
                                                     "a different name.")
                                     .arg(leName->text()),
                                     QMessageBox::Ok);
            leName->setFocus();
            leName->selectAll();
        }
        else {
            accept();
        }
    }
    else {    
        accept();
    }
}

void QG_LayerDialog::setLayerList( RS_LayerList * ll ){
    layerList = ll;
}

void QG_LayerDialog::setEditLayer( bool el ){
    editLayer = el;
}
