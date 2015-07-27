#include <cassert>
#include <iostream>

#include "RDebug.h"
#include "RMatrix.h"

/**
 * Creates and empty matrix (zero columns and zero rows).
 */
RMatrix::RMatrix() :
	m(NULL), rows(0), cols(0) {
}

/**
 * Creates a matrix with \c r rows and \c c columns.
 * All elements are reset to 0.
 *
 * \param r number of rows
 *
 * \param c number of columns
 *
 */
RMatrix::RMatrix(int r, int c) :
	m(NULL), rows(0), cols(0) {
	init(r, c);
}

/**
 * Copy constructor.
 */
RMatrix::RMatrix(const RMatrix& other) :
	m(NULL), rows(0), cols(0) {
	(*this) = other;
}

/**
 * Deletes the matrix.
 */
RMatrix::~RMatrix() {
	clear();
}

/**
 * \return True if this matrix is a valid matrix (more than 0 columns and rows).
 */
bool RMatrix::isValid() const {
	return (cols > 0 && rows > 0 && m != NULL);
}

/**
 * \return An identity matrix with 2 rows and 2 columns:
 * \f$
 *   \left( \begin{array}{ccc}
 *     1 & 0 \\
 *     0 & 1
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::createIdentity2x2() {
	return createIdentity(2);
}

/**
 * \return An identity matrix with 3 rows and 3 columns:
 * \f$
 *   \left( \begin{array}{ccc}
 *     1 & 0 & 0 \\
 *     0 & 1 & 0 \\
 *     0 & 0 & 1
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::createIdentity3x3() {
	return createIdentity(3);
}

/**
 * \return An identity matrix with \c size rows and \c size columns:
 * \f$
 *   \left( \begin{array}{cccc}
 *     1 & 0 & 0 & \ldots \\
 *     0 & 1 & 0 & \ldots \\
 *     0 & 0 & 1 & \ldots \\
 *     \ldots & \ldots & \ldots & \ldots
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::createIdentity(int size) {
	RMatrix ret(size, size);
	for (int i = 0; i < size; ++i) {
		ret.set(i, i, 1);
	}
	return ret;
}

/**
 * \return A matrix with 3 rows and 3 columns:
 * \f$
 *   \left( \begin{array}{ccc}
 *     a_{11} & a_{12} & a_{13} \\
 *     a_{21} & a_{22} & a_{23} \\
 *     a_{31} & a_{32} & a_{33}
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::create3x3(double a11, double a12, double a13, double a21,
		double a22, double a23, double a31, double a32, double a33) {
	RMatrix ret(3, 3);

	ret.set(0, 0, a11);
	ret.set(0, 1, a12);
	ret.set(0, 2, a13);

	ret.set(1, 0, a21);
	ret.set(1, 1, a22);
	ret.set(1, 2, a23);

	ret.set(2, 0, a31);
	ret.set(2, 1, a32);
	ret.set(2, 2, a33);

	return ret;
}

/**
 * \return A matrix with 3 rows and 1 column (vector):
 * \f$
 *   \left( \begin{array}{ccc}
 *     a_{11} \\
 *     a_{21} \\
 *     a_{31}
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::create3x1(double a11, double a21, double a31) {
	RMatrix ret(3, 1);

	ret.set(0, 0, a11);
	ret.set(1, 0, a21);
	ret.set(2, 0, a31);

	return ret;
}

/**
 * \return A matrix with 2 rows and 3 columns:
 * \f$
 *   \left( \begin{array}{ccc}
 *     a_{11} & a_{12} & a_{13} \\
 *     a_{21} & a_{22} & a_{23}
 *   \end{array} \right)
 * \f$
 */
RMatrix RMatrix::create2x3(double a11, double a12, double a13, double a21,
		double a22, double a23) {
	RMatrix ret(2, 3);

	ret.set(0, 0, a11);
	ret.set(0, 1, a12);
	ret.set(0, 2, a13);

	ret.set(1, 0, a21);
	ret.set(1, 1, a22);
	ret.set(1, 2, a23);

	return ret;
}

/**
 * Assignment operator.
 */
RMatrix& RMatrix::operator =(const RMatrix& other) {
	if (this == &other) {
		return *this;
	}

	init(other.rows, other.cols);

	for (int rc = 0; rc < rows; ++rc) {
		for (int cc = 0; cc < cols; ++cc) {
			m[rc][cc] = other.m[rc][cc];
		}
	}
	return *this;
}

/**
 * Comparison. All elements of the matrix as well as the matrix
 * dimensions have to match for the two matrices to be equal.
 */
bool RMatrix::operator ==(const RMatrix& other) const {
	if (rows != other.rows || cols != other.cols) {
		return false;
	}

	for (int rc = 0; rc < rows; ++rc) {
		for (int cc = 0; cc < cols; ++cc) {
			if (m[rc][cc] != other.m[rc][cc]) {
				return false;
			}
		}
	}

	return true;
}

/**
 * Multiplies all elements of this matrix with the given factor
 * and returns this matrix.
 */
RMatrix RMatrix::operator *=(double f) {
	*this = *this * f;
	return *this;
}

/**
 * Multiplies two matrices.
 */
RMatrix RMatrix::operator *(const RMatrix& other) const {
	return multiplyWith(other);
}

/**
 * \return \f$A \cdot W\f$
 * This matrix is not affected.
 */
RMatrix RMatrix::multiplyWith(const RMatrix& w) const {
	RMatrix r(rows, w.cols);

	for (int cc = 0; cc < r.cols; ++cc) {
		for (int rc = 0; rc < r.rows; ++rc) {
			for (int i = 0; i < cols; ++i) {
				r.set(rc, cc, r.get(rc, cc) + get(rc, i) * w.get(i, cc));
			}
		}
	}

	return r;
}

/**
 * Initializes the matrix for the given size and resets
 * all elements to 0.
 */
void RMatrix::init(int r, int c) {
	clear();
	m = new double*[r];
	for (int i = 0; i < r; i++) {
		m[i] = new double[c];
	}
	rows = r;
	cols = c;
	reset();
}

/**
 * Resets all elements to 0.
 */
void RMatrix::reset() {
	for (int rc = 0; rc < rows; ++rc) {
		for (int cc = 0; cc < cols; ++cc) {
			m[rc][cc] = 0.0;
		}
	}
}

/**
 * Deletes the elements of this matrix and resets the matrix size to 0/0.
 */
void RMatrix::clear() {
	if (m == NULL) {
		return;
	}

	for (int rc = 0; rc < rows; ++rc) {
		delete[] m[rc];
		m[rc] = NULL;
	}

	delete[] m;
	m = NULL;
	cols = 0;
	rows = 0;
}

/**
 * Sets the dimension of this matrix to 3x3 and the elements
 * to the given elements.
 */
void RMatrix::set3x3(double a11, double a12, double a13, double a21,
		double a22, double a23, double a31, double a32, double a33) {
	*this = create3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33);
}

/**
 * Sets the dimension of this matrix to 3x1 and the elements
 * to the given elements.
 */
void RMatrix::set3x1(double a11, double a21, double a31) {
	*this = create3x1(a11, a21, a31);
}

/**
 * Sets the dimension of this matrix to 2x3 and the elements
 * to the given elements.
 */
void RMatrix::set2x3(double a11, double a12, double a13, double a21,
		double a22, double a23) {
	*this = create2x3(a11, a12, a13, a21, a22, a23);
}

/**
 * Sets the element at the given row/column to the given value \c v.
 *
 * \param r the row
 *
 * \param c the column
 *
 * \param v the new value at the given row / column position
 */
void RMatrix::set(int r, int c, double v) {
	assert(r<rows);
	assert(c<cols);

	m[r][c] = v;
}

/**
 * \param r the row
 *
 * \param c the column
 *
 * \return The element at the given row/column.
 */
double RMatrix::get(int r, int c) const {
	assert(r<rows);
	assert(c<cols);

	return m[r][c];
}


/**
 * Changes this matrix into its \c ref.
 *
 * \internal
 *
 * \return True on success.
 */
bool RMatrix::ref(int startRow) {
	int pr, pc;

	// row which has elements most left becomes first row
	if ((pr = getPivotRow(startRow)) == -1) {
		return false;
	}
	swapRows(startRow, pr);
	// where is the pivot element in the 1st row?
	if ((pc = getPivotCol(startRow)) == -1) {
		return false;
	}
	multiplyRow(startRow, 1.0 / m[startRow][pc]);

	for (int rc = startRow + 1; rc < rows; ++rc) {
		addRow(rc, -m[rc][pc], startRow);
	}

	if (startRow < rows) {
		ref(startRow + 1);
	}
	return true;
}

/**
 * Changes this matrix into its \c rref.
 *
 * \sa http://en.wikipedia.org/wiki/Rref#Reduced_row_echelon_form
 *
 * \return True on success.
 */
bool RMatrix::rref() {
	bool ret = ref(0);

	for (int rc = 0; rc < rows; ++rc) {
		int i = 1;
		for (int cc = getPivotCol(rc) + 1; cc < cols; ++cc, ++i) {
			if (rc + i < rows && fabs(m[rc + i][cc]) > RMATRIX_TOLERANCE) {
				addRow(rc, -m[rc][cc] / m[rc + i][cc], rc + i);
			}
		}
	}

	return ret;
}

/**
 * Multiplies row \c r with \c factor.
 *
 * \internal
 *
 * \param r the row
 *
 * \param factor the factor to apply to the row
 *
 */
void RMatrix::multiplyRow(int r, double factor) {
	for (int cc = 0; cc < cols; ++cc) {
		m[r][cc] *= factor;
	}
}

/**
 * Adds to row \c r \c factor times row \c r2.
 *
 * \internal
 *
 * \param r the row
 *
 * \param r2 the row 2
 *
 * \param factor The factor to apply to \c r2 before it is added to row \c r.
 */
void RMatrix::addRow(int r, double factor, int r2) {
	for (int cc = 0; cc < cols; ++cc) {
		m[r][cc] += factor * m[r2][cc];
	}
}

/**
 * Swaps the given two rows.
 *
 * \internal
 *
 * \param r1 the row 1
 *
 * \param r2 the row 2
 *
 */
void RMatrix::swapRows(int r1, int r2) {
	if (r1 == r2) {
		return;
	}

	double t;
	for (int cc = 0; cc < cols; ++cc) {
		t = m[r1][cc];
		m[r1][cc] = m[r2][cc];
		m[r2][cc] = t;
	}
}

/**
 * \internal
 *
 * \param startRow the start row
 *
 * \return The pivot row of this matrix, starting with the given row \c startRow.
 */
int RMatrix::getPivotRow(int startRow) {
	for (int cc = 0; cc < cols; ++cc) {
		for (int rc = startRow; rc < rows; ++rc) {
			if (fabs(m[rc][cc]) > RMATRIX_TOLERANCE) {
				return rc;
			}
		}
	}

	return -1;
}

/**
 * \internal
 *
 * \param r the row
 *
 * \return The pivot column of the given matrix row \c r.
 */
int RMatrix::getPivotCol(int r) {
	for (int cc = 0; cc < cols; ++cc) {
		if (fabs(m[r][cc]) > RMATRIX_TOLERANCE) {
			return cc;
		}
	}

	return -1;
}

/**
 * \return The inverse matrix of this matrix \f$A^{-1}\f$ or
 * an empty matrix if this matrix is not invertible.
 */
RMatrix RMatrix::getInverse() const {
	if (cols != rows) {
		RDebug::warning("RMatrix::getInverse: "
			"matrix not invertible");
		return RMatrix();
	}

	RMatrix a = getAppended(createIdentity(cols));
	if (!a.rref()) {
		RDebug::warning("RMatrix::getInverse: "
			"matrix not invertible (no rref)");
		return RMatrix();
	}

	RMatrix ret(rows, cols);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			ret.set(r, c, a.get(r, c + cols));
		}
	}

	return ret;
}

/**
 * \return The transposed matrix \f$A^T\f$.
 * This matrix is not affected.
 */
RMatrix RMatrix::getTransposed() const {
	RMatrix t(cols, rows);

	for (int cc = 0; cc < cols; ++cc) {
		for (int rc = 0; rc < rows; ++rc) {
			t.set(cc, rc, m[rc][cc]);
		}
	}

	return t;
}

/**
 * Appends matrix \c v to the right side of this matrix
 * and returns the new matrix. This matrix is not affected.
 *
 * \param v the matrix to append to this matrix
 *
 */
RMatrix RMatrix::getAppended(const RMatrix& v) const {
	if (rows != v.rows) {
		RDebug::warning("RMatrix::getAppended: "
			"matrices don't have the same number of rows");
		return RMatrix();
	}

	RMatrix r(rows, cols + v.cols);

	for (int rc = 0; rc < rows; ++rc) {
		for (int cc = 0; cc < cols; ++cc) {
			r.set(rc, cc, get(rc, cc));
		}
		for (int cc = cols; cc < cols + v.cols; ++cc) {
			r.set(rc, cc, v.get(rc, cc - cols));
		}
	}

	return r;
}

/**
 * \return \f$matrix \cdot factor\f$.
 */
RMatrix operator *(const RMatrix& matrix, double factor) {
	RMatrix ret = matrix;

	for (int rc = 0; rc < ret.getRows(); ++rc) {
		for (int cc = 0; cc < ret.getCols(); ++cc) {
			ret.set(rc, cc, ret.get(rc, cc) * factor);
		}
	}

	return ret;
}

/**
 * \return \f$matrix \cdot factor\f$.
 */
RMatrix operator *(double factor, const RMatrix& matrix) {
	return matrix * factor;
}


//
// Updated Rutger for 3D as was used in GL project
//


/**
 * Generate transformation matrix based on direction vector, orientation angle
 * @param location location vector
 * @param direction direction vector
 * @param orientation orientation angle in radians
 * @param dest destination matrix to be created
 */
void RMatrix::generate(const RVector& location, const RVector& direction,
                         const double orientation, double matrix[4][4]) {
    // TODO: scaling not implemented yet, scaling to be done first here
/*
    // Get our direction vector (the X vector component of the matrix)
    // and make sure it's normalized into a unit vector
    RVector xAxis(direction);
    xAxis.normalize();

    // Build the Z vector of the matrix (handle the degenerate case
    // in the way that 3DS does) -- This is not the TRUE vector, only
    // a reference vector.
    RVector zAxis;

    if (!xAxis.y && !xAxis.x)
        / *
         * Below line is changed by Rutger 2007-03-22, otherwise if direction
         * is along z-axis the original y axis will also rotate to -x-axis. The
         * Biluna convention is that if direction in z-axis only than rotate only
         * around the y-axis
         * zAxis = RVector(0.0, -xAxis.z, 0.0);
         * /
                zAxis = RVector(-xAxis.z, 0.0, 0.0);
    else
        zAxis = RVector(0.0, 0.0, 1.0);

    // Build the Y axis vector based on the two existing vectors
    RVector yAxis = RVector::crossP(zAxis, xAxis);
    yAxis.normalize();

    // Correct the Z reference vector
    zAxis = RVector::crossP(yAxis, xAxis);
    zAxis.normalize();
    zAxis = -zAxis;

    // Generate rotation matrix without orientation roll included
    double gmat1[4][4];
    identity(gmat1);

    gmat1[0][0] = xAxis.x; gmat1[0][1] = xAxis.y; gmat1[0][2] = xAxis.z;
    gmat1[1][0] = yAxis.x; gmat1[1][1] = yAxis.y; gmat1[1][2] = yAxis.z;
    gmat1[2][0] = zAxis.x; gmat1[2][1] = zAxis.y; gmat1[2][2] = zAxis.z;
*/

    // Generate rotation matrix without orientation roll included
    double gmat1[4][4];
    identity(gmat1);
        dirRotate(gmat1, direction.x, direction.y, direction.z);

    // Generate the X rotation matrix for orientation roll (bank)
    double gmat2[4][4];
    identity(gmat2);

    rotate(gmat2, orientation, 0.0, 0.0);

    // Concatinate them for a complete rotation matrix that includes
    // all rotations roll * rot
    // multiplyMatrix(gmat2, gmat1, dest);
    multiplyMatrix(gmat2, gmat1, mat1);
    multiplyMatrix(matrix, mat1, mat2);
    copy(mat2, matrix);

    // translate matrix as last
    // translate(dest, location.x, location.y, location.z);
    translate(matrix, location.x, location.y, location.z);
}

/**
 * Create location vector (including translation of mat)
 * @param source source vector
 * @param mat transformation matrix
 * @param dest destination vector
 */
void RMatrix::location(RVector* source, double mat[4][4],
                       RVector* dest) {
    multiplyVector(source, mat, dest);
}

/**
 * Create direction vector (excluding translation of mat)
 * @param source source vector
 * @param mat transformation matrix
 * @param dest destination vector
 */
void RMatrix::direction(RVector* source, double mat[4][4],
                          RVector* dest) {
    double dmat[4][4];

    copy(mat, dmat);
    dmat[3][0] = 0.0; dmat[3][1] = 0.0; dmat[3][2] = 0.0;
    multiplyVector(source, dmat, dest);
}

/**
 * Copy a temporary matrix to an other matrix
 * @param source source matrix
 * @param dest destination matrix
 */
void RMatrix::copy(double source[4][4], double dest[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

/**
 * Multiply matrix (with transformation matrix)
 * @param mat1 matrix 1
 * @param mat2 matrix 2
 * @param dest result matrix of multiplication of mat1 with mat2
 */
void RMatrix::multiplyMatrix(double mat1[4][4], double mat2[4][4],
                               double dest[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = mat1[i][0] * mat2[0][j] + mat1[i][1] * mat2[1][j]
                    + mat1[i][2] * mat2[2][j] + mat1[i][3] * mat2[3][j];
        }
    }
}

/**
 * Mutliply a vector (with transformation matrix)
 * @param source source vector
 * @param mat transformation matrix
 * @param dest destination vector
 */
void RMatrix::multiplyVector(RVector* source, double mat[4][4],
                               RVector* dest) {
    dest->x = source->x * mat[0][0] + source->y * mat[1][0]
            + source->z * mat[2][0] + mat[3][0];
    dest->y = source->x * mat[0][1] + source->y * mat[1][1]
            + source->z * mat[2][1] + mat[3][1];
    dest->z = source->x * mat[0][2] + source->y * mat[1][2]
            + source->z * mat[2][2] + mat[3][2];
}

/**
 * Create identity matrix
 * @param mat matrix to be set as identity matrix
 */
void RMatrix::identity(double mat[4][4]) {
    mat[0][0] = 1.0; mat[0][1] = 0.0; mat[0][2] = 0.0; mat[0][3] = 0.0;
    mat[1][0] = 0.0; mat[1][1] = 1.0; mat[1][2] = 0.0; mat[1][3] = 0.0;
    mat[2][0] = 0.0; mat[2][1] = 0.0; mat[2][2] = 1.0; mat[2][3] = 0.0;
    mat[3][0] = 0.0; mat[3][1] = 0.0; mat[3][2] = 0.0; mat[3][3] = 1.0;
}

/**
 * Translate matrix based on a vector(tx, ty, tz)
 * @param matrix to be set
 * @param tx translation in x-axis direction
 * @param ty translation in y-axis direction
 * @param tz translation in z-axis direction
 */
void RMatrix::translate(double matrix[4][4], double tx, double ty, double tz) {
    double tmat[4][4];

    tmat[0][0] = 1.0; tmat[0][1] = 0.0; tmat[0][2] = 0.0; tmat[0][3] = 0.0;
    tmat[1][0] = 0.0; tmat[1][1] = 1.0; tmat[1][2] = 0.0; tmat[1][3] = 0.0;
    tmat[2][0] = 0.0; tmat[2][1] = 0.0; tmat[2][2] = 1.0; tmat[2][3] = 0.0;
    tmat[3][0] = tx; tmat[3][1] = ty; tmat[3][2] = tz; tmat[3][3] = 1.0;

    multiplyMatrix(matrix, tmat, mat1);
    copy(mat1, matrix);
}

/**
 * Scale matrix using factors sx, sy, sz
 * @param matrix to be set
 * @param sx scaling in x-axis direction
 * @param sy scaling in y-axis direction
 * @param sz scaling in z-axis direction
 */
void RMatrix::scale(double matrix[4][4], double sx, double sy, double sz) {
    double smat[4][4];

    smat[0][0] = sx; smat[0][1] = 0.0; smat[0][2] = 0.0; smat[0][3] = 0.0;
    smat[1][0] = 0.0; smat[1][1] = sy; smat[1][2] = 0.0; smat[1][3] = 0.0;
    smat[2][0] = 0.0; smat[2][1] = 0.0; smat[2][2] = sz; smat[2][3] = 0.0;
    smat[3][0] = 0.0; smat[3][1] = 0.0; smat[3][2] = 0.0; smat[3][3] = 1.0;

    multiplyMatrix(matrix, smat, mat1);
    copy(mat1, matrix);
}

/**
 * Rotate matrix with angles ax, ay, az
 * @param matrix to be set
 * @param ax rotation around x-axis
 * @param ay rotation around y-axis
 * @param az rotation around z-axis
 */
void RMatrix::rotate(double matrix[4][4], double ax, double ay, double az) {
    double xmat[4][4];
    double ymat[4][4];
    double zmat[4][4];

    xmat[0][0] = 1.0; xmat[0][1] = 0.0; xmat[0][2] = 0.0; xmat[0][3] = 0.0;
    xmat[1][0] = 0.0; xmat[1][1] = cos(ax); xmat[1][2] = sin(ax); xmat[1][3] = 0.0;
    xmat[2][0] = 0.0; xmat[2][1] = -sin(ax); xmat[2][2] = cos(ax); xmat[2][3] = 0.0;
    xmat[3][0] = 0.0; xmat[3][1] = 0.0; xmat[3][2] = 0.0; xmat[3][3] = 1.0;

    ymat[0][0] = cos(ay); ymat[0][1] = 0.0; ymat[0][2] = -sin(ay); ymat[0][3] = 0.0;
    ymat[1][0] = 0.0; ymat[1][1] = 1.0; ymat[1][2] = 0.0; ymat[1][3] = 0.0;
    ymat[2][0] = sin(ay); ymat[2][1] = 0.0; ymat[2][2] = cos(ay); ymat[2][3] = 0.0;
    ymat[3][0] = 0.0; ymat[3][1] = 0.0; ymat[3][2] = 0.0; ymat[3][3] = 1.0;

    zmat[0][0] = cos(az); zmat[0][1] = sin(az); zmat[0][2] = 0.0; zmat[0][3] = 0.0;
    zmat[1][0] = -sin(az); zmat[1][1] = cos(az); zmat[1][2] = 0.0; zmat[1][3] = 0.0;
    zmat[2][0] = 0.0; zmat[2][1] = 0.0; zmat[2][2] = 1.0; zmat[2][3] = 0.0;
    zmat[3][0] = 0.0; zmat[3][1] = 0.0; zmat[3][2] = 0.0; zmat[3][3] = 1.0;

    multiplyMatrix(matrix, ymat, mat1);
    multiplyMatrix(mat1, xmat, mat2);
    multiplyMatrix(mat2, zmat, matrix);
}


/**
 * Rotate matrix based on direction vector coordinates dirx, diry, dirz
 * @param matrix to be set
 * @param dirx x-axis coordinate
 * @param diry y-axis coordinate
 * @param dirz z-axis coordinate
 */
void RMatrix::dirRotate(double matrix[4][4], double dirx, double diry, double dirz) {
    // Get our direction vector (the X vector component of the matrix)
    // and make sure it's normalized into a unit vector
    RVector xAxis(dirx, diry, dirz);
    xAxis.getUnitVector();

    // Build the Z vector of the matrix (handle the degenerate case
    // in the way that 3DS does) -- This is not the TRUE vector, only
    // a reference vector.
    RVector zAxis;

    if (!xAxis.y && !xAxis.x)
        /*
         * Below line is changed by Rutger 2007-03-22, otherwise if direction
         * is along z-axis the original y axis will also rotate to -x-axis. The
         * Biluna convention is that if direction in z-axis only than rotate only
         * around the y-axis
         * zAxis = RVector(0.0, -xAxis.z, 0.0);
         */
        zAxis = RVector(-xAxis.z, 0.0, 0.0);
    else
        zAxis = RVector(0.0, 0.0, 1.0);

    // Build the Y axis vector based on the two existing vectors
    RVector yAxis = RVector::getCrossProduct(zAxis, xAxis);
    yAxis.getUnitVector();

    // Correct the Z reference vector
    zAxis = RVector::getCrossProduct(yAxis, xAxis);
    zAxis.getUnitVector();
    zAxis = -zAxis;

    // Generate rotation matrix
    double dmat[4][4];
    identity(dmat);

    dmat[0][0] = xAxis.x; dmat[0][1] = xAxis.y; dmat[0][2] = xAxis.z;
    dmat[1][0] = yAxis.x; dmat[1][1] = yAxis.y; dmat[1][2] = yAxis.z;
    dmat[2][0] = zAxis.x; dmat[2][1] = zAxis.y; dmat[2][2] = zAxis.z;

    multiplyMatrix(matrix, dmat, mat1);
    copy(mat1, matrix);
}


/**
 * Determinant for creating inverse matrix
 * @param m matrix
 */
double RMatrix::determinant(double m[4][4]) {
    return
    m[3][0]*m[2][1]*m[1][2]*m[0][3]-m[2][0]*m[3][1]*m[1][2]*m[0][3]-
    m[3][0]*m[1][1]*m[2][2]*m[0][3]+m[1][0]*m[3][1]*m[2][2]*m[0][3]+
    m[2][0]*m[1][1]*m[3][2]*m[0][3]-m[1][0]*m[2][1]*m[3][2]*m[0][3]-
    m[3][0]*m[2][1]*m[0][2]*m[1][3]+m[2][0]*m[3][1]*m[0][2]*m[1][3]+
    m[3][0]*m[0][1]*m[2][2]*m[1][3]-m[0][0]*m[3][1]*m[2][2]*m[1][3]-
    m[2][0]*m[0][1]*m[3][2]*m[1][3]+m[0][0]*m[2][1]*m[3][2]*m[1][3]+
    m[3][0]*m[1][1]*m[0][2]*m[2][3]-m[1][0]*m[3][1]*m[0][2]*m[2][3]-
    m[3][0]*m[0][1]*m[1][2]*m[2][3]+m[0][0]*m[3][1]*m[1][2]*m[2][3]+
    m[1][0]*m[0][1]*m[3][2]*m[2][3]-m[0][0]*m[1][1]*m[3][2]*m[2][3]-
    m[2][0]*m[1][1]*m[0][2]*m[3][3]+m[1][0]*m[2][1]*m[0][2]*m[3][3]+
    m[2][0]*m[0][1]*m[1][2]*m[3][3]-m[0][0]*m[2][1]*m[1][2]*m[3][3]-
    m[1][0]*m[0][1]*m[2][2]*m[3][3]+m[0][0]*m[1][1]*m[2][2]*m[3][3];
}

/**
 * Inverse matrix
 * @param s source matrix
 * @param d destination matrix
 */
void RMatrix::inverse(double s[4][4], double d[4][4]) {
    double det = determinant(s);
    if (det == 0) return;

    d[0][0] = (-s[3][1]*s[2][2]*s[1][3] +s[2][1]*s[3][2]*s[1][3] +s[3][1]*s[1][2]*s[2][3]
    -s[1][1]*s[3][2]*s[2][3] -s[2][1]*s[1][2]*s[3][3] +s[1][1]*s[2][2]*s[3][3])/det;
    d[1][0] = ( s[3][0]*s[2][2]*s[1][3] -s[2][0]*s[3][2]*s[1][3] -s[3][0]*s[1][2]*s[2][3]
    +s[1][0]*s[3][2]*s[2][3] +s[2][0]*s[1][2]*s[3][3] -s[1][0]*s[2][2]*s[3][3])/det;
    d[2][0] = (-s[3][0]*s[2][1]* s[1][3] +s[2][0]*s[3][1]*s[1][3] +s[3][0]*s[1][1]*s[2][3]
    -s[1][0]*s[3][1]*s[2][3] -s[2][0]*s[1][1]*s[3][3] +s[1][0]*s[2][1]* s[3][3])/det;
    d[3][0] = (s[3][0]*s[2][1]* s[1][2] -s[2][0]*s[3][1]*s[1][2] -s[3][0]*s[1][1]*s[2][2]
    +s[1][0]*s[3][1]*s[2][2] +s[2][0]*s[1][1]*s[3][2] -s[1][0]*s[2][1]* s[3][2])/det;
    d[0][1] = (s[3][1]*s[2][2]*s[0][3] -s[2][1]*s[3][2]*s[0][3] -s[3][1]*s[0][2]*s[2][3]
    +s[0][1]*s[3][2]*s[2][3] +s[2][1]*s[0][2]*s[3][3] -s[0][1]*s[2][2]*s[3][3])/det;
    d[1][1] = (-s[3][0]*s[2][2]*s[0][3] +s[2][0]*s[3][2]*s[0][3] +s[3][0]*s[0][2]*s[2][3]
    -s[0][0]*s[3][2]*s[2][3] -s[2][0]*s[0][2]*s[3][3] +s[0][0]*s[2][2]*s[3][3])/det;
    d[2][1] = (s[3][0]*s[2][1]* s[0][3] -s[2][0]*s[3][1]*s[0][3] -s[3][0]*s[0][1]*s[2][3]
    +s[0][0]*s[3][1]*s[2][3] +s[2][0]*s[0][1]*s[3][3] -s[0][0]*s[2][1]* s[3][3])/det;
    d[3][1] = (-s[3][0]*s[2][1]* s[0][2] +s[2][0]*s[3][1]*s[0][2] +s[3][0]*s[0][1]*s[2][2]
    -s[0][0]*s[3][1]*s[2][2] -s[2][0]*s[0][1]*s[3][2] +s[0][0]*s[2][1]* s[3][2])/det;
    d[0][2] = (-s[3][1]*s[1][2]* s[0][3] +s[1][1]*s[3][2]*s[0][3] +s[3][1]*s[0][2]*s[1][3]
    -s[0][1]*s[3][2]*s[1][3] -s[1][1]*s[0][2]*s[3][3] +s[0][1]*s[1][2]* s[3][3])/det;
    d[1][2] = ( s[3][0]*s[1][2]* s[0][3] -s[1][0]*s[3][2]*s[0][3] -s[3][0]*s[0][2]*s[1][3]
    +s[0][0]*s[3][2]*s[1][3] +s[1][0]*s[0][2]*s[3][3] -s[0][0]*s[1][2]* s[3][3])/det;
    d[2][2] = (-s[3][0]*s[1][1]* s[0][3] +s[1][0]*s[3][1]*s[0][3] +s[3][0]*s[0][1]*s[1][3]
    -s[0][0]*s[3][1]*s[1][3] -s[1][0]*s[0][1]*s[3][3] +s[0][0]*s[1][1]* s[3][3])/det;
    d[3][2] = (s[3][0]*s[1][1]* s[0][2] -s[1][0]*s[3][1]*s[0][2] -s[3][0]*s[0][1]*s[1][2]
    +s[0][0]*s[3][1]*s[1][2] +s[1][0]*s[0][1]*s[3][2] -s[0][0]*s[1][1]* s[3][2])/det;
    d[0][3] = (s[2][1]* s[1][2]* s[0][3] -s[1][1]*s[2][2]*s[0][3] -s[2][1]* s[0][2]*s[1][3]
    +s[0][1]*s[2][2]*s[1][3] +s[1][1]*s[0][2]*s[2][3] -s[0][1]*s[1][2]* s[2][3])/det;
    d[1][3] = (-s[2][0]* s[1][2]* s[0][3] +s[1][0]*s[2][2]*s[0][3] +s[2][0]* s[0][2]*s[1][3]
    -s[0][0]*s[2][2]*s[1][3] -s[1][0]*s[0][2]*s[2][3] +s[0][0]*s[1][2]* s[2][3])/det;
    d[2][3] = (s[2][0]* s[1][1]* s[0][3] -s[1][0]*s[2][1]* s[0][3] -s[2][0]* s[0][1]*s[1][3]
    +s[0][0]*s[2][1]* s[1][3] +s[1][0]*s[0][1]*s[2][3] -s[0][0]*s[1][1]* s[2][3])/det;
    d[3][3] = (-s[2][0]* s[1][1]* s[0][2] +s[1][0]*s[2][1]* s[0][2] +s[2][0]* s[0][1]*s[1][2]
    -s[0][0]*s[2][1]* s[1][2] -s[1][0]*s[0][1]*s[2][2] +s[0][0]*s[1][1]* s[2][2])/det;
}

/**
 * Print matrix
 * @param m matrix[4][4]
 */
void RMatrix::print(double m[4][4]) {
    std::cout<<" "<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<"\n";
    std::cout<<" "<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<"\n";
    std::cout<<" "<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<"\n";
    std::cout<<" "<<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<"\n";
}
