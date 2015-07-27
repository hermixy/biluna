#ifndef RRULERQT_H
#define RRULERQT_H

#include <QFrame>
#include <QMetaType>

#include "RCoordinateListener.h"
#include "RRuler.h"
#include "RVector.h"

class RGraphicsView;

/**
 * Widget that displays the current mouse position.
 * Usually shown in the status bar of an application.
 *
 * \scriptable
 */
class RRulerQt: public QFrame, public RRuler, public RCoordinateListener {

Q_OBJECT
Q_PROPERTY(Qt::Orientation orientation READ getOrientation WRITE setOrientation);

public:
    RRulerQt(QWidget* parent=0);
    virtual ~RRulerQt();

    virtual void setOrientation(Qt::Orientation orientation);

    virtual QSize sizeHint() const;

    virtual void paintTick(int pos, bool major, const QString& label);

    virtual QFont getFont() const;

    void updateViewport();

    virtual void setCoordinate(const RVector& wcsPosition, const RVector& ucsPosition);

protected:
    void paintEvent(QPaintEvent *);

private:
    QPainter *painter;
    QImage buffer;
    QSize lastSize;
    bool viewportChanged;
    QPainterPath cursorArrow;
    RVector cursorPosition;
};

Q_DECLARE_METATYPE(RRulerQt*)

#endif
