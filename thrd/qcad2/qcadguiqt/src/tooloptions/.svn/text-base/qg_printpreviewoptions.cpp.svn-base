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

#include "qg_printpreviewoptions.h"

#include "rs_actionfileprintpreview.h"
#include "rs_settings.h"

/**
 * Constructor
 */
QG_PrintPreviewOptions::QG_PrintPreviewOptions(QWidget* parent)
        : QG_OptionsBase(parent) {
    setupUi(this);

    connect(bCenter, SIGNAL(clicked()),
        this, SLOT(center()));
    connect(bBlackWhite, SIGNAL(toggled(bool)),
        this, SLOT(setBlackWhite(bool)));
    connect(cbScale, SIGNAL(textChanged(const QString&)),
        this, SLOT(scale(const QString&)));
    connect(cbScale, SIGNAL(activated(const QString&)),
        this, SLOT(scale(const QString&)));
    connect(bFit, SIGNAL(clicked()),
        this, SLOT(fit()));

    updateDisabled = false;
    imperialScales
    << "1\" = 1\""
    << "1\" = 2\""
    << "1\" = 4\""
    << "1\" = 8\""
    << "1\" = 16\""
    << "1\" = 32\""
    << "1\" = 64\""
    << "1\" = 128\""
    << "1\" = 256\""
    << "1/16\" = 1'-0\""
    << "1/8\" = 1'-0\""
    << "1/4\" = 1'-0\""
    << "1/2\" = 1'-0\""
    << "3/4\" = 1'-0\""
    << "1\" = 1'-0\"";

    metricScales
    << "1:1" << "1:2" << "1:5" << "1:10"
    << "1:20" << "1:25" << "1:50" << "1:75" << "1:100"
    << "1:125" << "1:150" << "1:175" << "1:200"
    << "1:250" << "1:500" << "1:750" << "1:1000"
    << "1:2500" << "1:5000" << "1:7500" << "1:10000"
    << "1:25000" << "1:50000" << "1:75000" << "1:100000"
    << "2:1" << "5:1" << "10:1"
    << "20:1" << "25:1" << "50:1" << "75:1" << "100:1"
    << "125:1" << "150:1" << "175:1" << "200:1"
    << "250:1" << "500:1" << "750:1" << "1000:1"
    << "2500:1" << "5000:1" << "7500:1" << "10000:1"
    << "25000:1" << "50000:1" << "75000:1" << "100000:1";   
    
}


/**
 * Destructor
 */
QG_PrintPreviewOptions::~QG_PrintPreviewOptions() {
    RS_DEBUG->print("QG_PrintPreviewOptions::~QG_PrintPreviewOptions");
    
}


void QG_PrintPreviewOptions::setAction(RS_ActionInterface* a, bool /*update*/) {
    action = dynamic_cast<RS_ActionFilePrintPreview*>(a);
    if (action!=NULL) {
        updateDisabled = true;
        RS2::Unit u = action->getUnit();
        if (u==RS2::Inch || u==RS2::Foot) {
            cbScale->addItems(imperialScales);
        } else {
            cbScale->addItems(metricScales);
        }
        
        updateScale();
        
        updateDisabled = false;

    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_PrintPreviewOptions::setAction: wrong action type");
        action = NULL;
    }
}

void QG_PrintPreviewOptions::updateScale() {
    if (action!=NULL) {
        QString s;
        s.setNum(action->getScale());
        cbScale->setEditText(s);
    }
}

void QG_PrintPreviewOptions::center() {
    if (action!=NULL) {
        action->center();
    }
}

void QG_PrintPreviewOptions::setBlackWhite(bool on) {
    if (action!=NULL) {
        action->setBlackWhite(on);
    }
}

void QG_PrintPreviewOptions::fit() {
    if (action!=NULL) {
        action->fit();
        updateScale();
    }
}

void QG_PrintPreviewOptions::scale(const QString& s) {
    if (updateDisabled || action==NULL) {
        return;
    }
    
    if (s.contains(':')) {
        bool ok1 = false;
        bool ok2 = false;
        int i = s.indexOf(':');
        double n = s.left(i).toDouble(&ok1);
        double d = s.mid(i+1).toDouble(&ok2);
        if (ok1 && ok2 && d>1.0e-6 && n>0.0) {
            action->setScale(n/d);
        }
    } else if (s.contains('=')) {
        if (s.endsWith(" = 1'-0\"")) {
            // 1/16" = 1'-0":
            int i = s.indexOf('"');
            double d = RS_Math::eval(s.mid(0, i));
            if (d>1.0e-6) {
                action->setScale(d/12.0);
            }
        }
        else {
            // 1" = 2"
            bool ok = false;
            int i = s.indexOf('=');
            double d = s.mid(i+2, s.length()-i-3).toDouble(&ok);
            if (ok && d>1.0e-6) {
                action->setScale(1.0/d);
            }
        }
    } else {
        bool ok = false;
        double f = RS_Math::eval(s, &ok);
        if (ok) {
            action->setScale(f);
        }
    }
    //updateScale();
}

