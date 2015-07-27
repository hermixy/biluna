/*****************************************************************
 * $Id: gv_commandevent.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 4:24:46 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_commandevent.h 4008
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_COMMANDEVENT_H
#define GV_COMMANDEVENT_H

#include "rb_string.h"

/**
 * Command Events, from command line
 */
class GV_CommandEvent {
	
public:
    /**
     * Creates a new command event that is not yet accepted.
     *
     * @param cmd the command that was triggered.
     */
    GV_CommandEvent(const RB_String& cmd) {
        this->cmd = cmd;
        accepted = false;
    }
 
    /**
     * @return the command that was triggered, usually by
     * the user.
     */
    RB_String getCommand() {
        return cmd;
    }

    /**
     * Sets the event state to accepted.
     */
    void accept() {
        accepted = true;
    }

    /**
     * @return Whether the event was already accepted or not.
     */
    bool isAccepted() {
        return accepted;
    }
 
protected:
    RB_String cmd;
    bool accepted;
};

#endif
