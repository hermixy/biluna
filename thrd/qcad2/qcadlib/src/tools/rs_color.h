/****************************************************************************
** $Id: rs_color.h 7877 2008-01-14 17:44:08Z andrew $
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


#ifndef RS_COLOR_H
#define RS_COLOR_H

#include <qcolor.h>

#include "rs_flags.h"


/**
 * Color class.
 *
 * @author Andrew Mustun
 */
class CAD_EXPORT RS_Color: public QColor, public RS_Flags {
public:
    RS_Color() : QColor(), RS_Flags() {}
    explicit RS_Color(Qt::GlobalColor gc) : QColor(gc), RS_Flags() {}
    RS_Color(int r, int g, int b, int a=255) : QColor(r, g, b, a), RS_Flags() {}
    RS_Color(const QColor& c) : QColor(c), RS_Flags() {}
    RS_Color(const RS_Color& c) : QColor(c), RS_Flags() {
        setFlags(c.getFlags());
    }
    explicit RS_Color(unsigned int f) : QColor(), RS_Flags(f) {}

    /** @return A copy of this color without flags. */
    RS_Color stripFlags() const {
        return RS_Color(red(), green(), blue());
    }

    /** @return true if the color is defined by layer. */
    bool isByLayer() const {
        return getFlag(RS2::FlagByLayer);
    }

    /** @return true if the color is defined by block. */
    bool isByBlock() const {
        return getFlag(RS2::FlagByBlock);
    }

    RS_String toString() {
        return RS_String("RS_Color: r: %1, g: %2, b: %3")
            .arg(red()).arg(green()).arg(blue());
    }


    RS_Color& operator = (const RS_Color& c) {
        setRgb(c.red(), c.green(), c.blue(), c.alpha());
        setFlags(c.getFlags());

        return *this;
    }

    bool operator == (const RS_Color& c) const {
        return (red()==c.red() &&
                green()==c.green() &&
                blue()==c.blue() &&
                alpha()==c.alpha() &&
                getFlags()==c.getFlags());
    }
};

#endif

