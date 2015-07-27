/*****************************************************************
 * $Id: gv_lineattributesdialog.cpp 1380 2011-03-07 20:47:51Z rutger $
 * Created: June 14, 2010 11:00:51 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_lineattributesdialog.h"

GV_LineAttributesDialog::GV_LineAttributesDialog(QWidget *parent) :
                                RB_Dialog(parent) {
    setupUi(this);
}

GV_LineAttributesDialog::~GV_LineAttributesDialog() {

}

void GV_LineAttributesDialog::init() {
    cbColor->addItem(QIcon(":/images/color01.png"), tr("Red"));
    cbColor->addItem(QIcon(":/images/color02.png"), tr("Yellow"));
    cbColor->addItem(QIcon(":/images/color03.png"), tr("Green"));
    cbColor->addItem(QIcon(":/images/color04.png"), tr("Cyan"));
    cbColor->addItem(QIcon(":/images/color05.png"), tr("Blue"));
    cbColor->addItem(QIcon(":/images/color06.png"), tr("Magenta"));
    cbColor->addItem(QIcon(":/images/color07.png"), tr("Black / White"));
    cbColor->addItem(QIcon(":/images/color08.png"), tr("Gray"));
    cbColor->addItem(QIcon(":/images/color09.png"), tr("Light Gray"));
    //    cbColor->addItem(QIcon(":/images/colorxx.png"), tr("Others..")); TODO: implement

    cbWidth->addItem(QIcon(":/images/width01.png"), tr("Default"));
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.00mm");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.05mm");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.09mm");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.13mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.15mm");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.18mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.20mm");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.25mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width01.png"), "0.30mm");
    cbWidth->addItem(QIcon(":/images/width03.png"), "0.35mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width03.png"), "0.40mm");
    cbWidth->addItem(QIcon(":/images/width04.png"), "0.50mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width05.png"), "0.53mm");
    cbWidth->addItem(QIcon(":/images/width05.png"), "0.60mm");
    cbWidth->addItem(QIcon(":/images/width06.png"), "0.70mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width07.png"), "0.80mm");
    cbWidth->addItem(QIcon(":/images/width08.png"), "0.90mm");
    cbWidth->addItem(QIcon(":/images/width09.png"), "1.00mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width10.png"), "1.06mm");
    cbWidth->addItem(QIcon(":/images/width10.png"), "1.20mm");
    cbWidth->addItem(QIcon(":/images/width12.png"), "1.40mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width12.png"), "1.58mm");
    cbWidth->addItem(QIcon(":/images/width12.png"), "2.00mm (ISO)");
    cbWidth->addItem(QIcon(":/images/width12.png"), "2.11mm");

    cbLineType->addItem(QIcon(":/images/linetype00.png"), tr("No Pen"));
    cbLineType->addItem(QIcon(":/images/linetype01.png"), tr("Continuous"));
    cbLineType->addItem(QIcon(":/images/linetype03.png"), tr("Dash"));
    cbLineType->addItem(QIcon(":/images/linetype02.png"), tr("Dot"));
    cbLineType->addItem(QIcon(":/images/linetype04.png"), tr("Dash Dot"));
    cbLineType->addItem(QIcon(":/images/linetype05.png"), tr("Dash Dot Dot"));
    //    cbLineType->addItem(QIcon(":/images/linetype07.png"), tr("Custom")); TODO: implement
    readSettings();
}

/**
 * Set pen attributes (and comboboxes) based on pen of selected item
 * @param pen
 */
void GV_LineAttributesDialog::setPen(const QPen& pen) {
    mPen.setBrush(pen.brush());
    mPen.setCapStyle(pen.capStyle());
    mPen.setColor(pen.color());
    mPen.setCosmetic(pen.isCosmetic());
    mPen.setDashOffset(pen.dashOffset());
    mPen.setDashPattern(pen.dashPattern());
    mPen.setJoinStyle(pen.joinStyle());
    mPen.setMiterLimit(pen.miterLimit());
    mPen.setStyle(pen.style());
    mPen.setWidthF(pen.widthF());

    if (pen.color() == Qt::red) {
        cbColor->setCurrentIndex(0);
    } else if (pen.color() == Qt::yellow) {
        cbColor->setCurrentIndex(1);
    } else if (pen.color() == Qt::green) {
        cbColor->setCurrentIndex(2);
    } else if (pen.color() == Qt::cyan) {
        cbColor->setCurrentIndex(3);
    } else if (pen.color() == Qt::blue) {
        cbColor->setCurrentIndex(4);
    } else if (pen.color() == Qt::magenta) {
        cbColor->setCurrentIndex(5);
    } else if (pen.color() == Qt::black) {
        cbColor->setCurrentIndex(6);
    } else if (pen.color() == Qt::gray) {
        cbColor->setCurrentIndex(7);
    } else if (pen.color() == Qt::lightGray) {
        cbColor->setCurrentIndex(8);
    } else {
        cbColor->setCurrentIndex(9);
    }

    if (mPen.widthF() < 0.05) {
        cbWidth->setCurrentIndex(0); // cosmetic pen
//    } else if (mPen.widthF() == 0.00) {
//        cbWidth->setCurrentIndex(1);
    } else if (mPen.widthF() == 0.05) {
        cbWidth->setCurrentIndex(2);
    } else if (mPen.widthF() == 0.09) {
        cbWidth->setCurrentIndex(3);
    } else if (mPen.widthF() == 0.13) {
        cbWidth->setCurrentIndex(4);
    } else if (mPen.widthF() == 0.15) {
        cbWidth->setCurrentIndex(5);
    } else if (mPen.widthF() == 0.18) {
        cbWidth->setCurrentIndex(6);
    } else if (mPen.widthF() == 0.20) {
        cbWidth->setCurrentIndex(7);
    } else if (mPen.widthF() == 0.25) {
        cbWidth->setCurrentIndex(8);
    } else if (mPen.widthF() == 0.30) {
        cbWidth->setCurrentIndex(9);
    } else if (mPen.widthF() == 0.35) {
        cbWidth->setCurrentIndex(10);
    } else if (mPen.widthF() == 0.40) {
        cbWidth->setCurrentIndex(11);
    } else if (mPen.widthF() == 0.50) {
        cbWidth->setCurrentIndex(12);
    } else if (mPen.widthF() == 0.53) {
        cbWidth->setCurrentIndex(13);
    } else if (mPen.widthF() == 0.60) {
        cbWidth->setCurrentIndex(14);
    } else if (mPen.widthF() == 0.70) {
        cbWidth->setCurrentIndex(15);
    } else if (mPen.widthF() == 0.80) {
        cbWidth->setCurrentIndex(16);
    } else if (mPen.widthF() == 0.90) {
        cbWidth->setCurrentIndex(17);
    } else if (mPen.widthF() == 1.00) {
        cbWidth->setCurrentIndex(18);
    } else if (mPen.widthF() == 1.06) {
        cbWidth->setCurrentIndex(19);
    } else if (mPen.widthF() == 1.20) {
        cbWidth->setCurrentIndex(20);
    } else if (mPen.widthF() == 1.40) {
        cbWidth->setCurrentIndex(21);
    } else if (mPen.widthF() == 1.58) {
        cbWidth->setCurrentIndex(22);
    } else if (mPen.widthF() == 2.00) {
        cbWidth->setCurrentIndex(23);
    } else if (mPen.widthF() == 2.11) {
        cbWidth->setCurrentIndex(24);
    }

    if (mPen.style() == Qt::NoPen) {
        cbLineType->setCurrentIndex(0);
    } else if (mPen.style() == Qt::SolidLine) {
        cbLineType->setCurrentIndex(1);
    } else if (mPen.style() == Qt::DashLine) {
        cbLineType->setCurrentIndex(2);
    } else if (mPen.style() == Qt::DotLine) {
        cbLineType->setCurrentIndex(3);
    } else if (mPen.style() == Qt::DashDotLine) {
        cbLineType->setCurrentIndex(4);
    } else if (mPen.style() == Qt::DashDotDotLine) {
        cbLineType->setCurrentIndex(5);
    } else if (mPen.style() == Qt::CustomDashLine) {
        cbLineType->setCurrentIndex(6);
    }
}

/**
 * @return pen with user selection
 */
QPen GV_LineAttributesDialog::getPen() {
    switch (cbColor->currentIndex()) {
    case 0:
        mPen.setBrush(Qt::red);
        break;
    case 1:
        mPen.setBrush(Qt::yellow);
        break;
    case 2:
        mPen.setBrush(Qt::green);
        break;
    case 3:
        mPen.setBrush(Qt::cyan);
        break;
    case 4:
        mPen.setBrush(Qt::blue);
        break;
    case 5:
        mPen.setBrush(Qt::magenta);
        break;
    case 6:
        mPen.setBrush(Qt::black);
        break;
    case 7:
        mPen.setBrush(Qt::gray);
        break;
    case 8:
        mPen.setBrush(Qt::lightGray);
        break;
    default:
        // custom as already been set
        break;
    }

    switch (cbWidth->currentIndex()) {
    case 2:
        mPen.setWidthF(0.05);
        break;
    case 3:
        mPen.setWidthF(0.09);
        break;
    case 4:
        mPen.setWidthF(0.13);
        break;
    case 5:
        mPen.setWidthF(0.15);
        break;
    case 6:
        mPen.setWidthF(0.18);
        break;
    case 7:
        mPen.setWidthF(0.20);
        break;
    case 8:
        mPen.setWidthF(0.25);
        break;
    case 9:
        mPen.setWidthF(0.30);
        break;
    case 10:
        mPen.setWidthF(0.35);
        break;
    case 11:
        mPen.setWidthF(0.40);
        break;
    case 12:
        mPen.setWidthF(0.50);
        break;
    case 13:
        mPen.setWidthF(0.53);
        break;
    case 14:
        mPen.setWidthF(0.60);
        break;
    case 15:
        mPen.setWidthF(0.70);
        break;
    case 16:
        mPen.setWidthF(0.80);
        break;
    case 17:
        mPen.setWidthF(0.90);
        break;
    case 18:
        mPen.setWidthF(1.00);
        break;
    case 19:
        mPen.setWidthF(1.06);
        break;
    case 20:
        mPen.setWidthF(1.20);
        break;
    case 21:
        mPen.setWidthF(1.40);
        break;
    case 22:
        mPen.setWidthF(1.58);
        break;
    case 23:
        mPen.setWidthF(2.00);
        break;
    case 24:
        mPen.setWidthF(2.11);
        break;
    default:
        mPen.setWidthF(0.0); // cosmetic
        break;
    }

    switch (cbLineType->currentIndex()) {
    case 0:
        mPen.setStyle(Qt::NoPen);
        break;
    case 1:
        mPen.setStyle(Qt::SolidLine);
        break;
    case 2:
        mPen.setStyle(Qt::DashLine);
        break;
    case 3:
        mPen.setStyle(Qt::DotLine);
        break;
    case 4:
        mPen.setStyle(Qt::DashDotLine);
        break;
    case 5:
        mPen.setStyle(Qt::DashDotDotLine);
        break;
    case 6:
        mPen.setStyle(Qt::CustomDashLine);
        break;
    default:
        mPen.setStyle(Qt::SolidLine);
        break;
    }

    return mPen;
}

void GV_LineAttributesDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
