/****************************************************************************
** $Id: gv_hatch.cpp 9008 2008-02-12 18:16:26Z andrew $
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


#include "gv_hatch.h"

#include "gv_application.h"
#include "gv_graphic.h"
#include "gv_graphicview.h"
#include "gv_information.h"
#include "gv_painter.h"
//#include "gv_painterqt.h"
#include "gv_pattern.h"
#include "gv_patternlist.h"
#include "gv_pointarray.h"
#include "gv_valuelist.h"


const char* GV_Hatch::propertySolid = QT_TRANSLATE_NOOP("QObject", "Solid");
const char* GV_Hatch::propertyScale = QT_TRANSLATE_NOOP("QObject", "Pattern|Scale");
const char* GV_Hatch::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Pattern|Angle");
const char* GV_Hatch::propertyPattern = QT_TRANSLATE_NOOP("QObject", "Pattern|Name");



/**
 * Constructor.
 */
GV_Hatch::GV_Hatch(GV_EntityContainer* parentContainer,
                   const GV_HatchData& d)
        : GV_Entity(parentContainer), GV_EntityContainer(parentContainer), data(d) {

    hatchPattern = NULL;
    updateRunning = false;
    needOptimization = true;
}

    
    
void GV_Hatch::initFromVectorList(const GV_VectorList& vectorList) {
    GV_EntityContainer* loop = new GV_EntityContainer(this);
    addEntity(loop);

    for (int i=0; i<vectorList.count()-1; i++) {
        loop->addEntity(
            new GV_Line(loop, 
                GV_LineData(
                    vectorList.at(i),
                    vectorList.at(i+1))));
    }
}



/**
 * Validates the hatch.
 */
bool GV_Hatch::validate() {
    bool ret = true;
    
    // loops:
    for (GV_Entity* l=firstEntity(GV2::ResolveNone);
            l!=NULL;
            l=nextEntity(GV2::ResolveNone)) {

        // allow all entity containers (including polylines)
        if (l->isContainer()) {
            GV_EntityContainer* loop = dynamic_cast<GV_EntityContainer*>(l);

            ret = loop->optimizeContours();
            if (!ret) {
                RB_DEBUG->print(RB_Debug::D_ERROR, 
                    "GV_Hatch::validate: "
                    "failed to optimize contours");
                break;
            }
        }
        else {
            RB_DEBUG->print(RB_Debug::D_ERROR, 
                "GV_Hatch::validate: "
                "hatch contains entities that are not in loops");
            ret = false;
            break;
        }
    }

    return ret;
}


    
GV_Entity* GV_Hatch::clone() {
    GV_Hatch* t = new GV_Hatch(parentContainer, data);
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



GV_VectorList GV_Hatch::getRefPoints(GV2::RefMode /*refMode*/) {
    RB_DEBUG->print("GV_Hatch::getRefPoints");

    GV_VectorList ret;

    GV_Entity* entityCursor = currentEntity();
    for (GV_Entity* l=firstEntity();
        l!=NULL;
        l=nextEntity()) {
    
        if (!l->isContainer()) {
            continue;
        }

        GV_EntityContainer* loop = dynamic_cast<GV_EntityContainer*>(l);
        if (isSolid() || loop!=hatchPattern) {
            for (GV_Entity* e=loop->firstEntity();
                e!=NULL;
                e = loop->nextEntity()) {

                if (!e->isAtomic()) {
                    continue;
                }

                GV_AtomicEntity* ae = dynamic_cast<GV_AtomicEntity*>(e);
                ret.append(ae->getEndpoint());
            }
        }
    }
    findEntity(entityCursor);
    
    return ret;
}



GV_Vector GV_Hatch::getNearestSelectedRef(const GV_Vector& coord,
        double* dist, GV2::RefMode refMode) {

    return GV_Entity::getNearestSelectedRef(coord, dist, refMode);
}




/**
 * @return Number of loops.
 */
int GV_Hatch::countLoops() {
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
void GV_Hatch::calculateBorders() {

    //activateContour(true);

    GV_EntityContainer::calculateBorders();

    RB_DEBUG->print("GV_Hatch::calculateBorders: size: %f,%f", 
        getSize().x, getSize().y);

    //activateContour(false);
}
*/



/**
 * Updates the Hatch. Called when the 
 * hatch or it's data, position, alignment, .. changes.
 */
void GV_Hatch::update() {
    RB_DEBUG->print("GV_Hatch::update");
//    RB_DEBUG->print("GV_Hatch::update: contour has %d loops", count());

    if (updateRunning) {
        RB_DEBUG->print("GV_Hatch::update: Update running. Abort.");
        return;
    }

    if (updateEnabled==false) {
        RB_DEBUG->print("GV_Hatch::update: Updates disabled. Abort.");
        return;
    }

    if (data.solid==true) {
        calculateBorders();
        RB_DEBUG->print("GV_Hatch::update: Hatch is solid. No Update needed.");
        return;
    }

    RB_DEBUG->print("GV_Hatch::update (not solid)");
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
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Hatch::update: invalid contour in hatch found");
        updateRunning = false;
        return;
    }

    // search pattern:
    RB_DEBUG->print("GV_Hatch::update: requesting pattern");
    GV_Pattern* pat = GV_PATTERNLIST->requestPattern(data.pattern);
    if (pat==NULL) {
        updateRunning = false;
        RB_DEBUG->print("GV_Hatch::update: requesting pattern: not found");
        return;
    }
    RB_DEBUG->print("GV_Hatch::update: requesting pattern: OK");

    RB_DEBUG->print("GV_Hatch::update: cloning pattern");
    pat = dynamic_cast<GV_Pattern*>(pat->clone());
    RB_DEBUG->print("GV_Hatch::update: cloning pattern: OK");

    // scale pattern
//    RB_DEBUG->print("GV_Hatch::update: scaling pattern: %f", data.scale);
    pat->scale(GV_Vector(0.0,0.0), GV_Vector(data.scale, data.scale));
    RB_DEBUG->print("GV_Hatch::update: calculate pattern borders");
    pat->calculateBorders();
    RB_DEBUG->print("GV_Hatch::update: calculate borders");
    calculateBorders();
    RB_DEBUG->print("GV_Hatch::update: scaling pattern: OK");

    // find out how many pattern-instances we need in x/y:
    int px1, py1, px2, py2;
    double f;
    RB_DEBUG->print("GV_Hatch::update: cloning hatch");
    GV_Hatch* copy = dynamic_cast<GV_Hatch*>(this->clone());
    copy->updateRunning = true;
    RB_DEBUG->print("GV_Hatch::update: cloning hatch: ok");
    copy->rotate(GV_Vector(0.0,0.0), -data.angle);
    copy->calculateBorders();

    // create a pattern over the whole contour.
    GV_Vector pSize = pat->getSize();
//    GV_Vector cPos = getMin();
    GV_Vector cSize = getSize();


//    RB_DEBUG->print("GV_Hatch::update: pattern size: %f/%f", pSize.x, pSize.y);
//    RB_DEBUG->print("GV_Hatch::update: contour size: %f/%f", cSize.x, cSize.y);

    if (cSize.x<1.0e-6 || cSize.y<1.0e-6 ||
            pSize.x<1.0e-6 || pSize.y<1.0e-6 ||
            cSize.x>GV_MAXDOUBLE-1 || cSize.y>GV_MAXDOUBLE-1 ||
            pSize.x>GV_MAXDOUBLE-1 || pSize.y>GV_MAXDOUBLE-1) {
        delete pat;
        delete copy;
        updateRunning = false;
        RB_DEBUG->print("GV_Hatch::update: contour size or pattern size too small");
        return;
    }

    // avoid huge memory consumption:
    else if (cSize.x/pSize.x>100 || cSize.y/pSize.y>100) {
        RB_DEBUG->print("GV_Hatch::update: contour size too large or pattern size too small");
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

    GV_EntityContainer tmp;   // container for untrimmed lines

    // adding array of patterns to tmp:
    RB_DEBUG->print("GV_Hatch::update: creating pattern carpet");

    for (int px=px1; px<=px2; px++) {
        for (int py=py1; py<=py2; py++) {
            for (GV_Entity* e=pat->firstEntity(); e!=NULL;
                    e=pat->nextEntity()) {

                GV_Entity* te = e->clone();
                te->rotate(GV_Vector(0.0,0.0), data.angle);
                GV_Vector v1, v2;
                v1.setPolar(px*pSize.x, data.angle);
                v2.setPolar(py*pSize.y, data.angle+M_PI/2.0);
                te->move(v1+v2);
                tmp.addEntity(te);
            }
        }
    }

    delete pat;
    pat = NULL;
    RB_DEBUG->print("GV_Hatch::update: creating pattern carpet: OK");


    RB_DEBUG->print("GV_Hatch::update: cutting pattern carpet");
    // cut pattern to contour shape:
    GV_EntityContainer tmp2;   // container for small cut lines
    GV_Line* line = NULL;
    GV_Arc* arc = NULL;
    GV_Circle* circle = NULL;
    for (GV_Entity* e=tmp.firstEntity(); e!=NULL;
            e=tmp.nextEntity()) {

        GV_Vector startPoint;
        GV_Vector endPoint;
        GV_Vector center = GV_Vector(false);
        bool reversed;

        if (e->rtti()==GV2::EntityLine) {
            line = dynamic_cast<GV_Line*>(e);
            arc = NULL;
            circle = NULL;
            startPoint = line->getStartpoint();
            endPoint = line->getEndpoint();
            center = GV_Vector(false);
            reversed = false;
        } else if (e->rtti()==GV2::EntityArc) {
            arc = dynamic_cast<GV_Arc*>(e);
            line = NULL;
            circle = NULL;
            startPoint = arc->getStartpoint();
            endPoint = arc->getEndpoint();
            center = arc->getCenter();
            reversed = arc->isReversed();
        } else if (e->rtti()==GV2::EntityCircle) {
            circle = dynamic_cast<GV_Circle*>(e);
            line = NULL;
            arc = NULL;
            startPoint = circle->getCenter()
                         + GV_Vector(circle->getRadius(), 0.0);
            endPoint = startPoint;
            center = circle->getCenter();
            reversed = false;
        } else {
            continue;
        }

        // getting all intersections of this pattern line with the contour:
        GV_ValueList<GV_Vector> is;
        is.append(startPoint);

        for (GV_Entity* loop=firstEntity(); loop!=NULL;
                loop=nextEntity()) {

            if (loop->isContainer()) {
                for (GV_Entity* p=dynamic_cast<GV_EntityContainer*>(loop)->firstEntity();
                        p!=NULL;
                        p=dynamic_cast<GV_EntityContainer*>(loop)->nextEntity()) {

                    GV_VectorList sol =
                        GV_Information::getIntersection(e, p, true);

                    for (int i=0; i<=1; ++i) {
                        if (sol.get(i).valid) {
                            is.append(sol.get(i));
//                            RB_DEBUG->print("  pattern line intersection: %f/%f",
//                                            sol.get(i).x, sol.get(i).y);
                        }
                    }
                }
            }
        }

        is.append(endPoint);

        // sort the intersection points into is2:
        GV_Vector sp = startPoint;
        double sa = center.angleTo(sp);
        GV_ValueList<GV_Vector> is2;
        bool done;
        double minDist;
        double dist = 0.0;
        GV_Vector* av;
        GV_Vector last = GV_Vector(false);
        do {
            done = true;
            minDist = GV_MAXDOUBLE;
            av = NULL;
            //for (GV_Vector* v = is.first(); v!=NULL; v = is.next()) {
            for (int i1=0; i1<is.size(); ++i1) {
                GV_Vector* v = &is[i1];
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
            GV_Vector* v1 = &is2[i2];
            GV_Vector* v2 = &is2[i2+1];

            if (v1!=NULL && v2!=NULL) {
                if (line!=NULL) {
                    tmp2.addEntity(new GV_Line(&tmp2,
                                               GV_LineData(*v1, *v2)));
                } else if (arc!=NULL || circle!=NULL) {
                    tmp2.addEntity(new GV_Arc(&tmp2,
                                              GV_ArcData(center,
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
    RB_DEBUG->print("GV_Hatch::update: cutting pattern carpet: OK");

    //GV_EntityContainer* rubbish = new GV_EntityContainer(getGraphic());

    // the hatch pattern entities:
    hatchPattern = new GV_EntityContainer(this);
    hatchPattern->setPen(GV_Pen(GV2::FlagInvalid));
    hatchPattern->setLayer(NULL);
    hatchPattern->setFlag(GV2::FlagTemp);

    //calculateBorders();

    for (GV_Entity* e=tmp2.firstEntity(); e!=NULL;
            e=tmp2.nextEntity()) {

        GV_Vector middlePoint;
        GV_Vector middlePoint2;
        if (e->rtti()==GV2::EntityLine) {
            GV_Line* line = dynamic_cast<GV_Line*>(e);
            middlePoint = line->getMiddlepoint();
            middlePoint2 = line->getNearestDist(line->getLength()/2.1,
                                                line->getStartpoint());
        } else if (e->rtti()==GV2::EntityArc) {
            GV_Arc* arc = dynamic_cast<GV_Arc*>(e);
            middlePoint = arc->getMiddlepoint();
            middlePoint2 = arc->getNearestDist(arc->getLength()/2.1,
                                               arc->getStartpoint());
        } else {
            middlePoint = GV_Vector(false);
            middlePoint2 = GV_Vector(false);
        }

        if (middlePoint.valid) {
            bool onContour=false;

            if (GV_Information::isPointInsideContour(
                        middlePoint,
                        this, &onContour) ||
                    GV_Information::isPointInsideContour(middlePoint2, this)) {

                GV_Entity* te = e->clone();
                te->setPen(GV_Pen(GV2::FlagInvalid));
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

    RB_DEBUG->print("GV_Hatch::update: OK");
}



/**
 * Activates of deactivates the hatch boundary.
 */
    /*
void GV_Hatch::activateContour(bool on) {
    RB_DEBUG->print("GV_Hatch::activateContour: %d", (int)on);
    for (GV_Entity* e=firstEntity(); e!=NULL;
            e=nextEntity()) {
        if (!e->isUndone()) {
            if (!e->getFlag(GV2::FlagTemp)) {
                RB_DEBUG->print("GV_Hatch::activateContour: set visible");
                e->setVisible(on);
            }
            else {
                RB_DEBUG->print("GV_Hatch::activateContour: entity temp");
            }
        }
        else {
            RB_DEBUG->print("GV_Hatch::activateContour: entity undone");
        }
    }
    RB_DEBUG->print("GV_Hatch::activateContour: OK");
}
    */


/**
 * Overrides drawing of subentities. 
 */
void GV_Hatch::draw(GV_Painter* painter, GV_GraphicView* view, 
    double /*patternOffset*/) {

    RB_DEBUG->print("GV_Hatch::draw");

    // draw hatch entities:
    if (!data.solid) {
        RB_DEBUG->print("GV_Hatch::draw: hatch");
        for (GV_Entity* se=firstEntity();
                se!=NULL;
                se = nextEntity()) {

            if (view->isDraftMode()) {
                if (!se->getFlag(GV2::FlagTemp)) {
                    view->drawEntity(se);
                }
            }
            else {
                if (se->getFlag(GV2::FlagTemp)) {
                    view->drawEntity(se);
                }
            }
        }
    }

    // draw solid:
    else {
        RB_DEBUG->print("GV_Hatch::draw: solid");
        
        GV_PointArray pa;
        GV_PointArray jp;   // jump points
        GV_Vector last(false);

        // optimize loops:
        if (needOptimization==true) {
            for (GV_Entity* l=firstEntity(GV2::ResolveNone);
                    l!=NULL;
                    l=nextEntity(GV2::ResolveNone)) {

                // allow all entity containers (polylines)
                if (l->isContainer()) {
                    GV_EntityContainer* loop = dynamic_cast<GV_EntityContainer*>(l);

                    RB_DEBUG->print("GV_Hatch::draw: optimizing loop");
                    loop->optimizeContours();
                }
            }
            needOptimization = false;
        }

        // loops:
        for (GV_Entity* l=firstEntity(GV2::ResolveNone);
                l!=NULL;
                l=nextEntity(GV2::ResolveNone)) {

            l->setLayer(getLayer());

            // allow all entity containers (polylines)
            if (l->isContainer()==false) {
                RB_DEBUG->print(RB_Debug::D_WARNING, 
                    "hatch contains entities that are no loops");
                continue;
            }
                
            RB_DEBUG->print("iterating through loops..");

            GV_EntityContainer* loop = dynamic_cast<GV_EntityContainer*>(l);

            // edges:
            for (GV_Entity* e=loop->firstEntity(GV2::ResolveNone);
                    e!=NULL;
                    e=loop->nextEntity(GV2::ResolveNone)) {

                e->setLayer(getLayer());
                switch (e->rtti()) {
                case GV2::EntityLine: {
                        GV_Line* line = dynamic_cast<GV_Line*>(e);

                        GV_Vector v1 = view->toGui(line->getStartpoint());
                        GV_Vector v2 = view->toGui(line->getEndpoint());

                        if (!last.valid || last.distanceTo(v1)>1.0e-2) {
                            pa.moveTo(v1.x, v1.y);
                        }

                        pa.lineTo(v2.x, v2.y);

                        last = v2;
                    }
                    break;

                case GV2::EntityArc: {
                        GV_Arc* arc = dynamic_cast<GV_Arc*>(e);

                        GV_Vector v1 = view->toGui(arc->getStartpoint());
                        GV_Vector v2 = view->toGui(arc->getEndpoint());
                        GV_Vector vc = view->toGui(arc->getCenter());
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

                case GV2::EntityCircle: {
                        GV_Circle* circle = dynamic_cast<GV_Circle*>(e);

                        GV_Vector vc = view->toGui(circle->getCenter());
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

    RB_DEBUG->print("GV_Hatch::draw: OK");
}


double GV_Hatch::getDistanceToPoint(
    const GV_Vector& coord,
    GV_Entity** entity,
    GV2::ResolveLevel level,
    double solidDist,
    bool /*visually*/) {

    if (data.solid==true) {
        if (entity!=NULL) {
            *entity = this;
        }

        bool onContour;
        if (GV_Information::isPointInsideContour(
                    coord,
                    this, &onContour)) {

            // distance is the snap range:
            return solidDist;
        }

        return GV_MAXDOUBLE;
    } else {
        return GV_EntityContainer::getDistanceToPoint(coord, entity,
                level, solidDist);
    }
}



void GV_Hatch::move(const GV_Vector& offset) {
    GV_EntityContainer::move(offset);
    //update();
}



void GV_Hatch::rotate(const GV_Vector& center, double angle) {
    GV_EntityContainer::rotate(center, angle);
    data.angle = GV_Math::correctAngle(data.angle+angle);
    //update();
}



void GV_Hatch::scale(const GV_Vector& center, const GV_Vector& factor) {
    GV_EntityContainer::scale(center, factor);
    data.scale *= factor.x;
    update();
}



void GV_Hatch::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    GV_EntityContainer::mirror(axisPoint1, axisPoint2);
    double ang = axisPoint1.angleTo(axisPoint2);
    data.angle = GV_Math::correctAngle(data.angle + ang*2.0);
    update();
}


void GV_Hatch::stretch(const GV_Vector& firstCorner,
                       const GV_Vector& secondCorner,
                       const GV_Vector& offset) {

    GV_EntityContainer::stretch(firstCorner, secondCorner, offset);
    update();
}


QVariant GV_Hatch::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_HATCH_SOLID) {
        return data.solid;
    } else if (name==GV_HATCH_SCALE) {
        return data.scale;
    } else if (name==GV_HATCH_ANGLE) {
        return GV_Math::rad2deg(data.angle);
    } else if (name==GV_HATCH_PATTERN) {
        return data.pattern;
    } else {
        return GV_EntityContainer::getProperty(name, def);
    }
}



QStringList GV_Hatch::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_EntityContainer::getPropertyNames(includeGeometry);
    ret 
    << GV_HATCH_SOLID
    << GV_HATCH_SCALE
    << GV_HATCH_ANGLE
    << GV_HATCH_PATTERN;
    return ret;
}


void GV_Hatch::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_HATCH_SOLID) {
        data.solid = variantToBool(value);
    } else if (name==GV_HATCH_SCALE) {
        data.scale = GV_Math::eval(value.toString());
    } else if (name==GV_HATCH_ANGLE) {
        data.angle = GV_Math::deg2rad(GV_Math::eval(value.toString()));
    } else if (name==GV_HATCH_PATTERN) {
        data.pattern = value.toString();
    } else {
        GV_EntityContainer::setProperty(name, value);
    }
}



/**
 * Limits the choices for the hatch pattern name property.
 */
GV_PropertyAttributes GV_Hatch::getPropertyAttributes(const QString& propertyName) {
    GV_PropertyAttributes ret =
        GV_EntityContainer::getPropertyAttributes(propertyName);

    if (propertyName==GV_HATCH_PATTERN) {
        for (GV_Pattern* p=GV_PATTERNLIST->firstPattern();
            p!=NULL;
            p=GV_PATTERNLIST->nextPattern()) {
            
            ret.choices << p->getName();
        }
    }

    return ret;
}
    
    
