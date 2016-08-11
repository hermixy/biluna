/****************************************************************************
** $Id: gv_image.h 7898 2008-01-15 17:26:19Z andrew $
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


#ifndef GV_IMAGE_H
#define GV_IMAGE_H

#include "gv_atomicentity.h"

#include "gv_img.h"
#include "gv_line.h"

#define GV_IMAGE_INSERTIONPOINT_X QObject::tr(GV_Image::propertyInsertionPointX)
#define GV_IMAGE_INSERTIONPOINT_Y QObject::tr(GV_Image::propertyInsertionPointY)
#define GV_IMAGE_SCALE_X QObject::tr(GV_Image::propertyScaleX)
#define GV_IMAGE_SCALE_Y QObject::tr(GV_Image::propertyScaleY)
#define GV_IMAGE_SIZE_X QObject::tr(GV_Image::propertySizeX)
#define GV_IMAGE_SIZE_Y QObject::tr(GV_Image::propertySizeY)
#define GV_IMAGE_FILE QObject::tr(GV_Image::propertyFile)
#define GV_IMAGE_BRIGHTNESS QObject::tr(GV_Image::propertyBrightness)
#define GV_IMAGE_CONTRAST QObject::tr(GV_Image::propertyContrast)
#define GV_IMAGE_FADE QObject::tr(GV_Image::propertyFade)
#define GV_IMAGE_ANGLE QObject::tr(GV_Image::propertyAngle)

/**
 * Holds the data that defines an image.
 */
class GV_ImageData {
public:
    /**
     * Default constructor. Leaves the data object uninitialized.
     */
    GV_ImageData() {}

    GV_ImageData(int handle,
                const GV_Vector& insertionPoint,
                const GV_Vector& uVector,
                const GV_Vector& vVector,
                const GV_Vector& size,
                const QString& file,
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
    GV_Vector insertionPoint;
    /** u vector. Points along visual bottom of image. */
    GV_Vector uVector;
    /** v vector. Points along visual left of image. */
    GV_Vector vVector;
    /** Image size in pixel. */
    GV_Vector size;
    /** Path to image file. */
    QString file;
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
class GV_Image : public GV_AtomicEntity {
public:
    GV_Image(GV_EntityContainer* parentContainer,
            const GV_ImageData& d);

    virtual GV_Entity* clone();

    virtual ~GV_Image();

    /**    @return GV2::EntityImage */
    virtual GV2::EntityType rtti() const {
        return GV2::EntityImage;
    }
    virtual bool isOfType(GV2::EntityType t) const {
        return (t==GV2::EntityImage || GV_AtomicEntity::isOfType(t));
    }
    virtual QString typeName() const {
        return QObject::tr("Image");
    }

    virtual void update();
    
    virtual GV_VectorList getRefPoints(GV2::RefMode refMode=GV2::RefAll);

    /** @return Copy of data that defines the image. */
    GV_ImageData getData() const {
        return data;
    }

    /** @return Insertion point of the entity */
    virtual GV_Vector getInsertionPoint() const {
        return data.insertionPoint;
    }
    /** Sets the insertion point for the image. */
    void setInsertionPoint(const GV_Vector& ip) {
        data.insertionPoint = ip;
        calculateBorders();
    }

    /** @return File name of the image. */
    QString getFile() const {
        return data.file;
    }
    
    /** Sets the file name of the image.  */
    void setFile(const QString& file) {
        data.file = file;
    }

    /** @return u Vector. Points along bottom, 1 pixel long. */
    GV_Vector getUVector() const {
        return data.uVector;
    }
    /** @return v Vector. Points along left, 1 pixel long. */
    GV_Vector getVVector() const {
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
    GV_VectorList getCorners() {
        GV_VectorList sol;

        sol.set(0, data.insertionPoint);
        sol.set(1, 
            data.insertionPoint + data.uVector*GV_Math::mround(data.size.x));
        sol.set(3, 
            data.insertionPoint + data.vVector*GV_Math::mround(data.size.y));
        sol.set(2, sol.get(3) + data.uVector*GV_Math::mround(data.size.x));

        return sol;
    }

    GV_Line** createBoundaryRectangle() {
        GV_VectorList corners = getCorners();
        GV_Line** l = new GV_Line*[4];
        l[0] = new GV_Line(NULL, GV_LineData(corners.get(0), corners.get(1)));
        l[1] = new GV_Line(NULL, GV_LineData(corners.get(1), corners.get(2)));
        l[2] = new GV_Line(NULL, GV_LineData(corners.get(2), corners.get(3)));
        l[3] = new GV_Line(NULL, GV_LineData(corners.get(3), corners.get(0)));
        return l;
    }

    void deleteBoundaryRectangle(GV_Line** l) {
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


    virtual GV_Vector getNearestEndpoint(const GV_Vector& coord,
                                         double* dist = NULL);
    virtual GV_Vector getNearestPointOnEntity(const GV_Vector& coord,
            bool onEntity=true, double* dist = NULL, GV_Entity** entity=NULL);
    virtual GV_Vector getNearestCenter(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestMiddle(const GV_Vector& coord,
                                       double* dist = NULL);
    virtual GV_Vector getNearestDist(double distance,
                                     const GV_Vector& coord,
                                     double* dist = NULL);
    virtual double getDistanceToPoint(const GV_Vector& coord,
                                      GV_Entity** entity=NULL,
                                      GV2::ResolveLevel level=GV2::ResolveNone,
                                      double solidDist = GV_MAXDOUBLE,
                                      bool visually = false);
    virtual GV2::Side getSideOfPoint(const GV_Vector& ) {
        return GV2::NoSide;
    }

    virtual double getLength() {
        return -1.0;
    }

    virtual void move(const GV_Vector& offset);
    virtual void rotate(const GV_Vector& center, double angle);
    virtual void scale(const GV_Vector& center, const GV_Vector& factor);
    virtual void mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    /*virtual void stretch(const GV_Vector& firstCorner,
                         const GV_Vector& secondCorner,
                         const GV_Vector& offset);*/
    virtual void moveRef(const GV_Vector& ref, const GV_Vector& offset);

    virtual void draw(GV_Painter* painter, GV_GraphicView* view, double patternOffset=0.0);

    virtual void calculateBorders(bool visibleOnly=false);
    
    virtual QVariant getProperty(const QString& name, 
        const QVariant& def=QVariant());
    virtual QStringList getPropertyNames(bool includeGeometry=true);
    virtual void setProperty(const QString& name, const QVariant& value);

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
    GV_ImageData data;
    GV_Img img;
};

#endif
