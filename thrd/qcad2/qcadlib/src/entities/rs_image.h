/****************************************************************************
** $Id: rs_image.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef RS_IMAGE_H
#define RS_IMAGE_H

#include "rs_atomicentity.h"

#include "rs_img.h"
#include "rs_line.h"

#define RS_IMAGE_INSERTIONPOINT_X QObject::tr(RS_Image::propertyInsertionPointX)
#define RS_IMAGE_INSERTIONPOINT_Y QObject::tr(RS_Image::propertyInsertionPointY)
#define RS_IMAGE_SCALE_X QObject::tr(RS_Image::propertyScaleX)
#define RS_IMAGE_SCALE_Y QObject::tr(RS_Image::propertyScaleY)
#define RS_IMAGE_SIZE_X QObject::tr(RS_Image::propertySizeX)
#define RS_IMAGE_SIZE_Y QObject::tr(RS_Image::propertySizeY)
#define RS_IMAGE_FILE QObject::tr(RS_Image::propertyFile)
#define RS_IMAGE_BRIGHTNESS QObject::tr(RS_Image::propertyBrightness)
#define RS_IMAGE_CONTRAST QObject::tr(RS_Image::propertyContrast)
#define RS_IMAGE_FADE QObject::tr(RS_Image::propertyFade)
#define RS_IMAGE_ANGLE QObject::tr(RS_Image::propertyAngle)

/**
 * Holds the data that defines an image.
 */
class RS_ImageData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    RS_ImageData() {}

    RS_ImageData(int handle,
                const RS_Vector& insertionPoint,
                const RS_Vector& uVector,
                const RS_Vector& vVector,
                const RS_Vector& size,
                const RS_String& file,
                int brightness,
                int contrast,
                int fade) {

        this->handle = handle;
        this->insertionPoint = insertionPoint;
        this->uVector = uVector;
        this->vVector = vVector;
        this->size = size;
        this->file = file;
        this->brightness = brightness;
        this->contrast = contrast;
        this->fade = fade;
    }

public:
    /** Handle of image definition. */
    int handle;
    /** Insertion point. */
    RS_Vector insertionPoint;
    /** u vector. Points along visual bottom of image. */
    RS_Vector uVector;
    /** v vector. Points along visual left of image. */
    RS_Vector vVector;
    /** Image size in pixel. */
    RS_Vector size;
    /** Path to image file. */
    RS_String file;
    /** Brightness (0..100, default: 50). */
    int brightness;
    /** Contrast (0..100, default: 50). */
    int contrast;
    /** Fade (0..100, default: 0). */
    int fade;
};



/**
 * Class for an image entity.
 *
 * @author Andrew Mustun
 */
class RS_Image : public RS_AtomicEntity {
public:
    RS_Image(RS_EntityContainer* parentContainer,
            const RS_ImageData& d);

    virtual RS_Entity* clone();

    virtual ~RS_Image();

    /**    @return RS2::EntityImage */
    virtual RS2::EntityType rtti() const {
        return RS2::EntityImage;
    }
    virtual bool isOfType(RS2::EntityType t) const {
        return (t==RS2::EntityImage || RS_AtomicEntity::isOfType(t));
    }
    virtual RS_String typeName() const {
        return QObject::tr("Image");
    }

    virtual void update();
    
    virtual RS_VectorList getRefPoints(RS2::RefMode refMode=RS2::RefAll);

    /** @return Copy of data that defines the image. */
    RS_ImageData getData() const {
        return data;
    }

    /** @return Insertion point of the entity */
    virtual RS_Vector getInsertionPoint() const {
        return data.insertionPoint;
    }
    /** Sets the insertion point for the image. */
    void setInsertionPoint(const RS_Vector& ip) {
        data.insertionPoint = ip;
        calculateBorders();
    }

    /** @return File name of the image. */
    RS_String getFile() const {
        return data.file;
    }
    
    /** Sets the file name of the image.  */
    void setFile(const RS_String& file) {
        data.file = file;
    }

    /** @return u Vector. Points along bottom, 1 pixel long. */
    RS_Vector getUVector() const {
        return data.uVector;
    }
    /** @return v Vector. Points along left, 1 pixel long. */
    RS_Vector getVVector() const {
        return data.vVector;
    }
    /** @return Width of image in pixels. */
    int getWidth() const {
        return (int)data.size.x;
    }
    /** @return Height of image in pixels. */
    int getHeight() const {
        return (int)data.size.y;
    }
    /** @return Brightness. */
    int getBrightness() const {
        return data.brightness;
    }
    /** @return Contrast. */
    int getContrast() const {
        return data.contrast;
    }
    /** @return Fade. */
    int getFade() const {
        return data.fade;
    }
    /** @return Image definition handle. */
    int getHandle() const {
        return data.handle;
    }
    /** Sets the image definition handle. */
    void setHandle(int h) {
        data.handle = h;
    }
    

    /** @return The four corners. **/
    RS_VectorList getCorners() {
        RS_VectorList sol;

        sol.set(0, data.insertionPoint);
        sol.set(1, 
            data.insertionPoint + data.uVector*RS_Math::mround(data.size.x));
        sol.set(3, 
            data.insertionPoint + data.vVector*RS_Math::mround(data.size.y));
        sol.set(2, sol.get(3) + data.uVector*RS_Math::mround(data.size.x));

        return sol;
    }

    RS_Line** createBoundaryRectangle() {
        RS_VectorList corners = getCorners();
        RS_Line** l = new RS_Line*[4];
        l[0] = new RS_Line(NULL, RS_LineData(corners.get(0), corners.get(1)));
        l[1] = new RS_Line(NULL, RS_LineData(corners.get(1), corners.get(2)));
        l[2] = new RS_Line(NULL, RS_LineData(corners.get(2), corners.get(3)));
        l[3] = new RS_Line(NULL, RS_LineData(corners.get(3), corners.get(0)));
        return l;
    }

    void deleteBoundaryRectangle(RS_Line** l) {
        for (int i=0; i<4; ++i) {
            delete l[i];
        }
        delete[] l;
    }

    /**
     * @return image with in graphic units.
     */
    double getImageWidth() {
        return data.size.x * data.uVector.magnitude();
    }
    
    /**
     * @return image height in graphic units.
     */
    double getImageHeight() {
        return data.size.y * data.vVector.magnitude();
    }


    virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                         double* dist = NULL);
    virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
            bool onEntity=true, double* dist = NULL, RS_Entity** entity=NULL);
    virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                       double* dist = NULL);
    virtual RS_Vector getNearestDist(double distance,
                                     const RS_Vector& coord,
                                     double* dist = NULL);
    virtual double getDistanceToPoint(const RS_Vector& coord,
                                      RS_Entity** entity=NULL,
                                      RS2::ResolveLevel level=RS2::ResolveNone,
                                      double solidDist = RS_MAXDOUBLE,
                                      bool visually = false);
    virtual RS2::Side getSideOfPoint(const RS_Vector& ) {
        return RS2::NoSide;
    }

    virtual double getLength() {
        return -1.0;
    }

    virtual void move(const RS_Vector& offset);
    virtual void rotate(const RS_Vector& center, double angle);
    virtual void scale(const RS_Vector& center, const RS_Vector& factor);
    virtual void mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2);
    /*virtual void stretch(const RS_Vector& firstCorner,
                         const RS_Vector& secondCorner,
                         const RS_Vector& offset);*/
    virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

    virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual RS_Variant getProperty(const RS_String& name, 
        const RS_Variant& def=RS_Variant());
    virtual RS_StringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const RS_String& name, const RS_Variant& value);

    static const char* propertyInsertionPointX;
    static const char* propertyInsertionPointY;
    static const char* propertyScaleX;
    static const char* propertyScaleY;
    static const char* propertySizeX;
    static const char* propertySizeY;
    static const char* propertyFile;
    static const char* propertyBrightness;
    static const char* propertyContrast;
    static const char* propertyFade;
    static const char* propertyAngle;

protected:
    RS_ImageData data;
    RS_Img img;
};

#endif
