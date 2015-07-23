/*****************************************************************
 * $Id: db_simplereport.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sept 27, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_SIMPLEREPORT_H
#define DB_SIMPLEREPORT_H

#include "rb_objectatomic.h"


/**
 * Atomic data class for the creation of a simple report from the database
 */
class DB_EXPORT DB_SimpleReport : public RB_ObjectAtomic {

public:
    DB_SimpleReport(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_SimpleReport(DB_SimpleReport* simple);
    virtual ~DB_SimpleReport();
    
private:
	void createMembers();
    
};

#endif /*DB_SIMPLEREPORT_H*/
