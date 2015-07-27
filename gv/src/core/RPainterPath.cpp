#include "QPen"

#include "RDebug.h"
#include "RLine.h"
#include "RMath.h"
#include "RPainterPath.h"

RPainterPath::RPainterPath() :
	zLevel(0), modes(RPainterPath::NoModes) {
}

RPainterPath::~RPainterPath() {
}

void RPainterPath::setMode(RPainterPath::Mode mode, bool on) {
	if (on) {
		modes |= mode;
	} else {
		modes &= ~mode;
	}
}

bool RPainterPath::getMode(RPainterPath::Mode mode) const {
	return (modes & mode) == mode;
}

void RPainterPath::setSelected(bool on) {
	setMode(RPainterPath::Selected, on);
}

bool RPainterPath::isSelected() const {
	return getMode(RPainterPath::Selected);
}

void RPainterPath::setHighlighted(bool on) {
	setMode(RPainterPath::Highlighted, on);
}

bool RPainterPath::isHighlighted() const {
	return getMode(RPainterPath::Highlighted);
}

/**
 * \return Z-level of the painter path.
 */
int RPainterPath::getZLevel() const {
	return zLevel;
}

/**
 * Sets the z-level of the painter path. Painter paths may be 
 * ordered by z-level when drawn.
 */
void RPainterPath::setZLevel(int zl) {
	zLevel = zl;
}

/**
 * \return Pen of this painter path.
 */
QPen RPainterPath::getPen() const {
	return pen;
}

/**
 * Sets the pen of this painter path. The pen may be set before painting
 * the path but this is up to the caller.
 */
void RPainterPath::setPen(const QPen& p) {
	pen = p;
}

QBrush RPainterPath::getBrush() const {
	return brush;
}

void RPainterPath::setBrush(const QBrush& b) {
	brush = b;
}

/*
 QDataStream& operator<< (QDataStream& stream, const RPainterPath& path) {
 stream << (QPainterPath&)path;
 stream << path.getZLevel();
 stream << path.getPen();
 return stream;
 }



 QDataStream& operator>> (QDataStream& stream, RPainterPath& path) {
 stream >> (QPainterPath&)path;
 int zl;
 stream >> zl;
 path.setZLevel(zl);
 QPen p;
 stream >> p;
 path.setPen(p);
 return stream;
 }
 */

/**
 * \return Distance from this painter path to the given point.
 *
 * This can be used for example to find out how close the visual
 * representation of an entity is to the mouse cursor on the screen.
 */
double RPainterPath::getDistanceTo(const RVector& point) const {
	double minDist = RMAXDOUBLE;
	QPointF cursor;

	for (int i = 0; i < elementCount(); ++i) {
		QPainterPath::Element el = elementAt(i);
		if (el.isLineTo()) {
            RLine line(RVector(cursor.x(), cursor.y()), RVector(el.x, el.y));
			double dist = line.getDistanceTo(point);
			if (!std::isnan(dist) && dist < minDist) {
				minDist = dist;
			}
		}
		cursor = el;
	}

	return minDist;
}

void RPainterPath::addPoint(const RVector& position) {
    points.append(position);
}


bool RPainterPath::hasPoints() {
    return points.count()!=0;
}


QList<RVector> RPainterPath::getPoints() {
    return points;
}

/**
 * This operator allows us to sort painter paths based on z-level.
 */
bool operator<(const RPainterPath& p1, const RPainterPath& p2) {
	return p1.getZLevel() < p2.getZLevel();
}

