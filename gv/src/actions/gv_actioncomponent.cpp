/*****************************************************************
 * $Id: gv_actioncomponent.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Apr 25, 2008 4:20:15 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actioncomponent.h"

#include "gv_coordinateevent.h"
#include "gv_drawing.h"
#include "gv_equipment.h"
#include "gv_graphicsscene.h"
#include "gv_modelfactory.h"
//#include "gv_pipeline.h"
#include "gv_port.h"
#include "gv_resizehandle.h"
#include "rb_objectfactory.h"


GV_ActionComponent::GV_ActionComponent(GV_Drawing& dwg, GV_GraphicsView& gv)
                                    : GV_Snapper(dwg, gv) {
    RB_DEBUG->print("GV_ActionComponent::~GV_ActionComponent()");

    mComponent = NULL;
    mPort = NULL;
    mSelectedItem = NULL;
}

GV_ActionComponent::~GV_ActionComponent() {
    // Nothing
    RB_DEBUG->print("GV_ActionComponent::~GV_ActionComponent() OK");
}

/**
 * Set selected component, this will at the same time select the
 * corresponding pipe line or equipment. If a port is selected than the
 * connecting node is also set.
 * @param selected component
 */
void GV_ActionComponent::setSelectedComponent(GV_Component* comp) {
    RB_DEBUG->print("GV_ActionComponent::setSelectedComponent()");

    if (!comp) return;
    mSelectedItem = comp->getGraphicsItem();
    mComponent = comp;
}

/**
 * Select an item from the scene, this will at the same time select the
 * relevant pipe line or equipment. If a port is selected than the
 * connecting node is also set.
 * @param pos scene mouse point position
 * @return selected item
 */
QGraphicsItem* GV_ActionComponent::setSelectedItem(const QPointF& pos) {
    RB_DEBUG->print("GV_ActionComponent::setSelectedItem()");

    clearSelection();
    mSelectedItem = mDrawing->itemAt(pos, QTransform());

    if (mSelectedItem) {
        mPort = dynamic_cast<GV_Port*>(mSelectedItem);
        mResHandle = dynamic_cast<GV_ResizeHandle*>(mSelectedItem);

        if (mPort || mResHandle) {
            mComponent = dynamic_cast<GV_Component*>(mSelectedItem->parentItem());
        } else {
            mComponent = dynamic_cast<GV_Component*>(mSelectedItem);
        }

    }

    return mSelectedItem;
}

/**
 * Clear selection in this action only. Current line, symbol, text, port and
 * resizehandle will be NULL.
 */
void GV_ActionComponent::clearSelection() {
    mComponent = NULL;
    mPort = NULL;
    mResHandle = NULL;
}

/**
 * Get selected component (symbol or line) of current drawing
 */
GV_Component* GV_ActionComponent::getSelectedComponent() {
    RB_DEBUG->print("GV_ActionComponent::getSelectedComponent()");

    if (!mComponent) {
        GV_Component* comp = mDrawing->getSelectedComponent();
        if (comp) setSelectedComponent(comp);
    }

    return mComponent;
}

/**
 * Add new node to the drawing
 * @return success
 */
GV_Node* GV_ActionComponent::addNewNode() {
    RB_DEBUG->print("GV_ActionComponent::addNewNode()");

    if (!mDrawing) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_ActionComponent::AddNode() no drawing ERROR");
    }

    RB_ObjectContainer* nodeList = mDrawing->getContainer("GV_NodeList");
    GV_Node* node = NULL;

    if (nodeList) {
        node = dynamic_cast<GV_Node*>(mObjectFactory->newObject("", nodeList, "", true));
        mDrawing->addItem(node);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::addNewNode() no nodeList ERROR");
    }

    return node;
}

/**
 * Remove node
 * @return success
 */
bool GV_ActionComponent::removeNode(GV_Node* node) {
    RB_DEBUG->print("GV_ActionComponent::removeNode()");

    if (!node || !mDrawing) return false;

    RB_ObjectContainer* nodeList = dynamic_cast<RB_ObjectContainer*>(node->getParent());

    if (!nodeList) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::removeNode() no nodeList ERROR");
        return false;
    }

    node->disconnect();
    mDrawing->removeItem(node);
    (dynamic_cast<RB_Node*>(node))->setFlag(RB2::FlagIsDeleted);

    return true;
}

/**
 * Get current node
 */
//GV_Node* GV_ActionComponent::getNode() {
//	RB_DEBUG->print("GV_ActionComponent::getNode()");
//	return mNode;
//}

/**
 * Set current node
 */
//void GV_ActionComponent::setNode(GV_Node* node) {
//	RB_DEBUG->print("GV_ActionComponent::setNode()");
//	mNode = node;
//}

/**
 * Add new symbol to current pipe line. If legendSymbol is not NULL copy
 * properties to newly added symbol and create origine node
 * @param legendSymbol symbol from legend sheet
 * @return success
 */
GV_Symbol* GV_ActionComponent::addNewSymbol(GV_Symbol* legendSymbol) {
    RB_DEBUG->print("GV_ActionComponent::addNewSymbol()");


    // GV implementation, should be generic
    //

    // Use legendSymbol only as type of component the container is not a parent
    RB_ObjectContainer* symbolList = getSymbolList(legendSymbol);
    if (!symbolList) return NULL;

    GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(mObjectFactory->newObject("", symbolList, "", true));
    symbol->createPorts();
    symbol->init();


    if (legendSymbol) {
        symbol->setPath(legendSymbol->path()); // set format
    } else {
        // Basic rectangle symbol
        QPainterPath path;
        path.moveTo(0.0, 0.0);
        path.lineTo(0.0, -15.0);
        path.lineTo(50.0, -15.0);
        path.lineTo(50.0, 15.0);
        path.lineTo(0.0, 15.0);
        path.lineTo(0.0, 0.0);
        // heads
        // path.arcTo(QRectF(-7.0, -15.0, 14.0, 30.0), 90, 180.0);
        path.moveTo(0.0, 15.0);
        path.cubicTo(-15.0, 15.0, -15.0, -15.0, 0.0, -15.0);
        path.moveTo(50.0, 15.0);
        path.cubicTo(65.0, 15.0, 65.0, -15.0, 50.0, -15.0);

        symbol->setPath(path);

        // custom port, base port (Port0) is created default
        GV_Port port;
        port.setDirection(RVector(1.0, 0.0, 0.0));
        port.setLocation(RVector(50.0, 0.0, 0.0));
        symbol->addPort(port);
        port.setDirection(RVector(0.0, -1.0, 0.0));
        port.setLocation(RVector(25.0, -15.0, 0.0));
        symbol->addPort(port);
        port.setDirection(RVector(0.0, 1.0, 0.0));
        port.setLocation(RVector(25.0, 15.0, 0.0));
        symbol->addPort(port);
    }

    // add to scene
    if (symbolList->getParent() == getDrawing()) {
        getDrawing()->addItem(symbol);
    } else {
        GV_Component* comp = dynamic_cast<GV_Component*>(symbolList->getParent());
        symbol->setParentItem(comp->getGraphicsItem());
    }

    GV_Node* node = addNewNode();
    symbol->connectNode(node, NULL); // NULL because origine node
    return symbol;
}

/**
 * Get symbol container, for example used to add new line
 * @param legendSymbol symbol from legendsheet, not used here
 * @return container with symbols
 */
RB_ObjectContainer* GV_ActionComponent::getSymbolList(GV_Component* /* legendSymbol */) {
    if (!getDrawing()) return NULL;
    return getDrawing()->getContainer("GV_SymbolList");
}

/**
 * Get symbol
 */
GV_Symbol* GV_ActionComponent::getSymbol() {
    return dynamic_cast<GV_Symbol*>(mComponent);
}

/**
 * Set symbol
 */
void GV_ActionComponent::setSymbol(GV_Symbol* symbol) {
    mComponent = symbol;
}

/**
 * Add new line to current pipe line
 * @param legendLine line type from legendsheet, not used here
 * @return success
 */
GV_Line* GV_ActionComponent::addNewLine(GV_Line* legendLine) {
    RB_DEBUG->print("GV_ActionComponent::addNewLine()");

    // GV implementation, should be generic
    //
    RB_ObjectContainer* lineList = getLineList(legendLine);
    if (!lineList) return NULL;

    GV_Line* line = dynamic_cast<GV_Line*>(mObjectFactory->newObject("", lineList, "", true));
    line->createPorts();
    line->init();

    if (legendLine) {
        line->setLine(legendLine->line()); // set format
    }

    // add to scene
    if (lineList->getParent() == getDrawing()) {
        getDrawing()->addItem(line);
    } else {
        GV_Component* comp = dynamic_cast<GV_Component*>(lineList->getParent());
        line->setParentItem(comp->getGraphicsItem());
    }

    GV_Node* node = addNewNode();
//    line->connectNode(node, NULL); // NULL because origine node
    node->setPort(line->port0());
    return line;
}

/**
 * Get line container, for example used to add new line
 * @param legendLine line from legendsheet, not used here
 * @return container with lines
 */
RB_ObjectContainer* GV_ActionComponent::getLineList(GV_Component* /* legendLine */) {
    if (!getDrawing()) return NULL;
    return getDrawing()->getContainer("GV_LineList");
}

/**
 * Get line
 */
GV_Line* GV_ActionComponent::getLine() {
    return dynamic_cast<GV_Line*>(mComponent);
}

/**
 * Set line
 */
void GV_ActionComponent::setLine(GV_Line* line) {
    mComponent = line;
}

GV_Text* GV_ActionComponent::addNewText(GV_Text* legendText) {
    // GV implementation, should be generic
    //
    RB_ObjectContainer* textList = getTextList(legendText);
    if (!textList) return NULL;

    GV_Text* text = dynamic_cast<GV_Text*>(mObjectFactory->newObject("", textList, "", true));
    text->createPorts();
    text->init();

    if (legendText) {
//        text->setText(legendTextLine->text()); // set text or format
    }

    // add to scene
    if (textList->getParent() == getDrawing()) {
        getDrawing()->addItem(text);
    } else {
        GV_Component* comp = dynamic_cast<GV_Component*>(textList->getParent());
        text->setParentItem(comp->getGraphicsItem());
    }

    GV_Node* node = addNewNode();
    text->connectNode(node, NULL); // NULL because origine node
    return text;
}

RB_ObjectContainer* GV_ActionComponent::getTextList(GV_Component* /*legendText*/) {
    if (!getDrawing()) return NULL;
    return getDrawing()->getContainer("GV_TextList");
}

GV_Text* GV_ActionComponent::getText() {
    return dynamic_cast<GV_Text*>(mComponent);
}

void GV_ActionComponent::setText(GV_Text* text) {
    mComponent = text;
}

/**
 * Remove a component
 * @param comp component
 */
bool GV_ActionComponent::removeComponent(GV_Component* comp) {
    RB_DEBUG->print("GV_ActionComponent::removeComponent()");

    if (!comp || !mDrawing) return false;

    RB_ObjectContainer* compList = dynamic_cast<RB_ObjectContainer*>(comp->getParent());

    if (!compList) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::removeComponent() no component list ERROR");
        return false;
    }

    // get origine node first than disconnect line and delete from model/scene
    GV_Node* node = comp->origineNode();
    comp->disconnect(); // component and line also disconnected
    removeNode(node);

    mDrawing->removeItem(comp->getGraphicsItem());
    (dynamic_cast<GV_Component*>(comp))->setFlag(RB2::FlagIsDeleted);

    // set ports as deleted
    RB_StringList strL = comp->getName().split("_");
    RB_String contextPrefix = strL.at(0);
    RB_ObjectContainer* portList = comp->getContainer(contextPrefix + "_PortList");
    RB_ObjectIterator* iter = portList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        obj->setFlag(RB2::FlagIsDeleted);
        GV_Port* port = dynamic_cast<GV_Port*>(obj);
        mDrawing->removeItem(port);
    }
    delete iter;

    return true;
}

/**
 * Add new port to current line or symbol
 * @return success
 */
GV_Port* GV_ActionComponent::addNewPort() {
    RB_DEBUG->print("GV_ActionComponent::addNewPort()");

    if (!mComponent) return NULL;
    RB_StringList strL = mComponent->getName().split("_");
    RB_String contextPrefix = strL.at(0);

    RB_ObjectContainer* portList = mComponent->getContainer(contextPrefix + "_PortList");
    if (portList) {
        mPort = dynamic_cast<GV_Port*>
                (mObjectFactory->newObject("", portList, "", true));
        mPort->init();
        portList->addObject(mPort);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::addNewPort() no portList ERROR");
        return NULL;
    }

    return mPort;
}

/**
 * Remove port from current component port list
 * @param port port to removed
 */
bool GV_ActionComponent::removePort(GV_Port* port) {
    RB_DEBUG->print("GV_ActionComponent::removePort()");

    RB_ObjectContainer* portList = dynamic_cast<RB_ObjectContainer*>(port->getParent());

    if (!portList) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::addNewPort() no portList ERROR");
        return false;
    }

    // port will be removed from scene by parent item
    mPort = NULL;
    bool deleteObject = true;
    return portList->remove(port, deleteObject);
}

/**
 * Get port
 */
GV_Port* GV_ActionComponent::getPort() {
    return mPort;
}

/**
 * Set port
 */
void GV_ActionComponent::setPort(GV_Port* port) {
    mPort = port;
}

/**
 * @return current selected resizehandle
 */
GV_ResizeHandle* GV_ActionComponent::getResizeHandle() {
    return mResHandle;
}

/* *
 * Connect a new component to a selected item under the mouse position
 * @param ce mouse event used for the position of the mouse click
 * @itemAdded item added to the scene
 * /
bool GV_ActionComponent::connectComponent(GV_CoordinateEvent* ce,
                                GV_Component* compAdded) {
    RB_DEBUG->print("GV_ActionComponent::connectComponent()");

    if (!mComponent || !compAdded) return false;

    if (mComponent->getName() == "GV_Line" && compAdded->getName() == "GV_Line") {
        return connectLineToLine(ce, (GV_Line*)compAdded, (GV_Line*)mComponent);
    } else if (mComponent->getName() == "GV_Symbol" && compAdded->getName() == "GV_Line") {
        return connectLineToSymbol(ce, (GV_Line*)compAdded, (GV_Symbol*)mComponent);



    } else if (mSelectedItem && compAdded->getName() == "GV_Symbol") {
        return connectSymbolTo(ce, (GV_Symbol*)compAdded, getSelectedComponent());
    }
    return false;
}

/ **
 * Connect line to selected line
 * @param ce mouse event used for the position of the mouse click
 * @param lineAdded line added to the scene
 * @param lineSelected line current selected
 * /
bool GV_ActionComponent::connectLineToLine(GV_CoordinateEvent* ce,
                                GV_Line* lineAdded,
                                GV_Line* lineSelected) {
    RB_DEBUG->print("GV_ActionComponent::connectLineToLine()");

    if (!ce || !lineAdded || !lineSelected  || !mDrawing
                        || lineSelected == lineAdded) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::connectLineToLine() pointer = NULL ERROR");
        return false;
    }

    GV_Node* node = lineAdded->port0()->connectNode();

    // check if closest free port has a node, which should normally
    // be the origineNode. If so place port1 first and then port0

    if (mPort) {
        // port on line is selected
        GV_Port* selPort = mPort;
        GV_Node* selNode = selPort->connectNode();

        if (selNode) {
            // port has already a node

            if (selNode->freeConnectors() > 0) {
                // normal: node has free port slot

                lineAdded->setReverseOrder(true);
                selNode->connect(selPort, lineAdded->port1());
                // could have used lineSelected but this is safer
                GV_Component* pComp = selPort->getParentComponent();
                // call node to update based on pComp = lineSelected, in this
                // case this will only result in the update of the line added
                selNode->updatePosition(pComp);
            } else {
                // less normal: node does not have free port slot,
                // create extra node at Port0 or Port1 if selected,
                // establish tee junction.
                return connectLineToLineCustomPort(ce, lineAdded);
            }
        } else {
            // no node available therefore connection with new node
            node->connect(lineAdded->port0(), selPort);
            GV_Component* pComp = selPort->getParentComponent();
            node->updatePosition(pComp);
        }
    } else {
        // line is selected
        GV_Line* selLine = lineSelected;
        GV_Port* selPort = selLine->getFreePort(
                QPointF(ce->getCoordinate().x, ce->getCoordinate().y));
        if (!selPort) {
            RB_DEBUG->print(RB_Debug::D_NOTICE,
                    "GV_ActionComponent::connectLineToLine() "
                    "  no free port on selected line NOTICE");
            return false;
        }
        GV_Node* selNode = selPort->connectNode();

        if (selNode) {
            lineAdded->setReverseOrder(true);
            selNode->connect(selPort, lineAdded->port1());
            selNode->updatePosition(selLine);
        } else {
            bool result = selLine->connectNode(node, ce);
            if (!result) return false;
        }
    }

    return true;
}

/ **
 * TODO: update similar to above
 * /
bool GV_ActionComponent::connectLineToSymbol(GV_CoordinateEvent* ce,
                                             GV_Line* lineAdded,
                                             GV_Symbol* symbolSelected) {
    RB_DEBUG->print("GV_ActionComponent::connectLineToSymbol()");

    if (!ce || !lineAdded || !symbolSelected  || !mDrawing) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::connectLineToLine() pointer = NULL ERROR");
        return false;
    }

    GV_Node* node = lineAdded->port0()->connectNode();

    if (symbolSelected->getName() == "GV_Symbol") {
        // symbol is selected

        GV_Symbol* selSymbol = dynamic_cast<GV_Symbol*>(symbolSelected);
        GV_Port* selPort = selSymbol->getFreePort(
                QPointF(ce->getCoordinate().x, ce->getCoordinate().y));
        if (!selPort) {
            RB_DEBUG->print(RB_Debug::D_NOTICE,
                    "GV_ActionComponent::connectLineTo() "
                    "  no free port on selected symbol NOTICE");

            // try to create custom port to component
            // ... TODO
            return false;
        }
        GV_Node* selNode = selPort->connectNode();

        if (selNode) {
            lineAdded->setReverseOrder(true);
            selNode->connect(selPort, lineAdded->port1());
            selNode->updatePosition(selSymbol);
        } else {
            bool result = selSymbol->connectNode(node, ce);
            if (!result) return false;
        }
    }


    return true;
}

/ **
 * Connect symbol to selected item
 * @param ce mouse event used for the position of the mouse click
 * @param symbolAdded symbol added to the scene
 * @param compSelected component current selected, can be line, symbol or port
 * /
bool GV_ActionComponent::connectSymbolTo(GV_CoordinateEvent* ce,
                                GV_Symbol* symbolAdded,
                                GV_Component* compSelected) {
    RB_DEBUG->print("GV_ActionComponent::connectSymbolTo()");

    if (!ce || !symbolAdded || !compSelected || !mDrawing) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_ActionComponent::connectSymbolTo() pointer = NULL ERROR");
        return false;
    }

    GV_Node* node = symbolAdded->port0()->connectNode();

    // if clicked on a component or port to connect to
    // if (compSelected && compSelected->getId() != symbolAdded->getId()) {
    if (compSelected && compSelected != symbolAdded) {
        // if component is selected check if closest free port has a node,
        // which should normally be the origineNode. If so place port1 first
        // and port0
        if (compSelected->getName() == "GV_Symbol") {
            // symbol is selected

            GV_Symbol* selSymbol = dynamic_cast<GV_Symbol*>(compSelected);
            GV_Port* selPort = selSymbol->getFreePort(
                        QPointF(ce->getCoordinate().x, ce->getCoordinate().y));
            if (!selPort) {
                RB_DEBUG->print(RB_Debug::D_NOTICE,
                        "GV_ActionComponent::connectSymbolTo() "
                        "  no free port on selected symbol NOTICE");
                return false;
            }
            GV_Node* selNode = selPort->connectNode();

            if (selNode) {
                symbolAdded->setReverseOrder(true);
                selNode->connect(selPort, symbolAdded->port1());
                selNode->updatePosition(selSymbol);
            } else {
                bool result = selSymbol->connectNode(node, ce);
                if (!result) return false;
            }
        } else if (compSelected->getName() == "GV_Line") {
            // line is selected

            GV_Line* selLine = dynamic_cast<GV_Line*>(compSelected);
            GV_Port* selPort = selLine->getFreePort(
                    QPointF(ce->getCoordinate().x, ce->getCoordinate().y));
            if (!selPort) {
                RB_DEBUG->print(RB_Debug::D_NOTICE,
                        "GV_ActionComponent::connectSymbolTo() "
                        "  no free port on selected line NOTICE");
                return false;
            }
            GV_Node* selNode = selPort->connectNode();

            if (selNode) {
                symbolAdded->setReverseOrder(true);
                selNode->connect(selPort, symbolAdded->port1());
                selNode->updatePosition(selLine);
            } else {
                bool result = selLine->connectNode(node, ce);
                if (!result) return false;
            }
        } else if (compSelected->getName() == "GV_Port") {
            // port is selected

            GV_Port* selPort = dynamic_cast<GV_Port*>(compSelected);
            GV_Node* selNode = selPort->connectNode();

            if (selNode) {
                symbolAdded->setReverseOrder(true);
                selNode->connect(selPort, symbolAdded->port1());
                GV_Component* pComp = selPort->getParentComponent();

                if (pComp->getName() == "GV_Symbol") {
                    GV_Symbol* selSymbol = (GV_Symbol*)pComp;
                    selNode->updatePosition(selSymbol);
                } else if (pComp->getName() == "GV_Line") {
                    GV_Line* selLine = (GV_Line*)pComp;
                    selNode->updatePosition(selLine);
                }
            } else {
                node->connect(symbolAdded->port0(), selPort);
                GV_Component* pComp = selPort->getParentComponent();

                if (pComp->getName() == "GV_Symbol") {
                    GV_Symbol* selSymbol = (GV_Symbol*)pComp;
                    node->updatePosition(selSymbol);
                } else if (pComp->getName() == "GV_Line") {
                    GV_Line* selLine = (GV_Line*)pComp;
                    node->updatePosition(selLine);
                }
            }
        }
    }

    return true;
}

/ **
 * Connect a line to a new custom port (to be created) of a selected line,
 * the user has selected a port of a line which is already connected to
 * another line. This will result in a junction of lines.
 * @param ce coordinate event
 * @param lineAdded line to be added to drawing
 * @param compSelected component selected to be connected
 * /
bool GV_ActionComponent::connectLineToLineCustomPort(GV_CoordinateEvent* ce,
                                                     GV_Line* lineAdded) {
    RB_DEBUG->print("GV_ActionComponent::connectLineToLineCustomPort()");

    if (!mPort) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_ActionComponent::connectLineToLineCustomPort() "
                        "mPort is NULL ERROR");
        return false;
    }

    GV_Node* node = mPort->connectNode();
    if (node->freeConnectors() > 0) return false;

    GV_Line* parentLine = dynamic_cast<GV_Line*>(node->parentPort()->parentItem());
    GV_Line* connectLine = dynamic_cast<GV_Line*>(node->connectPort()->parentItem());
    if (!parentLine && !connectLine) return false;


    // TODO: find out whether we are placing the first or the second point of the line





    // if first point of new line, always connect to upstream line at Port1
    GV_Port* newPort = parentLine->addCopyPort1();
    newPort->setValue("basePort", "Port1");

    // no node available at new port therefore connection with new node
    GV_Node* newNode = lineAdded->port0()->connectNode();
    if (!newNode) {
        return false;
    }

    newNode->connect(lineAdded->port0(), newPort);
    GV_Component* pComp = newPort->getParentComponent(); // is parentLine
    newNode->updatePosition(pComp);

    return true;
}


/ **
 * Connect two ports and create a node if required
 * @param staticPort existing or static port
 * @param movedPort new port or moved port
 * @return true on success
 * /
bool GV_ActionComponent::connectPorts(GV_Port* staticPort, GV_Port* movedPort) {
    if (!staticPort || !movedPort) return false;

    // Check status of connecting nodes
    GV_Node* staticNode = staticPort->connectNode();
    GV_Node* movedNode = movedPort->connectNode();
    if (staticNode && staticNode->freeConnectors() == 0) {
        return false;
    }
    if (movedNode && movedNode->freeConnectors() == 0) {
        return false;
    }

    // Now we have 0, 1 or 2 nodes
    if (staticNode) {
        if (movedNode) {
            removeNode(movedNode);
        }
        staticNode->setPort(movedPort);
    } else if (movedNode) {
        movedNode->setPort(staticPort);
    } else {
        staticNode = addNewNode();
        staticNode->setPort(movedPort);
        staticNode->setPort(staticPort);
    }
    return true;
}
*/
