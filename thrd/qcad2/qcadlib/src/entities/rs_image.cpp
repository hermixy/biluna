/****************************************************************************
** $Id: rs_image.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "rs_image.h"

#include "rs_constructionline.h"
#include "rs_debug.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_painterqt.h"


const char* RS_Image::propertyInsertionPointX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* RS_Image::propertyInsertionPointY = QT_TRANSLATE_NOOP("QObject", "Position|Y");
const char* RS_Image::propertyScaleX = QT_TRANSLATE_NOOP("QObject", "Scale|X");
const char* RS_Image::propertyScaleY = QT_TRANSLATE_NOOP("QObject", "Scale|Y");
const char* RS_Image::propertySizeX = QT_TRANSLATE_NOOP("QObject", "Size|X");
const char* RS_Image::propertySizeY = QT_TRANSLATE_NOOP("QObject", "Size|Y");
const char* RS_Image::propertyFile = QT_TRANSLATE_NOOP("QObject", "Image File");
const char* RS_Image::propertyBrightness = QT_TRANSLATE_NOOP("QObject", "Brightness");
const char* RS_Image::propertyContrast = QT_TRANSLATE_NOOP("QObject", "Contrast");
const char* RS_Image::propertyFade = QT_TRANSLATE_NOOP("QObject", "Fade");
const char* RS_Image::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 */
RS_Image::RS_Image(RS_EntityContainer* parentContainer,
                   const RS_ImageData& d)
        : RS_Entity(parentContainer), RS_AtomicEntity(parentContainer), data(d) {

    update();
    calculateBorders();
}



/**
 * Destructor.
 */
RS_Image::~RS_Image() {
    /*if (img!=NULL) {
        delete[] img;
    }*/
}




RS_Entity* RS_Image::clone() {
    RS_Image* i = new RS_Image(parentContainer, data);
    i->copyPropertiesFrom(this);
    i->img = img;
    i->initId();
    i->update();
    return i;
}



void RS_Image::update() {

    RS_DEBUG->print("RS_Image::update");

    // the whole image:
    img = RS_Img(data.file);
    if (!img.isNull()) {
        data.size = RS_Vector(img.width(), img.height());
    }

    RS_DEBUG->print("RS_Image::update: OK");
}




RS_VectorList RS_Image::getRefPoints(RS2::RefMode /*refMode*/) {
    RS_VectorList ret = getCorners();
    return ret;
}


void RS_Image::calculateBorders(bool /*visibleOnly*/) {
    resetBorders();

    RS_VectorList sol = getCorners();

    for (int i=0; i<4; ++i) {
        minV = RS_Vector::minimum(minV, sol.get(i));
        maxV = RS_Vector::maximum(maxV, sol.get(i));
    }
}



RS_Vector RS_Image::getNearestEndpoint(const RS_Vector& coord,
                                       double* dist) {
    RS_VectorList corners = getCorners();
    return corners.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestPointOnEntity(const RS_Vector& coord,
        bool onEntity, double* dist, RS_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    RS_VectorList points;

    RS_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        points.set(i, l[i]->getNearestPointOnEntity(coord, onEntity));
    }
                
    deleteBoundaryRectangle(l);

    return points.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestCenter(const RS_Vector& coord,
                                     double* dist) {

    RS_VectorList points;
    RS_VectorList corners = getCorners();

    points.set(0, (corners.get(0) + corners.get(1))/2.0);
    points.set(1, (corners.get(1) + corners.get(2))/2.0);
    points.set(2, (corners.get(2) + corners.get(3))/2.0);
    points.set(3, (corners.get(3) + corners.get(0))/2.0);

    return points.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestMiddle(const RS_Vector& coord,
                                     double* dist) {
    return getNearestCenter(coord, dist);
}



RS_Vector RS_Image::getNearestDist(double distance,
                                   const RS_Vector& coord,
                                   double* dist) {

    RS_VectorList points;
    
    RS_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        points.set(i, l[i]->getNearestDist(distance, coord, dist));
    }

    deleteBoundaryRectangle(l);

    return points.getClosest(coord, dist);
}



double RS_Image::getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity,
                                    RS2::ResolveLevel /*level*/,
                                    double /*solidDist*/,
                                    bool /*visually*/) {
    if (entity!=NULL) {
        *entity = this;
    }

    double dist;
    double minDist = RS_MAXDOUBLE;

    RS_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        dist = l[i]->getDistanceToPoint(coord, NULL);
        if (dist<minDist) {
            minDist = dist;
        }
    }

    deleteBoundaryRectangle(l);

    return minDist;
}



void RS_Image::move(const RS_Vector& offset) {
    data.insertionPoint.move(offset);
    calculateBorders();
}



void RS_Image::rotate(const RS_Vector& center, double angle) {
    data.insertionPoint.rotate(center, angle);
    data.uVector.rotate(angle);
    data.vVector.rotate(angle);
    calculateBorders();
}



void RS_Image::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.insertionPoint.scale(center, factor);
    data.uVector.scale(factor);
    data.vVector.scale(factor);
    calculateBorders();
}



void RS_Image::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.insertionPoint.mirror(axisPoint1, axisPoint2);
    data.uVector.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.vVector.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
    calculateBorders();
}


void RS_Image::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_VectorList corners = getCorners();
    if (ref.distanceTo(corners[0])<1.0e-4 ||
        ref.distanceTo(corners[1])<1.0e-4 ||
        ref.distanceTo(corners[2])<1.0e-4 ||
        ref.distanceTo(corners[3])<1.0e-4) {
        move(offset);
    } 
}



void RS_Image::draw(RS_Painter* painter, RS_GraphicView* view, double /*patternOffset*/) {
    RS_DEBUG->print("RS_Image::draw");

    if (painter==NULL || view==NULL || img.isNull()) {
        return;
    }

    int ox = 0;
    int oy = 0;
    int width = view->getWidth();
    int height = view->getHeight();

    RS_Vector scale = RS_Vector(view->toGuiDX(data.uVector.magnitude()),
                                view->toGuiDY(data.vVector.magnitude()));
    double angle = data.uVector.angle();

    int startX, stopX, startY, stopY;

    if (data.uVector.x>1.0e-6 && data.vVector.y>1.0e-6) {
        startX = (int)((view->toGraphX(ox)-data.insertionPoint.x) /
                       data.uVector.x) - 1;
        if (startX<0) {
            startX = 0;
        }
        stopX = (int)((view->toGraphX(width)-data.insertionPoint.x) /
                      data.uVector.x) + 1;
        if (stopX>(int)data.size.x) {
            stopX = (int)data.size.x;
        }
        startY = -(int)((view->toGraphY(oy) -
                         (data.insertionPoint.y+getImageHeight())) /
                        data.vVector.y) - 1;
        if (startY<0) {
            startY = 0;
        }
        stopY = -(int)((view->toGraphY(height) -
                        (data.insertionPoint.y+getImageHeight())) /
                       data.vVector.y) + 1;
        if (stopY>(int)data.size.y) {
            stopY = (int)data.size.y;
        }
    }
    else {
        startX = 0;
        startY = 0;
        stopX = 0;
        stopY = 0;
    }

    RS_DEBUG->print("RS_Image::draw: drawImg");
    painter->drawImg(img,
                     view->toGui(data.insertionPoint),
                     angle, scale,
                     startX, startY, stopX-startX, stopY-startY);
    RS_DEBUG->print("RS_Image::draw: drawImg: OK");

    if (isSelected()) {
        RS_VectorList sol = getCorners();

        painter->drawLine(view->toGui(sol.get(0)), view->toGui(sol.get(1)));
        painter->drawLine(view->toGui(sol.get(1)), view->toGui(sol.get(2)));
        painter->drawLine(view->toGui(sol.get(2)), view->toGui(sol.get(3)));
        painter->drawLine(view->toGui(sol.get(3)), view->toGui(sol.get(0)));
    }
    RS_DEBUG->print("RS_Image::draw: OK");
}



RS_Variant RS_Image::getProperty(const RS_String& name, const RS_Variant& def) {
    if (name==RS_IMAGE_INSERTIONPOINT_X) {
        return data.insertionPoint.x;
    } else if (name==RS_IMAGE_INSERTIONPOINT_Y) {
        return data.insertionPoint.y;
    } else if (name==RS_IMAGE_SCALE_X) {
        return data.uVector.magnitude();
    } else if (name==RS_IMAGE_SCALE_Y) {
        return data.vVector.magnitude();
    } else if (name==RS_IMAGE_SIZE_X) {
        return data.size.x;
    } else if (name==RS_IMAGE_SIZE_Y) {
        return data.size.y;
    } else if (name==RS_IMAGE_FILE) {
        return data.file;
    } else if (name==RS_IMAGE_ANGLE) {
        return RS_Math::rad2deg(data.uVector.angle());
    } else {
        return RS_AtomicEntity::getProperty(name, def);
    }
}



RS_StringList RS_Image::getPropertyNames(bool includeGeometry) {
    RS_StringList ret = RS_AtomicEntity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << RS_IMAGE_INSERTIONPOINT_X;
        ret << RS_IMAGE_INSERTIONPOINT_Y;
        ret << RS_IMAGE_SCALE_X;
        ret << RS_IMAGE_SCALE_Y;
        ret << RS_IMAGE_SIZE_X;
        ret << RS_IMAGE_SIZE_Y;
        ret << RS_IMAGE_ANGLE;
    }
    ret << RS_IMAGE_FILE;
    /*
    ret << RS_IMAGE_BRIGHTNESS;
    ret << RS_IMAGE_CONTRAST;
    ret << RS_IMAGE_FADE;
    */
    return ret;
}


void RS_Image::setProperty(const RS_String& name, const RS_Variant& value) {
    if (name==RS_IMAGE_INSERTIONPOINT_X) {
        data.insertionPoint.x = RS_Math::eval(value.toString());
        calculateBorders();
    } else if (name==RS_IMAGE_INSERTIONPOINT_Y) {
        data.insertionPoint.y = RS_Math::eval(value.toString());
        calculateBorders();
    } else if (name==RS_IMAGE_SCALE_X) {
        double len = fabs(RS_Math::eval(value.toString()));
        if (len<1.0e-6) {
            len = 1.0;
        }
        double angle = data.uVector.angle();
        data.uVector.setPolar(len, angle);
        calculateBorders();
    } else if (name==RS_IMAGE_SCALE_Y) {
        double len = fabs(RS_Math::eval(value.toString()));
        if (len<1.0e-6) {
            len = 1.0;
        }
        double angle = data.vVector.angle();
        data.vVector.setPolar(len, angle);
        calculateBorders();
    } else if (name==RS_IMAGE_SIZE_X) {
        data.size.x = RS_Math::eval(value.toString());
    } else if (name==RS_IMAGE_SIZE_Y) {
        data.size.y = RS_Math::eval(value.toString());
    } else if (name==RS_IMAGE_FILE) {
        data.file = value.toString();
    } else if (name==RS_IMAGE_ANGLE) {
        double len = data.uVector.magnitude();
        data.uVector.setPolar(len, 
            RS_Math::deg2rad(RS_Math::eval(value.toString())));
            
        len = data.vVector.magnitude();
        data.vVector.setPolar(len, 
            RS_Math::deg2rad(RS_Math::eval(value.toString()))+M_PI/2.0);
        calculateBorders();
    } else {
        RS_AtomicEntity::setProperty(name, value);
    }
}


