/****************************************************************************
** $Id: gv_image.cpp 7925 2008-01-17 18:01:07Z andrew $
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


#include "gv_image.h"

#include "gv_constructionline.h"
#include "rb_debug.h"
#include "gv_graphicview.h"
#include "gv_painter.h"
//#include "gv_painterqt.h"


const char* GV_Image::propertyInsertionPointX = QT_TRANSLATE_NOOP("QObject", "Position|X");
const char* GV_Image::propertyInsertionPointY = QT_TRANSLATE_NOOP("QObject", "Position|Y");
const char* GV_Image::propertyScaleX = QT_TRANSLATE_NOOP("QObject", "Scale|X");
const char* GV_Image::propertyScaleY = QT_TRANSLATE_NOOP("QObject", "Scale|Y");
const char* GV_Image::propertySizeX = QT_TRANSLATE_NOOP("QObject", "Size|X");
const char* GV_Image::propertySizeY = QT_TRANSLATE_NOOP("QObject", "Size|Y");
const char* GV_Image::propertyFile = QT_TRANSLATE_NOOP("QObject", "Image File");
const char* GV_Image::propertyBrightness = QT_TRANSLATE_NOOP("QObject", "Brightness");
const char* GV_Image::propertyContrast = QT_TRANSLATE_NOOP("QObject", "Contrast");
const char* GV_Image::propertyFade = QT_TRANSLATE_NOOP("QObject", "Fade");
const char* GV_Image::propertyAngle = QT_TRANSLATE_NOOP("QObject", "Angle");


/**
 * Constructor.
 */
GV_Image::GV_Image(GV_EntityContainer* parentContainer,
                   const GV_ImageData& d)
        : GV_Entity(parentContainer), GV_AtomicEntity(parentContainer), data(d) {

    update();
    calculateBorders();
}



/**
 * Destructor.
 */
GV_Image::~GV_Image() {
    /*if (img!=NULL) {
        delete[] img;
    }*/
}




GV_Entity* GV_Image::clone() {
    GV_Image* i = new GV_Image(parentContainer, data);
    i->copyPropertiesFrom(this);
    i->img = img;
    i->initId();
    i->update();
    return i;
}



void GV_Image::update() {

    RB_DEBUG->print("GV_Image::update");

    // the whole image:
    img = GV_Img(data.file);
    if (!img.isNull()) {
        data.size = GV_Vector(img.width(), img.height());
    }

    RB_DEBUG->print("GV_Image::update: OK");
}




GV_VectorList GV_Image::getRefPoints(GV2::RefMode /*refMode*/) {
    GV_VectorList ret = getCorners();
    return ret;
}


void GV_Image::calculateBorders(bool /*visibleOnly*/) {
    resetBorders();

    GV_VectorList sol = getCorners();

    for (int i=0; i<4; ++i) {
        minV = GV_Vector::minimum(minV, sol.get(i));
        maxV = GV_Vector::maximum(maxV, sol.get(i));
    }
}



GV_Vector GV_Image::getNearestEndpoint(const GV_Vector& coord,
                                       double* dist) {
    GV_VectorList corners = getCorners();
    return corners.getClosest(coord, dist);
}



GV_Vector GV_Image::getNearestPointOnEntity(const GV_Vector& coord,
        bool onEntity, double* dist, GV_Entity** entity) {

    if (entity!=NULL) {
        *entity = this;
    }

    GV_VectorList points;

    GV_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        points.set(i, l[i]->getNearestPointOnEntity(coord, onEntity));
    }
                
    deleteBoundaryRectangle(l);

    return points.getClosest(coord, dist);
}



GV_Vector GV_Image::getNearestCenter(const GV_Vector& coord,
                                     double* dist) {

    GV_VectorList points;
    GV_VectorList corners = getCorners();

    points.set(0, (corners.get(0) + corners.get(1))/2.0);
    points.set(1, (corners.get(1) + corners.get(2))/2.0);
    points.set(2, (corners.get(2) + corners.get(3))/2.0);
    points.set(3, (corners.get(3) + corners.get(0))/2.0);

    return points.getClosest(coord, dist);
}



GV_Vector GV_Image::getNearestMiddle(const GV_Vector& coord,
                                     double* dist) {
    return getNearestCenter(coord, dist);
}



GV_Vector GV_Image::getNearestDist(double distance,
                                   const GV_Vector& coord,
                                   double* dist) {

    GV_VectorList points;
    
    GV_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        points.set(i, l[i]->getNearestDist(distance, coord, dist));
    }

    deleteBoundaryRectangle(l);

    return points.getClosest(coord, dist);
}



double GV_Image::getDistanceToPoint(const GV_Vector& coord,
                                    GV_Entity** entity,
                                    GV2::ResolveLevel /*level*/,
                                    double /*solidDist*/,
                                    bool /*visually*/) {
    if (entity!=NULL) {
        *entity = this;
    }

    double dist;
    double minDist = GV_MAXDOUBLE;

    GV_Line** l = createBoundaryRectangle();

    for (int i=0; i<4; ++i) {
        dist = l[i]->getDistanceToPoint(coord, NULL);
        if (dist<minDist) {
            minDist = dist;
        }
    }

    deleteBoundaryRectangle(l);

    return minDist;
}



void GV_Image::move(const GV_Vector& offset) {
    data.insertionPoint.move(offset);
    calculateBorders();
}



void GV_Image::rotate(const GV_Vector& center, double angle) {
    data.insertionPoint.rotate(center, angle);
    data.uVector.rotate(angle);
    data.vVector.rotate(angle);
    calculateBorders();
}



void GV_Image::scale(const GV_Vector& center, const GV_Vector& factor) {
    data.insertionPoint.scale(center, factor);
    data.uVector.scale(factor);
    data.vVector.scale(factor);
    calculateBorders();
}



void GV_Image::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    data.insertionPoint.mirror(axisPoint1, axisPoint2);
    data.uVector.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    data.vVector.mirror(GV_Vector(0.0,0.0), axisPoint2-axisPoint1);
    calculateBorders();
}


void GV_Image::moveRef(const GV_Vector& ref, const GV_Vector& offset) {
    GV_VectorList corners = getCorners();
    if (ref.distanceTo(corners[0])<1.0e-4 ||
        ref.distanceTo(corners[1])<1.0e-4 ||
        ref.distanceTo(corners[2])<1.0e-4 ||
        ref.distanceTo(corners[3])<1.0e-4) {
        move(offset);
    } 
}



void GV_Image::draw(GV_Painter* painter, GV_GraphicView* view, double /*patternOffset*/) {
    RB_DEBUG->print("GV_Image::draw");

    if (painter==NULL || view==NULL || img.isNull()) {
        return;
    }

    int ox = 0;
    int oy = 0;
    int width = view->getWidth();
    int height = view->getHeight();

    GV_Vector scale = GV_Vector(view->toGuiDX(data.uVector.magnitude()),
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

    RB_DEBUG->print("GV_Image::draw: drawImg");
    painter->drawImg(img,
                     view->toGui(data.insertionPoint),
                     angle, scale,
                     startX, startY, stopX-startX, stopY-startY);
    RB_DEBUG->print("GV_Image::draw: drawImg: OK");

    if (isSelected()) {
        GV_VectorList sol = getCorners();

        painter->drawLine(view->toGui(sol.get(0)), view->toGui(sol.get(1)));
        painter->drawLine(view->toGui(sol.get(1)), view->toGui(sol.get(2)));
        painter->drawLine(view->toGui(sol.get(2)), view->toGui(sol.get(3)));
        painter->drawLine(view->toGui(sol.get(3)), view->toGui(sol.get(0)));
    }
    RB_DEBUG->print("GV_Image::draw: OK");
}



QVariant GV_Image::getProperty(const QString& name, const QVariant& def) {
    if (name==GV_IMAGE_INSERTIONPOINT_X) {
        return data.insertionPoint.x;
    } else if (name==GV_IMAGE_INSERTIONPOINT_Y) {
        return data.insertionPoint.y;
    } else if (name==GV_IMAGE_SCALE_X) {
        return data.uVector.magnitude();
    } else if (name==GV_IMAGE_SCALE_Y) {
        return data.vVector.magnitude();
    } else if (name==GV_IMAGE_SIZE_X) {
        return data.size.x;
    } else if (name==GV_IMAGE_SIZE_Y) {
        return data.size.y;
    } else if (name==GV_IMAGE_FILE) {
        return data.file;
    } else if (name==GV_IMAGE_ANGLE) {
        return GV_Math::rad2deg(data.uVector.angle());
    } else {
        return GV_AtomicEntity::getProperty(name, def);
    }
}



QStringList GV_Image::getPropertyNames(bool includeGeometry) {
    QStringList ret = GV_AtomicEntity::getPropertyNames(includeGeometry);
    if (includeGeometry) {
        ret << GV_IMAGE_INSERTIONPOINT_X;
        ret << GV_IMAGE_INSERTIONPOINT_Y;
        ret << GV_IMAGE_SCALE_X;
        ret << GV_IMAGE_SCALE_Y;
        ret << GV_IMAGE_SIZE_X;
        ret << GV_IMAGE_SIZE_Y;
        ret << GV_IMAGE_ANGLE;
    }
    ret << GV_IMAGE_FILE;
    /*
    ret << GV_IMAGE_BRIGHTNESS;
    ret << GV_IMAGE_CONTRAST;
    ret << GV_IMAGE_FADE;
    */
    return ret;
}


void GV_Image::setProperty(const QString& name, const QVariant& value) {
    if (name==GV_IMAGE_INSERTIONPOINT_X) {
        data.insertionPoint.x = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_IMAGE_INSERTIONPOINT_Y) {
        data.insertionPoint.y = GV_Math::eval(value.toString());
        calculateBorders();
    } else if (name==GV_IMAGE_SCALE_X) {
        double len = fabs(GV_Math::eval(value.toString()));
        if (len<1.0e-6) {
            len = 1.0;
        }
        double angle = data.uVector.angle();
        data.uVector.setPolar(len, angle);
        calculateBorders();
    } else if (name==GV_IMAGE_SCALE_Y) {
        double len = fabs(GV_Math::eval(value.toString()));
        if (len<1.0e-6) {
            len = 1.0;
        }
        double angle = data.vVector.angle();
        data.vVector.setPolar(len, angle);
        calculateBorders();
    } else if (name==GV_IMAGE_SIZE_X) {
        data.size.x = GV_Math::eval(value.toString());
    } else if (name==GV_IMAGE_SIZE_Y) {
        data.size.y = GV_Math::eval(value.toString());
    } else if (name==GV_IMAGE_FILE) {
        data.file = value.toString();
    } else if (name==GV_IMAGE_ANGLE) {
        double len = data.uVector.magnitude();
        data.uVector.setPolar(len, 
            GV_Math::deg2rad(GV_Math::eval(value.toString())));
            
        len = data.vVector.magnitude();
        data.vVector.setPolar(len, 
            GV_Math::deg2rad(GV_Math::eval(value.toString()))+M_PI/2.0);
        calculateBorders();
    } else {
        GV_AtomicEntity::setProperty(name, value);
    }
}


