/*****************************************************************
 * $Id: gv_actioncomponent.h 1249 2010-07-20 15:18:18Z rutger $
 * Created: Apr 25, 2008 4:20:04 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONCOMPONENT_H
#define GV_ACTIONCOMPONENT_H

#include "gv_snapper.h"

class GV_Component;
class GV_CoordinateEvent;
class GV_Drawing;
class GV_Equipment;
class GV_Line;
class GV_Node;
class GV_PipeLine;
class GV_Port;
class GV_ResizeHandle;
class GV_Symbol;
class GV_Text;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class RB_ObjectContainer;

/**
 * Action class handles line and symbol model actions, such as adding pipe line
 * to a drawing and adding a valve symbol to the active pipe line
 */
class GV_ActionComponent : public GV_Snapper {

public:
    GV_ActionComponent(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionComponent();

    // GV functions
    virtual void setSelectedComponent(GV_Component* comp);
    virtual QGraphicsItem* setSelectedItem(const QPointF& pos);
    virtual void clearSelection();
    virtual GV_Component* getSelectedComponent();

#if 0
    // P&ID functions TODO: move
    virtual GV_PipeLine* addNewPipeLine(const GV_PipeLine& line);
    virtual bool removePipeLine(GV_PipeLine* pipeLine);
    virtual GV_PipeLine* getPipeLine();
    virtual void setPipeLine(GV_PipeLine* pipeLine);
    virtual GV_PipeLine* setPipeLine(QGraphicsItem* itemSelected);
    virtual GV_PipeLine* setPipeLine(GV_Component* comp);

    // P&ID functions TODO: move
    virtual GV_Equipment* addNewEquipment();
    virtual bool removeEquipment(GV_Equipment* eq);
    virtual GV_Equipment* getEquipment();
    virtual void setEquipment(GV_Equipment* eq);
    virtual GV_Equipment* setEquipment(QGraphicsItem* itemSelected);
    virtual GV_Equipment* setEquipment(GV_Component* comp);
#endif

    // GV functions
    virtual GV_Node* addNewNode();
    virtual bool removeNode(GV_Node* node);

    virtual GV_Symbol* addNewSymbol(GV_Symbol* legendSymbol = NULL);
    virtual RB_ObjectContainer* getSymbolList(GV_Component* legendSymbol = NULL);
    virtual GV_Symbol* getSymbol();
    virtual void setSymbol(GV_Symbol* symbol);

    virtual GV_Line* addNewLine(GV_Line* legendLine = NULL);
    virtual RB_ObjectContainer* getLineList(GV_Component* legendLine = NULL);
    virtual GV_Line* getLine();
    virtual void setLine(GV_Line* line);

    virtual GV_Text* addNewText(GV_Text* legendText = NULL);
    virtual RB_ObjectContainer* getTextList(GV_Component* legendText = NULL);
    virtual GV_Text* getText();
    virtual void setText(GV_Text* text);

    virtual bool removeComponent(GV_Component* comp);

    virtual GV_Port* addNewPort();
    virtual bool removePort(GV_Port* port);
    virtual GV_Port* getPort();
    virtual void setPort(GV_Port* port);
    virtual GV_ResizeHandle* getResizeHandle();

//    virtual bool connectComponent(GV_CoordinateEvent* e,
//                        GV_Component* compAdded);
//    virtual bool connectLineToLine(GV_CoordinateEvent* e,
//                        GV_Line* lineAdded, GV_Line* lineSelected);
//    virtual bool connectLineToSymbol(GV_CoordinateEvent* e,
//                        GV_Line* lineAdded, GV_Symbol* symbolSelected);
//
//
//    virtual bool connectSymbolTo(GV_CoordinateEvent* e,
//						GV_Symbol* symbolAdded, GV_Component* compSelected);
//
//
//    virtual bool connectLineToLineCustomPort(GV_CoordinateEvent* e,
//                        GV_Line* lineAdded);


    // new approach
//    virtual bool connectPorts(GV_Port* port1, GV_Port* port2);

protected:
    //! Current drawing from the model
//	GV_Drawing* mDrawing; // already in GV_Snapper
    //! Current equipment, P&ID item TODO: move
//	GV_Equipment* mEquipment;
    //! Current pipe line, P&ID item TODO: move
//	GV_PipeLine* mPipeLine;
    //! Current node
    //	GV_Node* mNode;

//    //! Current symbol
//	GV_Symbol* mSymbol;
//    //! Current line
//	GV_Line* mLine;
//    //! Current text
//    GV_Text* mText;

    //! Current component
    GV_Component* mComponent;
    //! Current port
    GV_Port* mPort;
    //! Current resizehandle
    GV_ResizeHandle* mResHandle;
    //! Selected item
    QGraphicsItem* mSelectedItem;

};

#endif /*GV_ACTIONCOMPONENT_H*/
