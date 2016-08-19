/****************************************************************************
** $Id: rs_hatch.cpp 9008 2008-02-12 18:16:26Z andrew $
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


#include "rs_hatch.h"

#include "rs_application.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_information.h"
#include "rs_painter.h"
#include "rs_painterqt.h"
#include "rs_pattern.h"
#include "rs_patternlist.h"
#include "rs_pointarray.h"
#include "rs_valuelist.h"


const char* RS_Hatch::propertySolid = QT_TRANSLATE_NOOP("QObject", "Solid");
const char* RS_Hatch::propertyScale = QT_TRANSLATE_NOOP("QObject", "Pattern|Scale");
const char* RS_Hatch::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Pattern|Angle");
const char* RS_Hatch::propertyPattern = QT_TRANSLATE_NOOP("QObject", "Pattern|Name");



/**
 * Constructor.
 */
RS_Hatch::RS_Hatch(RS_EntityContainer* parentContainer,
                   const RS_HatchData& d)
        : RS_Entity(parentContainer), RS_EntityContainer(parentContainer), data(d) {

    hatchPattern = NULL;
    updateRunning = false;
    needOptimization = true;
}

    
    
void RS_Hatch::initFromVectorList(const RS_VectorList& vectorList) {
    RS_EntityContainer* loop = new RS_EntityContainer(this);
    addEntity(loop);

    for (int i=0; i<vectorList.count()-1; i++) {
        loop->addEntity(
            new RS_Line(loop, 
                RS_LineData(
                    vectorList.at(i),
                    vectorList.at(i+1))));
    }
}



/**
 * Validates the hatch.
 */
bool RS_Hatch::validate() {
    bool ret = true;
    
    // loops:
    for (RS_Entity* l=firstEntity(RS2::ResolveNone);
            l!=NULL;
            l=nextEntity(RS2::ResolveNone)) {

        // allow all entity containers (including polylines)
        if (l->isContainer()) {
            RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);

            ret = loop->optimizeContours();
            if (!ret) {
                RS_DEBUG->print(RS_Debug::D_ERROR, 
                    "RS_Hatch::validate: "
                    "failed to optimize contours");
                break;
            }
        }
        else {
            RS_DEBUG->print(RS_Debug::D_ERROR, 
                "RS_Hatch::validate: "
                "hatch contains entities that are not in loops");
            ret = false;
            break;
        }
    }

    return ret;
}


    
RS_Entity* RS_Hatch::clone() {
    RS_Hatch* t = new RS_Hatch(parentContainer, data);
    //t->updateRunning = true;
    t->initId();
    t->entities.setAutoDelete(entities.autoDelete());
    t->copyPropertiesFrom(this);
    t->entities = entities;
    t->detach();
    t->hatchPattern = NULL;
    t->updateRunning = false;
    t->needOptimization = needOptimization;
    
    return t;
}



RS_VectorList RS_Hatch::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_DEBUG->print("RS_Hatch::getRefPoints");

    RS_VectorList ret;

    RS_Entity* entityCursor = currentEntity();
    for (RS_Entity* l=firstEntity();
        l!=NULL;
        l=nextEntity()) {
    
        if (!l->isContainer()) {
            continue;
        }

        RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);
        if (isSolid() || loop!=hatchPattern) {
            for (RS_Entity* e=loop->firstEntity();
                e!=NULL;
                e = loop->nextEntity()) {

                if (!e->isAtomic()) {
                    continue;
                }

                RS_AtomicEntity* ae = dynamic_cast<RS_AtomicEntity*>(e);
                ret.append(ae->getEndpoint());
            }
        }
    }
    findEntity(entityCursor);
    
    return ret;
}



RS_Vector RS_Hatch::getNearestSelectedRef(const RS_Vector& coord,
        double* dist, RS2::RefMode refMode) {

    return RS_Entity::getNearestSelectedRef(coord, dist, refMode);
}




/**
 * @return Number of loops.
 */
int RS_Hatch::countLoops() {
    if (data.solid) {
        return count();
    } else {
        return count() - 1;
    }
}



/**
 * Recalculates the borders of this hatch.
 */
/*
void RS_Hatch::calculateBorders() {

    //activateContour(true);

    RS_EntityContainer::calculateBorders();

    RS_DEBUG->print("RS_Hatch::calculateBorders: size: %f,%f", 
        getSize().x, getSize().y);

    //activateContour(false);
}
*/



/**
 * Updates the Hatch. Called when the 
 * hatch or it's data, position, alignment, .. changes.
 */
void RS_Hatch::update() {
    RS_DEBUG->print("RS_Hatch::update");
    RS_DEBUG->print("RS_Hatch::update: contour has %d loops", count());

    if (updateRunning) {
        RS_DEBUG->print("RS_Hatch::update: Update running. Abort.");
        return;
    }

    if (updateEnabled==false) {
        RS_DEBUG->print("RS_Hatch::update: Updates disabled. Abort.");
        return;
    }

    if (data.solid==true) {
        calculateBorders();
        RS_DEBUG->print("RS_Hatch::update: Hatch is solid. No Update needed.");
        return;
    }

    RS_DEBUG->print("RS_Hatch::update (not solid)");
    updateRunning = true;

    // delete old hatch pattern:
    if (hatchPattern!=NULL) {
        removeEntity(hatchPattern);
        hatchPattern = NULL;
    }

    if (isUndone()) {
        updateRunning = false;
        return;
    }

    if (!validate()) {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_Hatch::update: invalid contour in hatch found");
        updateRunning = false;
        return;
    }

    // search pattern:
    RS_DEBUG->print("RS_Hatch::update: requesting pattern");
    RS_Pattern* pat = RS_PATTERNLIST->requestPattern(data.pattern);
    if (pat==NULL) {
        updateRunning = false;
        RS_DEBUG->print("RS_Hatch::update: requesting pattern: not found");
        return;
    }
    RS_DEBUG->print("RS_Hatch::update: requesting pattern: OK");

    RS_DEBUG->print("RS_Hatch::update: cloning pattern");
    pat = dynamic_cast<RS_Pattern*>(pat->clone());
    RS_DEBUG->print("RS_Hatch::update: cloning pattern: OK");

    // scale pattern
    RS_DEBUG->print("RS_Hatch::update: scaling pattern: %f", data.scale);
    pat->scale(RS_Vector(0.0,0.0), RS_Vector(data.scale, data.scale));
    RS_DEBUG->print("RS_Hatch::update: calculate pattern borders");
    pat->calculateBorders();
    RS_DEBUG->print("RS_Hatch::update: calculate borders");
    calculateBorders();
    RS_DEBUG->print("RS_Hatch::update: scaling pattern: OK");

    // find out how many pattern-instances we need in x/y:
    int px1, py1, px2, py2;
    double f;
    RS_DEBUG->print("RS_Hatch::update: cloning hatch");
    RS_Hatch* copy = dynamic_cast<RS_Hatch*>(this->clone());
    copy->updateRunning = true;
    RS_DEBUG->print("RS_Hatch::update: cloning hatch: ok");
    copy->rotate(RS_Vector(0.0,0.0), -data.angle);
    copy->calculateBorders();

    // create a pattern over the whole contour.
    RS_Vector pSize = pat->getSize();
    RS_Vector cPos = getMin();
    RS_Vector cSize = getSize();


    RS_DEBUG->print("RS_Hatch::update: pattern size: %f/%f", pSize.x, pSize.y);
    RS_DEBUG->print("RS_Hatch::update: contour size: %f/%f", cSize.x, cSize.y);

    if (cSize.x<1.0e-6 || cSize.y<1.0e-6 ||
            pSize.x<1.0e-6 || pSize.y<1.0e-6 ||
            cSize.x>RS_MAXDOUBLE-1 || cSize.y>RS_MAXDOUBLE-1 ||
            pSize.x>RS_MAXDOUBLE-1 || pSize.y>RS_MAXDOUBLE-1) {
        delete pat;
        delete copy;
        updateRunning = false;
        RS_DEBUG->print("RS_Hatch::update: contour size or pattern size too small");
        return;
    }

    // avoid huge memory consumption:
    else if (cSize.x/pSize.x>100 || cSize.y/pSize.y>100) {
        RS_DEBUG->print("RS_Hatch::update: contour size too large or pattern size too small");
        return;
    }

    f = copy->getMin().x/pat->getSize().x;
    px1 = (int)floor(f);
    f = copy->getMin().y/pat->getSize().y;
    py1 = (int)floor(f);
    f = copy->getMax().x/pat->getSize().x;
    px2 = (int)ceil(f) - 1;
    f = copy->getMax().y/pat->getSize().y;
    py2 = (int)ceil(f) - 1;

    RS_EntityContainer tmp;   // container for untrimmed lines

    // adding array of patterns to tmp:
    RS_DEBUG->print("RS_Hatch::update: creating pattern carpet");

    for (int px=px1; px<=px2; px++) {
        for (int py=py1; py<=py2; py++) {
            for (RS_Entity* e=pat->firstEntity(); e!=NULL;
                    e=pat->nextEntity()) {

                RS_Entity* te = e->clone();
                te->rotate(RS_Vector(0.0,0.0), data.angle);
                RS_Vector v1, v2;
                v1.setPolar(px*pSize.x, data.angle);
                v2.setPolar(py*pSize.y, data.angle+M_PI/2.0);
                te->move(v1+v2);
                tmp.addEntity(te);
            }
        }
    }

    delete pat;
    pat = NULL;
    RS_DEBUG->print("RS_Hatch::update: creating pattern carpet: OK");


    RS_DEBUG->print("RS_Hatch::update: cutting pattern carpet");
    // cut pattern to contour shape:
    RS_EntityContainer tmp2;   // container for small cut lines
    RS_Line* line = NULL;
    RS_Arc* arc = NULL;
    RS_Circle* circle = NULL;
    for (RS_Entity* e=tmp.firstEntity(); e!=NULL;
            e=tmp.nextEntity()) {

        RS_Vector startPoint;
        RS_Vector endPoint;
        RS_Vector center = RS_Vector(false);
        bool reversed;

        if (e->rtti()==RS2::EntityLine) {
            line = dynamic_cast<RS_Line*>(e);
            arc = NULL;
            circle = NULL;
            startPoint = line->getStartpoint();
            endPoint = line->getEndpoint();
            center = RS_Vector(false);
            reversed = false;
        } else if (e->rtti()==RS2::EntityArc) {
            arc = dynamic_cast<RS_Arc*>(e);
            line = NULL;
            circle = NULL;
            startPoint = arc->getStartpoint();
            endPoint = arc->getEndpoint();
            center = arc->getCenter();
            reversed = arc->isReversed();
        } else if (e->rtti()==RS2::EntityCircle) {
            circle = dynamic_cast<RS_Circle*>(e);
            line = NULL;
            arc = NULL;
            startPoint = circle->getCenter()
                         + RS_Vector(circle->getRadius(), 0.0);
            endPoint = startPoint;
            center = circle->getCenter();
            reversed = false;
        } else {
            continue;
        }

        // getting all intersections of this pattern line with the contour:
        RS_ValueList<RS_Vector> is;
        is.append(startPoint);

        for (RS_Entity* loop=firstEntity(); loop!=NULL;
                loop=nextEntity()) {

            if (loop->isContainer()) {
                for (RS_Entity* p=dynamic_cast<RS_EntityContainer*>(loop)->firstEntity();
                        p!=NULL;
                        p=dynamic_cast<RS_EntityContainer*>(loop)->nextEntity()) {

                    RS_VectorList sol =
                        RS_Information::getIntersection(e, p, true);

                    for (int i=0; i<=1; ++i) {
                        if (sol.get(i).valid) {
                            is.append(sol.get(i));
                            RS_DEBUG->print("  pattern line intersection: %f/%f",
                                            sol.get(i).x, sol.get(i).y);
                        }
                    }
                }
            }
        }

        is.append(endPoint);

        // sort the intersection points into is2:
        RS_Vector sp = startPoint;
        double sa = center.angleTo(sp);
        RS_ValueList<RS_Vector> is2;
        bool done;
        double minDist;
        double dist = 0.0;
        RS_Vector* av;
        RS_Vector last = RS_Vector(false);
        do {
            done = true;
            minDist = RS_MAXDOUBLE;
            av = NULL;
            //for (RS_Vector* v = is.first(); v!=NULL; v = is.next()) {
            for (int i1=0; i1<is.size(); ++i1) {
                RS_Vector* v = &is[i1];
                if (line!=NULL) {
                    dist = sp.distanceTo(*v);
                } else if (arc!=NULL || circle!=NULL) {
                    double a = center.angleTo(*v);
                    if (reversed) {
                        if (a>sa) {
                            a-=2*M_PI;
                        }
                        dist = sa-a;
                    } else {
                        if (a<sa) {
                            a+=2*M_PI;
                        }
                        dist = a-sa;
                    }
                    if (fabs(dist-2*M_PI)<1.0e-6) {
                        dist = 0.0;
                    }
                }
                if (dist<minDist) {
                    minDist = dist;
                    done = false;
                    av = v;
                    //idx = is.at();
                }
            }

            // copy to sorted list, removing double points
            if (!done && av!=NULL) {
                if (last.valid==false || last.distanceTo(*av)>1.0e-10) {
                    is2.append(*av);
                    last = *av;
                }
                is.removeAt(is.indexOf(*av));
                av = NULL;
            }
        } while(!done);

        // add small cut lines / arcs to tmp2:
        for (int i2=0; i2<is2.size()-1; ++i2) {
            RS_Vector* v1 = &is2[i2];
            RS_Vector* v2 = &is2[i2+1];

            if (v1!=NULL && v2!=NULL) {
                if (line!=NULL) {
                    tmp2.addEntity(new RS_Line(&tmp2,
                                               RS_LineData(*v1, *v2)));
                } else if (arc!=NULL || circle!=NULL) {
                    tmp2.addEntity(new RS_Arc(&tmp2,
                                              RS_ArcData(center,
                                                         center.distanceTo(*v1),
                                                         center.angleTo(*v1),
                                                         center.angleTo(*v2),
                                                         reversed)));
                }
            }

            v1 = v2;
        }
    }

    // updating hatch / adding entities that are inside
    RS_DEBUG->print("RS_Hatch::update: cutting pattern carpet: OK");

    //RS_EntityContainer* rubbish = new RS_EntityContainer(getGraphic());

    // the hatch pattern entities:
    hatchPattern = new RS_EntityContainer(this);
    hatchPattern->setPen(RS_Pen(RS2::FlagInvalid));
    hatchPattern->setLayer(NULL);
    hatchPattern->setFlag(RS2::FlagTemp);

    //calculateBorders();

    for (RS_Entity* e=tmp2.firstEntity(); e!=NULL;
            e=tmp2.nextEntity()) {

        RS_Vector middlePoint;
        RS_Vector middlePoint2;
        if (e->rtti()==RS2::EntityLine) {
            RS_Line* line = dynamic_cast<RS_Line*>(e);
            middlePoint = line->getMiddlepoint();
            middlePoint2 = line->getNearestDist(line->getLength()/2.1,
                                                line->getStartpoint());
        } else if (e->rtti()==RS2::EntityArc) {
            RS_Arc* arc = dynamic_cast<RS_Arc*>(e);
            middlePoint = arc->getMiddlepoint();
            middlePoint2 = arc->getNearestDist(arc->getLength()/2.1,
                                               arc->getStartpoint());
        } else {
            middlePoint = RS_Vector(false);
            middlePoint2 = RS_Vector(false);
        }

        if (middlePoint.valid) {
            bool onContour=false;

            if (RS_Information::isPointInsideContour(
                        middlePoint,
                        this, &onContour) ||
                    RS_Information::isPointInsideContour(middlePoint2, this)) {

                RS_Entity* te = e->clone();
                te->setPen(RS_Pen(RS2::FlagInvalid));
                te->setLayer(NULL);
                te->reparent(hatchPattern);
                hatchPattern->addEntity(te);
            }
        }
    }

    addEntity(hatchPattern);
    //getGraphic()->addEntity(rubbish);

    calculateBorders();

    // deactivate contour:
    //activateContour(false);

    updateRunning = false;

    RS_DEBUG->print("RS_Hatch::update: OK");
}



/**
 * Activates of deactivates the hatch boundary.
 */
    /*
void RS_Hatch::activateContour(bool on) {
    RS_DEBUG->print("RS_Hatch::activateContour: %d", (int)on);
    for (RS_Entity* e=firstEntity(); e!=NULL;
            e=nextEntity()) {
        if (!e->isUndone()) {
            if (!e->getFlag(RS2::FlagTemp)) {
                RS_DEBUG->print("RS_Hatch::activateContour: set visible");
                e->setVisible(on);
            }
            else {
                RS_DEBUG->print("RS_Hatch::activateContour: entity temp");
            }
        }
        else {
            RS_DEBUG->print("RS_Hatch::activateContour: entity undone");
        }
    }
    RS_DEBUG->print("RS_Hatch::activateContour: OK");
}
    */


/**
 * Overrides drawing of subentities. 
 */
void RS_Hatch::draw(RS_Painter* painter, RS_GraphicView* view, 
    double /*patternOffset*/) {

    RS_DEBUG->print("RS_Hatch::draw");

    // draw hatch entities:
    if (!data.solid) {
        RS_DEBUG->print("RS_Hatch::draw: hatch");
        for (RS_Entity* se=firstEntity();
                se!=NULL;
                se = nextEntity()) {

            if (view->isDraftMode()) {
                if (!se->getFlag(RS2::FlagTemp)) {
                    view->drawEntity(se);
                }
            }
            else {
                if (se->getFlag(RS2::FlagTemp)) {
                    view->drawEntity(se);
                }
            }
        }
    }

    // draw solid:
    else {
        RS_DEBUG->print("RS_Hatch::draw: solid");
        
        RS_PointArray pa;
        RS_PointArray jp;   // jump points
        RS_Vector last(false);

        // optimize loops:
        if (needOptimization==true) {
            for (RS_Entity* l=firstEntity(RS2::ResolveNone);
                    l!=NULL;
                    l=nextEntity(RS2::ResolveNone)) {

                // allow all entity containers (polylines)
                if (l->isContainer()) {
                    RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);

                    RS_DEBUG->print("RS_Hatch::draw: optimizing loop");
                    loop->optimizeContours();
                }
            }
            needOptimization = false;
        }

        // loops:
        for (RS_Entity* l=firstEntity(RS2::ResolveNone);
                l!=NULL;
                l=nextEntity(RS2::ResolveNone)) {

            l->setLayer(getLayer());

            // allow all entity containers (polylines)
            if (l->isContainer()==false) {
                RS_DEBUG->print(RS_Debug::D_WARNING, 
                    "hatch contains entities that are no loops");
                continue;
            }
                
            RS_DEBUG->print("iterating through loops..");

            RS_EntityContainer* loop = dynamic_cast<RS_EntityContainer*>(l);

            // edges:
            for (RS_Entity* e=loop->firstEntity(RS2::ResolveNone);
                    e!=NULL;
                    e=loop->nextEntity(RS2::ResolveNone)) {

                e->setLayer(getLayer());
                switch (e->rtti()) {
                case RS2::EntityLine: {
                        RS_Line* line = dynamic_cast<RS_Line*>(e);

                        RS_Vector v1 = view->toGui(line->getStartpoint());
                        RS_Vector v2 = view->toGui(line->getEndpoint());

                        if (!last.valid || last.distanceTo(v1)>1.0e-2) {
                            pa.moveTo(v1.x, v1.y);
                        }

                        pa.lineTo(v2.x, v2.y);

                        last = v2;
                    }
                    break;

                case RS2::EntityArc: {
                        RS_Arc* arc = dynamic_cast<RS_Arc*>(e);

                        RS_Vector v1 = view->toGui(arc->getStartpoint());
                        RS_Vector v2 = view->toGui(arc->getEndpoint());
                        RS_Vector vc = view->toGui(arc->getCenter());
                        double cr = view->toGuiDX(arc->getRadius());
                        double sweepAngle = arc->getAngleLength() * ARAD;
                        if (arc->isReversed()) {
                            sweepAngle*=-1;
                        }

                        if (!last.valid || last.distanceTo(v1)>1.0e-2) {
                            pa.moveTo(v1.x, v1.y);
                        }

                        pa.arcTo(vc.x-cr, vc.y-cr, 2*cr, 2*cr, arc->getAngle1() * ARAD,
                                 sweepAngle);

                        last = v2;
                    }
                    break;

                case RS2::EntityCircle: {
                        RS_Circle* circle = dynamic_cast<RS_Circle*>(e);

                        RS_Vector vc = view->toGui(circle->getCenter());
                        double cr = view->toGuiDX(circle->getRadius());

                        pa.addEllipse(vc.x-cr, vc.y-cr, 2*cr, 2*cr);

                        last.valid=false;
                    }
                    break;

                default:
                    break;
                }
            }
        }

        if (!view->isDraftMode()) {
            painter->setBrush(painter->getPen().getColor());
            painter->disablePen();
        }
        painter->drawPolygon(pa);
        painter->disableBrush();
    }

    RS_DEBUG->print("RS_Hatch::draw: OK");
}


double RS_Hatch::getDistanceToPoint(
    const RS_Vector& coord,
    RS_Entity** entity,
    RS2::ResolveLevel level,
    double solidDist,
    bool /*visually*/) {

    if (data.solid==true) {
        if (entity!=NULL) {
            *entity = this;
        }

        bool onContour;
        if (RS_Information::isPointInsideContour(
                    coord,
                    this, &onContour)) {

            // distance is the snap range:
            return solidDist;
        }

        return RS_MAXDOUBLE;
    } else {
        return RS_EntityContainer::getDistanceToPoint(coord, entity,
                level, solidDist);
    }
}



void RS_Hatch::move(const RS_Vector& offset) {
    RS_EntityContainer::move(offset);
    //update();
}



void RS_Hatch::rotate(const RS_Vector& center, double angle) {
    RS_EntityContainer::rotate(center, angle);
    data.angle = RS_Math::correctAngle(data.angle+angle);
    //update();
}



void RS_Hatch::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_EntityContainer::scale(center, factor);
    data.scale *= factor.x;
    update();
}



void RS_Hatch::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    RS_EntityContainer::mirror(axisPoint1, axisPoint2);
    double ang = axisPoint1.angleTo(axisPoint2);
    data.angle = RS_Math::correctAngle(data.angle + ang*2.0);
    update();
}


void RS_Hatch::stretch(const RS_Vector& firstCorner,
                       const RS_Vector& secondCorner,
                       const RS_Vector& offset) {

    RS_EntityContainer::stretch(firstCorner, secondCorner, offset);
    update();
}


RS_Variant RS_Hatch::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_HATCH_SOLID) {
        return data.solid;
    } else if (name==RS_HATCH_SCALE) {
        return data.scale;
    } else if (name==RS_HATCH_ANGLE) {
        return RS_Math::rad2deg(data.angle);
    } else if (name==RS_HATCH_PATTERN) {
        return data.pattern;
    } else {
        return RS_EntityContainer::getProperty(name, def);
    }
}



RS_StringList RS_Hatch::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_EntityContainer::getPropertyNames(includeGeometry);
    ret 
    << RS_HATCH_SOLID
    << RS_HATCH_SCALE
    << RS_HATCH_ANGLE
    << RS_HATCH_PATTERN;
    return ret;
}


void RS_Hatch::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_HATCH_SOLID) {
        data.solid = variantToBool(value);
    } else if (name==RS_HATCH_SCALE) {
        data.scale = RS_Math::eval(value.toString());
    } else if (name==RS_HATCH_ANGLE) {
        data.angle = RS_Math::deg2rad(RS_Math::eval(value.toString()));
    } else if (name==RS_HATCH_PATTERN) {
        data.pattern = value.toString();
    } else {
        RS_EntityContainer::setProperty(name, value);
    }
}



/**
 * Limits the choices for the hatch pattern name property.
 */
RS_PropertyAttributes RS_Hatch::getPropertyAttributes(const RS_String& propertyName) {
    RS_PropertyAttributes ret =
        RS_EntityContainer::getPropertyAttributes(propertyName);

    if (propertyName==RS_HATCH_PATTERN) {
        for (RS_Pattern* p=RS_PATTERNLIST->firstPattern();
            p!=NULL;
            p=RS_PATTERNLIST->nextPattern()) {
            
            ret.choices << p->getName();
        }
    }

    return ret;
}
    
    
