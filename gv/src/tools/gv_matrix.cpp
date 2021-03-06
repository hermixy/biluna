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
** Updated for Biluna 2016-08-11
**********************************************************************/

#include "gv_matrix.h"

GV_Matrix::GV_Matrix() : m(NULL), rows(0), cols(0) {
}
    
GV_Matrix::GV_Matrix(int r, int c) : m(NULL) {
    init(r,c);
}
    
GV_Matrix::GV_Matrix(const GV_Matrix& other) : m(NULL) {
    (*this) = other;
}

GV_Matrix::~GV_Matrix() {
    clear();
}
    
    
GV_Matrix GV_Matrix::create3x3(
        double a11, double a12, double a13,
        double a21, double a22, double a23,
        double a31, double a32, double a33) {
    GV_Matrix ret(3,3);

    ret.set(0,0,a11);
    ret.set(0,1,a12);
    ret.set(0,2,a13);
    
    ret.set(1,0,a21);
    ret.set(1,1,a22);
    ret.set(1,2,a23);
    
    ret.set(2,0,a31);
    ret.set(2,1,a32);
    ret.set(2,2,a33);

    return ret;
}

GV_Matrix GV_Matrix::create3x1(
        double a11,
        double a21,
        double a31) {
    GV_Matrix ret(3,1);

    ret.set(0,0,a11);
    ret.set(1,0,a21);
    ret.set(2,0,a31);

    return ret;
}

void GV_Matrix::operator = (const GV_Matrix& other) {
    if (this == &other) {
        return;
    }

    init(other.rows, other.cols);

    for (int rc=0; rc<rows; ++rc) {
        for (int cc=0; cc<cols; ++cc) {
            m[rc][cc] = other.m[rc][cc];
        }
    }
    
}

bool GV_Matrix::operator == (const GV_Matrix& other) const {
    for (int rc=0; rc<rows; ++rc) {
        for (int cc=0; cc<cols; ++cc) {
            if (m[rc][cc] != other.m[rc][cc]) {
                return false;
            }
        }
    }

    return true;
}
    
GV_Matrix GV_Matrix::operator *= (double f) {
    *this = *this * f;
    return *this;
}

GV_Matrix GV_Matrix::operator * (const GV_Matrix& other) const {
    return multiplyWith(other);
}


/**
 * @return A*W
 * This matrix is not affected.
 */
GV_Matrix GV_Matrix::multiplyWith(const GV_Matrix& w) const {
    GV_Matrix r(rows, w.cols);

    for (int cc=0; cc<r.cols; ++cc) {
        for (int rc=0; rc<r.rows; ++rc) {
            for (int i=0; i<cols; ++i) {
                r.set(rc, cc, r.get(rc, cc) + get(rc, i)*w.get(i, cc));
            }
        }
    }

    return r;
}

    
void GV_Matrix::init(int r, int c) {
    clear();
    m = new double*[r];
    for (int i = 0; i < r; i++) {
        m[i] = new double[c];
    }
    rows = r;
    cols = c;
    reset();
}

void GV_Matrix::reset() {
    for (int rc=0; rc<rows; ++rc) {
        for (int cc=0; cc<cols; ++cc) {
            m[rc][cc] = 0.0;
        }
    }
}

void GV_Matrix::clear() {
    if (m==NULL) {
        return;
    }


    for (int rc=0; rc<rows; ++rc) {
        delete[] m[rc];
        m[rc] = NULL;
    }
    delete[] m;
    m = NULL;

}
    
void GV_Matrix::set3x3(double a11, double a12, double a13,
                       double a21, double a22, double a23,
                       double a31, double a32, double a33) {
    *this = create3x3(
        a11, a12, a13,
        a21, a22, a23,
        a31, a32, a33);
}
    
void GV_Matrix::set3x1(double a11,
                       double a21,
                       double a31) {
    *this = create3x1(
        a11,
        a21,
        a31);
}

void GV_Matrix::set(int r, int c, double v) {
    m[r][c] = v;
}

double GV_Matrix::get(int r, int c) const {
    return m[r][c];
}

void GV_Matrix::print() const {
    for (int rc=0; rc<rows; ++rc) {
        printRow(rc);
        printf("\n");
    }
}

void GV_Matrix::printRow(int r) const {
    for (int cc=0; cc<cols; ++cc) {
        printf("  %f", GV_Math::mround(m[r][cc]*10)/10.0);
    }
}

/**
 * ref for this matrix. This matrix is directly affected.
 */
bool GV_Matrix::ref(int startRow) {
    int pr, pc;

    // row which has elements most left becomes first row
    if ((pr = getPivotRow(startRow))==-1) {
        return false;
    }
    swapRows(startRow, pr);
    // where is the pivot element in the 1st row?
    if ((pc = getPivotCol(startRow))==-1) {
        return false;
    }
    multiplyRow(startRow, 1.0/m[startRow][pc]);

    for (int rc=startRow+1; rc<rows; ++rc) {
        addRow(rc, -m[rc][pc], startRow);
    }

    if (startRow<rows) {
        ref(startRow+1);
    }
    return true;
}

/**
 * rref for this matrix. This matrix is directly affected.
 */
bool GV_Matrix::rref() {
    bool ret = ref(0);

    for (int rc=0; rc<rows; ++rc) {
        int i=1;
        for (int cc=getPivotCol(rc)+1; cc<cols; ++cc, ++i) {
            if (rc+i<rows && fabs(m[rc+i][cc])>GV_MATRIX_TOLERANCE) {
                addRow(rc, -m[rc][cc] / m[rc+i][cc], rc+i);
            }
        }
    }

    return ret;
}

/**
 * Multiplies row 'r' with 'factor'.
 */
void GV_Matrix::multiplyRow(int r, double factor) {
    for (int cc=0; cc<cols; ++cc) {
        m[r][cc] *= factor;
    }
}

/**
 * Adds to row 'r' 'factor' times row 'r2'.
 */
void GV_Matrix::addRow(int r, double factor, int r2) {
    for (int cc=0; cc<cols; ++cc) {
        m[r][cc] += factor * m[r2][cc];
    }
}

/**
 * Swaps to rows..
 */
void GV_Matrix::swapRows(int r1, int r2) {
    if (r1==r2) {
        return;
    }

    double t;
    for (int cc=0; cc<cols; ++cc) {
        t = m[r1][cc];
        m[r1][cc] = m[r2][cc];
        m[r2][cc] = t;
    }
}

int GV_Matrix::getPivotRow(int startRow) {
    for (int cc=0; cc<cols; ++cc) {
        for (int rc=startRow; rc<rows; ++rc) {
            if (fabs(m[rc][cc])>GV_MATRIX_TOLERANCE) {
                return rc;
            }
        }
    }

    return -1;
}

int GV_Matrix::getPivotCol(int r) {
    for (int cc=0; cc<cols; ++cc) {
        if (fabs(m[r][cc])>GV_MATRIX_TOLERANCE) {
            return cc;
        }
    }

    return -1;
}

/**
 * @return A^T
 * This matrix is not affected.
 */
GV_Matrix GV_Matrix::transform() {
    GV_Matrix t(cols, rows);

    for (int cc=0; cc<cols; ++cc) {
        for (int rc=0; rc<rows; ++rc) {
            t.set(cc,rc, m[rc][cc]);
        }
    }

    return t;
}

/**
 * Appends matrix 'v' to the right side of this matrix
 * and returns the new matrix. This matrix is not affected.
 */
GV_Matrix GV_Matrix::append(const GV_Matrix& v) {
    GV_Matrix r(rows, cols+1);

    for (int rc=0; rc<rows; ++rc) {
        for (int cc=0; cc<cols; ++cc) {
            r.set(rc, cc, get(rc,cc));
        }
        r.set(rc, cols, v.get(rc,0));
    }

    return r;
}


GV_Matrix operator * (const GV_Matrix& matrix, double factor) {
    GV_Matrix ret = matrix;

    for (int rc=0; rc<ret.getRows(); ++rc) {
        for (int cc=0; cc<ret.getCols(); ++cc) {
            ret.set(rc, cc, ret.get(rc, cc)*factor);
        }
    }

    return ret;
}

GV_Matrix operator * (double factor, const GV_Matrix& matrix) {
    return matrix * factor;
}
