/****************************************************************************
** $Id: gv_insert.cpp 9471 2008-03-28 11:12:04Z andrew $
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
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
**********************************************************************/


#include "gv_insert.h"

#include "gv_block.h"
#include "gv_graphic.h"
#include "gv_settings.h"


const char* GV_Insert::propertyName = QT_TRANSLATE_NOOP("QObject", "Block Name");
const char* GV_Insert::propertyInsertionPointX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* GV_Insert::propertyInsertionPointY = QT_TRANSLATE_NOOP("QObject", "Position|Y");
const char* GV_Insert::propertyScaleFactorX = QT_TRANSLATE_NOOP("QObject", "Scale|X");
const char* GV_Insert::propertyScaleFactorY = QT_TRANSLATE_NOOP("QObject", "Scale|Y");
const char* GV_Insert::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");
const char* GV_Insert::propertyCols = QT_TRANSLATE_NOOP("QObject", "Columns");
const char* GV_Insert::propertyRows = QT_TRANSLATE_NOOP("QObject", "Rows");
const char* GV_Insert::propertySpacingX = QT_TRANSLATE_NOOP("QObject", "Spacing|X");
const char* GV_Insert::propertySpacingY = QT_TRANSLATE_NOOP("QObject", "Spacing|Y");



/**
 * @param parentContainer The graphic this block belongs to.
 */
GV_Insert::GV_Insert(GV_EntityContainer* parentContainer,
                     const GV_InsertData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {

    block = NULL;

    if (data.updateMode!=GV2::NoUpdate) {
        update();
        //calculateBorders();
    }
}


/**
 * Destructor.
 */
GV_Insert::~GV_Insert() {
    RB_DEBUG->print("GV_Insert::~GV_Insert");
}


/**
 * Updates the entity buffer of this insert entity. This method
 * needs to be called whenever the block this insert is based on changes.
 */
void GV_Insert::update() {
    
    if (updateEnabled==false) {
        return;
    }

    /*
    RB_DEBUG->print("GV_Insert::update");
    RB_DEBUG->print("GV_Insert::update: name: %s", (const char*)data.name.toLatin1());
    RB_DEBUG->print("GV_Insert::update: insertionPoint: %f/%f", 
        data.insertionPoint.x, data.insertionPoint.y);
    */


    clear();

    GV_Block* blk = getBlockForInsert();
    if (blk==NULL) {
        //return NULL;
        RB_DEBUG->print("GV_Insert::update: Block is NULL");
        return;
    }

    if (isUndone()) {
        RB_DEBUG->print("GV_Insert::update: Insert is in undo list");
        return;
    }

    if (fabs(data.scaleFactor.x)<1.0e-6 || fabs(data.scaleFactor.y)<1.0e-6) {
        RB_DEBUG->print("GV_Insert::update: scale factor is 0");
        return;
    }
    
    GV_Pen tmpPen;

    /*GV_MutableListIterator<GV_Entity> it = createIterator();
    GV_Entity* e;
    while ( (e = it.current()) != NULL ) {
        ++it;*/

//    RB_DEBUG->print("GV_Insert::update: cols: %d, rows: %d",
//        data.cols, data.rows);
//    RB_DEBUG->print("GV_Insert::update: block has %d entities",
//        blk->count());
    
    int max = 0;
    int pCounter = 0;
    if (data.updateMode==GV2::PreviewUpdate) {
        GV_SETTINGS->beginGroup("/Appearance");
        max = GV_SETTINGS->readNumEntry("/MaxPreview", 100);
        GV_SETTINGS->endGroup();
    }
    
    for (GV_Entity* e=blk->firstEntity(); e!=NULL; e=blk->nextEntity()) {
        for (int c=0; c<data.cols; ++c) {
            for (int r=0; r<data.rows; ++r) {
            
                if (data.updateMode==GV2::PreviewUpdate) {
                    if (pCounter>max) {
                        break;
                    }
                    else {
                        pCounter++;
                    }
                }

                if (e->rtti()==GV2::EntityInsert) {
                    RB_DEBUG->print("GV_Insert::update: updating sub-insert");
                    dynamic_cast<GV_Insert*>(e)->update();
                }
                
                GV_Entity* ne = e->clone();
                ne->initId();
                ne->setUpdateEnabled(false);
                ne->setParentContainer(this);
                ne->setVisible(getFlag(GV2::FlagVisible));
                
                // Move:
                if (fabs(data.scaleFactor.x)>1.0e-6 && 
                    fabs(data.scaleFactor.y)>1.0e-6) {
                    ne->move(data.insertionPoint +
                             GV_Vector(data.spacing.x/data.scaleFactor.x*c,
                                       data.spacing.y/data.scaleFactor.y*r));
                }
                else {
                    ne->move(data.insertionPoint);
                }
                // Move because of block base point:
                ne->move(blk->getBasePoint()*-1);
                // Scale:
                ne->scale(data.insertionPoint, data.scaleFactor);
                // Rotate:
                ne->rotate(data.insertionPoint, data.angle);
                // Select:
                ne->setSelected(isSelected());

                // individual entities can be on indiv. layers
                tmpPen = ne->getPen(false);

                // color from block (free floating):
                if (tmpPen.getColor()==GV_Color(GV2::FlagByBlock)) {
                    tmpPen.setColor(getPen().getColor());
                }

                // line width from block (free floating):
                if (tmpPen.getWidth()==GV2::WidthByBlock) {
                    tmpPen.setWidth(getPen().getWidth());
                }

                // line type from block (free floating):
                if (tmpPen.getLineType()==GV2::LineByBlock) {
                    tmpPen.setLineType(getPen().getLineType());
                }

                // now that we've evaluated all flags, let's strip them:
                // TODO: strip all flags (width, line type)
                //tmpPen.setColor(tmpPen.getColor().stripFlags());

                ne->setPen(tmpPen);
                
                ne->setUpdateEnabled(true);
                
                ne->update();

                addEntity(ne);
                
            }
        }
    }
    calculateBorders();

    RB_DEBUG->print("GV_Insert::update: OK");
}



/**
 * @return Pointer to the block associated with this Insert or
 *   NULL if the block couldn't be found. Blocks are requested
 *   from the blockSource if one was supplied and otherwise from 
 *   the closest parent graphic.
 */
GV_Block* GV_Insert::getBlockForInsert() {
    if (block!=NULL) {
        return block;
    }

    GV_BlockList* blkList;

    if (data.blockSource==NULL) {
        if (getGraphic()!=NULL) {
            blkList = getGraphic()->getBlockList();
            RB_DEBUG->print("GV_Insert::getBlockForInsert: "
                "got block list from graphic");
        } else {
            blkList = NULL;
            RB_DEBUG->print(RB_Debug::D_WARNING, 
                "GV_Insert::getBlockForInsert: "
                "cannot find a graphic to get the block list");
        }
    } else {
        blkList = data.blockSource;
        RB_DEBUG->print("GV_Insert::getBlockForInsert: "
           "got block list from explicit block source");
    }

    GV_Block* blk = NULL;
    if (blkList!=NULL) {
        blk = blkList->find(data.name);
    }
    else {
        RB_DEBUG->print(RB_Debug::D_WARNING, 
            "GV_Insert::getBlockForInsert: "
            "block list is NULL");
    }

    block = blk;

    return blk;
}


/**
 * Is this insert visible? (re-implementation from GV_Entity)
 *
 * @return true Only if the entity and the block and the layer it is on 
 * are visible.
 * The Layer might also be NULL. In that case the layer visiblity 
 * is ignored.
 * The Block might also be NULL. In that case the block visiblity 
 * is ignored.
 */
bool GV_Insert::isVisible() {
    GV_Block* blk = getBlockForInsert();
    if (blk!=NULL) {
        if (blk->isFrozen()) {
            return false;
        }
    }

    return GV_Entity::isVisible();
}


GV_VectorList GV_Insert::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret(data.insertionPoint);
    return ret;
}
    


GV_Vector GV_Insert::getNearestRef(const GV_Vector& coord,
                                     double* dist,
                                     GV2::RefMode /*refMode*/) {

    return getRefPoints().getClosest(coord, dist);
}



void GV_Insert::move(const GV_Vector& offset) {
    data.insertionPoint.move(offset);
    update();
}



void GV_Insert::rotate(const GV_Vector& center, double angle) {
    data.insertionPoint.rotate(center, angle);
    data.angle = GV_Math::correctAngle(data.angle+angle);
    update();
}



void GV_Insert::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.insertionPoint.scale(center, factor);
    data.scaleFactor.scale(GV_Vector(0.0, 0.0), factor);
    data.spacing.scale(GV_Vector(0.0, 0.0), factor);
    update();
}



void GV_Insert::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.insertionPoint.mirror(axisPoint1, axisPoint2);
    
    GV_Vector vec;
    vec.setPolar(1.0, data.angle);
    vec.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.angle = vec.angle();

    data.scaleFactor.y*=-1;

    update();
}



void GV_Insert::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    if (ref.distanceTo(data.insertionPoint)<1.0e-4) {
        data.insertionPoint += offset;
    } 
    calculateBorders();
}



QVariant GV_Insert::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_INSERT_NAME) {
        return data.name;
    } else if (name==GV_INSERT_INSERTIONPOINT_X) {
        return data.insertionPoint.x;
    } else if (name==GV_INSERT_INSERTIONPOINT_Y) {
        return data.insertionPoint.y;
    } else if (name==GV_INSERT_SCALEFACTOR_X) {
        return data.scaleFactor.x;
    } else if (name==GV_INSERT_SCALEFACTOR_Y) {
        return data.scaleFactor.y;
    } else if (name==GV_INSERT_ANGLE) {
        return GV_Math::rad2deg(data.angle);
    } else if (name==GV_INSERT_COLS) {
        return data.cols;
    } else if (name==GV_INSERT_ROWS) {
        return data.rows;
    } else if (name==GV_INSERT_SPACING_X) {
        return data.spacing.x;
    } else if (name==GV_INSERT_SPACING_Y) {
        return data.spacing.y;
    } else {
        return GV_EntityContainer::getProperty(name, def);
    }
}



QStringList GV_Insert::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_EntityContainer::getPropertyNames(includeGeometry);
    ret << GV_INSERT_NAME;
    if (includeGeometry) {
        ret << GV_INSERT_INSERTIONPOINT_X;
        ret << GV_INSERT_INSERTIONPOINT_Y;
        ret << GV_INSERT_SCALEFACTOR_X;
        ret << GV_INSERT_SCALEFACTOR_Y;
        ret << GV_INSERT_ANGLE;
        ret << GV_INSERT_COLS;
        ret << GV_INSERT_ROWS;
        ret << GV_INSERT_SPACING_X;
        ret << GV_INSERT_SPACING_Y;
    }
    return ret;
}


void GV_Insert::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_INSERT_NAME) {
        data.name = value.toString();
    } else if (name==GV_INSERT_INSERTIONPOINT_X) {
        data.insertionPoint.x = GV_Math::eval(value.toString());
    } else if (name==GV_INSERT_INSERTIONPOINT_Y) {
        data.insertionPoint.y = GV_Math::eval(value.toString());
    } else if (name==GV_INSERT_SCALEFACTOR_X) {
        data.scaleFactor.x = GV_Math::eval(value.toString());
    } else if (name==GV_INSERT_SCALEFACTOR_Y) {
        data.scaleFactor.y = GV_Math::eval(value.toString());
    } else if (name==GV_INSERT_ANGLE) {
        data.angle = GV_Math::deg2rad(GV_Math::eval(value.toString()));
    } else if (name==GV_INSERT_COLS) {
        data.cols = value.toInt();
    } else if (name==GV_INSERT_ROWS) {
        data.rows = value.toInt();
    } else if (name==GV_INSERT_SPACING_X) {
        data.spacing.x = GV_Math::eval(value.toString());
    } else if (name==GV_INSERT_SPACING_Y) {
        data.spacing.y = GV_Math::eval(value.toString());
    } else {
        GV_EntityContainer::setProperty(name, value);
    }
    update();
}
