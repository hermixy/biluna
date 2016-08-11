/****************************************************************************
** $Id: gv_variabledict.h 4861 2007-03-07 15:29:43Z andrew $
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


#ifndef GV_VARIABLEDICT_H
#define GV_VARIABLEDICT_H


#include "gv_variabledict.h"
#include "gv_variable.h"
#include "gv_hash.h"
#include <QString>
#include "rb_debug.h"

/**
 * Dictionary of variables. The variables are stored as key / value
 * pairs (string / string).
 *
 * @author Andrew Mustun
 */
class GV_VariableDict {
public:
    GV_VariableDict();
    virtual ~GV_VariableDict() {}

    void clear();
    /**
     * @return Number of variables available.
     */
    int count() {
        return variables.count();
    }

    void add(const QString& key, const GV_Vector& value, int code);
    void add(const QString& key, const QString& value, int code);
    void add(const QString& key, int value, int code);
    void add(const QString& key, double value, int code);

    GV_Vector getVector(const QString& key, const GV_Vector& def);
    QString getString(const QString& key, const QString& def);
    int getInt(const QString& key, int def);
    double getDouble(const QString& key, double def);

    virtual void remove(const QString& key);

    GV_Hash<QString, GV_Variable>& getVariableDict() {
        return variables;
    }

private:
    //! Variables for the graphic
    GV_Hash<QString, GV_Variable> variables;
    GV_Hash<QString, GV_Variable>::iterator iter;
};

#endif
