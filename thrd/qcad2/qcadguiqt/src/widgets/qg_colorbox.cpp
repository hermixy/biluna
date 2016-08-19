/****************************************************************************
** $Id: qg_colorbox.cpp 9483 2008-03-31 12:18:51Z andrew $
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

#include "qg_colorbox.h"

#include <qcolordialog.h>
#include <qpainter.h>
#include <qpixmap.h>

/*
#include "xpm/color00.xpm"
#include "xpm/color01.xpm"
#include "xpm/color02.xpm"
#include "xpm/color03.xpm"
#include "xpm/color04.xpm"
#include "xpm/color05.xpm"
#include "xpm/color06.xpm"
#include "xpm/color07.xpm"
#include "xpm/color08.xpm"
#include "xpm/color09.xpm"
#include "xpm/color11.xpm"
#include "xpm/color14.xpm"
#include "xpm/colorxx.xpm"
*/


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_ColorBox::QG_ColorBox(QWidget* parent)
        : QComboBox(parent) {

    showByLayer = false;
    showUnchanged = false;
    unchanged = false;
}

/**
 * Constructor that calls init and provides a fully functional 
 * combobox for choosing colors.
 *
 * @param showByLayer true: Show attribute ByLayer, ByBlock.
 * @param showUnchanged true: Show item 'unchanged' for color change
 *                      dialogs to present an option for not changing
 *                      the color
 */
QG_ColorBox::QG_ColorBox(bool showByLayer, bool showUnchanged, 
    QWidget* parent)
        : QComboBox(parent) {
    
    unchanged = false;
    init(showByLayer, showUnchanged);
}


/**
 * Destructor
 */
QG_ColorBox::~QG_ColorBox() {}


/**
 * Initialisation (called from constructor or manually but only
 * once).
 *
 * @param showByLayer true: Show attribute ByLayer, ByBlock.
 */
void QG_ColorBox::init(bool showByLayer, bool showUnchanged) {
    this->showByLayer = showByLayer;
    this->showUnchanged = showUnchanged;

    if (showUnchanged) {
        addItem(QIcon(":/qcad/images/color00.png"), tr("Unchanged"));
    }
    if (showByLayer) {
        addItem(QIcon(":/qcad/images/color00.png"), tr("By Layer"));
        addItem(QIcon(":/qcad/images/color00.png"), tr("By Block"));
    }
    addItem(QIcon(":/qcad/images/color01.png"), tr("Red"));
    addItem(QIcon(":/qcad/images/color02.png"), tr("Yellow"));
    addItem(QIcon(":/qcad/images/color03.png"), tr("Green"));
    addItem(QIcon(":/qcad/images/color04.png"), tr("Cyan"));
    addItem(QIcon(":/qcad/images/color05.png"), tr("Blue"));
    addItem(QIcon(":/qcad/images/color06.png"), tr("Magenta"));
    addItem(QIcon(":/qcad/images/color07.png"), tr("Black / White"));
    addItem(QIcon(":/qcad/images/color08.png"), tr("Gray"));
    addItem(QIcon(":/qcad/images/color09.png"), tr("Light Gray"));
    //addItem(QIcon(":/qcad/images/color11.png"), tr("11"));
    //addItem(QIcon(":/qcad/images/color14.png"), tr("14"));
    addItem(QIcon(":/qcad/images/colorxx.png"), tr("Others.."));

    connect(this, SIGNAL(activated(int)),
            this, SLOT(slotColorChanged(int)));

    if (showUnchanged) {
        setCurrentIndex(0);
    }
    else if (showByLayer) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(6);
    }

    slotColorChanged(currentIndex());
}

/**
 * Sets the color shown in the combobox to the given color.
 */
void QG_ColorBox::setColor(const RS_Color& color) {
    currentColor = color;
        
    if (color.isByLayer() && showByLayer) {
        setCurrentIndex(0);
    } else if (color.isByBlock() && showByLayer) {
        setCurrentIndex(1);
    } else if (color==RS_Color(Qt::red)) {
        setCurrentIndex(0+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::yellow)) {
        setCurrentIndex(1+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::green)) {
        setCurrentIndex(2+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::cyan)) {
        setCurrentIndex(3+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::blue)) {
        setCurrentIndex(4+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::magenta)) {
        setCurrentIndex(5+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::white) || color==RS_Color(Qt::black)) {
        setCurrentIndex(6+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::darkGray)) {
        setCurrentIndex(7+(int)showByLayer*2 + (int)showUnchanged);
    } else if (color==RS_Color(Qt::lightGray)) {
        setCurrentIndex(8+(int)showByLayer*2 + (int)showUnchanged);
    } else {
        setCurrentIndex(9+(int)showByLayer*2 + (int)showUnchanged);
    }

    if (currentIndex()!=9+(int)showByLayer*2 + (int)showUnchanged) {
        slotColorChanged(currentIndex());
    }
}



/**
 * Sets the color of the pixmap next to the "By Layer" item
 * to the given color.
 */
void QG_ColorBox::setLayerColor(const RS_Color& color) {
    if (showByLayer) {
        QPixmap pixmap;
        if (color==RS_Color(Qt::black) || color==RS_Color(Qt::white)) {
            pixmap = QPixmap(":/qcad/images/color07.png");
        } else {
            pixmap = QPixmap(":/qcad/images/color00.png");
            int w = pixmap.width();
            int h = pixmap.height();
            QPainter painter(&pixmap);
            painter.fillRect(1, 1, w-2, h-2, color);
            painter.end();
        }

        setItemIcon(0, pixmap);

        // needed for the first time a layer is added:
        if (currentIndex()!=9) {
            slotColorChanged(currentIndex());
        }
    }
}



/**
 * Called when the color has changed. This method 
 * sets the current color to the value chosen or even
 * offers a dialog to the user that allows him/ her to
 * choose an individual color.
 */
void QG_ColorBox::slotColorChanged(int index) {
    currentColor.resetFlags();
    
    if (showUnchanged) {
        if (index==0) {
            unchanged = true;
        }
        else {
            unchanged = false;
        }
    }

    if (showByLayer) {
        switch (index-(int)showUnchanged) {
        case 0:
            currentColor = RS_Color(RS2::FlagByLayer);
            break;
        case 1:
            currentColor = RS_Color(RS2::FlagByBlock);
            break;
        default:
            break;
        }
    }

    switch (index-(int)showByLayer*2-(int)showUnchanged) {
    case 0:
        currentColor.setRgb(255, 0, 0);
        break;
    case 1:
        currentColor.setRgb(255, 255, 0);
        break;
    case 2:
        currentColor.setRgb(0, 255, 0);
        break;
    case 3:
        currentColor.setRgb(0, 255, 255);
        break;
    case 4:
        currentColor.setRgb(0, 0, 255);
        break;
    case 5:
        currentColor.setRgb(255, 0, 255);
        break;
    case 6:
        currentColor.setRgb(0, 0, 0);
        break;
    case 7:
        currentColor.setRgb(128, 128, 128);
        break;
    case 8:
        currentColor.setRgb(192, 192, 192);
        break;
    case 9:
#if QT_VERSION>=0x030000
        currentColor = QColorDialog::getColor(currentColor, this);
#endif
        break;
    default:
        break;
    }

    //printf("Current color is (%d): %s\n",
    //       index, currentColor.name().latin1());

    emit colorChanged(currentColor);
}


