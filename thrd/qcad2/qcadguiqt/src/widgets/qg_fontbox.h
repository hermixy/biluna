/****************************************************************************
** $Id: qg_fontbox.h 5087 2007-03-29 09:48:14Z andrew $
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

#ifndef QG_FONTBOX_H
#define QG_FONTBOX_H

#include <qcombobox.h>

#include "rs.h"
#include "rs_font.h"
#include "rs_fontlist.h"

/**
 * A combobox for choosing a font name.
 */
class CAD_EXPORT QG_FontBox: public QComboBox {
    Q_OBJECT

public:
    QG_FontBox(QWidget* parent=0);
    virtual ~QG_FontBox();

    RS_Font* getFont() {
        return currentFont;
    }
    void setFont(const RS_String& fName);

    void init();

private slots:
    void slotFontChanged(int index);

signals:
    void fontChanged(RS_Font* font);

private:
    RS_Font* currentFont;
};

#endif

