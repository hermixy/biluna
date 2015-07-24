/*****************************************************************
 * $Id: peng_pedclassif.h 2162 2014-08-31 08:49:37Z rutger $
 * Created: Feb 17, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_PEDCLASSIF_H
#define PENG_PEDCLASSIF_H

#include "rb_objectatomic.h"


/**
 * Class that holds the equipment PED classification data,
 * child of PENG_Equipment and PENG_Line
 */
class PENG_PedClassif : public RB_ObjectAtomic {
    
public:
    PENG_PedClassif(const RB_String& id = "", RB_ObjectBase* p = NULL,
                    const RB_String& n = "", RB_ObjectFactory* f = NULL);

    PENG_PedClassif(PENG_PedClassif* classif);
    virtual ~PENG_PedClassif();
    
private:
	void createMembers();


};

#endif /*PENG_PEDCLASSIF_H*/
