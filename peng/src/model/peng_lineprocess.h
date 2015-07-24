/*****************************************************************
 * $Id: peng_lineprocess.h 2154 2014-07-30 19:44:11Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_LINEPROCESS_H
#define PENG_LINEPROCESS_H

#include "rb_objectcontainer.h"

/**
 * Class for line designation table (LDT) line data, subdata process
 */
class PENG_LineProcess : public RB_ObjectContainer {
    
public:
    PENG_LineProcess(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_LineProcess(PENG_LineProcess* line);
    virtual ~PENG_LineProcess ();
    
    void setValue(int number, const RB_Variant& var);
    void setValue(const RB_String& name, const RB_Variant& var);
    
     
private:
    void createMembers();

};
#endif //PENG_LINEPROCESS_H
