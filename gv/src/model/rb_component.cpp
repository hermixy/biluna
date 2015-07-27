/*****************************************************************
 * $Id: rb_component.cpp 1452 2011-09-10 08:19:50Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna EQL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_component.h"

#include "rb_objectfactory.h"


RB_Component::RB_Component(const RB_String& id, RB_ObjectBase* p,
                           const RB_String& n, RB_ObjectFactory* f)
    : RB_ObjectContainer(id, p, n, f) {
	
    /*  for derived classes such as EQL_Cylinder and EQL_Elbow
     createMembers();
     */

    numberOfStandardPorts = 0;
    visibility = GV2::CompSolid;
}


RB_Component::RB_Component(RB_Component* component) : RB_ObjectContainer(component) {
//	createMembers();
//	*this = *component;
}

RB_Component::~RB_Component() {
    // RB_DEBUG->print("RB_Component::~RB_Component()");
    // clean up of children done in RB_ObjectBase and RB_ObjectContainer
}


/**
 * Set number of standard ports. This number needs to be set at creation of 
 * object otherwise the custom ports will not work properly
 */
void RB_Component::setStandardPortCount(int nosp) {
    numberOfStandardPorts = nosp;
}


/**
 * @return the number of standard ports of this component
 */
int RB_Component::standardPortCount() {
    return numberOfStandardPorts;
}


/**
 * @return the component total number of standard ports plus the custom ports 
 */
int RB_Component::portCount() {
    RB_ObjectContainer* objC = getContainer("RB_PortList");
    int customPortCount = 0;

    if (objC) {
        customPortCount = objC->countObject();
    }

    return standardPortCount() + customPortCount;
}


/**
 * Add new custom port to component and set port data, for example a nozzle 
 * to a shell
 * @param p port data
 */
RB_Port* RB_Component::addPort(const RB_Port& p) {
    RB_ObjectContainer* objC = getContainer("RB_PortList");
    RB_Port* newP = (RB_Port*)objC->newObject();
    *newP = p;
    return newP;
}


/**
 * Remove custom port
 * @param portNumber port number to be removed
 */
void RB_Component::removePort(const RB_String& portId) {
	if (portId.startsWith("Port")) {
            RB_DEBUG->print("RB_Component::removePort() standard PortX ERROR");
            return;
	}
		
	RB_ObjectContainer* objC = getContainer("RB_PortList");

	if (!objC) {
            RB_DEBUG->print("RB_Component::setPort() no port list ERROR");
            return;
	}
		
	RB_Port* port = (RB_Port*)objC->getObject(portId); 
	objC->remove(port, true); 
}


/**
 * Remove custom port
 * @param p port (object) to be removed
 */
void RB_Component::removePort(RB_Port* p) {
	RB_ObjectContainer* objC = getContainer("RB_PortList");

	if (!objC) {
            RB_DEBUG->print("RB_Component::setPort() no port list ERROR");
            return;
	}
		
	objC->remove(p, true); 
}


/**
 * Set port data of existing custom port, for example a nozzle to a shell
 * @param p port data
 */
void RB_Component::setPort(const RB_String& portId, const RB_Port& p) {
	if (portId.startsWith("Port")) {
            RB_DEBUG->print("RB_Component::setPort() standard PortX ERROR");
            return;
	}
		
	RB_ObjectContainer* objC = getContainer("RB_PortList");

	if (!objC) {
            RB_DEBUG->print("RB_Component::setPort() no port list ERROR");
            return;
	}
		
	RB_Port* port = (RB_Port*)objC->getObject(portId); 

	// if port number does not exist do nothing
	if (!port) return;

	*port = p; 
}


/**
 * Get custom port data. Custom ports get an Uuid as ID, the stardard ports are
 * indicated by Port0, Port1, Port2 and should be captured by the actual
 * component (for example EQL_Cylinder) class
 * @param portId id of the port
 * @param p to contain port data
 */
void RB_Component::port(const RB_String& portId, RB_Port& p) {
    // check if standard port
    if (!portId.startsWith("Port")) {
        RB_ObjectContainer* objC = getContainer("RB_PortList");

        if (!objC) {
            RB_DEBUG->print("RB_Component::port(RB_String, RB_Port)"
                            " no port list ERROR");
            return;
        }

        RB_Port* port = (RB_Port*)objC->getObject(portId);

        if (!port) {
            RB_DEBUG->print("RB_Component::port() port does not exist ERROR");
        } else {
            p = *port;
        }
    }
}


/**
 * Get custom port data. Custom ports get an Uuid as ID, the stardard 
 * ports are indicated by Port0, Port1, Port2 and should be captured by the 
 * actual component (for example EQL_Cylinder) class
 * @param portNumber number of the custom port starting with 0
 * @param p to contain port data
 */
void RB_Component::port(int portNumber, RB_Port& p) {
    RB_ObjectContainer* objC = getContainer("RB_PortList");

    if (!objC) {
        RB_DEBUG->print("RB_Component::port(RB_String, RB_Port)"
                        " no port list ERROR");
        return;
    }

    RB_Port* port = (RB_Port*)objC->getObject(portNumber);

    if (!port) {
        RB_DEBUG->print("RB_Component::port() port does not exist ERROR");
    } else {
        p = *port;
    }
}


/**
 * Set the connecting node when connecting 'portId' to another component port
 * @param parentNode parent node that determines the position of this component
 * @param portId port id of this parent component to connect to other 
 * new component
 * @param currentNode current node to be set to global vectors of portId
 * @param updateCustomPort true for the update of custom ports if dimensions
 * are changed
 */
void RB_Component::setNode(const RB_Node& parentNode, const RB_String& portId, 
							RB_Node* currentNode) {
    RVector localLoc(0.0, 0.0, 0.0);
    RVector localDir(0.0, 0.0, 0.0);
    double localOrient = 0.0;

    if (portId.startsWith("Port")) {
        RB_Port p("", NULL, "RB_Port", NULL);
        this->port(portId, p);
        localLoc = p.location();
        localDir = p.direction();
        localOrient = currentNode->orientation();
    } else {
        RB_DEBUG->print("RB_Component::setNode() ERROR");
        return;
    }

    // continue here with matrix transformations to set node,
    // rotation around z, y and translation
    RMatrix* m = new RMatrix;
    double mat[4][4];
    m->identity(mat);
    double orient = M_PI/180 * parentNode.orientation();
    m->generate(parentNode.location(), parentNode.direction(), orient, mat);

    // if this component not connected with Port0 to positioning node
    RB_String pPortId = parentNode.getValue("connectPort").toString();

    if (pPortId.startsWith("Port")) {
        // connected to standard port

        if (pPortId != "Port0") {
            RB_Port pPort("", NULL, "RB_Port", NULL);
            this->port(pPortId, pPort);
            localLoc = pPort.location();
            localDir = pPort.direction();

            /**
             * localLoc and localDir is now also used to give the coordinates
             * of the port that is connected to the positioning node of this
             * component. After updatePort the localLoc and localDir will
             * contain the position of the port (for the new node) to which the
             * new component will be connected
             */
            updatePort(portId, localLoc, localDir);
        }
    }

    RVector globalLoc;
    RVector globalDir;
    m->location(&localLoc, mat, &globalLoc);
    m->direction(&localDir, mat, &globalDir);
    delete m;

    currentNode->setLocation(globalLoc);
    currentNode->setDirection(globalDir);
    currentNode->setOrientation(localOrient);
}   


/**
 * Update this component port positions for setting node when the component 
 * is not connected with its origine Port0 to the parent positioning node
 * @param portId the parent component port id to connect the component
 * @param port relevant port data to portNumber
 * @param localLoc temporarily contains the location of the current 'origine' 
 * port (which is not Port0) that needs to be updated to a node position 
 * for the new component 
 * @param localDir temporarily contains the direction of the current 'origine' 
 * port (which is not Port0) that needs to be updated to a node position for 
 * the new component
 */
void RB_Component::updatePort(const RB_String& portId,
                              RVector& localLoc, RVector& localDir) {
    // localLoc and localDir contain here temporarily the position of the port
    // that is connected to positioning node but will be updated to contain
    // the translated position of the port to connect the new component to.
    RVector orgLoc(localLoc);
    RVector orgDir(localDir);

    // compensate for the Port0 direction (-1, 0, 0)
    orgDir *= -1;

    // create temp origine
    RB_Port p("", NULL, "RB_Port", NULL);
    port(portId, p);
    RVector tmpLoc(p.location());
    RVector tmpDir(p.direction());

    // create transformation to port
    RMatrix* m = new RMatrix;
    double mat1[4][4];
    m->identity(mat1);
    double orient = 0.0; // M_PI/180 * this->orientation();
    m->generate(orgLoc, orgDir, orient, mat1);

    // invert transformation
    double mat[4][4];
    m->inverse(mat1, mat);

    // transform temporary origine to new position
    m->location(&tmpLoc, mat, &localLoc);
    m->direction(&tmpDir, mat, &localDir);
    delete m;
}

/**
 * Get the visibility flag of this component, visible, wire frame, transparent
 * or hidden.
 */
GV2::CompVisibility RB_Component::getVisibility() {
    return visibility;
}

/**
 * Set the visibility flag of this component, visible, wire frame, transparent
 * or hidden.
 */
void RB_Component::setVisibility(GV2::CompVisibility vis) {
    visibility = vis;
}

