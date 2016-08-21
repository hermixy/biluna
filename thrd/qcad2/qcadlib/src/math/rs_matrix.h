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
** not clear to you.
**
**********************************************************************/

#ifndef RS_MATRIX_H
#define RS_MATRIX_H

#include "rs_debug.h"
#include "rs_math.h"

#define RS_MATRIX_TOLERANCE 1.0e-8

/**
 * Simple matrix class.
 */
class CAD_EXPORT RS_Matrix {
public:
    RS_Matrix();
    RS_Matrix(int r, int c);
    RS_Matrix(const RS_Matrix& other);
    ~RS_Matrix();

    static RS_Matrix create3x3(
        double a11, double a12, double a13,
        double a21, double a22, double a23,
        double a31, double a32, double a33);
    static RS_Matrix create3x1(
        double a11,
        double a21,
        double a31);

    void operator = (const RS_Matrix& other);
    bool operator == (const RS_Matrix& other) const;
    RS_Matrix operator *= (double f);
    RS_Matrix operator * (const RS_Matrix& other) const;
    RS_Matrix multiplyWith(const RS_Matrix& w) const;
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
    RS_Matrix transform();
    RS_Matrix append(const RS_Matrix& v);

private:
    double** m;
    int rows, cols;
};
    
RS_Matrix operator * (const RS_Matrix& matrix, double factor);
RS_Matrix operator * (double factor, const RS_Matrix& matrix);

#endif
