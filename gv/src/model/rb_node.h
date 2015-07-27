/*****************************************************************
 * $Id: rb_node.h 1452 2011-09-10 08:19:50Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_NODE_H
#define RB_NODE_H

#include "gv.h"
#include "rb_objectatomic.h"
#include "RVector.h"


class RB_Node : public RB_ObjectAtomic {
public:
    RB_Node(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    RB_Node(RB_Node* node);
    virtual ~RB_Node();

    virtual RVector location() const;
    virtual RVector direction() const;
    virtual double orientation() const;

    virtual void setLocation(const RVector& v);
    virtual void setDirection(const RVector& v);
    virtual void setOrientation(double angle);

    virtual RB_String parentComponentId() const;
    virtual RB_String parentPortId() const;
    virtual RB_String connectComponentId() const;
    virtual RB_String connectPortId() const;
    virtual GV2::NodeType parentNodeType() const;
    virtual GV2::NodeType connectNodeType() const;

    virtual void setParentComponentId(const RB_String& componentId);
    virtual void setParentPortId(const RB_String& portId);
    virtual void setConnectComponentId(const RB_String& componentId);
    virtual void setConnectPortId(const RB_String& portId);
    virtual void setParentNodeType(GV2::NodeType nodeType);
    virtual void setConnectNodeType(GV2::NodeType nodeType);

private:
    void createMembers();

};

#endif /*RB_NODE_H*/
