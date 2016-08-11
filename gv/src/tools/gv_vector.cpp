/****************************************************************************
** $Id: gv_vector.cpp 10447 2008-04-10 12:03:19Z andrew $
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
** Updated for Biluna 2016-08-11
**********************************************************************/


#include "gv_vector.h"

#include "rb_debug.h"
#include "gv_math.h"
#include "gv_matrix.h"
//#include "gv_constructionline.h"


/*
#ifndef isnan
static int isnan(const double x) {
  unsigned int e_mask, f_mask;
  double as_double = x;
  unsigned int *as_int;

  vtkByteSwap::Swap8BE(&as_double);
  as_int = (unsigned int *)&as_double;
  vtkByteSwap::Swap4BE(as_int);

  e_mask = 0x7ff0;
  e_mask <<= 16;

  if ((*as_int & e_mask) != e_mask) {
    return 0;
  }

  f_mask = 1;
  f_mask <<= 20;
  f_mask--;

  return (*as_int & f_mask) != 0;
}
#endif
*/



/**
 * Destructor.
 */
//GV_Vector::~GV_Vector() {
//}
    
    
bool GV_Vector::isValid() const {
#if defined(Q_OS_SOLARIS) || defined(Q_OS_MAC)
    return valid;
#else
    return valid && !isnan(x) && !isnan(y) && !isnan(z);
#endif
}


/**
 * Sets a new position for the vector in polar coordinates.
 */
void GV_Vector::setPolar(double radius, double angle) {
    x = radius * cos(angle);
    y = radius * sin(angle);
    z = 0.0;
    valid = true;
}



/**
 * @return The angle from zero to this vector (in rad).
 */
double GV_Vector::angle() const {
    double ret = 0.0;
    double m = magnitude2d();

    if (m>1.0e-6) {
        double dp = dotP(*this, GV_Vector(1.0, 0.0));
        if (dp/m>=1.0) {
            ret = 0.0;
        }
        else if (dp/m<-1.0) {
            ret = M_PI;
        }
        else {
            ret = acos( dp / m);
        }
        if (y<0.0) {
            ret = 2*M_PI - ret;
        }
    }
    return ret;
}
    
    
    
double GV_Vector::angleToPlaneXY() const {
    GV_Vector n(0,0,1);

    if (magnitude()<1.0e-4) {
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: magnitude is 0");
        return M_PI/2;
    }
    else if ((dotP(*this, n) / (magnitude() * 1)) > 1.0) {
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: value for cos is > 1");
        return 0.0;
    }
    else {
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: "
//            "%f,%f,%f", x, y, z);
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: "
//            "dotp: %f", dotP(*this, n));
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: "
//            "magnitude: %f", magnitude());
//        RB_DEBUG->print("GV_Vector::angleToPlaneXY: "
//            "acos: %f", acos(dotP(*this, n) / (magnitude() * 1)));

        return M_PI/2 - acos(dotP(*this, n) / (magnitude() * 1));
    }
}



/**
 * @return The angle from this and the given coordinate (in rad).
 */
double GV_Vector::angleTo(const GV_Vector& v) const {
    if (!valid || !v.valid) {
        return 0.0;
    }
    else {
        return (v-(*this)).angle();
    }
}



/**
 * @return Magnitude (length) of the vector.
 */
double GV_Vector::magnitude() const {
    double ret = 0.0;
    // Note that the z coordinate is also needed for 2d
    //   (due to definition of crossP())
    if (!valid) {
        ret = 0.0;
    }
    else {
        ret = sqrt(GV_Math::pow(x,2) + GV_Math::pow(y,2) + GV_Math::pow(z,2));
    }

    return ret;
}



/**
 * @return Magnitude (length) of the vector in x/y (2d).
 */
double GV_Vector::magnitude2d() const {
    double ret = 0.0;
    if (!valid) {
        ret = 0.0;
    }
    else {
        ret = sqrt(GV_Math::pow(x,2) + GV_Math::pow(y,2));
    }

    return ret;
}



/** 
 * 
 */
GV_Vector GV_Vector::lerp(const GV_Vector& v, double t) const {
    return GV_Vector(x+(v.x-x)*t, y+(v.y-y)*t);
}


/**
 * @return The distance between this and the given coordinate.
 */
double GV_Vector::distanceTo(const GV_Vector& v) const {
    if (!valid || !v.valid) {
        return GV_MAXDOUBLE;
    }
    else {
        return (*this-v).magnitude();
    }
}



/**
 * @return The distance between this and the given coordinate on the XY plane.
 */
double GV_Vector::distanceTo2d(const GV_Vector& v) const {
    if (!valid || !v.valid) {
        return GV_MAXDOUBLE;
    }
    else {
        return (*this-v).magnitude2d();
    }
}



/**
 * @return true is this vector is within the given range.
 */
bool GV_Vector::isInWindow(const GV_Vector& firstCorner,
        const GV_Vector& secondCorner) {

    double minX = std::min(firstCorner.x, secondCorner.x);
    double maxX = std::max(firstCorner.x, secondCorner.x);
    double minY = std::min(firstCorner.y, secondCorner.y);
    double maxY = std::max(firstCorner.y, secondCorner.y);

    return (x>=minX && x<=maxX && y>=minY && y<=maxY);
}



/**
 * Moves this vector by the given offset. Equal to the operator +=.
 */
GV_Vector GV_Vector::move(const GV_Vector& offset) {
    *this+=offset;
    return *this;
}



/**
 * Rotates this vector around 0/0 by the given angle.
 */
GV_Vector GV_Vector::rotate(double ang) {
    double r = magnitude2d();
    double a = angle() + ang;
    
    x = cos(a) * r;
    y = sin(a) * r;

    return *this;
}


/**
 * Rotates this vector around the given center by the given angle.
 */
GV_Vector GV_Vector::rotate(const GV_Vector& center, double ang) {
    *this = center + (*this-center).rotate(ang);
    return *this;
}



/**
 * Scales this vector by the given factor with 0/0/0 as center.
 */
GV_Vector GV_Vector::scale(double factor) {
    return scale(GV_Vector(factor, factor, factor));
}


/**
 * Scales this vector by the given factors with 0/0/0 as center.
 */
GV_Vector GV_Vector::scale(const GV_Vector& factor) {
    x *= factor.x;
    y *= factor.y;
    // 20071207: scale z as well
    z *= factor.z;
    return *this;
}



/**
 * Scales this vector by the given factor with the given center.
 */
GV_Vector GV_Vector::scale(const GV_Vector& center, double factor) {
    return scale(center, GV_Vector(factor, factor, factor));
}


/**
 * Scales this vector by the given factors with the given center.
 */
GV_Vector GV_Vector::scale(const GV_Vector& center, const GV_Vector& factor) {
    *this = center + (*this-center).scale(factor);
    return *this;
}



/**
 * Mirrors this vector at the given axis.
 */
GV_Vector GV_Vector::mirror(const GV_Vector& axisPoint1, const GV_Vector& axisPoint2) {
    double phi1 = axisPoint1.angleTo(*this);
    double phi2 = axisPoint1.angleTo(axisPoint2) - phi1;
    double r1 = axisPoint1.distanceTo(*this);
    double r2 = axisPoint2.distanceTo(*this);

    if (r1<1.0e-6 || r2<1.0e-6) {
        // point touches one axis point
    }
    else {
        setPolar(r1, phi1 + 2*phi2);
        (*this) += axisPoint1;
    }

    return *this;
}
    
    
    
/**
 * Changes this vector into its isometric projection.
 */
GV_Vector GV_Vector::isoProject(GV2::IsoProjectionType type) {
    static GV_Matrix iso =
        GV_Matrix::create3x3(
            sqrt(3.0),       0.0, -sqrt(3.0),
                 1.0,        2.0,       1.0,
            sqrt(2.0), -sqrt(2.0), sqrt(2.0))
        * (1.0 / sqrt(6.0));
        
    GV_Matrix input;
    switch (type) {
    case GV2::IsoSide:
        input = GV_Matrix::create3x1(
            x,
            y,
            0.0);
        break;
    case GV2::IsoTop:
        input = GV_Matrix::create3x1(
            y,
            0.0,
            -x);
        break;
    case GV2::IsoFront:
        input = GV_Matrix::create3x1(
            0.0,
            y,
            -x);
        break;
    }

    GV_Matrix res = iso * input;
    
    x = res.get(0,0);
    y = res.get(1,0);

    return *this;
}



/**
 * binary + operator.
 */
GV_Vector GV_Vector::operator + (const GV_Vector& v) const {
    return GV_Vector(x + v.x, y + v.y, z + v.z, valid && v.valid);
}



/**
 * binary - operator.
 */
GV_Vector GV_Vector::operator - (const GV_Vector& v) const {
    return GV_Vector(x - v.x, y - v.y, z - v.z, valid && v.valid);
}


/**
 * binary * operator.
 */
GV_Vector GV_Vector::operator * (double s) const {
    return GV_Vector(x * s, y * s, z * s, valid);
}



/**
 * binary / operator.
 */
GV_Vector GV_Vector::operator / (double s) const {
    return GV_Vector(x / s, y / s, z / s, valid);
}



/**
 * unary - operator.
 */
GV_Vector GV_Vector::operator - () const {
    return GV_Vector(-x, -y, -z, valid);
}



/**
 * Scalarproduct (dot product).
 */
double GV_Vector::dotP(const GV_Vector& v1, const GV_Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



/**
 * += operator. Assert: both vectors must be valid.
 */
void GV_Vector::operator += (const GV_Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    valid = valid && v.valid;
}


/**
 * -= operator
 */
void GV_Vector::operator -= (const GV_Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    valid = valid && v.valid;
}



/**
 * *= operator
 */
void GV_Vector::operator *= (double s) {
    x *= s;
    y *= s;
    z *= s;
}



/**
 * == operator
 */
bool GV_Vector::operator == (const GV_Vector& v) const {
    return (x==v.x && y==v.y && z==v.z && valid==v.valid);
}



/**
 * @return A vector with the minimum components from the vectors v1 and v2.
 * These might be mixed components from both vectors.
 */
GV_Vector GV_Vector::minimum (const GV_Vector& v1, const GV_Vector& v2) {
    return GV_Vector (std::min(v1.x, v2.x),
                      std::min(v1.y, v2.y),
                      std::min(v1.z, v2.z),
                      v1.valid && v2.valid);
}



/**
 * @return A vector with the maximum values from the vectors v1 and v2
 */
GV_Vector GV_Vector::maximum (const GV_Vector& v1, const GV_Vector& v2) {
    return GV_Vector (std::max(v1.x, v2.x),
                      std::max(v1.y, v2.y),
                      std::max(v1.z, v2.z),
                      v1.valid && v2.valid);
}



/**
 * @return Cross product of two vectors.
 */
GV_Vector GV_Vector::crossP(const GV_Vector& v1, const GV_Vector& v2) {
    return GV_Vector(v1.y*v2.z - v1.z*v2.y,
                     v1.z*v2.x - v1.x*v2.z,
                     v1.x*v2.y - v1.y*v2.x,
                     v1.valid && v2.valid);
}
    
    
/**
 * Generic 3d transformation. m must be a 3x3 matrix.
 */
GV_Vector GV_Vector::transform(const GV_Matrix& m) {
    GV_Matrix input;

    input = GV_Matrix::create3x1(x, y, z);
    GV_Matrix res = m * input;
    x = res.get(0,0);
    y = res.get(1,0);
    z = res.get(2,0);

    return *this;
}


/**
 * Constructor for no solution.
 */
GV_VectorList::GV_VectorList() {
}



/**
 * Copy constructor
 */
/*
GV_VectorList::GV_VectorList(const GV_VectorList& s) {
    setTangent(s.isTangent());
    for (int i=0; i<s.count(); ++i) {
        set(i, s.get(i));
    }
}
*/



/**
 * Constructor for num solutions.
 */
#ifdef GV_COMP
GV_VectorList::GV_VectorList(int /*num*/) {
}
#endif



/**
 * Constructor for one solution.
 */
GV_VectorList::GV_VectorList(const GV_Vector& v1) {
    append(v1);
    tangent = false;
}



/**
 * Constructor for two solutions.
 */
GV_VectorList::GV_VectorList(const GV_Vector& v1,
                                       const GV_Vector& v2) {
    append(v1);
    append(v2);
    tangent = false;
}



/**
 * Constructor for three solutions.
 */
GV_VectorList::GV_VectorList(const GV_Vector& v1,
                                       const GV_Vector& v2,
                                       const GV_Vector& v3) {
    append(v1);
    append(v2);
    append(v3);
    tangent = false;
}


/**
 * Constructor for four solutions.
 */
GV_VectorList::GV_VectorList(const GV_Vector& v1,
                                       const GV_Vector& v2,
                                       const GV_Vector& v3,
                                       const GV_Vector& v4) {
    append(v1);
    append(v2);
    append(v3);
    append(v4);
    tangent = false;
}


/**
 * Constructor for four solutions.
 */
GV_VectorList::GV_VectorList(const GV_Vector& v1,
                                       const GV_Vector& v2,
                                       const GV_Vector& v3,
                                       const GV_Vector& v4,
                                       const GV_Vector& v5) {
    append(v1);
    append(v2);
    append(v3);
    append(v4);
    append(v5);
    tangent = false;
}



/**
 * Destructor.
 */
GV_VectorList::~GV_VectorList() {
}


/**
 * Allocates 'num' vectors.
 */
#ifdef GV_COMP
void GV_VectorList::alloc(int num) {
    clear();
    for (int i=0; i<num; ++i)  {
        append(GV_Vector(false));
    }
    tangent = false;
}
#endif



/**
 * Deletes vector array and resets everything.
 */
#ifdef GV_COMP
void GV_VectorList::clean() {
    clear();
}
#endif


/**
 * Deletes vector array and resets everything.
 */
void GV_VectorList::clear() {
    QList<GV_Vector>::clear();
    tangent = false;
}



/**
 * @return vector solution number i or an invalid vector if there
 * are less solutions.
 */
GV_Vector GV_VectorList::get(int i) const {
    if (i<count()) {
        return at(i);
    } else {
        return GV_Vector(false);
    }
}



/**
 * @return Number of solutions available.
 */
#ifdef GV_COMP
int GV_VectorList::getNumber() const {
    return count();
}
#endif



/**
 * @retval true There's at least one valid solution.
 * @retval false There's no valid solution.
 */
bool GV_VectorList::hasValid() const {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            return true;
        }
    }

    return false;
}



/**
 * Sets the solution i to the given vector.
 * If i is greater than the current number of solutions available,
 * nothing happens.
 */
void GV_VectorList::set(int i, const GV_Vector& v) {
    if (i<count()) {
        operator [](i) = v;
    }
    else {
        for (int c=count(); c<i; ++c) {
            append(GV_Vector(false));
        }
        append(v);
    }
}



/**
 * Sets the tangent flag.
 */
void GV_VectorList::setTangent(bool t) {
    tangent = t;
}



/**
 * @return true if at least one of the solutions is a double solution
 * (tangent).
 */
bool GV_VectorList::isTangent() const {
    return tangent;
}

    
    
/**
 * Moves all vectors by the given offset.
 */
void GV_VectorList::move(const GV_Vector& offset) {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            operator[](i).move(offset);
        }
    }
}


/**
 * Rotates all vectors around the given center by the given angle.
 */
void GV_VectorList::rotate(const GV_Vector& center, double ang) {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            operator[](i).rotate(center, ang);
        }
    }
}


/**
 * Scales all vectors by the given factors with the given center.
 */
void GV_VectorList::scale(const GV_Vector& center, const GV_Vector& factor) {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            operator[](i).scale(center, factor);
        }
    }
}


/**
 * @return vector solution which is the closest to the given coordinate.
 * dist will contain the distance if it doesn't point to NULL (default).
 */
GV_Vector GV_VectorList::getClosest(const GV_Vector& coord,
        double* dist, int* index) const {
        
    double curDist;
    double minDist = GV_MAXDOUBLE;
    GV_Vector closestPoint(false);

    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            curDist = coord.distanceTo(at(i));

            if (curDist<minDist) {
                closestPoint = at(i);
                minDist = curDist;
                if (dist!=NULL) {
                    *dist = curDist;
                }
                if (index!=NULL) {
                    *index = i;
                }
            }
        }
    }

    return closestPoint;
}



/**
 * Sorts this list of vectors by distance to the given vector.
 * 
 * @param unique: Remove double vectors.
 */
void GV_VectorList::sortByDistanceTo(const GV_Vector& v, bool unique) {
    GV_VectorList ret;
    double minDist = GV_MAXDOUBLE;
    int nextIndex = 0;

    while (nextIndex!=-1) {
        minDist = GV_MAXDOUBLE;
        nextIndex = -1;

        for (int i=0; i<count(); ++i) {
            if (at(i).valid) {
                double dist = at(i).distanceTo(v);
                if (dist<minDist) {
                    minDist = dist;
                    nextIndex = i;
                }
            }
        }

        if (nextIndex!=-1) {
            ret.append(at(nextIndex));
            operator[](nextIndex).valid=false;
        }
    }

    clear();

    GV_Vector previous(false);
    for (int i=0; i<ret.count(); ++i) {
        if (!unique || !previous.valid || previous.distanceTo(ret.at(i))>1.0e-6) {
            append(ret.at(i));
            previous = ret.at(i);
        }
    }
}



/*
GV_VectorList GV_VectorList::operator = (const GV_VectorList& s) {
    alloc(s.getNumber());
    setTangent(s.isTangent());
    for (int i=0; i<s.getNumber(); ++i) {
        set(i, s.get(i));
    }

    return *this;
}
*/
    
    
GV_VectorList GV_VectorList::operator + (
    const GV_VectorList& s1) {
    
    GV_VectorList vs;
    int i = 0;
    for (int k=0; k<count(); ++k) {
        vs.set(i++, get(k));
    }
    for (int k=0; k<s1.count(); ++k) {
        vs.set(i++, s1.get(k));
    }
    return vs;
}

