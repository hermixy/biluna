/*****************************************************************
 * $Id: gv_project.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 24, 2008 9:54:12 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PROJECT_H
#define GV_PROJECT_H

#include "rb_objectcontainer.h"


class GV_Project : public RB_ObjectContainer {
    
public:
    GV_Project(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
	GV_Project(GV_Project* project);
    virtual ~GV_Project ();
    
    //virtual void synchronizeModelData(RB_ObjectBase* obj = NULL);
    
private:
	
	void createMembers();

};

#endif /*GV_PROJECT_H*/
