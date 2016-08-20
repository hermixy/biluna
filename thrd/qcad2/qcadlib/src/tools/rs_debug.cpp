/****************************************************************************
** $Id: rs_debug.cpp 8994 2008-02-11 18:22:42Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#include "rs_debug.h"

#include <stdio.h>
#include <stdarg.h>

#include "rs_arc.h"
#include "rs_block.h"
#include "rs_datetime.h"
#include "rs_entity.h"
#include "rs_hatch.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_leader.h"
#include "rs_line.h"
#include "rs_polyline.h"
#include "rs_string.h"
#include "rs_solid.h"
#include "rs_system.h"

RS_Debug* RS_Debug::uniqueInstance = NULL;


/**
 *  Gets the one and only RS_Debug instance
 *  (creates a new one on first call only)
 *
 *  @return Pointer to the single instance of this
 * singleton class
 */
RS_Debug* RS_Debug::instance() {
    if(uniqueInstance==NULL) {
        //RS_DateTime now = RS_DateTime::currentDateTime();
        //RS_String nowStr;
        //nowStr = now.toString("yyyyMMdd_hhmmss");
        //RS_String fName = RS_String("debug_%1.log")
        //    .arg(nowStr);
        // very unreliable like everything else under windows 
        //RS_String fName = RS_String("C:\\temp\\rwbe.log");

        uniqueInstance = new RS_Debug;
        //uniqueInstance->stream = fopen((const char*)fName.toLatin1(), "wt");
        uniqueInstance->stream = stderr;
    }
    return uniqueInstance;
}


/**
 * Deletes the one and only RS_Debug instance.
 */
void RS_Debug::deleteInstance() {
    if (uniqueInstance!=NULL) {
        fclose(uniqueInstance->stream);
        delete uniqueInstance;
        uniqueInstance = NULL;
    }
}


/**
 * Constructor for a point with default coordinates.
 */
RS_Debug::RS_Debug() {
    debugLevel = D_DEBUGGING;
}

/**
 * Sets the debugging level.
 */
void RS_Debug::setLevel(RS_DebugLevel level) {
    debugLevel = level;
    print("RS_DEBUG: Warnings", D_WARNING);
    print("RS_DEBUG: Errors", D_ERROR);
    print("RS_DEBUG: Notice", D_NOTICE);
    print("RS_DEBUG: Informational", D_INFORMATIONAL);
    print("RS_DEBUG: Testing", D_TESTING);
    print("RS_DEBUG: Debugging", D_DEBUGGING);
}


/**
 * Gets the current debugging level.
 */
RS_Debug::RS_DebugLevel RS_Debug::getLevel() {
    return debugLevel;
}



/**
 * Prints a time stamp in the format yyyyMMdd_hhmmss.
 */
void RS_Debug::timestamp(const RS_String& text) {
    RS_DateTime now = RS_DateTime::currentDateTime();
    RS_String nowStr;

#if QT_VERSION>=0x030000
        nowStr = now.toString("yyyyMMdd_hh:mm:ss:zzz ");
#else
        nowStr = now.toString();
#endif
    print(RS_String("%1: %2").arg(text).arg(nowStr));
    //fprintf(stream, (const char*)nowStr.toLatin1());
    //fprintf(stream, "\n");
    //fflush(stream);
}


/**
 * Prints the unicode for every character in the given string.
 */
void RS_Debug::printUnicode(const RS_String& text) {
    for (int i=0; i<(int)text.length(); i++) {
        print("[%000X] %c", text.at(i).unicode(), text.at(i).toLatin1());
    }
}


/**
 * Prints some identifying info about the given entity.
 */
void RS_Debug::printEntity(RS_DebugLevel level, RS_Entity* e, 
    const RS_String& prepend) {
    
    if (debugLevel>=level) {
        if (e==NULL) {
            print(level, "%sEntity: NULL", (const char*)prepend.toLatin1());
            return;
        }
        else {
            RS_Layer* layer = e->getLayer(false);
            RS_String layerName = (layer==NULL ? "NULL" : layer->getName());
            bool layerVisible = true;
            if (e->getLayer(false)!=NULL) {
                layerVisible = !e->getLayer(false)->isFrozen();
            }
            bool blockVisible = true;
            if (e->getBlock()!=NULL) {
                blockVisible = !e->getBlock()->isFrozen();
            }
            bool insertVisible = true;
            if (e->getInsert()!=NULL) {
                insertVisible = e->getInsert()->isVisible();
            }
            print(level, 
                "\n%sENTITY: id: %ld, parent id: %ld, address: %d, parent address: %d"
                "\n%sundone: %d, visibleFlag: %d, visible: %d, selected: %d,"
                "\n%slayer: %s, layer visible: %d, block visible: %d, "
                "insert visible: %d"
                "\n%sborders: %f/%f - %f/%f"
                "\n%supdate enabled: %d",
                (const char*)prepend.toLatin1(),
                e->getId(), 
                e->getParentContainer() ? e->getParentContainer()->getId() : 0,
                (int)e, (int)e->getParentContainer(),
                (const char*)prepend.toLatin1(),
                e->isUndone(), e->getFlag(RS2::FlagVisible), e->isVisible(), 
                e->isSelected(), 
                (const char*)prepend.toLatin1(),
                (const char*)layerName.toLatin1(),
                (int)layerVisible, (int)blockVisible, (int)insertVisible,
                (const char*)prepend.toLatin1(),
                e->getMin().x, e->getMin().y, e->getMax().x, e->getMax().y,
                (const char*)prepend.toLatin1(),
                (int)e->isUpdateEnabled());
                
            RS_Pen pen = e->getPen(false);
            if (!pen.isValid()) {
                print(level, "%spen: INVALID", (const char*)prepend.toLatin1());
            }
            else {
                print(level, "%spen:", (const char*)prepend.toLatin1());
                if (pen.getColor().isByLayer()) {
                    print(level, "%scolor: BYLAYER", (const char*)prepend.toLatin1());
                }
                else {
                    print(level, "%scolor: %s, alpha: %d", 
                        (const char*)prepend.toLatin1(),
                        (const char*)pen.getColor().name().toLatin1(),
                        pen.getColor().alpha());
                }
            }

            RS_StringList propertyNames = e->getPropertyNames(true);
            for (int i=0; i<propertyNames.count(); ++i) {
                print(level, "%sPROPERTY: %s: '%s'", 
                    (const char*)prepend.toLatin1(),
                    (const char*)propertyNames[i].toLatin1(), 
                    (const char*)e->getProperty(propertyNames[i]).toString().toLatin1());
            }
#ifdef RS_CAM
            RS_String* contBeg = e->getUserDefVar("ContBeg");
            RS_String* contEnd = e->getUserDefVar("ContEnd");
            RS_String* conv = e->getUserDefVar("Conv");
            RS_String* contour = e->getUserDefVar("Contour");
            int contNum = -1;
            if (contour!=NULL) {
                contNum = contour->toInt();
            }
            
            print(level, "%sCAM DATA: contour begin: %c, contour end: %c, converted: %c, contour: %d", 
                (const char*)prepend.toLatin1(),
                (contBeg==NULL ? '-' : 'Y'),
                (contEnd==NULL ? '-' : 'Y'),
                (conv==NULL ? '-' : 'Y'),
                contNum);
#endif
        }
        switch (e->rtti()) {
        case RS2::EntityGraphic: {
            RS_Graphic* g = dynamic_cast<RS_Graphic*>(e);
            print(level, "%sGRAPHIC: %d entities, filename: '%s'",
                (const char*)prepend.toLatin1(),
                g->count(),
                (const char*)g->getFilename().toLatin1());
            } break;

        case RS2::EntityPoint: {
            RS_Point* l = dynamic_cast<RS_Point*>(e);
            print(level, "%sPOINT: %f,%f",
                (const char*)prepend.toLatin1(),
                l->getPos().x, l->getPos().y);
            } break;
            
        case RS2::EntityLine: {
            RS_Line* l = dynamic_cast<RS_Line*>(e);
            print(level, "%sLINE: from %f,%f,%f to %f,%f,%f",
                (const char*)prepend.toLatin1(),
                l->getStartpoint().x, l->getStartpoint().y, l->getStartpoint().z,
                l->getEndpoint().x, l->getEndpoint().y, l->getEndpoint().z);
            } break;
        
        case RS2::EntityArc: {
            RS_Arc* a = dynamic_cast<RS_Arc*>(e);
            print(level, "%sARC: from %f,%f to %f,%f C:%f,%f R:%f A1:%f A2:%f REV:%d",
                (const char*)prepend.toLatin1(),
                a->getStartpoint().x, a->getStartpoint().y,
                a->getEndpoint().x, a->getEndpoint().y,
                a->getCenter().x, a->getCenter().y,
                a->getRadius(),
                a->getAngle1()/M_PI*180.0, a->getAngle2()/M_PI*180.0, 
                (int)a->isReversed());
            } break;
        
        case RS2::EntityCircle: {
            RS_Circle* c = dynamic_cast<RS_Circle*>(e);
            print(level, "%sCIRCLE: C:%f,%f R:%f",
                (const char*)prepend.toLatin1(),
                c->getCenter().x, c->getCenter().y,
                c->getRadius());
            } break;
        
        case RS2::EntityDimLeader: {
            //RS_Leader* l = dynamic_cast<RS_Leader*>(e);
            print(level, "%sLEADER",
                (const char*)prepend.toLatin1());
            } break;
            
        case RS2::EntityContainer: {
            RS_EntityContainer* c = dynamic_cast<RS_EntityContainer*>(e);
            print(level, "%sENTITYCONTAINER: %d entities",
                (const char*)prepend.toLatin1(),
                c->count());
            } break;
        
        case RS2::EntityBlock: {
            RS_EntityContainer* c = dynamic_cast<RS_EntityContainer*>(e);
            print(level, "%sBLOCK: %d entities",
                (const char*)prepend.toLatin1(),
                c->count());
            } break;
        
        case RS2::EntityInsert: {
            RS_EntityContainer* c = dynamic_cast<RS_EntityContainer*>(e);
            print(level, "%sINSERT: %d entities",
                (const char*)prepend.toLatin1(),
                c->count());
            } break;
        
        case RS2::EntityPolyline: {
            RS_Polyline* c = dynamic_cast<RS_Polyline*>(e);
            print(level, "%sPOLYLINE: %d entities, valid: %d, closed: %d",
                (const char*)prepend.toLatin1(),
                c->count(),
                (int)c->validate(),
                (int)c->isClosed());
            } break;
        
        case RS2::EntityHatch: {
            RS_Hatch* h = dynamic_cast<RS_Hatch*>(e);
            print(level, "%sHATCH: solid: %d",
                (const char*)prepend.toLatin1(),
                h->isSolid());
            } break;
        
        case RS2::EntitySolid: {
            RS_Solid* s = dynamic_cast<RS_Solid*>(e);
            print(level, "%sSOLID: %f,%f,%f - %f,%f,%f - %f,%f,%f - %f,%f,%f",
                (const char*)prepend.toLatin1(),
                s->getCorner(0).x, s->getCorner(0).y, s->getCorner(0).z, 
                s->getCorner(1).x, s->getCorner(1).y, s->getCorner(1).z, 
                s->getCorner(2).x, s->getCorner(2).y, s->getCorner(2).z, 
                s->getCorner(3).x, s->getCorner(3).y, s->getCorner(3).z
            );
            } break;

        default:
            print(level, "%sUNKNOWN", (const char*)prepend.toLatin1());
            break;
        }

        if (e->isOfType(RS2::EntityContainer)) {
            RS_EntityContainer* c = dynamic_cast<RS_EntityContainer*>(e);
            bool ok;
            RS2::ContourOrientation contourOrientation = c->getContourOrientation(&ok);
            if (ok) {
                if (contourOrientation==RS2::CounterClockWise) {
                    print(level, "%sClosed contour with orientation: CCW", (const char*)prepend.toLatin1());
                }
                else {
                    print(level, "%sClosed contour with orientation: CW", (const char*)prepend.toLatin1());
                }
            }
            else {
                print(level, "%sIs not one closed contour", (const char*)prepend.toLatin1());
            }
            for (RS_Entity* se = c->firstEntity(); se!=NULL; 
                se = c->nextEntity()) {

                if (se->isSelected()) {
                    printEntity(level, se, QString("%1####").arg(prepend));
                }
                else {
                    printEntity(level, se, QString("%1    ").arg(prepend));
                }
            }
        }
    }
}
