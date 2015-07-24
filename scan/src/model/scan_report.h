/*****************************************************************
 * $Id: scan_report.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_REPROT_H
#define SCAN_REPROT_H

#include "rb_objectcontainer.h"


/**
 * Class that holds the title and all chapters, possible paragraphs
 * and graphs of a report
 */
class SCAN_Report : public RB_ObjectContainer {

public:
    SCAN_Report(const RB_String& id = "", RB_ObjectBase* p = 0,
                    const RB_String& n = "", RB_ObjectFactory* f = NULL);
    SCAN_Report(SCAN_Report* report);
    virtual ~SCAN_Report ();

private:
    void createMembers();

};

#endif /*SCAN_REPROT_H*/
