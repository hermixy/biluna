/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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
** Updated for Biluna
**********************************************************************/

#ifndef GV_MATRIX_H
#define GV_MATRIX_H

#include "gv_math.h"
#include "rb_debug.h"

#define GV_MATRIX_TOLERANCE 1.0e-8

/**
 * Simple matrix class.
 */
class GV_Matrix {
public:
    GV_Matrix();
    GV_Matrix(int r, int c);
    GV_Matrix(const GV_Matrix& other);
    ~GV_Matrix();

    static GV_Matrix create3x3(
        double a11, double a12, double a13,
        double a21, double a22, double a23,
        double a31, double a32, double a33);
    static GV_Matrix create3x1(
        double a11,
        double a21,
        double a31);

    void operator = (const GV_Matrix& other);
    bool operator == (const GV_Matrix& other) const;
    GV_Matrix operator *= (double f);
    GV_Matrix operator * (const GV_Matrix& other) const;
    GV_Matrix multiplyWith(const GV_Matrix& w) const;
    double* operator [] (int i);

    void init(int r, int c);
    void reset();
    void clear();
    
    void set3x3(double a11, double a12, double a13,
                double a21, double a22, double a23,
                double a31, double a32, double a33);
    void set3x1(double a11,
                double a21,
                double a31);

    void set(int r, int c, double v);
    double get(int r, int c) const;

    int getRows() const { 
        return rows; 
    }
    int getCols() const { 
        return cols; 
    }

    void print() const;
    void printRow(int r) const;

    bool ref(int startRow);
    bool rref();

    void multiplyRow(int r, double factor);
    void addRow(int r, double factor, int r2);
    void swapRows(int r1, int r2);
    int getPivotRow(int startRow);
    int getPivotCol(int r);
    GV_Matrix transform();
    GV_Matrix append(const GV_Matrix& v);

private:
    double** m;
    int rows, cols;
};
    
GV_Matrix operator * (const GV_Matrix& matrix, double factor);
GV_Matrix operator * (double factor, const GV_Matrix& matrix);

#endif
