/*****************************************************************
 * $Id: peng_lineclassif.h 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_LINECLASSIF_H
#define PENG_LINECLASSIF_H

#include "rb_objectcontainer.h"

/**
 * Class for line designation table (LDT) line data, classification subdata
 */
class PENG_LineClassif : public RB_ObjectContainer {
    
public:
    PENG_LineClassif(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_LineClassif(PENG_LineClassif* line);
    virtual ~PENG_LineClassif ();
    
    void setValue(int number, const RB_Variant& var);
    void setValue(const RB_String& name, const RB_Variant& var);
    
     
private:
    void createMembers();

};
#endif //PENG_LINECLASSIF_H
