/****************************************************************************
** $Id: gv_variable.h 4861 2007-03-07 15:29:43Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_VARIABLE_H
#define GV_VARIABLE_H


#include "gv.h"
#include <QString>
#include "gv_vector.h"
#include "rb_debug.h"

/**
 * A variable of type int, double, string or vector.
 * The variable also contains its type and an int code
 * which can identify its contents in any way.
 *
 * @author Andrew Mustun
 */
class GV_Variable {
public:
    typedef struct {
        QString s;
        int i;
        double d;
        GV_Vector v;
    } GV_VariableContents;
    
    GV_Variable() {
        type = GV2::VariableVoid;
        code = 0;
    }
    GV_Variable(const GV_Vector& v, int c) {
        setVector(v);
        code = c;
    }
    GV_Variable(const QString& v, int c) {
        setString(v);
        code = c;
    }
    GV_Variable(int v, int c) {
        setInt(v);
        code = c;
    }
    GV_Variable(double v, int c) {
        setDouble(v);
        code = c;
    }
    virtual ~GV_Variable() {}

    void setString(const QString& str) {
        contents.s = str;
        type = GV2::VariableString;
    }
    void setInt(int i) {
        contents.i = i;
        type = GV2::VariableInt;
    }
    void setDouble(double d) {
        contents.d = d;
        type = GV2::VariableDouble;
    }
    void setVector(const GV_Vector& v) {
        contents.v = v;
        type = GV2::VariableVector;
    }

    QString getString() {
        return contents.s;
    }
    int getInt() {
        return contents.i;
    }
    double getDouble() {
        return contents.d;
    }
    GV_Vector getVector() {
        return contents.v;
    }

    GV2::VariableType getType() {
        return type;
    }
    int getCode() {
        return code;
    }

private:
    GV_VariableContents contents;
    GV2::VariableType type;
    int code;
};

#endif
