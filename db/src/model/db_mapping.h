/*****************************************************************
 * $Id: db_mapping.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MAPPING_H
#define DB_MAPPING_H

#include "rb_objectatomic.h"


/**
 * Class DB_Mapping
 * For mapping of CSV file element to database table field
 */
class DB_EXPORT DB_Mapping : public RB_ObjectAtomic {

public:
    DB_Mapping(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_Mapping(DB_Mapping* map);
    virtual ~DB_Mapping();
    
private:
	void createMembers();
    
};

#endif /*DB_MAPPING_H*/
