#include <cmath>

#include "RLine.h"
#include "RBox.h"

/**
 * Creates a line object with invalid start and end points.
 */
RLine::RLine() {
}

/**
 * Creates a line object with the given start and end point.
 */
RLine::RLine(const RVector& startPoint, const RVector& endPoint) :
	startPoint(startPoint), endPoint(endPoint) {
}

RLine::~RLine() {
}

double RLine::getLength() const{
    return startPoint.getDistanceTo(endPoint);
}

double RLine::getAngle() const{
    return startPoint.getAngleTo(endPoint);
}

RVector RLine::getStartPoint() const{
	return startPoint;
}

void RLine::setStartPoint(const RVector& vector){
	startPoint = vector;
}

RVector RLine::getEndPoint() const{
	return endPoint;
}

void RLine::setEndPoint(const RVector& vector){
	endPoint = vector;
}

RVector RLine::getMiddlePoint() const {
    return (startPoint + endPoint) / 2.0;
}

RBox RLine::getBoundingBox() const {
	return RBox(RVector::getMinimum(startPoint, endPoint), RVector::getMaximum(
			startPoint, endPoint));
}

QList<RVector> RLine::getEndPoints() const {
    QList<RVector> ret;
    ret.append(startPoint);
    ret.append(endPoint);
    return ret;
}

QList<RVector> RLine::getMiddlePoints() const {
    QList<RVector> ret;
    ret.append(getMiddlePoint());
    return ret;
}

QList<RVector> RLine::getCenterPoints() const {
    return getMiddlePoints();
}

QList<RVector> RLine::getPointsWithDistanceToEnd(double distance) const {
    QList<RVector> ret;
    double a1 = getAngle();

    RVector dv;
    dv.setPolar(distance, a1);

    ret.append(startPoint + dv);
    ret.append(endPoint - dv);

    return ret;
}

RVector RLine::getVectorTo(const RVector& point, bool limited) const {

	RVector ae = endPoint - startPoint;
	RVector ap = point - startPoint;

	if (ae.getMagnitude2d() < 1.0e-6) {
		return RVector::invalid;
	}

	double b = RVector::getDotProduct(ap, ae) / RVector::getDotProduct(ae, ae);

	if (limited && (b < 0 || b > 1.0)) {
		return getVectorFromEndpointTo(point);
	}

	RVector closestPoint = startPoint + ae * b;

	return point - closestPoint;
}

/**
 * \todo implement limited param
 */
/*
RVector RLine::getVectorTo(const RLine& line, bool limited) const {
    RVector u = endPoint - startPoint;
    RVector v = line.endPoint - line.startPoint;
    RVector w = startPoint - line.startPoint;
    double a = RVector::getDotProduct(u,u);        // always >= 0
    double b = RVector::getDotProduct(u,v);
    double c = RVector::getDotProduct(v,v);        // always >= 0
    double d = RVector::getDotProduct(u,w);
    double e = RVector::getDotProduct(v,w);
    double D = a*c - b*b;       // always >= 0
    double sc, tc;

    // compute the line parameters of the two closest points
    if (D < 1.0e-9) {         // the lines are almost parallel
        sc = 0.0;
        tc = (b>c ? d/b : e/c);   // use the largest denominator
    }
    else {
        sc = (b*e - c*d) / D;
        tc = (a*e - b*d) / D;
    }

    // get the difference of the two closest points
    return w + (sc * u) - (tc * v);  // = L1(sc) - L2(tc)
}
*/

/**
 * \copydoc RShape::intersectsWith
 *
 * \todo implement for all other shapes
 */
//bool RLine::intersectsWith(const RShape& other, bool limited) const {
//	const RTriangle* triangle = dynamic_cast<const RTriangle*> (&other);
//	if (triangle != NULL) {
//		return triangle->intersectsWith(*this, limited);
//	}
//
//	return false;
//}

//QList<RVector> RLine::getIntersectionPoints(const RShape& other,
//    bool limited) const {
//
//	const RLine* line = dynamic_cast<const RLine*> (&other);
//	if (line != NULL) {
//        QList<RVector> res;
//        double a1 = endPoint.y - startPoint.y;
//        double b1 = startPoint.x - endPoint.x;
//        double c1 = a1*startPoint.x + b1*startPoint.y;
//
//        double a2 = line->endPoint.y - line->startPoint.y;
//        double b2 = line->startPoint.x - line->endPoint.x;
//        double c2 = a2*line->startPoint.x + b2*line->startPoint.y;
//
//        double det = a1*b2 - a2*b1;
//        if (fabs(det) < 1.0e-6) {
//            return res;
//        }else{
//            RVector v(
//                (b2*c1 - b1*c2)/det,
//                (a1*c2 - a2*c1)/det
//            );
//
//            if (!limited || (isOnShape(v) && line->isOnShape(v))) {
//                res.append(v);
//                return res;
//            }
//        }
//        return res;
//	}
//
//	const RTriangle* triangle = dynamic_cast<const RTriangle*> (&other);
//	if (triangle != NULL) {
//		return triangle->getIntersectionPoints(*this, limited);
//	}
//
//    return QList<RVector>();
//}

void RLine::clipToXY(const RBox& box) {
    double x1 = startPoint.x;
    double y1 = startPoint.y;
    double x2 = endPoint.x;
    double y2 = endPoint.y;
    double xmin = box.getMinimum().x;
    double ymin = box.getMinimum().y;
    double xmax = box.getMaximum().x;
    double ymax = box.getMaximum().y;

    double deltaX, deltaY, p, q;
    double u1 = 0.0, u2 = 1.0;
    double r;

    deltaX = (x2 - x1);
    deltaY = (y2 - y1);

    /*
     * left edge, right edge, bottom edge and top edge checking
     */
    double pPart[] = {-1 * deltaX, deltaX, -1 * deltaY, deltaY};
    double qPart[] = {x1 - xmin, xmax - x1, y1 - ymin, ymax - y1};

    bool accept = true;

    for( int i = 0; i < 4; i ++ ) {
        p = pPart[ i ];
        q = qPart[ i ];

        if( p == 0 && q < 0 ) {
            accept = false;
            break;
        }

        r = q / p;

        if( p < 0 ) {
            u1 =qMax(u1, r);
        }

        if( p > 0 ) {
            u2 = qMin(u2, r);
        }

        if( u1 > u2 ) {
            accept = false;
            break;
        }
    }
    if (accept) {
        if( u2 < 1 ) {
            x2 = x1 + u2 * deltaX;
            y2 = y1 + u2 * deltaY;
        }
        if( u1 > 0) {
            x1 = x1 + u1 * deltaX;
            y1 = y1 + u1 * deltaY;
        }

        startPoint = RVector(x1, y1);
        endPoint = RVector(x2, y2);
    }
    else {
        startPoint = RVector::invalid;
        endPoint = RVector::invalid;
    }
}

bool RLine::move(const RVector& offset) {
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance) {
        return false;
    }
    startPoint += offset;
    endPoint += offset;
    return true;
}

bool RLine::rotate(double rotation, const RVector& center){
	if (fabs(rotation) < RS::AngleTolerance) {
		return false;
	}
	startPoint.rotate(rotation, center);
	endPoint.rotate(rotation, center);
	return true;
}

bool RLine::scale(const RVector& scaleFactors, const RVector& center) {
    startPoint.scale(scaleFactors, center);
    endPoint.scale(scaleFactors, center);
    return true;
}

bool RLine::mirror(const RLine& axis) {
    startPoint.mirror(axis);
    endPoint.mirror(axis);
    return true;
}

bool RLine::flipHorizontal() {
    startPoint.flipHorizontal();
    endPoint.flipHorizontal();
    return true;
}

bool RLine::flipVertical() {
    startPoint.flipVertical();
    endPoint.flipVertical();
    return true;
}
