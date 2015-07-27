/*****************************************************************
 * $Id: gv_pipeline.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 24, 2008 10:50:41 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PIPELINE_H
#define GV_PIPELINE_H

#include "gv_component.h"
#include "gv_node.h"
//#include "ldt_line.h"


/**
 * Class that holds the pipe line data applicable for all lines on the drawing
 * such as nominal dimensions, design conditions, the line (SVG) information 
 * and line location information
 */
class GV_PipeLine : public LDT_Line {
    
public:
    GV_PipeLine(const RB_String& Id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);

    GV_PipeLine(GV_PipeLine* pipeLine);
    virtual ~GV_PipeLine();
    
//    int countComponent();
//    int countNode();
//
//	GV_Component* component(const RB_String& id);
//	GV_Node* node(GV_Component* comp);
//
//	void setValue(int number, const RB_Variant& var);
//	void setValue(const RB_String& name, const RB_Variant& var);
	
private:
	void createMembers();
	
};

#endif /*GV_PIPELINE_H*/
