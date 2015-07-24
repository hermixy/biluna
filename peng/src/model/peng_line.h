/*****************************************************************
 * $Id: peng_line.h 1826 2012-12-08 00:31:33Z rutger $
 * Created: 2004 - rutger
 *
 * Copyright (C) 2004-2005 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_LINE_H
#define PENG_LINE_H

#include "rb_objectcontainer.h"

/**
 * Class for line designation table (LDT) line data
 */
class PENG_Line : public RB_ObjectContainer {
    
public:
    PENG_Line(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    PENG_Line(PENG_Line* line);
    virtual ~PENG_Line ();
    
    void setValue(int number, const RB_Variant& var);
    void setValue(const RB_String& name, const RB_Variant& var);
    
     
private:
    void createMembers();

    // Not used
    void createProcessMembers();
    void createEnginDesMembers();
    void createClassifMembers();
    void createOperMaintMembers();
    void createRemarkMembers();
    void createCustomMembers();

};
#endif //PENG_LINE_H
