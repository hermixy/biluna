#ifndef RCOORDINATELISTENERADAPTER_H_
#define RCOORDINATELISTENERADAPTER_H_

#include <QObject>

#include "RVector.h"
#include "RCoordinateListener.h"

/**
 * This coordinate listener adapter emits signals on coordinate events.
 * Used to simplify listening to coordinate events from QObjects,
 * for instance from an ECMA script.
 *
 * \scriptable
 */
class RCoordinateListenerAdapter: public QObject, public RCoordinateListener {
Q_OBJECT

public:
	RCoordinateListenerAdapter(QObject* parent=NULL) : QObject(parent) { }
	virtual ~RCoordinateListenerAdapter(){ }

	virtual void setCoordinate(const RVector& wcsPosition,
			const RVector& ucsPosition) {
		emit coordinateSet(wcsPosition, ucsPosition);
	}

signals:
	void coordinateSet(const RVector& wcsPosition, const RVector& ucsPosition);
};

Q_DECLARE_METATYPE(RCoordinateListenerAdapter*)

#endif
