/****************************************************************************
** $Id: gv_vector.h 7519 2007-12-07 10:26:06Z andrew $
**
** Copyright (C) 2001-2007 RibbonSoft. All rights reserved.
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
** Updated for Biluna 2016-08-11
**********************************************************************/


#ifndef GV_VECTOR_H
#define GV_VECTOR_H

#include <iostream>
#include <QList>

#include "gv.h"

class GV_Matrix;


/**
 * Represents a 3d vector (x/y/z)
 *
 * @author Andrew Mustun
 */
class GV_Vector {
public:
    inline GV_Vector();
    inline GV_Vector(double vx, double vy, double vz=0.0, bool valid=true);
    inline explicit GV_Vector(bool valid_in);
    //~GV_Vector();

    inline void set(double vx, double vy, double vz=0.0) {
        x = vx;
        y = vy;
        z = vz;
        valid = true;
    }
    void setPolar(double radius, double angle);

    bool isValid() const;

    double distanceTo(const GV_Vector& v) const;
    double distanceTo2d(const GV_Vector& v) const;
    double angle() const;
    double angleToPlaneXY() const;
    double angleTo(const GV_Vector& v) const;
    double magnitude() const;
    double magnitude2d() const;
    GV_Vector lerp(const GV_Vector& v, double t) const;

    bool isInWindow(const GV_Vector& firstCorner, const GV_Vector& secondCorner);

    GV_Vector move(const GV_Vector& offset);
    GV_Vector rotate(double ang);
    GV_Vector rotate(const GV_Vector& center, double ang);
    GV_Vector scale(double factor);
    GV_Vector scale(const GV_Vector& factor);
    GV_Vector scale(const GV_Vector& center, double factor);
    GV_Vector scale(const GV_Vector& center, const GV_Vector& factor);
    GV_Vector mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2);
    GV_Vector isoProject(GV2::IsoProjectionType type);
    GV_Vector transform(const GV_Matrix& m);

    GV_Vector operator + (const GV_Vector& v) const;
    GV_Vector operator - (const GV_Vector& v) const;
    GV_Vector operator * (double s) const;
    GV_Vector operator / (double s) const;
    GV_Vector operator - () const;

    void operator += (const GV_Vector& v);
    void operator -= (const GV_Vector& v);
    void operator *= (double s);

    bool operator == (const GV_Vector& v) const;
    bool operator != (const GV_Vector& v) const {
        return !operator==(v);
    }

    static GV_Vector minimum(const GV_Vector& v1, const GV_Vector& v2);
    static GV_Vector maximum(const GV_Vector& v1, const GV_Vector& v2);
    static GV_Vector crossP(const GV_Vector& v1, const GV_Vector& v2);
    static double dotP(const GV_Vector& v1, const GV_Vector& v2);
    static GV_Vector polar(double radius, double angle) {
        GV_Vector ret;
        ret.setPolar(radius, angle);
        return ret;
    }

public:
    double x;
    double y;
    double z;
    bool valid;
};


#ifdef GV_COMP
#define GV_VectorSolutions GV_VectorList
#endif

/**
 * Represents a list of vectors. Typically used to return multiple
 * solutions from a function. Previously known as GV_VectorSolutions.
 */
class GV_VectorList : public QList<GV_Vector> {
public:
    GV_VectorList();
    //GV_VectorList(const GV_VectorList& s);
#ifdef GV_COMP
    GV_VectorList(int num);
#endif
    GV_VectorList(const GV_Vector& v1);
    GV_VectorList(const GV_Vector& v1, const GV_Vector& v2);
    GV_VectorList(const GV_Vector& v1, const GV_Vector& v2,
                       const GV_Vector& v3);
    GV_VectorList(const GV_Vector& v1, const GV_Vector& v2,
                       const GV_Vector& v3, const GV_Vector& v4);
    GV_VectorList(const GV_Vector& v1, const GV_Vector& v2,
                       const GV_Vector& v3, const GV_Vector& v4,
                       const GV_Vector& v5);
    
    ~GV_VectorList();

#ifdef GV_COMP
    void alloc(int num);
    void clean();
#endif
    void clear();
    GV_Vector get(int i) const;
#ifdef GV_COMP
    int getNumber() const;
#endif
    bool hasValid() const;
    void set(int i, const GV_Vector& v);
    void setTangent(bool t);
    bool isTangent() const;
    GV_Vector getClosest(const GV_Vector& coord,
            double* dist=NULL, int* index=NULL) const;
    void move(const GV_Vector& offset);
    void rotate(const GV_Vector& center, double ang);
    void scale(const GV_Vector& center, const GV_Vector& factor);

    void sortByDistanceTo(const GV_Vector& v, bool unique=false);

    GV_VectorList operator + (const GV_VectorList& s1);

private:
    //GV_Vector* vector;
    bool tangent;
};


/**
 * Constructor for a point with default coordinates.
 */
inline GV_Vector::GV_Vector() :
   x(0.0), y(0.0), z(0.0), valid(true)
{}

/**
 * Constructor for a point with given valid flag.
 *
 * @param valid true: a valid vector with default coordinates is created.
 *              false: an invalid vector is created
 */
inline GV_Vector::GV_Vector(bool valid_in) :
   x(0.0), y(0.0), z(0.0), valid(valid_in)
{}

/**
 * Constructor for a point with given coordinates.
 */
inline GV_Vector::GV_Vector(double vx, double vy, double vz, bool valid_in)  :
   x(vx), y(vy), z(vz), valid(valid_in)
{}

#endif
