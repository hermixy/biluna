#ifndef RACTIONADAPTER_H
#define RACTIONADAPTER_H

#include "RAction.h"
#include "RVector.h"
#include "RDebug.h"
#include "REntity.h"

class RPropertyEvent;
class RGraphicsScene;
class RDocumentInterface;
class RCommandEvent;
class RCoordinateEvent;
class RDocument;
class REntityPickEvent;
class RMouseEvent;
class RWheelEvent;
class RGuiAction;
class RStorage;
class RTabletEvent;

/**
 * \brief Abstract base class for all action classes.
 *
 * Action classes handle user interaction with a document. A menu,
 * toolbar button or command usually triggers an action.
 *
 * \ingroup core
 * \scriptable
 */
class RActionAdapter : public RAction {

public:
    RActionAdapter(RGuiAction* guiAction) : RAction(guiAction) {}
    virtual void beginEvent() {}
    virtual void finishEvent() {}
    virtual void suspendEvent() {}
    virtual void resumeEvent() {}
    virtual void escapeEvent() {}
    virtual void keyPressEvent(QKeyEvent& /*event*/) {}
    virtual void keyReleaseEvent(QKeyEvent& /*event*/) {}
    virtual void mousePressEvent(RMouseEvent& /*event*/) {}
    virtual void mouseMoveEvent(RMouseEvent& /*event*/) {}
    virtual void mouseReleaseEvent(RMouseEvent& /*event*/) {}
    virtual void wheelEvent(RWheelEvent& /*event*/) {}
    virtual void tabletEvent(RTabletEvent& /*event*/) {}
    virtual void commandEvent(RCommandEvent& /*event*/) {}
    virtual void coordinateEvent(RCoordinateEvent& /*event*/) {}
    virtual void coordinateEventPreview(RCoordinateEvent& /*event*/) {}
    virtual void entityPickEvent(REntityPickEvent& /*event*/) {}
    virtual void entityPickEventPreview(REntityPickEvent& /*event)*/) {}
    virtual void propertyChangeEvent(RPropertyEvent& /*event*/) {}
};

Q_DECLARE_METATYPE(RActionAdapter*)

#endif
