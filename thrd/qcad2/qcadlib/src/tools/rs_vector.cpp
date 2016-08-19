/****************************************************************************
** $Id: rs_vector.cpp 10447 2008-04-10 12:03:19Z andrew $
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


#include "rs_vector.h"

#include "rs_debug.h"
#include "rs_math.h"
#include "rs_matrix.h"
#include "rs_constructionline.h"


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
//RS_Vector::~RS_Vector() {
//}
    
    
bool RS_Vector::isValid() const {
#if defined(Q_OS_SOLARIS) || defined(Q_OS_MAC)
    return valid;
#else
    return valid && !isnan(x) && !isnan(y) && !isnan(z);
#endif
}


/**
 * Sets a new position for the vector in polar coordinates.
 */
void RS_Vector::setPolar(double radius, double angle) {
    x = radius * cos(angle);
    y = radius * sin(angle);
    z = 0.0;
    valid = true;
}



/**
 * @return The angle from zero to this vector (in rad).
 */
double RS_Vector::angle() const {
    double ret = 0.0;
    double m = magnitude2d();

    if (m>1.0e-6) {
        double dp = dotP(*this, RS_Vector(1.0, 0.0));
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
    
    
    
double RS_Vector::angleToPlaneXY() const {
    RS_Vector n(0,0,1);

    if (magnitude()<1.0e-4) {
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: magnitude is 0");
        return M_PI/2;
    }
    else if ((dotP(*this, n) / (magnitude() * 1)) > 1.0) {
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: value for cos is > 1");
        return 0.0;
    }
    else {
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: "
            "%f,%f,%f", x, y, z);
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: "
            "dotp: %f", dotP(*this, n));
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: "
            "magnitude: %f", magnitude());
        RS_DEBUG->print("RS_Vector::angleToPlaneXY: "
            "acos: %f", acos(dotP(*this, n) / (magnitude() * 1)));

        return M_PI/2 - acos(dotP(*this, n) / (magnitude() * 1));
    }
}



/**
 * @return The angle from this and the given coordinate (in rad).
 */
double RS_Vector::angleTo(const RS_Vector& v) const {
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
double RS_Vector::magnitude() const {
    double ret = 0.0;
    // Note that the z coordinate is also needed for 2d
    //   (due to definition of crossP())
    if (!valid) {
        ret = 0.0;
    }
    else {
        ret = sqrt(RS_Math::pow(x,2) + RS_Math::pow(y,2) + RS_Math::pow(z,2));
    }

    return ret;
}



/**
 * @return Magnitude (length) of the vector in x/y (2d).
 */
double RS_Vector::magnitude2d() const {
    double ret = 0.0;
    if (!valid) {
        ret = 0.0;
    }
    else {
        ret = sqrt(RS_Math::pow(x,2) + RS_Math::pow(y,2));
    }

    return ret;
}



/** 
 * 
 */
RS_Vector RS_Vector::lerp(const RS_Vector& v, double t) const {
    return RS_Vector(x+(v.x-x)*t, y+(v.y-y)*t);
}


/**
 * @return The distance between this and the given coordinate.
 */
double RS_Vector::distanceTo(const RS_Vector& v) const {
    if (!valid || !v.valid) {
        return RS_MAXDOUBLE;
    }
    else {
        return (*this-v).magnitude();
    }
}



/**
 * @return The distance between this and the given coordinate on the XY plane.
 */
double RS_Vector::distanceTo2d(const RS_Vector& v) const {
    if (!valid || !v.valid) {
        return RS_MAXDOUBLE;
    }
    else {
        return (*this-v).magnitude2d();
    }
}



/**
 * @return true is this vector is within the given range.
 */
bool RS_Vector::isInWindow(const RS_Vector& firstCorner, 
        const RS_Vector& secondCorner) {

    double minX = std::min(firstCorner.x, secondCorner.x);
    double maxX = std::max(firstCorner.x, secondCorner.x);
    double minY = std::min(firstCorner.y, secondCorner.y);
    double maxY = std::max(firstCorner.y, secondCorner.y);

    return (x>=minX && x<=maxX && y>=minY && y<=maxY);
}



/**
 * Moves this vector by the given offset. Equal to the operator +=.
 */
RS_Vector RS_Vector::move(const RS_Vector& offset) {
    *this+=offset;
    return *this;
}



/**
 * Rotates this vector around 0/0 by the given angle.
 */
RS_Vector RS_Vector::rotate(double ang) {
    double r = magnitude2d();
    double a = angle() + ang;
    
    x = cos(a) * r;
    y = sin(a) * r;

    return *this;
}


/**
 * Rotates this vector around the given center by the given angle.
 */
RS_Vector RS_Vector::rotate(const RS_Vector& center, double ang) {
    *this = center + (*this-center).rotate(ang);
    return *this;
}



/**
 * Scales this vector by the given factor with 0/0/0 as center.
 */
RS_Vector RS_Vector::scale(double factor) {
    return scale(RS_Vector(factor, factor, factor));
}


/**
 * Scales this vector by the given factors with 0/0/0 as center.
 */
RS_Vector RS_Vector::scale(const RS_Vector& factor) {
    x *= factor.x;
    y *= factor.y;
    // 20071207: scale z as well
    z *= factor.z;
    return *this;
}



/**
 * Scales this vector by the given factor with the given center.
 */
RS_Vector RS_Vector::scale(const RS_Vector& center, double factor) {
    return scale(center, RS_Vector(factor, factor, factor));
}


/**
 * Scales this vector by the given factors with the given center.
 */
RS_Vector RS_Vector::scale(const RS_Vector& center, const RS_Vector& factor) {
    *this = center + (*this-center).scale(factor);
    return *this;
}



/**
 * Mirrors this vector at the given axis.
 */
RS_Vector RS_Vector::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
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
RS_Vector RS_Vector::isoProject(RS2::IsoProjectionType type) {
    static RS_Matrix iso = 
        RS_Matrix::create3x3(
            sqrt(3.0),       0.0, -sqrt(3.0),
                 1.0,        2.0,       1.0,
            sqrt(2.0), -sqrt(2.0), sqrt(2.0))
        * (1.0 / sqrt(6.0));
        
    RS_Matrix input;
    switch (type) {
    case RS2::IsoSide:
        input = RS_Matrix::create3x1(
            x,
            y,
            0.0);
        break;
    case RS2::IsoTop:
        input = RS_Matrix::create3x1(
            y,
            0.0,
            -x);
        break;
    case RS2::IsoFront:
        input = RS_Matrix::create3x1(
            0.0,
            y,
            -x);
        break;
    }

    RS_Matrix res = iso * input;
    
    x = res.get(0,0);
    y = res.get(1,0);

    return *this;
}



/**
 * binary + operator.
 */
RS_Vector RS_Vector::operator + (const RS_Vector& v) const {
    return RS_Vector(x + v.x, y + v.y, z + v.z, valid && v.valid);
}



/**
 * binary - operator.
 */
RS_Vector RS_Vector::operator - (const RS_Vector& v) const {
    return RS_Vector(x - v.x, y - v.y, z - v.z, valid && v.valid);
}


/**
 * binary * operator.
 */
RS_Vector RS_Vector::operator * (double s) const {
    return RS_Vector(x * s, y * s, z * s, valid);
}



/**
 * binary / operator.
 */
RS_Vector RS_Vector::operator / (double s) const {
    return RS_Vector(x / s, y / s, z / s, valid);
}



/**
 * unary - operator.
 */
RS_Vector RS_Vector::operator - () const {
    return RS_Vector(-x, -y, -z, valid);
}



/**
 * Scalarproduct (dot product).
 */
double RS_Vector::dotP(const RS_Vector& v1, const RS_Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



/**
 * += operator. Assert: both vectors must be valid.
 */
void RS_Vector::operator += (const RS_Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    valid = valid && v.valid;
}


/**
 * -= operator
 */
void RS_Vector::operator -= (const RS_Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    valid = valid && v.valid;
}



/**
 * *= operator
 */
void RS_Vector::operator *= (double s) {
    x *= s;
    y *= s;
    z *= s;
}



/**
 * == operator
 */
bool RS_Vector::operator == (const RS_Vector& v) const {
    return (x==v.x && y==v.y && z==v.z && valid==v.valid);
}



/**
 * @return A vector with the minimum components from the vectors v1 and v2.
 * These might be mixed components from both vectors.
 */
RS_Vector RS_Vector::minimum (const RS_Vector& v1, const RS_Vector& v2) {
    return RS_Vector (std::min(v1.x, v2.x),
                      std::min(v1.y, v2.y),
                      std::min(v1.z, v2.z),
                      v1.valid && v2.valid);
}



/**
 * @return A vector with the maximum values from the vectors v1 and v2
 */
RS_Vector RS_Vector::maximum (const RS_Vector& v1, const RS_Vector& v2) {
    return RS_Vector (std::max(v1.x, v2.x),
                      std::max(v1.y, v2.y),
                      std::max(v1.z, v2.z),
                      v1.valid && v2.valid);
}



/**
 * @return Cross product of two vectors.
 */
RS_Vector RS_Vector::crossP(const RS_Vector& v1, const RS_Vector& v2) {
    return RS_Vector(v1.y*v2.z - v1.z*v2.y,
                     v1.z*v2.x - v1.x*v2.z,
                     v1.x*v2.y - v1.y*v2.x,
                     v1.valid && v2.valid);
}
    
    
/**
 * Generic 3d transformation. m must be a 3x3 matrix.
 */
RS_Vector RS_Vector::transform(const RS_Matrix& m) {
    RS_Matrix input;

    input = RS_Matrix::create3x1(x, y, z);
    RS_Matrix res = m * input;
    x = res.get(0,0);
    y = res.get(1,0);
    z = res.get(2,0);

    return *this;
}


/**
 * Constructor for no solution.
 */
RS_VectorList::RS_VectorList() {
}



/**
 * Copy constructor
 */
/*
RS_VectorList::RS_VectorList(const RS_VectorList& s) {
    setTangent(s.isTangent());
    for (int i=0; i<s.count(); ++i) {
        set(i, s.get(i));
    }
}
*/



/**
 * Constructor for num solutions.
 */
#ifdef RS_COMP
RS_VectorList::RS_VectorList(int /*num*/) {
}
#endif



/**
 * Constructor for one solution.
 */
RS_VectorList::RS_VectorList(const RS_Vector& v1) {
    append(v1);
    tangent = false;
}



/**
 * Constructor for two solutions.
 */
RS_VectorList::RS_VectorList(const RS_Vector& v1,
                                       const RS_Vector& v2) {
    append(v1);
    append(v2);
    tangent = false;
}



/**
 * Constructor for three solutions.
 */
RS_VectorList::RS_VectorList(const RS_Vector& v1,
                                       const RS_Vector& v2,
                                       const RS_Vector& v3) {
    append(v1);
    append(v2);
    append(v3);
    tangent = false;
}


/**
 * Constructor for four solutions.
 */
RS_VectorList::RS_VectorList(const RS_Vector& v1,
                                       const RS_Vector& v2,
                                       const RS_Vector& v3,
                                       const RS_Vector& v4) {
    append(v1);
    append(v2);
    append(v3);
    append(v4);
    tangent = false;
}


/**
 * Constructor for four solutions.
 */
RS_VectorList::RS_VectorList(const RS_Vector& v1,
                                       const RS_Vector& v2,
                                       const RS_Vector& v3,
                                       const RS_Vector& v4,
                                       const RS_Vector& v5) {
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
RS_VectorList::~RS_VectorList() {
}


/**
 * Allocates 'num' vectors.
 */
#ifdef RS_COMP
void RS_VectorList::alloc(int num) {
    clear();
    for (int i=0; i<num; ++i)  {
        append(RS_Vector(false));
    }
    tangent = false;
}
#endif



/**
 * Deletes vector array and resets everything.
 */
#ifdef RS_COMP
void RS_VectorList::clean() {
    clear();
}
#endif


/**
 * Deletes vector array and resets everything.
 */
void RS_VectorList::clear() {
    RS_List<RS_Vector>::clear();
    tangent = false;
}



/**
 * @return vector solution number i or an invalid vector if there
 * are less solutions.
 */
RS_Vector RS_VectorList::get(int i) const {
    if (i<count()) {
        return at(i);
    } else {
        return RS_Vector(false);
    }
}



/**
 * @return Number of solutions available.
 */
#ifdef RS_COMP
int RS_VectorList::getNumber() const {
    return count();
}
#endif



/**
 * @retval true There's at least one valid solution.
 * @retval false There's no valid solution.
 */
bool RS_VectorList::hasValid() const {
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
void RS_VectorList::set(int i, const RS_Vector& v) {
    if (i<count()) {
        operator [](i) = v;
    }
    else {
        for (int c=count(); c<i; ++c) {
            append(RS_Vector(false));
        }
        append(v);
    }
}



/**
 * Sets the tangent flag.
 */
void RS_VectorList::setTangent(bool t) {
    tangent = t;
}



/**
 * @return true if at least one of the solutions is a double solution
 * (tangent).
 */
bool RS_VectorList::isTangent() const {
    return tangent;
}

    
    
/**
 * Moves all vectors by the given offset.
 */
void RS_VectorList::move(const RS_Vector& offset) {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            operator[](i).move(offset);
        }
    }
}


/**
 * Rotates all vectors around the given center by the given angle.
 */
void RS_VectorList::rotate(const RS_Vector& center, double ang) {
    for (int i=0; i<count(); i++) {
        if (at(i).valid) {
            operator[](i).rotate(center, ang);
        }
    }
}


/**
 * Scales all vectors by the given factors with the given center.
 */
void RS_VectorList::scale(const RS_Vector& center, const RS_Vector& factor) {
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
RS_Vector RS_VectorList::getClosest(const RS_Vector& coord,
        double* dist, int* index) const {
        
    double curDist;
    double minDist = RS_MAXDOUBLE;
    RS_Vector closestPoint(false);

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
void RS_VectorList::sortByDistanceTo(const RS_Vector& v, bool unique) {
    RS_VectorList ret;
    double minDist = RS_MAXDOUBLE;
    int nextIndex = 0;

    while (nextIndex!=-1) {
        minDist = RS_MAXDOUBLE;
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

    RS_Vector previous(false);
    for (int i=0; i<ret.count(); ++i) {
        if (!unique || !previous.valid || previous.distanceTo(ret.at(i))>1.0e-6) {
            append(ret.at(i));
            previous = ret.at(i);
        }
    }
}



/*
RS_VectorList RS_VectorList::operator = (const RS_VectorList& s) {
    alloc(s.getNumber());
    setTangent(s.isTangent());
    for (int i=0; i<s.getNumber(); ++i) {
        set(i, s.get(i));
    }

    return *this;
}
*/
    
    
RS_VectorList RS_VectorList::operator + (
    const RS_VectorList& s1) {
    
    RS_VectorList vs;
    int i = 0;
    for (int k=0; k<count(); ++k) {
        vs.set(i++, get(k));
    }
    for (int k=0; k<s1.count(); ++k) {
        vs.set(i++, s1.get(k));
    }
    return vs;
}

