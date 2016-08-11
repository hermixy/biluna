/****************************************************************************
** $Id: gv_variabledict.cpp 8371 2008-01-23 18:14:20Z andrew $
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


#include "gv_variabledict.h"


/**
 * Constructor.
 */
GV_VariableDict::GV_VariableDict() {
}


/**
 * Removes all variables in the blocklist.
 */
void GV_VariableDict::clear() {
    variables.clear();
}


/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void GV_VariableDict::add(const QString& key,
                                  const QString& value, int code) {
    if (key.isEmpty()) {
//        RB_DEBUG->print("GV_VariableDict::addVariable(): "
//                        "No empty keys allowed.",
//                        RB_Debug::D_WARNING);
        return;
    }

    variables[key] = GV_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void GV_VariableDict::add(const QString& key, int value, int code) {
    if (key.isEmpty()) {
        RB_DEBUG->warning("GV_VariableDict::addVariable(): "
                          "No empty keys allowed.");
        return;
    }

    variables[key] = GV_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void GV_VariableDict::add(const QString& key, double value, int code) {
    if (key.isEmpty()) {
        RB_DEBUG->warning("GV_VariableDict::addVariable(): "
                          "No empty keys allowed.");
        return;
    }

    variables[key] = GV_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void GV_VariableDict::add(const QString& key,
                                  const GV_Vector& value, int code) {
    if (key.isEmpty()) {
        RB_DEBUG->warning("GV_VariableDict::addVariable(): "
                          "No empty keys allowed.");
        return;
    }

    variables[key] = GV_Variable(value, code);
}



/**
 * Gets the value for the given variable.
 *
 * @param key Key of the variable.
 * @param def Default value.
 *
 * @return The value for the given variable or the given default value
 * if the variable couldn't be found.
 */
GV_Vector GV_VariableDict::getVector(const QString& key,
        const GV_Vector& def) {

    GV_Vector ret;

    iter = variables.find(key);
    if (iter==variables.end()) { 
        ret = def;
    } else {
        GV_Variable var = *iter;
        if (var.getType()!=GV2::VariableVector) {
            ret = def;
        }
        else {
            ret = var.getVector();
        }
    }
    
    return ret;
}



/**
 * Gets the value for the given variable.
 *
 * @param key Key of the variable.
 * @param def Default value.
 *
 * @return The value for the given variable or the given default value
 * if the variable couldn't be found.
 */
QString GV_VariableDict::getString(const QString& key,
        const QString& def) {

    QString ret;
    
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        GV_Variable var = *iter;
        if (var.getType()!=GV2::VariableString) {
            ret = def;
        } else {
            ret = var.getString();
        }
    }
    
    return ret;
}



/**
 * Gets the value as int for the given variable.
 *
 * @param key Key of the variable.
 * @param def Default value.
 *
 * @return The value for the given variable or the given default value
 * if the variable couldn't be found.
 */
int GV_VariableDict::getInt(const QString& key,
                                    int def) {

    int ret;
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        GV_Variable var = *iter;
        if (var.getType()!=GV2::VariableInt) {
            ret = def;
        }
        else {
            ret = var.getInt();
        }
    }
    return ret;
}



/**
 * Gets the value as double for the given variable.
 *
 * @param key Key of the variable.
 * @param def Default value.
 *
 * @return The value for the given variable or the given default value
 * if the variable couldn't be found.
 */
double GV_VariableDict::getDouble(const QString& key,
        double def) {

    double ret;
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        GV_Variable var = *iter;
        if (var.getType()!=GV2::VariableDouble) {
            ret = def;
        }
        else {
            ret = var.getDouble();
        }
    }

    return ret;
}



/**
 * Removes a variable from the list.
 */
void GV_VariableDict::remove(const QString& key) {
    variables.remove(key);
}

