#ifndef RACTION_H
#define RACTION_H

#include <QList>
#include <QString>
#include <QMap>
#include <QKeyEvent>

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
class RAction {
public:
    /**
     * Current mouse click mode.
     */
    enum ClickMode {
        /**
         * Mouse click will pick a coordinate and trigger \ref coordinateEvent.
         * This is the default mode.
         */
        PickCoordinate,
        /**
         * Mouse click will pick an entity and trigger \ref entityPickEvent.
         */
        PickEntity,
        /**
         * Mouse click will not trigger any events except for \ref mousePressEvent
         * and \ref mouseReleaseEvent.
         */
        PickingDisabled
    };

public:
    RAction(RGuiAction* guiAction);
    virtual ~RAction();

    void terminate();
    bool isTerminated();

    void setOverride();
    bool isOverride();

    void setUniqueGroup(const QString& ug);
    QString getUniqueGroup();

    void setOverrideBase(RAction* base);
    RAction* getOverrideBase();

    /**
     * \return The GUI action which can be used to trigger this action
     *  or NULL.
     */
    virtual RGuiAction* getGuiAction() {
        return guiAction;
    }

    QList<RGraphicsScene*> getGraphicsScenes();
    RDocument* getDocument();
    RDocumentInterface* getDocumentInterface();
    RStorage* getStorage();

    void setDocumentInterface(RDocumentInterface* di){
    	documentInterface = di;
    }

    /**
     * Sets the current click mode.
     *
     * \see ClickMode
     */
    void setClickMode(RAction::ClickMode m) {
        clickMode = m;
    }

    /**
     * \return The current click mode of this action as previously
     *      requested by the action.
     */
    RAction::ClickMode getClickMode() {
        return clickMode;
    }

    /**
     * Called as soon as the action is created (menu chosen, toolbutton activated, etc).
     */
    virtual void beginEvent() = 0;
    /**
     * Called just before the action is deleted.
     */
    virtual void finishEvent() = 0;
    /**
     * Called with the action is Suspended, for example when another action
     * becomes active or the mouse cursor leaves the view.
     */
    virtual void suspendEvent() = 0;
    /**
     * Resumes the action (for example when another action became active and
     * then terminated or if the mouse cursor enters the view).
     */
    virtual void resumeEvent() = 0;
    /**
     * Escape events are triggered if the user clicks the right mouse button or
     * hits the Escape key. The action is expected to go one step back, to the
     * last state or terminate itself.
     */
    virtual void escapeEvent() = 0;

    virtual void keyPressEvent(QKeyEvent& event) = 0;

    virtual void keyReleaseEvent(QKeyEvent& event) = 0;

    /**
     * Called on mouse press events.
     */
    virtual void mousePressEvent(RMouseEvent& event) = 0;

    /**
     * Called on mouse move events.
     */
    virtual void mouseMoveEvent(RMouseEvent& event) = 0;

    /**
     * Called on mouse release events.
     */
    virtual void mouseReleaseEvent(RMouseEvent& event) = 0;

    /**
     * Called on mouse wheel events.
     */
    virtual void wheelEvent(RWheelEvent& event) = 0;

    /**
     * Called on tablet events.
     */
    virtual void tabletEvent(RTabletEvent& event) = 0;

    /**
     * Called when a command is entered in the command line.
     */
    virtual void commandEvent(RCommandEvent& event) = 0;

    /**
     * Called for every coordinate event.
     * Coordinate events can be the result of a mouse click or an entered
     * coordinate from the command line.
     * These events are fired after mouse click events in mode "PickCoordinate".
     */
    virtual void coordinateEvent(RCoordinateEvent& event) = 0;

    /**
     * Called if the action is in \c PickCoordinate mode and the mouse is moved
     * around. Typically, some sort of preview is shown to the user to indicate
     * what would happen if that coordinate would be picked.
     */
    virtual void coordinateEventPreview(RCoordinateEvent& event) = 0;

    /**
     * \override
     */
    virtual void entityPickEvent(REntityPickEvent& event) = 0;

    /**
     * \override
     */
    virtual void entityPickEventPreview(REntityPickEvent& event) = 0;

    /**
     * Called when the value of a property is modified while this action is active.
     */
    virtual void propertyChangeEvent(RPropertyEvent& event) = 0;

    RVector snap(RMouseEvent& event);

protected:
    bool terminated;
    bool override;
    QString uniqueGroup;
    RAction::ClickMode clickMode;
    RGuiAction* guiAction;
    RAction* overrideBase;
    RDocumentInterface* documentInterface;
};

Q_DECLARE_METATYPE(RAction*)
Q_DECLARE_METATYPE(RAction::ClickMode)

#endif
