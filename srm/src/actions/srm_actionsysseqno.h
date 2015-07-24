/*****************************************************************
 * $Id: srm_actionsysseqno.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Jan 26, 2014 10:51:42 AM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna SRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONSYSSEQNO_H
#define SRM_ACTIONSYSSEQNO_H

#include "rb_action.h"


/**
 * Action opens system sequence number edit dialog
 */
class SRM_ActionSysSeqNo : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionSysSeqNo();
    virtual ~SRM_ActionSysSeqNo() {}

    static RB_String getName() {
        return "System sequence numbers";
    }

    virtual RB_String name() {
        return SRM_ActionSysSeqNo::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONSYSSEQNO_H
