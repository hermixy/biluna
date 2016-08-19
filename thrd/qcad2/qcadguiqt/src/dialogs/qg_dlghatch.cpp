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

#include "qg_dlghatch.h"

#include "rs_debug.h"
#include "rs_hatch.h"
#include "rs_pattern.h"
#include "rs_settings.h"
#include "rs_entitycontainer.h"

#include "qg_patternbox.h"
#include "qg_graphicview.h"

/**
 * Constructor
 */
QG_DlgHatch::QG_DlgHatch(QWidget* parent)
        : QDialog(parent) {
    RS_DEBUG->print("QG_DlgHatch::QG_DlgHatch");
    setupUi(this);
    
    connect(cbSolid, SIGNAL(toggled(bool)),
        cbPattern, SLOT(setDisabled(bool)));
    connect(cbSolid, SIGNAL(toggled(bool)),
        leScale, SLOT(setDisabled(bool)));
    connect(cbSolid, SIGNAL(toggled(bool)),
        lScale, SLOT(setDisabled(bool)));
    connect(cbSolid, SIGNAL(toggled(bool)),
        leAngle, SLOT(setDisabled(bool)));
    connect(cbSolid, SIGNAL(toggled(bool)),
        lAngle, SLOT(setDisabled(bool)));
    connect(cbPattern, SIGNAL(patternChanged(RS_Pattern*)),
        this, SLOT(updatePreview(RS_Pattern*)));
    connect(cbSolid, SIGNAL(toggled(bool)),
        this, SLOT(updatePreview()));
    connect(leAngle, SIGNAL(textChanged(const QString&)),
        this, SLOT(updatePreview()));
    connect(cbEnablePreview, SIGNAL(toggled(bool)),
        this, SLOT(updatePreview()));

    pattern=NULL;
    hatch = NULL;
    isNew = false;

    preview = new RS_EntityContainer();
    gvPreview->setContainer(preview);
    gvPreview->showRulers(false);
    gvPreview->setBorders(15,15,15,15);
    gvPreview->setAlwaysAutoZoom(true);

    cbPattern->init();
    
    RS_DEBUG->print("QG_DlgHatch::QG_DlgHatch: OK");
}


/**
 * Destructor
 */
QG_DlgHatch::~QG_DlgHatch() {
    if (isNew) {
        RS_SETTINGS->beginGroup("/Draw");
        RS_SETTINGS->writeEntry("/HatchSolid", (int)cbSolid->isChecked());
        RS_SETTINGS->writeEntry("/HatchPattern", cbPattern->currentText());
        RS_SETTINGS->writeEntry("/HatchScale", leScale->text());
        RS_SETTINGS->writeEntry("/HatchAngle", leAngle->text());
        RS_SETTINGS->writeEntry("/HatchPreview",
                                (int)cbEnablePreview->isChecked());
        RS_SETTINGS->endGroup();
    }
    delete preview;
}


/*bool QG_DlgHatch::event(QEvent* e) {
    if (e->type()==QEvent::Polish) {
        RS_DEBUG->print("QG_DlgHatch::event: polishing..");
        gvPreview->zoomAuto();
        QDialog::event(e);
        return true;
    }
    return false;
}*/

/*void QG_DlgHatch::showEvent ( QShowEvent * e) {
    QDialog::showEvent(e);
    gvPreview->zoomAuto();
}*/


void QG_DlgHatch::setHatch(RS_Hatch& h, bool isNew) {
    hatch = &h;
    this->isNew = isNew;
    
    RS_SETTINGS->beginGroup("/Draw");
    QString enablePrev = RS_SETTINGS->readEntry("/HatchPreview", "0");
    RS_SETTINGS->endGroup();
    
    cbEnablePreview->setChecked(enablePrev=="1");

    // read defaults from config file:
    if (isNew) {
        RS_SETTINGS->beginGroup("/Draw");
        QString solid = RS_SETTINGS->readEntry("/HatchSolid", "0");
        QString pat = RS_SETTINGS->readEntry("/HatchPattern", "ANSI31");
        QString scale = RS_SETTINGS->readEntry("/HatchScale", "1.0");
        QString angle = RS_SETTINGS->readEntry("/HatchAngle", "0.0");
        RS_SETTINGS->endGroup();

        cbSolid->setChecked(solid=="1");
        setPattern(pat);
        leScale->setText(scale);
        leAngle->setText(angle);
    }
    // initialize dialog based on given hatch:
    else {
        cbSolid->setChecked(hatch->isSolid());
        setPattern(hatch->getPattern());
        QString s;
        s.setNum(hatch->getScale());
        leScale->setText(s);
        s.setNum(RS_Math::rad2deg(hatch->getAngle()));
        leAngle->setText(s);
    }
}

void QG_DlgHatch::updateHatch() {
    if (hatch!=NULL) {
        hatch->setSolid(cbSolid->isChecked());
        hatch->setPattern(cbPattern->currentText());
        hatch->setScale(RS_Math::eval(leScale->text()));
        hatch->setAngle(RS_Math::deg2rad(RS_Math::eval(leAngle->text())));
    }
}

void QG_DlgHatch::setPattern(const QString& p) {
    if (!RS_PATTERNLIST->contains(p)) {
        cbPattern->addItem(p);
    }
    cbPattern->setCurrentIndex(cbPattern->findText(p.toLower()));
    pattern = cbPattern->getPattern();
}

/*void QG_DlgHatch::resizeEvent(QResizeEvent *) {
    updatePreview(NULL);
}*/

void QG_DlgHatch::updatePreview() {
    updatePreview(NULL);
}

void QG_DlgHatch::updatePreview(RS_Pattern* ) {
    if (preview==NULL) {
        return;
    }
    if (hatch==NULL || !cbEnablePreview->isChecked()) {
        preview->clear();
        gvPreview->zoomAuto();
        gvPreview->setDisabled(true);
        return;
    }
    else {
        gvPreview->setDisabled(false);
    }

    QString patName = cbPattern->currentText();
    bool isSolid = cbSolid->isChecked();
    double prevSize;
    //double scale = RS_Math::eval(leScale->text(), 1.0);
    double angle = RS_Math::deg2rad(RS_Math::eval(leAngle->text(), 0.0));
    if (pattern!=NULL) {
        prevSize = pattern->getSize().x*2;
    } else {
        prevSize = 10.0;
    }

    preview->clear();

    RS_Hatch* prevHatch = new RS_Hatch(preview,
                                       RS_HatchData(isSolid, 1.0, angle, patName));
    prevHatch->setPen(hatch->getPen());

    RS_EntityContainer* loop = new RS_EntityContainer(prevHatch);
    loop->setPen(RS_Pen(RS2::FlagInvalid));
    loop->addEntity(new RS_Line(loop,
                                RS_LineData(RS_Vector(0.0,0.0),
                                            RS_Vector(prevSize,0.0))));
    loop->addEntity(new RS_Line(loop,
                                RS_LineData(RS_Vector(prevSize,0.0),
                                            RS_Vector(prevSize,prevSize))));
    loop->addEntity(new RS_Line(loop,
                                RS_LineData(RS_Vector(prevSize,prevSize),
                                            RS_Vector(0.0,prevSize))));
    loop->addEntity(new RS_Line(loop,
                                RS_LineData(RS_Vector(0.0,prevSize),
                                            RS_Vector(0.0,0.0))));
    prevHatch->addEntity(loop);
    preview->addEntity(prevHatch);
    if (!isSolid) {
        prevHatch->update();
    }

    gvPreview->zoomAuto();

}
