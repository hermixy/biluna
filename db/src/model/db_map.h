/*****************************************************************
 * $Id: db_map.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_MAP_H
#define DB_MAP_H

#include "rb_objectcontainer.h"


/**
 * Class DB_Map
 * Container for mapping elements of CSV file to database
 */
class DB_EXPORT DB_Map : public RB_ObjectContainer {

public:
    DB_Map(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    DB_Map(DB_Map* map);
    virtual ~DB_Map();
    
private:
	void createMembers();
    
};

#endif /*DB_MAP_H*/
