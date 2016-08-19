/****************************************************************************
** $Id: rs_variabledict.cpp 8371 2008-01-23 18:14:20Z andrew $
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


#include "rs_variabledict.h"


/**
 * Constructor.
 */
RS_VariableDict::RS_VariableDict() {
}


/**
 * Removes all variables in the blocklist.
 */
void RS_VariableDict::clear() {
    variables.clear();
}


/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void RS_VariableDict::add(const RS_String& key,
                                  const RS_String& value, int code) {
    if (key.isEmpty()) {
        RS_DEBUG->print("RS_VariableDict::addVariable(): "
                        "No empty keys allowed.",
                        RS_Debug::D_WARNING);
        return;
    }

    variables[key] = RS_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void RS_VariableDict::add(const RS_String& key, int value, int code) {
    if (key.isEmpty()) {
        RS_DEBUG->print("RS_VariableDict::addVariable(): "
                        "No empty keys allowed.",
                        RS_Debug::D_WARNING);
        return;
    }

    variables[key] = RS_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void RS_VariableDict::add(const RS_String& key, double value, int code) {
    if (key.isEmpty()) {
        RS_DEBUG->print("RS_VariableDict::addVariable(): "
                        "No empty keys allowed.",
                        RS_Debug::D_WARNING);
        return;
    }

    variables[key] = RS_Variable(value, code);
}



/**
 * Adds a variable to the variable dictionary. If a variable with the 
 * same name already exists, is will be overwritten.
 */
void RS_VariableDict::add(const RS_String& key,
                                  const RS_Vector& value, int code) {
    if (key.isEmpty()) {
        RS_DEBUG->print("RS_VariableDict::addVariable(): "
                        "No empty keys allowed.",
                        RS_Debug::D_WARNING);
        return;
    }

    variables[key] = RS_Variable(value, code);
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
RS_Vector RS_VariableDict::getVector(const RS_String& key,
        const RS_Vector& def) {

    RS_Vector ret;

    iter = variables.find(key);
    if (iter==variables.end()) { 
        ret = def;
    } else {
        RS_Variable var = *iter;
        if (var.getType()!=RS2::VariableVector) {
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
RS_String RS_VariableDict::getString(const RS_String& key,
        const RS_String& def) {

    RS_String ret;
    
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        RS_Variable var = *iter;
        if (var.getType()!=RS2::VariableString) {
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
int RS_VariableDict::getInt(const RS_String& key,
                                    int def) {

    int ret;
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        RS_Variable var = *iter;
        if (var.getType()!=RS2::VariableInt) {
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
double RS_VariableDict::getDouble(const RS_String& key,
        double def) {

    double ret;
    iter = variables.find(key);

    if (iter==variables.end()) {
        ret = def;
    } else {
        RS_Variable var = *iter;
        if (var.getType()!=RS2::VariableDouble) {
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
void RS_VariableDict::remove(const RS_String& key) {
    variables.remove(key);
}

