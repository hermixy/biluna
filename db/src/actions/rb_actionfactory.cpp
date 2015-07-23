/*****************************************************************
 * $Id: rb_actionfactory.cpp 1338 2010-12-29 16:55:02Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_actionfactory.h"

#include "rb_debug.h"

/**
 * Constructor
 */
RB_ActionFactory::RB_ActionFactory(RB_MainWindow* mw)
        : QObject(mw) {

    mMainWindow = mw;
    mPerspective = RB2::PerspectiveNone;
}

RB_ActionFactory::~RB_ActionFactory() {
    // nothing
    RB_DEBUG->print("RB_ActionFactory::~RB_ActionFactory() OK");
}

/**
 * Get actionfactory perspective
 * @return RB2::PerspectiveType as integer
 */
int RB_ActionFactory::getPerspective() {
    return mPerspective;
}

/**
 * Set actionfactory perspective type
 * @param RB2::PerspectiveType as integer
 */
void RB_ActionFactory::setPerspective(int perspectiveType) {
    mPerspective = perspectiveType;
}

/**
 * Virtual slot receiving signals to update corresponding actions, does nothing
 */
void RB_ActionFactory::slotUpdateAction() {
    RB_DEBUG->print(RB_Debug::D_WARNING,
                    "RB_ActionFactory::slotUpdateAction() "
                    "not implemented WARNING");
}
