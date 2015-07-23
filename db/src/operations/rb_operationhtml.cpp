/*****************************************************************
 * $Id: rb_operationhtml.cpp 2011 2013-10-16 16:09:55Z rutger $
 * Created: Oct 5, 2013 10:14:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Database (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "rb_operationhtml.h"

/**
 * Constructor
 */
RB_OperationHtml::RB_OperationHtml() {
    mObject = NULL;

}

/**
 * Destructor
 */
RB_OperationHtml::~RB_OperationHtml() {

}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool RB_OperationHtml::execute(RB_ObjectBase* /* obj */) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "RB_OperationHtml::execute() "
                    "not implemented ERROR");
    return false;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool RB_OperationHtml::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "RB_OperationHtml::execute() "
                    "not implemented ERROR");
    return false;
}

/**
 * HACK: to create horizontal line, one pixel transparent PNG image in <p></p>
 * for QTextEdit
 * @brief getOnePixelTransparentImage
 * @return one pixel transparent image in p tag
 */
RB_String RB_OperationHtml::getOnePixelTransparentImage() const {
    return RB_String("<p style='margin-top:0px; margin-bottom:0px; margin-left:0px; "
                     "margin-right:0px; -qt-block-indent:0; text-indent:0px;'>"
                     "<img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAY"
                     "AAAAfFcSJAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAGXRFWHRDb21tZW50AENyZWF0ZW"
                     "Qgd2l0aCBHSU1QV4EOFwAAAA1JREFUCJljYGBgYAAAAAUAAYehTtQAAAAASUVORK5CYII=' "
                     "height=\"1\" width=\"1\"/></p>");
}
