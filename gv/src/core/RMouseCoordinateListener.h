#ifndef RMOUSECOORDINATELISTENER_H
#define RMOUSECOORDINATELISTENER_H

class RVector;

/**
 * Abstract base class for classes that are interested in
 * the current coordinate of the mouse cursor. This can for example
 * be a widget that displays the cursor coordinate.
 *
 * \ingroup core
 */
class RMouseCoordinateListener {
public:
    virtual ~RMouseCoordinateListener() {}
    /**
     * Notifies this listener if the current mouse position has changed.
     *
     * \param wcsPosition world coordinate position
     * \param ucsPosition user coordinate position
     */
    virtual void setCoordinate(const RVector& wcsPosition, const RVector& ucsPosition) = 0;
};

#endif
