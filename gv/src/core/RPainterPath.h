#ifndef RPAINTERPATH_H
#define RPAINTERPATH_H

#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QFlags>

#include "RVector.h"



/**
 * Extended painter path with a z-level and a pen.
 *
 * \ingroup qcadguiqt
 */
class RPainterPath : public QPainterPath {

public:
	enum Mode {
		NoModes = 0x0,
		Selected = 0x1,
		Highlighted = 0x2
	};
	Q_DECLARE_FLAGS(Modes, Mode)

public:
    RPainterPath();
    ~RPainterPath();

    int getZLevel() const;
    void setZLevel(int zl);

    bool hasPen() const;
    QPen getPen() const;
    void setPen(const QPen& p);

    QBrush getBrush() const;
    void setBrush(const QBrush& b);

    void setMode(RPainterPath::Mode mode, bool on = true);
    bool getMode(RPainterPath::Mode mode) const;

    void setHighlighted(bool on);
    bool isHighlighted() const;

    void setSelected(bool on);
    bool isSelected() const;

    double getDistanceTo(const RVector& point) const;

    void addPoint(const RVector& position);
    bool hasPoints();
    QList<RVector> getPoints();

private:
    int zLevel;
    QPen pen;
    QBrush brush;
    Modes modes;
    QList<RVector> points;
};

QDataStream& operator<< (QDataStream& stream, const RPainterPath& path);
QDataStream& operator>> (QDataStream& stream, RPainterPath& path);
bool operator< (const RPainterPath& p1, const RPainterPath& p2);

Q_DECLARE_OPERATORS_FOR_FLAGS(RPainterPath::Modes)
Q_DECLARE_METATYPE(RPainterPath)
Q_DECLARE_METATYPE(RPainterPath*)
Q_DECLARE_METATYPE(QList<RPainterPath>)

#endif
