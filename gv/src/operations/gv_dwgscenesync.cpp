/*****************************************************************
 * $Id: gv_dwgscenesync.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Feb 11, 2009 7:01:55 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_dwgscenesync.h"
#include "gv_objectfactory.h"
//#include "gv_pipeline.h"


GV_DwgSceneSync::GV_DwgSceneSync() {
    mConnectPort = NULL;
    mParentPort = NULL;
    mContextPrefix = "GV";
}

GV_DwgSceneSync::~GV_DwgSceneSync() {

}

/**
 * Synchronize the Qt graphic item data with the GV object data before saving.
 * The function is re-entrant for iterating thru the model.
 * @param obj drawing and later objects in the drawing
 */
void GV_DwgSceneSync::synchronizeModelData(RB_ObjectBase* obj) {
    RB_DEBUG->print("GV_DwgSceneSync::synchronizeModelData()");

    if (!obj) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                 "GV_DwgSceneSync::synchronizeModelData() object NULL ERROR");
        return;
    }

    // Set the context prefix
    RB_String str = obj->getName(); // For example GV_Symbol or RPT_Symbol
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);

    // write children if children are atomic or container
    if (obj->isContainer()) {
        RB_ObjectContainer* tmpObjc = (RB_ObjectContainer*)obj;
        RB_ObjectIterator* iter = tmpObjc->createIterator();

        for (iter->first(); !iter->isDone(); iter->next()) {
            RB_ObjectBase* tmpObj = iter->currentObject();

            if (tmpObj->getName() == mContextPrefix + "_Line"
                            || tmpObj->getName() == mContextPrefix + "_Symbol"
                            || tmpObj->getName() == mContextPrefix + "_Text") {
                GV_Component* comp = (GV_Component*)tmpObj;
//                if (comp && comp->getFlag(RB2::FlagIsDirty)) {
                    comp->synchronizeObjectData();
//                }
            }

            if (tmpObj->getName() == mContextPrefix + "_Node") {
                GV_Node* node = (GV_Node*)tmpObj;
//                if (node && node->getFlag(RB2::FlagIsDirty)) {
                    node->synchronizeObjectData();
//                }
            }

            // re-enter for children
            this->synchronizeModelData(tmpObj);
        }

        delete iter;
    }
}

/**
 * Synchronize the scene data after for example opening of an XML model file.
 * Used reentrant, for example symbols having again nodes lines etc.
 * @param dwg drawing to be handled
 */
void GV_DwgSceneSync::synchronizeSceneData(GV_Drawing* dwg, RB_ObjectBase* obj) {
    RB_DEBUG->print("GV_DwgSceneSync::synchronizeSceneData()");

    mDrawing = dwg;
    // Set the context prefix
    RB_String str = dwg->getName(); // For example RPT_Report or RPT_Symbol
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);

    if (strL.count() > 0) {
        mContextPrefix = strL.at(0);
    } else {
        mContextPrefix = "GV";
    }

    if (!dwg) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_DwgSceneSync::synchronizeSceneData() drawing NULL ERROR");
        return;
    }

    // component list first to be added to the scene, the ports are added
    // by the components to the scene
    synchronizeSceneList(mContextPrefix + "_LineList", obj);
    synchronizeSceneList(mContextPrefix + "_SymbolList", obj);
    synchronizeSceneList(mContextPrefix + "_TextList", obj);
    synchronizeSceneList(mContextPrefix + "_NodeList",obj);
}

/**
 * Synchronize scene with objects in lists. The node list is used to find
 * the connecting component. Objects in the other lists are just added to the
 * scene.
 * @param listName name of list (GV_NodeList, GV_LineList, GV_SymbolList, GV_TextList)
 */
void GV_DwgSceneSync::synchronizeSceneList(const QString& listName, RB_ObjectBase* obj) {
    RB_ObjectContainer* list = NULL;
    if (!obj) {
        list = mDrawing->getContainer(listName);
    } else {
        list = obj->getContainer(listName);
    }
    if (!list) {
        return;
    }
    RB_ObjectIterator* iter = list->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        if (listName == mContextPrefix + "_NodeList") {
            GV_Node* node = dynamic_cast<GV_Node*>(iter->currentObject());
            synchronizeSceneNodeData(node, obj);
            if (obj) {
                GV_Component* parentComp = dynamic_cast<GV_Component*>(obj);
                QGraphicsItem* parentItem = parentComp->getGraphicsItem();
                node->setParentItem(parentItem);
            } else {
                mDrawing->addItem(node);
            }
        } else {
            GV_Component* comp = dynamic_cast<GV_Component*>(iter->currentObject());
            comp->synchronizeSceneData(); // includes createHandles() createPorts()
            QGraphicsItem* item = comp->getGraphicsItem();
            if (obj) {
                GV_Component* parentComp = dynamic_cast<GV_Component*>(obj);
                QGraphicsItem* parentItem = parentComp->getGraphicsItem();
                item->setParentItem(parentItem);
            } else {
                mDrawing->addItem(item);
            }
            synchronizeSceneData(mDrawing, comp);
        }
    }

    delete iter;
}

/**
 * Synchronization of node data with relevant graphics items
 */
void GV_DwgSceneSync::synchronizeSceneNodeData(GV_Node* node, RB_ObjectBase* obj) {
    RB_DEBUG->print("GV_DwgSceneSync::synchronizeSceneNodeData()");

    mConnectPort = NULL;
    mParentPort = NULL;

    if (!matchNodeComponent(node, mContextPrefix + "_LineList", obj)) {
        if (!matchNodeComponent(node, mContextPrefix + "_SymbolList", obj)) {
            if (!matchNodeComponent(node, mContextPrefix + "_TextList", obj)) {
                RB_DEBUG->print(RB_Debug::D_ERROR,
                                "GV_DwgSceneSync::synchronizeSceneNodeData() "
                                "connecting component not found ERROR");
            }
        }
    }

    connectPortNode(node, mConnectPort, mParentPort);

}

/**
 * Find and connect the node to the connecting components
 * @param node
 * @param compListName component list name:
 *          GV_LineList, GV_SymbolList and GV_TextList.
 * @return true when all connecting components have been found
 */
bool GV_DwgSceneSync::matchNodeComponent(GV_Node* node, const QString& compListName, RB_ObjectBase* obj) {
    bool completed = false;
    RB_ObjectContainer* compList = NULL;
    if (!obj) {
        compList = mDrawing->getContainer(compListName);
    } else {
        compList = obj->getContainer(compListName);
    }
    if (!compList) return completed;
    RB_ObjectIterator* compIter = compList->createIterator();

    for (compIter->first(); !compIter->isDone(); compIter->next()) {
        GV_Component* comp = dynamic_cast<GV_Component*>(compIter->currentObject());
        RB_String id = comp->getId();

        if (id == node->getValue("parentComponentId").toString()) {
            RB_ObjectContainer* portList = comp->getContainer(mContextPrefix + "_PortList");
            RB_ObjectIterator* portIter = portList->createIterator();

            for (portIter->first(); !portIter->isDone() && !mParentPort; portIter->next()) {
                GV_Port* port = (GV_Port*)portIter->currentObject();
                RB_String pId = port->getId();

                if (pId == node->getValue("parentPortId").toString()) {
                    mParentPort = port;
                }
            }

            delete portIter;
        } else if (id == node->getValue("connectComponentId").toString()) {
            // set comp origine node
            comp->setOrigineNode(node);
            RB_ObjectContainer* portList = comp->getContainer(mContextPrefix + "_PortList");
            RB_ObjectIterator* portIter = portList->createIterator();

            for (portIter->first(); !portIter->isDone() && !mConnectPort; portIter->next()) {
                GV_Port* port = (GV_Port*)portIter->currentObject();
                RB_String pId = port->getId();

                if (pId == node->getValue("connectPortId").toString()) {
                    mConnectPort = port;
                }
            }

            delete portIter;
        }

        // stop if both components are already found
        if (mConnectPort && (mParentPort
                || "" == node->getValue("parentPortId").toString())) {
            compIter->setDone();
            completed = true;
        }
    }

    delete compIter;
    return completed;
}

void GV_DwgSceneSync::connectPortNode(GV_Node* node,
                                      GV_Port* connectPort,
                                      GV_Port* parentPort) {
    // Connect objects, does not change the location and direction data
    node->connect(connectPort, parentPort);
    node->setParentItem(NULL); // just to make sure that scene is parent item
    node->synchronizeSceneData(); // set position
    node->setVisible(false);
    if (connectPort) connectPort->setVisible(false);
    if (parentPort) parentPort->setVisible(false);

    // Set graphics item data
    GV_Component* comp = node->connectComponent();
    if (comp) {
        comp->setPos(QPointF(node->getValue("locX").toDouble(),
                             node->getValue("locY").toDouble()));
    }
}
