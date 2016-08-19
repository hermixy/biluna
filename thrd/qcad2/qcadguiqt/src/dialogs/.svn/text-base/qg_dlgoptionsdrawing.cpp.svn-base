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

#include "qg_dlgoptionsdrawing.h"

#include <QMessageBox>

#include "rs.h"
#include "rs_filterdxf.h"
#include "rs_graphic.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_units.h"

/**
 * Constructor
 */
QG_DlgOptionsDrawing::QG_DlgOptionsDrawing(QWidget* parent)
        : QDialog(parent) {
    setupUi(this);

    cbDimTextHeight->addItem("1");
    cbDimTextHeight->addItem("2");
    cbDimTextHeight->addItem("5");

    cbDimExe->addItem("1");
    cbDimExo->addItem("1");
    cbDimGap->addItem("1");
    cbDimAsz->addItem("1");
    
    cbSplineSegs->addItem("2");
    cbSplineSegs->addItem("4");
    cbSplineSegs->addItem("8");
    cbSplineSegs->addItem("16");
    cbSplineSegs->addItem("32");
    cbSplineSegs->addItem("64");

    cbXSpacing->addItem("0.01");
    cbXSpacing->addItem("0.1");
    cbXSpacing->addItem("1");
    cbXSpacing->addItem("10");
    
    cbYSpacing->addItem("0.01");
    cbYSpacing->addItem("0.1");
    cbYSpacing->addItem("1");
    cbYSpacing->addItem("10");
    
    connect(okButton, SIGNAL(clicked()),
            this, SLOT(validate()));
    connect(cancelButton, SIGNAL(clicked()),
        this, SLOT(reject()));
    connect(cbLengthFormat, SIGNAL(activated(int)),
        this, SLOT(updateLengthPrecision()));
    connect(cbAngleFormat, SIGNAL(activated(int)),
        this, SLOT(updateAnglePrecision()));
    connect(cbUnit, SIGNAL(activated(int)),
        this, SLOT(updatePreview()));
    connect(cbAngleFormat, SIGNAL(activated(int)),
        this, SLOT(updatePreview()));
    connect(cbLengthFormat, SIGNAL(activated(const QString&)),
        this, SLOT(updatePreview()));
    connect(cbAnglePrecision, SIGNAL(activated(int)),
        this, SLOT(updatePreview()));
    connect(cbLengthPrecision, SIGNAL(activated(int)),
        this, SLOT(updatePreview()));
    connect(cbPaperFormat, SIGNAL(activated(int)),
        this, SLOT(updatePaperSize()));
    connect(cbUnit, SIGNAL(activated(int)),
        this, SLOT(updateUnitLabels()));

    connect(cbDimTextHeight, SIGNAL(editTextChanged(const QString&)),
        this, SLOT(keepDimProportions(const QString&)));
    
    graphic = NULL;

    // precision list:
    QString s;
    QString format;
    for (int i=0; i<=8; i++) {
        format.sprintf("%%.0%df", i);
        s.sprintf((const char*)format.toLatin1(), 0.0);
        listPrec1 << s;
    }

    // Main drawing unit:
    for (int i=RS2::None; i<RS2::LastUnit; i++) {
        cbUnit->addItem(RS_Units::unitToString((RS2::Unit)i));
    }

    // init units combobox:
    QStringList unitList;
    unitList << tr("Scientific")
    << tr("Decimal")
    << tr("Engineering")
    << tr("Architectural")
    << tr("Fractional");
    cbLengthFormat->addItems(unitList);

    // init angle units combobox:
    QStringList aunitList;
    aunitList << tr("Decimal Degrees")
    << tr("Deg/min/sec")
    << tr("Gradians")
    << tr("Radians")
    << tr("Surveyor's units");
    cbAngleFormat->addItems(aunitList);

    // Paper format:
    for (int i=RS2::Custom; i<=RS2::NPageSize; i++) {
        cbPaperFormat->addItem(RS_Units::paperFormatToString((RS2::PaperFormat)i));
    }
}


/**
 * Destructor
 */
QG_DlgOptionsDrawing::~QG_DlgOptionsDrawing() {}


/**
 * Sets the graphic and updates the GUI to match the drawing.
 */
void QG_DlgOptionsDrawing::setGraphic(RS_Graphic* g) {
    graphic = g;

    if (graphic==NULL) {
        return;
    }

    // main drawing unit:
    int insunits = graphic->getVariableInt("$INSUNITS", 0);
    cbUnit->setCurrentIndex(cbUnit->findText(RS_Units::unitToString(RS_FilterDxf::numberToUnit(insunits))));

    // units / length format:
    int lunits = graphic->getVariableInt("$LUNITS", 2);
    cbLengthFormat->setCurrentIndex(lunits-1);

    // units length precision:
    int luprec = graphic->getVariableInt("$LUPREC", 4);
    updateLengthPrecision();
    cbLengthPrecision->setCurrentIndex(luprec);

    // units / angle format:
    int aunits = graphic->getVariableInt("$AUNITS", 0);
    cbAngleFormat->setCurrentIndex(aunits);

    // units angle precision:
    int auprec = graphic->getVariableInt("$AUPREC", 2);
    updateAnglePrecision();
    cbAnglePrecision->setCurrentIndex(auprec);

    // paper format:
    bool landscape;
    RS2::PaperFormat format = graphic->getPaperFormat(&landscape);
    RS_DEBUG->print("QG_DlgOptionsDrawing::setGraphic: paper format is: %d", (int)format);
    cbPaperFormat->setCurrentIndex((int)format);

    // paper orientation:
    if (landscape) {
        rbLandscape->setChecked(true);
    } else {
        rbPortrait->setChecked(true);
    }

    // Grid:
    cbGridOn->setChecked(graphic->isGridOn());

    RS_Vector spacing = graphic->getVariableVector("$GRIDUNIT",
                        RS_Vector(0.0,0.0));
    cbXSpacing->setEditText(QString("%1").arg(spacing.x));
    cbYSpacing->setEditText(QString("%1").arg(spacing.y));

    if (cbXSpacing->currentText()=="0") {
        cbXSpacing->setEditText(tr("auto"));
    }
    if (cbYSpacing->currentText()=="0") {
        cbYSpacing->setEditText(tr("auto"));
    }

    // dimension text height:
    RS2::Unit unit = (RS2::Unit)cbUnit->currentIndex();

    double dimtxt = graphic->getVariableDouble("$DIMTXT",
                    RS_Units::convert(2.5, RS2::Millimeter, unit));
    cbDimTextHeight->setEditText(QString("%1").arg(dimtxt));

    // dimension extension line extension:
    double dimexe = graphic->getVariableDouble("$DIMEXE",
                    RS_Units::convert(1.25, RS2::Millimeter, unit));
    cbDimExe->setEditText(QString("%1").arg(dimexe));

    // dimension extension line offset:
    double dimexo = graphic->getVariableDouble("$DIMEXO",
                    RS_Units::convert(0.625, RS2::Millimeter, unit));
    cbDimExo->setEditText(QString("%1").arg(dimexo));

    // dimension line gap:
    double dimgap = graphic->getVariableDouble("$DIMGAP",
                    RS_Units::convert(0.625, RS2::Millimeter, unit));
    cbDimGap->setEditText(QString("%1").arg(dimgap));

    // dimension arrow size:
    double dimasz = graphic->getVariableDouble("$DIMASZ",
                    RS_Units::convert(2.5, RS2::Millimeter, unit));
    cbDimAsz->setEditText(QString("%1").arg(dimasz));
    
    // dimension arrow type:
    QString dimblk = graphic->getVariableString("$DIMBLK", "");
    if (dimblk.toUpper()=="ARCHTICK") {
        rbDimensionArrowArchTick->setChecked(true);
    }
    else {
        rbDimensionArrowArrow->setChecked(true);
    }
    
    // dimension: show leading / trailing zeroes
    int dimzin = graphic->getVariableInt("$DIMZIN", 8);
    RS_DEBUG->print("QG_DlgOptionsDrawing::setGraphic: DIMZIN: %d", dimzin);
    cbShowTrailingZerosLength->setChecked(!((dimzin&8)==8));
    //cbShowLeadingZerosLength->setChecked(!(dimzin&1==1));
    
    int dimazin = graphic->getVariableInt("$DIMAZIN", 8);
    RS_DEBUG->print("QG_DlgOptionsDrawing::setGraphic: DIMAZIN: %d", dimazin);
    cbShowTrailingZerosAngle->setChecked(!((dimazin&8)==8));
    //cbShowLeadingZerosAngle->setChecked(!(dimazin&1==1));
    
    // spline line segments per patch:
    int splinesegs = graphic->getVariableInt("$SPLINESEGS", 8);
    cbSplineSegs->setEditText(QString("%1").arg(splinesegs));
    
    RS_DEBUG->print("QG_DlgOptionsDrawing::setGraphic: splinesegs is: %d",
                    splinesegs);
    
    // encoding:
    /*
    QString encoding = graphic->getVariableString("$DWGCODEPAGE",
                                                  "ANSI_1252");
    encoding=RS_System::getEncoding(encoding);
    cbEncoding->setCurrentIndex(cbEncoding->findText(encoding));
    */

    updatePaperSize();
    updatePreview();
    updateUnitLabels();
}


/**
 * Called when OK is clicked.
 */
void QG_DlgOptionsDrawing::validate() {
    RS2::LinearFormat f = (RS2::LinearFormat)cbLengthFormat->currentIndex();
    if (f==RS2::Engineering || f==RS2::Architectural) {
        if (RS_Units::stringToUnit(cbUnit->currentText())!=RS2::Inch) {
            QMessageBox::warning( this, tr("Options"),
                                  tr("For the length formats 'Engineering' "
                                     "and 'Architectural', the "
                                     "unit must be set to Inch."),
                                  QMessageBox::Ok,
                                  Qt::NoButton);
            return;
        }
    }

    if (graphic!=NULL) {
        // units:
        graphic->setUnit((RS2::Unit)cbUnit->currentIndex());

        graphic->addVariable("$LUNITS", cbLengthFormat->currentIndex()+1, 70);
        graphic->addVariable("$DIMLUNIT", cbLengthFormat->currentIndex()+1, 70);
        graphic->addVariable("$LUPREC", cbLengthPrecision->currentIndex(), 70);

        graphic->addVariable("$AUNITS", cbAngleFormat->currentIndex(), 70);
        graphic->addVariable("$DIMAUNIT", cbAngleFormat->currentIndex(), 70);
        graphic->addVariable("$AUPREC", cbAnglePrecision->currentIndex(), 70);
        graphic->addVariable("$DIMADEC", cbAnglePrecision->currentIndex(), 70);
        
        graphic->addVariable("$DIMZIN", 
            ((int)(!cbShowTrailingZerosLength->isChecked()))<<3, 70);
        graphic->addVariable("$DIMAZIN", 
            ((int)(!cbShowTrailingZerosAngle->isChecked()))<<3, 70);

        // paper:
        graphic->setPaperFormat(
            (RS2::PaperFormat)cbPaperFormat->currentIndex(),
            rbLandscape->isChecked());
        // custom paper size:
        if ((RS2::PaperFormat)cbPaperFormat->currentIndex()==RS2::Custom) {
            graphic->setPaperSize(
                RS_Vector(RS_Math::eval(lePaperWidth->text()),
                          RS_Math::eval(lePaperHeight->text())));
        }

        // grid:
        //graphic->addVariable("$GRIDMODE", (int)cbGridOn->isChecked() , 70);
        graphic->setGridOn(cbGridOn->isChecked());
        RS_Vector spacing(0.0,0.0,0.0);
        if (cbXSpacing->currentText()==tr("auto")) {
            spacing.x = 0.0;
        } else {
            spacing.x = cbXSpacing->currentText().toDouble();
        }
        if (cbYSpacing->currentText()==tr("auto")) {
            spacing.y = 0.0;
        } else {
            spacing.y = cbYSpacing->currentText().toDouble();
        }
        graphic->addVariable("$GRIDUNIT", spacing, 10);

        // dim:
        graphic->addVariable("$DIMTXT",
                             RS_Math::eval(cbDimTextHeight->currentText()), 40);
        graphic->addVariable("$DIMEXE",
                             RS_Math::eval(cbDimExe->currentText()), 40);
        graphic->addVariable("$DIMEXO",
                             RS_Math::eval(cbDimExo->currentText()), 40);
        graphic->addVariable("$DIMGAP",
                             RS_Math::eval(cbDimGap->currentText()), 40);
        graphic->addVariable("$DIMASZ",
                             RS_Math::eval(cbDimAsz->currentText()), 40);
        if (rbDimensionArrowArchTick->isChecked()) {
            graphic->addVariable("$DIMBLK", "ArchTick", 1);
        }
        else {
            graphic->addVariable("$DIMBLK", "", 1);
        }

        // splines:
        graphic->addVariable("$SPLINESEGS",
                             (int)RS_Math::eval(cbSplineSegs->currentText()), 70);
        
        RS_DEBUG->print("QG_DlgOptionsDrawing::validate: splinesegs is: %s",
                        (const char*)cbSplineSegs->currentText().toLatin1());
        
        // update all dimension and spline entities in the graphic to match the new settings:
        graphic->updateDimensions(false);
        graphic->updateSplines();
        
        graphic->setModified(true);
    }
    accept();
}


/**
 * Updates the length precision combobox
 */
void QG_DlgOptionsDrawing::updateLengthPrecision() {
    int index = cbLengthPrecision->currentIndex();
    cbLengthPrecision->clear();

    switch (cbLengthFormat->currentIndex()) {
        // scientific
    case 0:
        cbLengthPrecision->addItem("0E+01");
        cbLengthPrecision->addItem("0.0E+01");
        cbLengthPrecision->addItem("0.00E+01");
        cbLengthPrecision->addItem("0.000E+01");
        cbLengthPrecision->addItem("0.0000E+01");
        cbLengthPrecision->addItem("0.00000E+01");
        cbLengthPrecision->addItem("0.000000E+01");
        cbLengthPrecision->addItem("0.0000000E+01");
        cbLengthPrecision->addItem("0.00000000E+01");
        break;

        // decimal
        //   (0, 0.1, 0.01, ...)
    case 1:
        cbLengthPrecision->addItems(listPrec1);
        break;

        // architectural:
    case 3:
        cbLengthPrecision->addItem("0'-0\"");
        cbLengthPrecision->addItem("0'-0 1/2\"");
        cbLengthPrecision->addItem("0'-0 1/4\"");
        cbLengthPrecision->addItem("0'-0 1/8\"");
        cbLengthPrecision->addItem("0'-0 1/16\"");
        cbLengthPrecision->addItem("0'-0 1/32\"");
        cbLengthPrecision->addItem("0'-0 1/64\"");
        cbLengthPrecision->addItem("0'-0 1/128\"");
        break;

        // engineering:
    case 2:
        cbLengthPrecision->addItem("0'-0\"");
        cbLengthPrecision->addItem("0'-0.0\"");
        cbLengthPrecision->addItem("0'-0.00\"");
        cbLengthPrecision->addItem("0'-0.000\"");
        cbLengthPrecision->addItem("0'-0.0000\"");
        cbLengthPrecision->addItem("0'-0.00000\"");
        cbLengthPrecision->addItem("0'-0.000000\"");
        cbLengthPrecision->addItem("0'-0.0000000\"");
        cbLengthPrecision->addItem("0'-0.00000000\"");
        break;

        // fractional
    case 4:
        cbLengthPrecision->addItem("0");
        cbLengthPrecision->addItem("0 1/2");
        cbLengthPrecision->addItem("0 1/4");
        cbLengthPrecision->addItem("0 1/8");
        cbLengthPrecision->addItem("0 1/16");
        cbLengthPrecision->addItem("0 1/32");
        cbLengthPrecision->addItem("0 1/64");
        cbLengthPrecision->addItem("0 1/128");
        break;

    default:
        RS_DEBUG->print(RS_Debug::D_ERROR, 
            "QG_DlgOptionsDrawing::updateLengthPrecision: error");
        break;
    }

    cbLengthPrecision->setCurrentIndex(index);
}



/**
 * Updates the angle precision combobox
 */
void QG_DlgOptionsDrawing::updateAnglePrecision() {
    int index = cbAnglePrecision->currentIndex();
    cbAnglePrecision->clear();

    switch (cbAngleFormat->currentIndex()) {
        // decimal degrees:
    case 0:
        cbAnglePrecision->addItems(listPrec1);
        break;

        // deg/min/sec:
    case 1:
        cbAnglePrecision->addItem(QString("0%1").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'00\"").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'00.0\"").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'00.00\"").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'00.000\"").arg(QChar(0xB0)));
        cbAnglePrecision->addItem(QString("0%100'00.0000\"").arg(QChar(0xB0)));
        break;

        // gradians:
    case 2:
        cbAnglePrecision->addItem("0g");
        cbAnglePrecision->addItem("0.0g");
        cbAnglePrecision->addItem("0.00g");
        cbAnglePrecision->addItem("0.000g");
        cbAnglePrecision->addItem("0.0000g");
        cbAnglePrecision->addItem("0.00000g");
        cbAnglePrecision->addItem("0.000000g");
        cbAnglePrecision->addItem("0.0000000g");
        cbAnglePrecision->addItem("0.00000000g");
        break;

        // radians:
    case 3:
        cbAnglePrecision->addItem("0r");
        cbAnglePrecision->addItem("0.0r");
        cbAnglePrecision->addItem("0.00r");
        cbAnglePrecision->addItem("0.000r");
        cbAnglePrecision->addItem("0.0000r");
        cbAnglePrecision->addItem("0.00000r");
        cbAnglePrecision->addItem("0.000000r");
        cbAnglePrecision->addItem("0.0000000r");
        cbAnglePrecision->addItem("0.00000000r");
        break;

        // surveyor's units:
    case 4:
        cbAnglePrecision->addItem("N 0d E");
        cbAnglePrecision->addItem("N 0d00' E");
        cbAnglePrecision->addItem("N 0d00'00\" E");
        cbAnglePrecision->addItem("N 0d00'00.0\" E");
        cbAnglePrecision->addItem("N 0d00'00.00\" E");
        cbAnglePrecision->addItem("N 0d00'00.000\" E");
        cbAnglePrecision->addItem("N 0d00'00.0000\" E");
        break;

    default:
        break;
    }

    cbAnglePrecision->setCurrentIndex(index);
}

/**
 * Updates the preview of unit display.
 */
void QG_DlgOptionsDrawing::updatePreview() {
    QString prev;
    prev = RS_Units::formatLinear(14.43112351,
                                  (RS2::Unit)cbUnit->currentIndex(),
                                  (RS2::LinearFormat)(cbLengthFormat->currentIndex()),
                                  cbLengthPrecision->currentIndex());
    lLinear->setText(prev);

    prev = RS_Units::formatAngle(0.5327714,
                                 (RS2::AngleFormat)cbAngleFormat->currentIndex(),
                                 cbAnglePrecision->currentIndex());
    lAngular->setText(prev);
}



/**
 * Updates the paper size. Called for initialisation as well as when the 
 * paper format changes.
 */
void  QG_DlgOptionsDrawing::updatePaperSize() {
    RS2::PaperFormat format = (RS2::PaperFormat)cbPaperFormat->currentIndex();

    if (format==RS2::Custom) {
        RS_Vector s = graphic->getPaperSize();
        //RS_Vector plimmin = graphic->getVariableVector("$PLIMMIN", RS_Vector(0,0));
        //RS_Vector plimmax = graphic->getVariableVector("$PLIMMAX", RS_Vector(100,100));
        lePaperWidth->setText(QString("%1").arg(s.x));
        lePaperHeight->setText(QString("%1").arg(s.y));
    }
    else {
        RS_Vector s = RS_Units::paperFormatToSize(format);
        lePaperWidth->setText(QString("%1").arg(s.x));
        lePaperHeight->setText(QString("%1").arg(s.y));
    }

    if (cbPaperFormat->currentIndex()==0) {
        lePaperWidth->setEnabled(true);
        lePaperHeight->setEnabled(true);
        rbLandscape->setEnabled(false);
        rbPortrait->setEnabled(false);
    } else {
        lePaperWidth->setEnabled(false);
        lePaperHeight->setEnabled(false);
        rbLandscape->setEnabled(true);
        rbPortrait->setEnabled(true);
    }
}



/**
 * Updates all unit labels that depend on the global unit.
 */
void QG_DlgOptionsDrawing::updateUnitLabels() {
    RS2::Unit u = (RS2::Unit)cbUnit->currentIndex();
    QString sign = RS_Units::unitToSign(u);
    lDimUnit1->setText(sign);
    lDimUnit2->setText(sign);
    lDimUnit3->setText(sign);
    lDimUnit4->setText(sign);
    lDimUnit5->setText(sign);
}


void QG_DlgOptionsDrawing::keepDimProportions(const QString&) {
    if (cbProportions->checkState()==Qt::Checked) {
        double h = cbDimTextHeight->currentText().toDouble();
        cbDimExe->setEditText(QString("%1").arg(h/2));
        cbDimExo->setEditText(QString("%1").arg(h/4));
        cbDimGap->setEditText(QString("%1").arg(h/4));
        cbDimAsz->setEditText(QString("%1").arg(h));
    }
}
