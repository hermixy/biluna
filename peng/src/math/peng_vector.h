/*****************************************************************
 * $Id: peng_vector.h 1826 2012-12-08 00:31:33Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna ENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_VECTOR_H
#define PENG_VECTOR_H

#include <iostream>
#include <cmath>

// Windoze XP can't handle the original MAX/MINDOUBLE's, already in rb.h
#ifndef RB_MAXDOUBLE
#define RB_MAXDOUBLE 1.0E+10
#endif

#ifndef RB_MINDOUBLE
#define RB_MINDOUBLE -1.0E+10
#endif

// The magic number PI, already in rb.h
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/**
 * Represents a 3d vector (x/y/z)
 * @author Andrew Mustun updated for openGL by Rutger Botermans
 */
class PENG_Vector {
public:
    inline PENG_Vector();
    inline PENG_Vector(double vx, double vy, double vz=0.0);
    inline explicit PENG_Vector(bool valid);
    //~PENG_Vector();

    inline void set(double vx, double vy, double vz=0.0) {
        x = vx;
        y = vy;
        z = vz;
        valid = true;
    }
    void setPolar(double radius, double angle);

	// added by Rutger
    void normalize();


    double distanceTo(const PENG_Vector& v) const;
    double angle() const;
    double angleTo(const PENG_Vector& v) const;
    double magnitude() const;
    
    PENG_Vector lerp(const PENG_Vector& v, double t) const;

    bool isInWindow(const PENG_Vector& firstCorner, const PENG_Vector& secondCorner);

    PENG_Vector move(const PENG_Vector& offset);
    PENG_Vector rotate(double ang);
    PENG_Vector rotate(const PENG_Vector& center, double ang);
    PENG_Vector scale(const PENG_Vector& factor);
    PENG_Vector scale(const PENG_Vector& center, const PENG_Vector& factor);
    PENG_Vector mirror(const PENG_Vector& axisPoint1, const PENG_Vector& axisPoint2);

    PENG_Vector operator+ (const PENG_Vector& v) const;
    PENG_Vector operator- (const PENG_Vector& v) const;
    PENG_Vector operator* (double s) const;
    PENG_Vector operator/ (double s) const;
    PENG_Vector operator- () const;

    void operator += (const PENG_Vector& v);
    void operator -= (const PENG_Vector& v);
    void operator *= (double s);

    bool operator == (const PENG_Vector& v) const;
    bool operator != (const PENG_Vector& v) const {
        return !operator==(v);
    }
    
    
    
    
//    double operator[] (int axis) const;


//    glVertex3fv operator& () const {
//    	return glVertex3fv(x,y,z);
//    }
    //! Bracket operator, with a constant return value
//    float operator[](int i) const { return (&x)[i]; }

    //! Bracket operator, returns an l-value
//    float& operator[](int i) { return (&x)[i]; }

    //! The memory address of the vector. Useful as an argument for glVertex3fv, glNormal3fv...
    const double* address() const { return &x; };

    //! Dereferencing operator that returns the memory address of the vector. Same as address().
//    operator const float*() const { return &x; };


    
    
    
    static PENG_Vector minimum(const PENG_Vector& v1, const PENG_Vector& v2);
    static PENG_Vector maximum(const PENG_Vector& v1, const PENG_Vector& v2);
    static PENG_Vector crossP(const PENG_Vector& v1, const PENG_Vector& v2);
    static double dotP(const PENG_Vector& v1, const PENG_Vector& v2);

    friend std::ostream& operator << (std::ostream&, const PENG_Vector& v);

public:
    double x;
    double y;
    double z;
    bool valid;
};


/**
 * Represents one to 4 vectors. Typically used to return multiple
 * solutions from a function.
 */
class PENG_VectorSolutions {
public:
    PENG_VectorSolutions();
    PENG_VectorSolutions(const PENG_VectorSolutions& s);
    PENG_VectorSolutions(int num);
    PENG_VectorSolutions(const PENG_Vector& v1);
    PENG_VectorSolutions(const PENG_Vector& v1, const PENG_Vector& v2);
    PENG_VectorSolutions(const PENG_Vector& v1, const PENG_Vector& v2,
                       const PENG_Vector& v3);
    PENG_VectorSolutions(const PENG_Vector& v1, const PENG_Vector& v2,
                       const PENG_Vector& v3, const PENG_Vector& v4);
    PENG_VectorSolutions(const PENG_Vector& v1, const PENG_Vector& v2,
                       const PENG_Vector& v3, const PENG_Vector& v4,
                       const PENG_Vector& v5);
    
    ~PENG_VectorSolutions();

	void alloc(int num);
    void clean();
    PENG_Vector get(int i) const;
	int getNumber() const;
	bool hasValid() const;
    void set(int i, const PENG_Vector& v);
	void setTangent(bool t);
    bool isTangent() const;
    PENG_Vector getClosest(const PENG_Vector& coord,
			double* dist=NULL, int* index=NULL) const;
    void rotate(PENG_Vector center, double ang);
    void scale(PENG_Vector center, PENG_Vector factor);

    PENG_VectorSolutions operator = (const PENG_VectorSolutions& s);
    PENG_VectorSolutions operator + (const PENG_VectorSolutions& s);

    friend std::ostream& operator << (std::ostream& os,
                                      const PENG_VectorSolutions& s);
    double getTetrahedronVolume();	

private:
    PENG_Vector* vector;
    int num;
    bool tangent;
};


/**
 * Constructor for a point with default coordinates.
 */
inline PENG_Vector::PENG_Vector() :
   x(0), y(0), z(0), valid(true)
{}

/**
 * Constructor for a point with given valid flag.
 *
 * @param valid true: a valid vector with default coordinates is created.
 *              false: an invalid vector is created
 */
inline PENG_Vector::PENG_Vector(bool valid_in) :
   x(0), y(0), z(0), valid(valid_in)
{}

/**
 * Constructor for a point with given coordinates.
 */
inline PENG_Vector::PENG_Vector(double vx, double vy, double vz)  :
   x(vx), y(vy), z(vz), valid(true)
{}

#endif // PENG_VECTOR_H
