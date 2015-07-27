#ifndef RMATRIX_H
#define RMATRIX_H

#include "RMath.h"
#include "RVector.h"

#define RMATRIX_TOLERANCE 1.0e-8

/**
 * Simple matrix class.
 *
 * \ingroup math
 * \copyable
 */
class RMatrix {
public:
    RMatrix();
    RMatrix(int r, int c);
    RMatrix(const RMatrix& other);
    ~RMatrix();

    bool isValid() const;

    static RMatrix createIdentity2x2();
    static RMatrix createIdentity3x3();
    static RMatrix createIdentity(int size);
    static RMatrix create3x3(double a11, double a12, double a13, double a21,
                    double a22, double a23, double a31, double a32, double a33);
    static RMatrix create3x1(double a11, double a21, double a31);
    static RMatrix create2x3(double a11, double a12, double a13, double a21,
                    double a22, double a23);

    RMatrix& operator =(const RMatrix& other);
    bool operator ==(const RMatrix& other) const;
    RMatrix operator *=(double f);
    RMatrix operator *(const RMatrix& other) const;
    RMatrix multiplyWith(const RMatrix& w) const;
    double* operator [](int i);

    void init(int r, int c);
    void reset();
    void clear();

    void set3x3(double a11, double a12, double a13, double a21, double a22,
                    double a23, double a31, double a32, double a33);
    void set3x1(double a11, double a21, double a31);
    void set2x3(double a11, double a12, double a13, double a21, double a22,
                    double a23);

    void set(int r, int c, double v);
    double get(int r, int c) const;

    /**
     * \return Number of rows of this matrix.
     */
    int getRows() const {
        return rows;
    }
    /**
     * \return Number of columns of this matrix.
     */
    int getCols() const {
        return cols;
    }

    void print() const;
    void printRow(int r) const;

    bool rref();

    RMatrix getInverse() const;
    RMatrix getTransposed() const;
    RMatrix getAppended(const RMatrix& v) const;

    //
    // Used by 3D Rutger, previously GL project
    //
    virtual void generate(const RVector& location, const RVector& direction,
                          const double orientation, double dest[4][4]);
    virtual void location(RVector* source, double mat[4][4],
                          RVector* dest);
    virtual void direction(RVector* source, double mat[4][4],
                           RVector* dest);

    virtual void copy(double source[4][4], double dest[4][4]);
    virtual void multiplyMatrix(double mat1[4][4], double mat2[4][4],
                                double dest[4][4]);
    virtual void multiplyVector(RVector* source, double mat[4][4],
                                RVector* dest);
    virtual void identity(double mat[4][4]);
    virtual void translate(double matrix[4][4], double tx, double ty, double tz);
    virtual void scale(double matrix[4][4], double sx, double sy, double sz);
    virtual void rotate(double matrix[4][4], double ax, double ay, double az);
    virtual void dirRotate(double matrix[4][4], double dirx, double diry, double dirz);

    virtual double determinant(double m[4][4]);
    virtual void inverse(double s[4][4], double d[4][4]);

    virtual void print(double m[4][4]);

protected:
    bool ref(int startRow);
    void multiplyRow(int r, double factor);
    void addRow(int r, double factor, int r2);
    void swapRows(int r1, int r2);
    int getPivotRow(int startRow);
    int getPivotCol(int r);

private:
    double** m;
    int rows, cols;

    //
    // Used by 3D Rutger, previously GL project
    //
    /** base transformation matrix */
    double matrix[4][4];
    /** temporary storage matrix for internal calculation */
    double mat1[4][4];
    /** temporary storage matrix for internal calculation */
    double mat2[4][4];

};

RMatrix operator *(const RMatrix& matrix, double factor);
RMatrix operator *(double factor, const RMatrix& matrix);

//Q_DECLARE_METATYPE(RMatrix)
//Q_DECLARE_METATYPE(RMatrix*)

#endif
