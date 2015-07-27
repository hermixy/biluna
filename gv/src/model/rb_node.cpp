/*****************************************************************
 * $Id: rb_node.cpp 1983 2013-09-02 18:10:02Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_node.h"


RB_Node::RB_Node(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f)
                    : RB_ObjectAtomic(id, p, n, f) {
    createMembers();
}


RB_Node::RB_Node(RB_Node* node) : RB_ObjectAtomic(node) {
    createMembers();
    *this = *node;
}

RB_Node::~RB_Node() {
    // nothing
}


RVector RB_Node::location() const {
    // RB_DEBUG->print("RB_Node::getLocation()");
    return RVector(getValue("locX").toDouble(),
                        getValue("locY").toDouble(),
                        getValue("locZ").toDouble());
}


RVector RB_Node::direction() const {
    // RB_DEBUG->print("RB_Node::getDirection()");
    return RVector(getValue("dirX").toDouble(),
                        getValue("dirY").toDouble(),
                        getValue("dirZ").toDouble());
}


double RB_Node::orientation() const {
    // RB_DEBUG->print("RB_Node::getOrientation()");
    return getValue("oriAngle").toDouble();
}


void RB_Node::setLocation(const RVector& v) {
    // RB_DEBUG->print("setLocation()");
    setValue("locX", v.x);
    setValue("locY", v.y);
    setValue("locZ", v.z);
}

void RB_Node::setDirection(const RVector& v) {
    // RB_DEBUG->print("RB_Node::setDirection()");
    setValue("dirX", v.x);
    setValue("dirY", v.y);
    setValue("dirZ", v.z);
}

void RB_Node::setOrientation(double angle) {
    // RB_DEBUG->print("RB_Node::setOrientation()");
    setValue("oriAngle", angle);
}

/**
 * Get parent component ID
 */
RB_String RB_Node::parentComponentId() const {
    return getValue("parentComponentId").toString();
}

/**
 * Get parent component port number
 */
RB_String RB_Node::parentPortId() const {
    return getValue("parentPortId").toString();
}

/**
 * Get connected component ID
 */
RB_String RB_Node::connectComponentId() const {
    return getValue("connectComponentId").toString();
}

/**
 * Get connected component port number
 */
RB_String RB_Node::connectPortId() const {
    return getValue("connectPortId").toString();
}

/**
 * Get parent node type, what is this node for the parent component
 */
GV2::NodeType RB_Node::parentNodeType() const {
    return (GV2::NodeType)getValue("parentNodeType").toInt();
}

/**
 * Get connected component node type, what is this node for the connecting
 * component
 */
GV2::NodeType RB_Node::connectNodeType() const {
    return (GV2::NodeType)getValue("connectNodeType").toInt();
}

/**
 * Set parent component ID
 */
void RB_Node::setParentComponentId(const RB_String& componentId) {
    setValue("parentComponentId", componentId);
}

/**
 * Set parent component port number
 * @param portId port id or in case of standard port, Port0, Port1, etc.
 */
void RB_Node::setParentPortId(const RB_String& portId) {
    setValue("parentPortId", portId);
}

/**
 * Set connected component ID
 */
void RB_Node::setConnectComponentId(const RB_String& componentId) {
    setValue("connectComponentId", componentId);
}

/**
 * Set connected component port number
 * @param portId port id or in case of standard port, Port0, Port1, etc.
 */
void RB_Node::setConnectPortId(const RB_String& portId) {
    setValue("connectPortId", portId);
}

/**
 * Set parent node type, what this node is for the parent component
 */
void RB_Node::setParentNodeType(GV2::NodeType nodeType) {
    setValue("parentNodeType", (int)nodeType);
}

/**
 * Set connected component node type, what this node is for connected component
 */
void RB_Node::setConnectNodeType(GV2::NodeType nodeType) {
    setValue("connectNodeType", (int)nodeType);
}

/**
 * Create members of this node class
 */
void RB_Node::createMembers() {
    // Location vector coordinates
    addMember(this, "locX", "mm", 0.0, RB2::MemberDouble);			// 0
    addMember(this, "locY", "mm", 0.0, RB2::MemberDouble);			// 1
    addMember(this, "locZ", "mm", 0.0, RB2::MemberDouble);			// 2

    // Direction vector coordinates
    addMember(this, "dirX", "mm", 0.0, RB2::MemberDouble);			// 3
    addMember(this, "dirY", "mm", 0.0, RB2::MemberDouble);			// 4
    addMember(this, "dirZ", "mm", 0.0, RB2::MemberDouble);			// 5

    // Orientation vector coordinates
    addMember(this, "oriAngle", "degree", 0.0, RB2::MemberDouble);	// 6

    // Component id and port id of connecting components
    addMember(this, "parentComponentId", "-", "",RB2::MemberChar40);	// 7
    addMember(this, "parentPortId", "-", "",RB2::MemberChar40);		// 8
    addMember(this, "connectComponentId", "-", "",RB2::MemberChar40);	// 9
    addMember(this, "connectPortId", "-", "",RB2::MemberChar40);		// 10

    // Type of this node for the parent of connecting port:
    // - component origine port,
    // - component local,
    // - model global
    // not component type!
    addMember(this, "parentNodeType", "-", 0, RB2::MemberInteger);	// 11
    addMember(this, "connectNodeType", "-", 0, RB2::MemberInteger);	// 12

}
