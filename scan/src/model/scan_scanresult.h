/*****************************************************************
 * $Id: scan_scanresult.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_SCANRESULT_H
#define SCAN_SCANRESULT_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the equipment PED classification data,
 * child of EQL_Equipment and LDT_Line
 */
class SCAN_ScanResult : public RB_ObjectContainer {

public:
    SCAN_ScanResult(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);

    SCAN_ScanResult(SCAN_ScanResult* scanRes);
    virtual ~SCAN_ScanResult();

private:
    void createMembers();


};

#endif /*SCAN_SCANRESULT_H*/
