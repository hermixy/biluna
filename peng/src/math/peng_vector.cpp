/*****************************************************************
 * $Id: peng_vector.cpp 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/
// based on rs_vector of QCad

#include "peng_vector.h"

#include <algorithm>

// for min, max
using namespace std;

/**
 * Sets a new position for the vector in polar coordinates.
 */
void PENG_Vector::setPolar(double radius, double angle) {
    x = radius * cos(angle);
    y = radius * sin(angle);
    z = 0.0;
    valid = true;
}


/**
 * Sets length of vector to 1.
 * @author Rutger Botermans
 */
void PENG_Vector::normalize() {
	double n = magnitude();
	
	if (n == 0) {
		valid = false;
		return;
	}

    x = x/n;
    y = y/n;
    z = z/n;
    valid = true;
}

/**
 * Updated by Rutger
 * @return The angle from zero (x-axis) to this vector (in rad).
 */
double PENG_Vector::angle() const {
    double ret = 0.0;
    double m = magnitude();

    if (m>1.0e-6) {
        double dp = dotP(*this, PENG_Vector(1.0, 0.0, 0.0)); // added z = ', 0.0'
		
		if (dp/m >= 1.0) {
			ret = 0.0;
		} else if (dp/m <= -1.0) { // added '='
			ret = M_PI;
		} else {
        	ret = acos(dp / m);
		}
        
        if (y < 0.0) {
            ret = 2 * M_PI - ret;
        }
    }
    return ret;
}



/**
 * Updated by Rutger for 3D
 * @return The angle from this vector and the given vector (in rad).
 */
double PENG_Vector::angleTo(const PENG_Vector& v) const {
    double ret = 0.0;
    double m = magnitude();

	if (!valid || !v.valid) {
		return 0.0;
	} else {
    	// return (v-(*this)).angle();

        PENG_Vector vn = v;
    	vn.normalize();
    	
	    if (m>1.0e-6) {
			double dp = dotP(*this, vn);
			
			if (dp/m >= 1.0) {
				ret = 0.0;
			} else if (dp/m <= -1.0) {
				ret = M_PI;
			} else {
	        	ret = acos(dp / m);
			}
	    }
	    return ret;
	}

}



/**
 * @return Magnitude (length) of the vector.
 */
double PENG_Vector::magnitude() const {
	double ret = 0.0;
    // Note that the z coordinate is also needed for 2d
    //   (due to definition of crossP())
	if (!valid) {
		ret = 0.0;
	}
	else {
		ret = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	}

	return ret;
}


/** 
 * Difference with vector v multiplied with t moved origine to 'this' vector
 */
PENG_Vector PENG_Vector::lerp(const PENG_Vector& v, double t) const {
    return PENG_Vector(x+(v.x-x)*t, y+(v.y-y)*t);
}


/**
 * @return The distance between this and the given coordinate.
 */
double PENG_Vector::distanceTo(const PENG_Vector& v) const {
	if (!valid || !v.valid) {
		return RB_MAXDOUBLE;
	}
	else {
    	return (*this-v).magnitude();
	}
}



/**
 * @return true is this vector is within the given range.
 */
bool PENG_Vector::isInWindow(const PENG_Vector& firstCorner,
        const PENG_Vector& secondCorner) {

    double minX = min(firstCorner.x, secondCorner.x);
	double maxX = max(firstCorner.x, secondCorner.x);
	double minY = min(firstCorner.y, secondCorner.y);
	double maxY = max(firstCorner.y, secondCorner.y);

	return (x>=minX && x<=maxX && y>=minY && y<=maxY);
}



/**
 * Moves this vector by the given offset. Equal to the operator +=.
 */
PENG_Vector PENG_Vector::move(const PENG_Vector& offset) {
    *this+=offset;
    return *this;
}



/**
 * Rotates this vector around 0/0 by the given angle.
 */
PENG_Vector PENG_Vector::rotate(double ang) {
    double r = magnitude();
    double a = angle() + ang;
	
    x = cos(a) * r;
    y = sin(a) * r;

    return *this;
}


/**
 * Rotates this vector around the given center by the given angle.
 */
PENG_Vector PENG_Vector::rotate(const PENG_Vector& center, double ang) {
    *this = center + (*this-center).rotate(ang);
    return *this;
}


/**
 * Scales this vector by the given factors with 0/0 as center.
 */
PENG_Vector PENG_Vector::scale(const PENG_Vector& factor) {
    x *= factor.x;
    y *= factor.y;
    return *this;
}



/**
 * Scales this vector by the given factors with the given center.
 */
PENG_Vector PENG_Vector::scale(const PENG_Vector& center, const PENG_Vector& factor) {
    *this = center + (*this-center).scale(factor);
    return *this;
}



/**
 * Mirrors this vector at the given axis.
 */
PENG_Vector PENG_Vector::mirror(const PENG_Vector& axisPoint1, const PENG_Vector& axisPoint2) {
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
 * Streams the vector components to stdout. e.g.: "1/4/0"
 */
std::ostream& operator << (std::ostream& os, const PENG_Vector& v) {
    if(v.valid) {
        os << v.x << "/" << v.y << "/" << v.z;
    } else {
        os << "invalid vector";
    }
    return os;
}



/**
 * binary + operator.
 */
PENG_Vector PENG_Vector::operator + (const PENG_Vector& v) const {
    return PENG_Vector(x + v.x, y + v.y, z + v.z);
}



/**
 * binary - operator.
 */
PENG_Vector PENG_Vector::operator - (const PENG_Vector& v) const {
    return PENG_Vector(x - v.x, y - v.y, z - v.z);
}


/**
 * binary * operator.
 */
PENG_Vector PENG_Vector::operator * (double s) const {
    return PENG_Vector(x * s, y * s, z * s);
}



/**
 * binary / operator.
 */
PENG_Vector PENG_Vector::operator / (double s) const {
    return PENG_Vector(x / s, y / s, z / s);
}



/**
 * unary - operator.
 */
PENG_Vector PENG_Vector::operator - () const {
    return PENG_Vector(-x, -y, -z);
}



/**
 * Scalarproduct (dot product).
 */
double PENG_Vector::dotP(const PENG_Vector& v1, const PENG_Vector& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}



/**
 * += operator. Assert: both vectors must be valid.
 */
void PENG_Vector::operator += (const PENG_Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}


/**
 * -= operator
 */
void PENG_Vector::operator -= (const PENG_Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}



/**
 * *= operator
 */
void PENG_Vector::operator *= (double s) {
    x *= s;
    y *= s;
    z *= s;
}



/**
 * == operator
 */
bool PENG_Vector::operator == (const PENG_Vector& v) const {
    return (x==v.x && y==v.y && z==v.z && valid==v.valid);
}


/**
 * @return A vector with the minimum components from the vectors v1 and v2.
 * These might be mixed components from both vectors.
 */
PENG_Vector PENG_Vector::minimum (const PENG_Vector& v1, const PENG_Vector& v2) {
    return PENG_Vector (min(v1.x, v2.x),
                      min(v1.y, v2.y),
                      min(v1.z, v2.z));
}



/**
 * @return A vector with the maximum values from the vectors v1 and v2
 */
PENG_Vector PENG_Vector::maximum (const PENG_Vector& v1, const PENG_Vector& v2) {
    return PENG_Vector (max(v1.x, v2.x),
                      max(v1.y, v2.y),
                      max(v1.z, v2.z));
}



/**
 * @return Cross product of two vectors.
 */
PENG_Vector PENG_Vector::crossP(const PENG_Vector& v1, const PENG_Vector& v2) {
    return PENG_Vector(v1.y*v2.z - v1.z*v2.y,
                     v1.z*v2.x - v1.x*v2.z,
                     v1.x*v2.y - v1.y*v2.x);
}


/**
 * Constructor for no solution.
 */
PENG_VectorSolutions::PENG_VectorSolutions() : vector(NULL) {
    clean();
}



/**
 * Copy constructor
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_VectorSolutions& s)
	: vector(NULL) {
	alloc(s.getNumber());
	setTangent(s.isTangent());
	for (int i=0; i<s.getNumber(); ++i) {
		set(i, s.get(i));
	}
}



/**
 * Constructor for num solutions.
 */
PENG_VectorSolutions::PENG_VectorSolutions(int num) : vector(NULL) {
	alloc(num);
}



/**
 * Constructor for one solution.
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_Vector& v1) {
    num = 1;
    vector = new PENG_Vector[num];
    vector[0] = v1;
    tangent = false;
}



/**
 * Constructor for two solutions.
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_Vector& v1,
                                       const PENG_Vector& v2) {
    num = 2;
    vector = new PENG_Vector[num];
    vector[0] = v1;
    vector[1] = v2;
    tangent = false;
}



/**
 * Constructor for three solutions.
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_Vector& v1,
                                       const PENG_Vector& v2,
                                       const PENG_Vector& v3) {
    num = 3;
    vector = new PENG_Vector[num];
    vector[0] = v1;
    vector[1] = v2;
    vector[2] = v3;
    tangent = false;
}


/**
 * Constructor for four solutions.
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_Vector& v1,
                                       const PENG_Vector& v2,
                                       const PENG_Vector& v3,
                                       const PENG_Vector& v4) {
    num = 4;
    vector = new PENG_Vector[num];
    vector[0] = v1;
    vector[1] = v2;
    vector[2] = v3;
    vector[3] = v4;
    tangent = false;
}


/**
 * Constructor for five solutions.
 */
PENG_VectorSolutions::PENG_VectorSolutions(const PENG_Vector& v1,
                                       const PENG_Vector& v2,
                                       const PENG_Vector& v3,
                                       const PENG_Vector& v4,
                                       const PENG_Vector& v5) {
    num = 5;
    vector = new PENG_Vector[num];
    vector[0] = v1;
    vector[1] = v2;
    vector[2] = v3;
    vector[3] = v4;
    vector[4] = v5;
    tangent = false;
}



/**
 * Destructor.
 */
PENG_VectorSolutions::~PENG_VectorSolutions() {
    clean();
}


/**
 * Allocates 'num' vectors.
 */
void PENG_VectorSolutions::alloc(int num) {
	clean();
	this->num = num;
    vector = new PENG_Vector[num];
	for (int i=0; i<num; ++i)  {
        vector[i] = PENG_Vector(false);
	}
    tangent = false;
}

/**
 * Deletes vector array and resets everything.
 */
void PENG_VectorSolutions::clean() {
    if (vector!=NULL) {
        delete[] vector;
    }
    vector = NULL;
    num = 0;
    tangent = false;
}



/**
 * @return vector solution number i or an invalid vector if there
 * are less solutions.
 */
PENG_Vector PENG_VectorSolutions::get(int i) const {
    if (i<num) {
        return vector[i];
    } else {
        return PENG_Vector(false);
    }
}



/**
 * @return Number of solutions available.
 */
int PENG_VectorSolutions::getNumber() const {
    return num;
}



/**
 * @retval true There's at least one valid solution.
 * @retval false There's no valid solution.
 */
bool PENG_VectorSolutions::hasValid() const {
	for (int i=0; i<num; i++) {
        if (vector[i].valid) {
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
void PENG_VectorSolutions::set(int i, const PENG_Vector& v) {
    if (i<num) {
        vector[i] = v;
    }
}



/**
 * Sets the tangent flag.
 */
void PENG_VectorSolutions::setTangent(bool t) {
    tangent = t;
}



/**
 * @return true if at least one of the solutions is a double solution
 * (tangent).
 */
bool PENG_VectorSolutions::isTangent() const {
    return tangent;
}



/**
 * Rotates all vectors around the given center by the given angle.
 */
void PENG_VectorSolutions::rotate(PENG_Vector center, double ang) {
    for (int i=0; i<num; i++) {
        if (vector[i].valid) {
			vector[i].rotate(center, ang);
		}
	}
}


/**
 * Scales all vectors by the given factors with the given center.
 */
void PENG_VectorSolutions::scale(PENG_Vector center, PENG_Vector factor) {
    for (int i=0; i<num; i++) {
        if (vector[i].valid) {
			vector[i].scale(center, factor);
		}
	}
}


/**
 * @return vector solution which is the closest to the given coordinate.
 * dist will contain the distance if it doesn't point to NULL (default).
 */
PENG_Vector PENG_VectorSolutions::getClosest(const PENG_Vector& coord,
        double* dist, int* index) const {
		
    double curDist;
    double minDist = RB_MAXDOUBLE;
    PENG_Vector closestPoint(false);

    for (int i=0; i<num; i++) {
        if (vector[i].valid) {
            curDist = coord.distanceTo(vector[i]);

            if (curDist<minDist) {
                closestPoint = vector[i];
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


PENG_VectorSolutions PENG_VectorSolutions::operator = (const PENG_VectorSolutions& s) {
	alloc(s.getNumber());
	setTangent(s.isTangent());
	for (int i=0; i<s.getNumber(); ++i) {
		set(i, s.get(i));
	}

	return *this;
}
	
    
PENG_VectorSolutions PENG_VectorSolutions::operator + (
    const PENG_VectorSolutions& s1) {
    
    PENG_VectorSolutions vs;
    vs.alloc(getNumber() + s1.getNumber());
    int i = 0;
	for (int k=0; k<getNumber(); ++k) {
		vs.set(i++, get(k));
	}
	for (int k=0; k<s1.getNumber(); ++k) {
		vs.set(i++, s1.get(k));
	}
    return vs;
}



std::ostream& operator << (std::ostream& os,
                                  const PENG_VectorSolutions& s) {
    for (int i=0; i<s.num; ++i) {
        os << "(" << s.get(i) << ")\n";
    }
    os << " tangent: " << (int)s.isTangent() << "\n";
    return os;
}

/**
 * Calculate volume Tetrahedron
 */
double PENG_VectorSolutions::getTetrahedronVolume() {
    if (!vector[0].valid || !vector[1].valid
            || !vector[2].valid || !vector[3].valid) {
        return 0.0;
    }

    PENG_Vector d12 = vector[0] - vector[1];
    PENG_Vector d13 = vector[0] - vector[2];
    PENG_Vector d14 = vector[0] - vector[3];
    PENG_Vector d23 = vector[1] - vector[2];
    PENG_Vector d24 = vector[1] - vector[3];
    PENG_Vector d34 = vector[2] - vector[3];

    double d122 = pow(d12.magnitude(), 2.0);
    double d132 = pow(d13.magnitude(), 2.0);
    double d142 = pow(d14.magnitude(), 2.0);
    double d232 = pow(d23.magnitude(), 2.0);
    double d242 = pow(d24.magnitude(), 2.0);
    double d342 = pow(d34.magnitude(), 2.0);

    double vol = d122*d122*d342 + d132*d132*d242 + d142*d142*d232
                    + d232*d232*d142 + d242*d242*d132 + d342*d342*d122
                    + d122*d232*d132 + d122*d242*d142 + d132*d342*d142
                    + d232*d342*d242 - d122*d232*d342 - d132*d232*d242
                    - d122*d242*d342 - d142*d242*d232 - d132*d342*d242
                    - d142*d342*d232 - d232*d132*d142 - d122*d132*d342
                    - d242*d142*d132 - d122*d142*d342 - d132*d122*d242
                    - d232*d122*d142;
    return pow(fabs(vol)/144.0, 0.5);
}
