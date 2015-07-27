/*****************************************************************
 * $Id: gv_dwgscenesync.h 1245 2010-07-14 20:08:00Z rutger $
 * Created: Feb 11, 2009 7:01:35 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_DWGSCENESYNC_H
#define GV_DWGSCENESYNC_H

#include "gv_drawing.h"

/**
 * Utility class for the synchronization of model to scene data and vice versa.
 * 
 * When an XML file is opened the model will be completed with data, but for 
 * the correct working of the GV application the nodes, ports and items need
 * to be connected first. The exchange of pointers is done in this class. For 
 * the saving of an XML the model needs to be updated with the graphicsitem data
 * such as the location, direction, scale and length of a line 
 */
class GV_DwgSceneSync {
	
public:
    GV_DwgSceneSync();
	virtual ~GV_DwgSceneSync();
	
    virtual void synchronizeModelData(RB_ObjectBase* obj);
    virtual void synchronizeSceneData(GV_Drawing* dwg, RB_ObjectBase* obj = NULL);

private:
    void synchronizeSceneList(const QString& listName, RB_ObjectBase* obj);
    void synchronizeSceneNodeData(GV_Node* node, RB_ObjectBase* obj);
//    void synchronizeSceneItemData(GV_Node* node);
    bool matchNodeComponent(GV_Node* node, const QString& compListName, RB_ObjectBase* obj);
    void connectPortNode(GV_Node* node, GV_Port* connectPort, GV_Port* parentPort);

    GV_Drawing* mDrawing;
    GV_Port* mConnectPort;
    GV_Port* mParentPort;
    RB_String mContextPrefix;
};

#endif /*GV_DWGSCENESYNC_H*/
