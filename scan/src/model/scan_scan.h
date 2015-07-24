/*****************************************************************
 * $Id: scan_scan.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_SCAN_H
#define SCAN_SCAN_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the questions and the different type of reports
 */
class SCAN_Scan : public RB_ObjectContainer {

public:
    SCAN_Scan(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);

    SCAN_Scan(SCAN_Scan* scan);
    virtual ~SCAN_Scan();

private:
    void createMembers();


};

#endif /*SCAN_SCAN_H*/
