/****************************************************************************
** $Id: rs_preview.cpp 9522 2008-04-02 21:18:34Z andrew $
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


#include "rs_preview.h"

#include "rs_entitycontainer.h"
#include "rs_graphicview.h"
#include "rs_grid.h"
#include "rs_hatch.h"
#include "rs_infoarea.h"
#include "rs_information.h"
#include "rs_mouseevent.h"
#include "rs_settings.h"
#include "rs_text.h"
#include "rs_pen.h"

RS_Pen RS_Preview::previewPen =
    RS_Pen(RS_Color(128,0,0),
           RS2::Width00,
           RS2::DashLine);
RS_Pen RS_Preview::previewInfoPen =
    RS_Pen(RS_Color(60,192,0),
           RS2::Width00,
           RS2::SolidLine);
RS_Pen RS_Preview::previewLabelPen =
    RS_Pen(RS_Color(255,255,255),
           RS2::Width00,
           RS2::SolidLine);

/**
 * Constructor.
 */
RS_Preview::RS_Preview(RS_EntityContainer* parent)
        : RS_Entity(parent), RS_EntityContainer(parent), snapMarker(NULL)/*, overridePen(RS2::FlagInvalid)*/ {

    RS_SETTINGS->beginGroup("/Appearance");
    maxEntities = RS_SETTINGS->readNumEntry("/MaxPreview", 100);
    previewPen.setColor(QColor(RS_SETTINGS->readEntry("/PreviewColor", "#7F0000")));
    RS_SETTINGS->endGroup();
}



/**
 * Destructor.
 */
RS_Preview::~RS_Preview() {}



/**
 * Adds an entity to this preview and removes any attributes / layer
 * connections before that. This function ensures that the preview
 * will not get too big and might drop or simplify entities. In any
 * case the given entity will be either added to the preview or
 * immediately deleted from memory.
 */
void RS_Preview::addPreviewEntity(RS_Entity* entity, 
    RS_EntityContainer* subContainer) {

    if (entity==NULL || entity->isUndone()) {
        return;
    }
    
    RS_EntityContainer* ec;

    if (subContainer!=NULL) {
        ec = subContainer;
    } else {
        ec = this;
    }

    // only border preview for complex entities:
    bool addBorder = false;
    bool addEntity = true;
    
    // for hatches: add only border contours:
    if (entity->isOfType(RS2::EntityHatch)) {
        RS_Hatch* hatch = dynamic_cast<RS_Hatch*>(entity);
        if (!hatch->isSolid()) {
            addEntity = false;
            for (RS_Entity* e=hatch->firstEntity();
                e!=NULL;
                e=hatch->nextEntity()) {

                if (!e->getFlag(RS2::FlagTemp)) {
                    RS_Entity* cl = e->clone();
                    cl->reparent(ec);
                    /*
                    ec->addEntity(cl);
                    */
                    addPreviewEntity(cl, ec);
                }
            }
        }
    }

    // images: only preview border:
    else if (entity->rtti()==RS2::EntityImage ||
        entity->rtti()==RS2::EntityInsert) {

        addBorder = true;
        addEntity = false;
    } 
    
    // containers in general: only add border if they are complex:
    else if (entity->isContainer() && 
        entity->rtti()!=RS2::EntitySpline &&
        entity->rtti()!=RS2::EntityEllipse) {
        if (entity->countDeep() > maxEntities-countDeep()) {
            addBorder = true;
        }
    }

    // add border to preview:
    if (addBorder) {
        addRectangle(entity->getMin(), entity->getMax(), ec);
    }

    // add complete entity to preview:
    if (addEntity) {
        entity->setSelected(false, false);
        ec->addEntity(entity);
    }
    else {
        delete entity;
        entity = NULL;
    }
}



void RS_Preview::addRectangle(const RS_Vector& v1, const RS_Vector& v2,
        RS_EntityContainer* subContainer, bool forSelection) {
    
    RS_EntityContainer* ec;

    if (subContainer!=NULL) {
        ec = subContainer;
    } else {
        ec = this;
    }

    bool cross = (v2.x<v1.x);

    RS_SETTINGS->beginGroup("/Appearance");
    previewPen.setColor(QColor(RS_SETTINGS->readEntry("/PreviewColor", "#7F0000")));
    RS_SETTINGS->endGroup();
    
    RS_Pen pen = previewPen;

    if (forSelection) {
        RS_Color col(0,0,0,128);
        if (cross) {
            col.setGreen(255);
        }
        else {
            pen.setLineType(RS2::SolidLine);
            col.setBlue(255);
        }

        RS_Hatch* hatch = new RS_Hatch(ec, RS_HatchData(true, 1.0, 0.0, "SOLID"));
        hatch->setPen(RS_Pen(col, RS2::Width00, RS2::SolidLine));
        RS_EntityContainer* loop = new RS_EntityContainer(hatch);
        hatch->addEntity(loop);
        RS_Line* l;
        l = new RS_Line(loop,
                        RS_LineData(RS_Vector(v1.x, v1.y),
                                    RS_Vector(v2.x, v1.y)));
        loop->addEntity(l);
        l = new RS_Line(loop,
                        RS_LineData(RS_Vector(v2.x, v2.y),
                                    RS_Vector(v2.x, v1.y)));
        loop->addEntity(l);
        l = new RS_Line(loop,
                        RS_LineData(RS_Vector(v2.x, v2.y),
                                    RS_Vector(v1.x, v2.y)));
        loop->addEntity(l);
        l = new RS_Line(loop,
                        RS_LineData(RS_Vector(v1.x, v1.y),
                                    RS_Vector(v1.x, v2.y)));
        loop->addEntity(l);
        hatch->update();
        ec->addEntity(hatch);
    }

    RS_Line* l;
    l = new RS_Line(ec,
                    RS_LineData(RS_Vector(v1.x, v1.y),
                                RS_Vector(v2.x, v1.y)));
    l->setPen(pen);
    l->setLayer(NULL);
    l->setFlag(RS2::FlagTemp);
    ec->addEntity(l);
    l = new RS_Line(ec,
                    RS_LineData(RS_Vector(v2.x, v2.y),
                                RS_Vector(v2.x, v1.y)));
    l->setPen(pen);
    l->setLayer(NULL);
    l->setFlag(RS2::FlagTemp);
    ec->addEntity(l);
    l = new RS_Line(ec,
                    RS_LineData(RS_Vector(v2.x, v2.y),
                                RS_Vector(v1.x, v2.y)));
    l->setPen(pen);
    l->setLayer(NULL);
    l->setFlag(RS2::FlagTemp);
    ec->addEntity(l);
    l = new RS_Line(ec,
                    RS_LineData(RS_Vector(v1.x, v1.y),
                                RS_Vector(v1.x, v2.y)));
    l->setPen(pen);
    l->setLayer(NULL);
    l->setFlag(RS2::FlagTemp);
    ec->addEntity(l);
}


/**
 * Clones the given entity and adds the clone to the preview.
 */
void RS_Preview::addCloneOf(RS_Entity* entity) {
    if (entity==NULL) {
        return;
    }

    RS_Entity* clone = entity->clone();
    addPreviewEntity(clone);
}



/**
 * Adds all selected entities from 'container' to the preview (unselected).
 * 
 * @param subContainer Add the selection to this sub container of the
 *        preview. Can be NULL for direct adding to the preview.
 */
void RS_Preview::addSelectionFrom(RS_EntityContainer& srcContainer,
                                  RS_EntityContainer* subContainer) {

    RS_EntityContainer* ec;

    if (subContainer!=NULL) {
        ec = subContainer;
    } else {
        ec = this;
    }

    int c=0;
    for (RS_Entity* e=srcContainer.firstEntity();
            e!=NULL; e=srcContainer.nextEntity()) {

        if (e->isSelected() && c<maxEntities) {
            RS_Entity* clone = e->clone();
            clone->reparent(ec);
            clone->setSelected(false);

            c+=clone->countDeep();
            addPreviewEntity(clone, ec);
            // note: clone might have been deleted after this point
        }
    }
}



/**
 * Adds all entities from 'container' to the preview (unselected).
 */
void RS_Preview::addAllFrom(RS_EntityContainer& container,
    RS_EntityContainer* subContainer) {
    
    RS_EntityContainer* ec;

    if (subContainer!=NULL) {
        ec = subContainer;
    } else {
        ec = this;
    }

    int c=0;
    for (RS_Entity* e=container.firstEntity();
            e!=NULL; e=container.nextEntity()) {

        if (c<maxEntities) {
            RS_Entity* clone = e->clone();
            clone->setSelected(false, false);
            clone->reparent(ec);

            c+=clone->countDeep();
            addPreviewEntity(clone, ec);
            // clone might be NULL after this point
        }
    }
}




/**
 * Adds all entities in the given range and those which have endpoints
 * in the given range to the preview. Invisible and locked entities
 * are not added.
 */
void RS_Preview::addStretchablesFrom(RS_EntityContainer& container,
                                     const RS_Vector& v1, const RS_Vector& v2,
                                     RS_EntityContainer* subContainer) {

    int c=0;

    RS_EntityContainer* ec;

    if (subContainer!=NULL) {
        ec = subContainer;
    } else {
        ec = this;
    }

    for (RS_Entity* e=container.firstEntity();
            e!=NULL; e=container.nextEntity()) {

        if (e->isVisible() &&
            !e->isLocked() &&
                e->rtti()!=RS2::EntityHatch &&
                (e->isInWindow(v1, v2) ||
                 e->hasEndpointsWithinWindow(v1, v2)) && c<maxEntities) {

            RS_Entity* clone = e->clone();
            clone->reparent(ec);

            c+=clone->countDeep();
            addPreviewEntity(clone, ec);
            // clone might be NULL after this point
        }
    }
}


/**
 * Adds a measuring line 
 */
void RS_Preview::addMeasuringLine(const RS_Vector& p1, const RS_Vector& p2,
    RS_GraphicView& graphicView) {

    RS_DEBUG->print("RS_Preview::addMeasuringLine");

    if (!p1.valid || !p2.valid) {
        return;
    }

    RS_Line* l = new RS_Line(this,
        RS_LineData(p1, p2));
    l->setPen(getPreviewInfoPen());
    l->setLayer(NULL);
    addEntity(l);
    
    double spacing = 1.0;
    if (graphicView.getGrid()!=NULL) {
        spacing = graphicView.getGrid()->getSpacing();
    }
    
    double sideAngle = l->getDirection1()+M_PI/2;
    RS_Vector toSide;
    toSide.setPolar(graphicView.toGraphDX(2), sideAngle);
    
    if (spacing>1.0e-12 && l->getLength()/spacing<500) {
        for (double m=0.0; m<l->getLength(); m+=spacing) {
            RS_Vector pos = l->getNearestDist(m, true);
            RS_Line* sl = new RS_Line(this,
                                      RS_LineData(pos-toSide,
                                                  pos+toSide));
            sl->setPen(getPreviewInfoPen());
            sl->setLayer(NULL);
            addPreviewEntity(sl);
        }
    }
    
    RS_String label;
    label = RS_String("%1").arg(l->getLength());
    
    addMeasuringLabel(label, p2, 
        RS_Math::correctAngle(l->getDirection2()+M_PI),
        graphicView);

    RS_DEBUG->print("RS_Preview::addMeasuringLine: OK");
}
    
    
void RS_Preview::addMeasuringArc(const RS_Vector& center, 
        const RS_Vector& p1, const RS_Vector& p2,
        const RS_Vector& position,
        RS_GraphicView& graphicView) {
    
    if (!p1.valid || !p2.valid || !center.valid || !position.valid) {
        return;
    }

    double a1 = center.angleTo(p1);
    double a2 = center.angleTo(p2);
    double r = center.distanceTo(p2);
    double posAngle = center.angleTo(position);
    bool reversed = RS_Math::isAngleBetween(posAngle, a1, a2, true);

    RS_Arc* a = new RS_Arc(this,
        RS_ArcData(center, r, a1, a2, reversed));
    a->setPen(getPreviewInfoPen());
    a->setLayer(NULL);
    addEntity(a);
    
    /*
    double margin = graphicView.toGraphDX(8);
    RS2::HAlign halign;
    RS2::VAlign valign;
    RS_Vector pos = p2;
    if (RS_Math::correctAngle(a->getDirection2()+M_PI)<M_PI/2) {
        halign = RS2::HAlignLeft;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(margin, margin);
    } else if (RS_Math::correctAngle(a->getDirection2()+M_PI)<M_PI) {
        halign = RS2::HAlignRight;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(-margin, margin);
    } else if (RS_Math::correctAngle(a->getDirection2()+M_PI)<M_PI/2*3) {
        halign = RS2::HAlignRight;
        valign = RS2::VAlignTop;
        pos += RS_Vector(-margin, -margin);
    } else {
        halign = RS2::HAlignLeft;
        valign = RS2::VAlignTop;
        pos += RS_Vector(margin, -margin);
    }
    */

    RS_String label;
    label = RS_String("%1%2")
        .arg(RS_Math::rad2deg(a->getAngleLength()))
        .arg(RS_Char(0xB0));

    /*
    RS_Text* text = new RS_Text(this,
                                RS_TextData(pos,
                                            graphicView.toGraphDX(10),
                                            100.0,
                                            valign, halign,
                                            RS2::LeftToRight,
                                            RS2::Exact,
                                            1.0,
                                            label,
                                            "standard",
                                            0.0));
    text->setPen(previewLabelPen);
    addEntity(text);
    */
    addMeasuringLabel(label, p2, 
        RS_Math::correctAngle(a->getDirection2()+M_PI),
        graphicView);
}



void RS_Preview::addMeasuringArea(RS_InfoArea& area, 
    RS_GraphicView& graphicView) {

    RS_Line* l = NULL;
    for (int i=0; i<area.count(); ++i) {
        int k=i+1;
        if (k>=area.count()) {
            k=0;
        }
        l = new RS_Line(this, 
            RS_LineData(area.at(i), area.at(k)));
        l->setPen(previewInfoPen);
        l->setLayer(NULL);
        addEntity(l);
    }
    
    area.calculate();
    RS_String label;
    label = RS_String("%1").arg(area.getArea());
   
    if (l!=NULL) {
        addMeasuringLabel(label, l->getEndpoint(), 
            RS_Math::correctAngle(l->getDirection2()+M_PI),
            graphicView);
    }
}


/**
 * Adds a label typically used to display a measuring result.
 *
 * @param label The string to display.
 * @param position Position of the label.
 * @param direction The direction of the element that would
 *              be in the way of the label. E.g. direction PI/4 means 
 *              that the label will be left-bottom aligned so it does
 *              not intersect with a line that points in direction 
 *              PI/4.
 */
void RS_Preview::addMeasuringLabel(const RS_String& label, 
    const RS_Vector& position, double direction,
    RS_GraphicView& graphicView) {
    
    double margin = graphicView.toGraphDX(8);
    RS2::HAlign halign;
    RS2::VAlign valign;
    RS_Vector pos = position;
    // 20071215: always place label above mouse cursor, so it does not 
    //  interfer with snap point indicator:
    if (direction < M_PI || direction < M_PI/2*3) {
        halign = RS2::HAlignRight;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(-margin, margin);
    } else {
        halign = RS2::HAlignLeft;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(margin, margin);
    }
    /*
    if (direction < M_PI/2) {
        halign = RS2::HAlignLeft;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(margin, margin);
    } else if (direction < M_PI) {
        halign = RS2::HAlignRight;
        valign = RS2::VAlignBottom;
        pos += RS_Vector(-margin, margin);
    } else if (direction < M_PI/2*3) {
        halign = RS2::HAlignRight;
        valign = RS2::VAlignTop;
        pos += RS_Vector(-margin, -margin);
    } else {
        halign = RS2::HAlignLeft;
        valign = RS2::VAlignTop;
        pos += RS_Vector(margin, -margin);
    }
    */

    RS_Text* text = new RS_Text(this,
                                RS_TextData(pos,
                                            graphicView.toGraphDX(10),
                                            100.0,
                                            valign, halign,
                                            RS2::LeftToRight,
                                            RS2::Exact,
                                            1.0,
                                            label,
                                            "standard",
                                            0.0));
    text->setPen(previewLabelPen);
    text->setLayer(NULL);
    addEntity(text);
}



/**
 * Adds a snapper symbol to the preview.
 */
/*
void RS_Preview::setSnapMarker(const RS_Snapper& snapper, double factor) {
    if (snapMarker!=NULL) {
        removeEntity(snapMarker);
        snapMarker = NULL;
    }
    if (factor<=0.0) {
      factor = 1.0;
    }
 
    snapMarker = new RS_EntityContainer(this);
    addPreviewEntity(snapMarker);
 
    double size = 5.0/factor;
    RS_DEBUG->print("snapper size: %f", size);
    
    RS_Line* l = new RS_Line(snapMarker,
                    RS_LineData(RS_Vector(v.x-size, v.y-size),
                                RS_Vector(v.x+size, v.y+size)));
    l->setPen(previewPen);
    snapMarker->addEntity(l);
}
*/

