#ifndef RCOORDINATELISTENER_H
#define RCOORDINATELISTENER_H

#include <QMetaType>

class RVector;


/**
 * \brief Abstract base class for classes that are interested in 
 * the current coordinate of the mouse cursor. This can for example
 * be a widget that displays the cursor coordinate.
 *
 * \ingroup core
 * \scriptable
 */
class RCoordinateListener {
public:
    virtual ~RCoordinateListener() {}
    virtual void setCoordinate(const RVector& wcsPosition, const RVector& ucsPosition) = 0;
};

Q_DECLARE_METATYPE(QList<RCoordinateListener*>)
Q_DECLARE_METATYPE(RCoordinateListener*)

#endif
