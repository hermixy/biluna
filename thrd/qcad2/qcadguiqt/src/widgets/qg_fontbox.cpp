/****************************************************************************
** $Id: qg_fontbox.cpp 5087 2007-03-29 09:48:14Z andrew $
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

#include "qg_fontbox.h"

#include <qpixmap.h>
#include <qstringlist.h>

#include "rs_debug.h"


/**
 * Default Constructor. You must call init manually if you choose
 * to use this constructor.
 */
QG_FontBox::QG_FontBox(QWidget* parent)
        : QComboBox(parent) {}



/**
 * Destructor
 */
QG_FontBox::~QG_FontBox() {}


/**
 * Initialisation (called from constructor or manually but only
 * once).
 */
void QG_FontBox::init() {
    RS_DEBUG->print("QG_FontBox::init");
    QStringList fonts;

    for (RS_Font* f = RS_FONTLIST->firstFont();
            f!=NULL;
            f = RS_FONTLIST->nextFont()) {

        fonts.append(f->getFileName());
    }
    
    RS_DEBUG->print("QG_FontBox::init: 001");

    fonts.sort();
    addItems(fonts);
    
    RS_DEBUG->print("QG_FontBox::init: 002");

    connect(this, SIGNAL(activated(int)),
            this, SLOT(slotFontChanged(int)));

    RS_DEBUG->print("QG_FontBox::init: 003");
    setCurrentIndex(0);
    RS_DEBUG->print("QG_FontBox::init: 004");
    slotFontChanged(currentIndex());
    RS_DEBUG->print("QG_FontBox::init: OK");
}



/**
 * Sets the currently selected width item to the given width.
 */
void QG_FontBox::setFont(const RS_String& fName) {

    RS_DEBUG->print("QG_FontBox::setFont %s\n", (const char*)fName.toLatin1());

    setEditText(fName);

    slotFontChanged(currentIndex());
}



/**
 * Called when the font has changed. This method 
 * sets the current font to the value chosen.
 */
void QG_FontBox::slotFontChanged(int index) {

    RS_DEBUG->print("QG_FontBox::slotFontChanged %d\n", index);

    currentFont = RS_FONTLIST->requestFont(currentText());

    if (currentFont!=NULL) {
        RS_DEBUG->print("Current font is (%d): %s\n",
                        index, (const char*)currentFont->getFileName().toLatin1());
    }

    emit fontChanged(currentFont);
}


