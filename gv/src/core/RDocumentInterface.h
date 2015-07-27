#ifndef RDOCUMENTINTERFACE_H
#define RDOCUMENTINTERFACE_H

#include <QKeyEvent>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QSharedPointer>
#include <QStack>

#include "RAction.h"
#include "RCoordinateEvent.h"
#include "RCoordinateListener.h"
#include "RDocument.h"
#include "RExporter.h"
#include "RStorage.h"
#include "RUcs.h"

class RGraphicsScene;
class RGraphicsView;
class RMouseEvent;
class ROperation;
class RPropertyEvent;
class RSnap;
class RWheelEvent;
class RScriptHandler;



/**
 * \brief Interface for interaction between a graphics document
 * and a user.
 *
 * Typically one document interface exists for every document that
 * is open in an MDI application. The document interface owns and
 * links the various scenes, views and the currently active action.
 *
 * A document interface can own multiple graphics scenes, each of
 * which can have multiple views attached to it. The views forward
 * all user events (mouse moves, mouse clicks, etc.) to the document
 * interface for processing. The document interface dispatches the
 * events to the currently active action object.
 *
 * \ingroup core
 * \scriptable
 */
class RDocumentInterface {
public:
    RDocumentInterface(RDocument& document);
    virtual ~RDocumentInterface();

    RDocument& getDocument();
    RStorage& getStorage();
    QList<RGraphicsScene*> getGraphicsScenes();
    RGraphicsView* getGraphicsViewWithFocus();

    void addCoordinateListener(RCoordinateListener* l);
    void notifyCoordinateListeners(const RVector& wcsPosition,
                    const RVector& ucsPosition);

    void clear();

    /**
     * \nonscriptable
     */
    RScriptHandler* getScriptHandler(const QString& extension);

    void setDefaultAction(RAction* action);
    void setCurrentAction(RAction* action);
    void killAllActions();

    RAction* getCurrentAction();

    void suspend();
    void resume();

    void setClickMode(RAction::ClickMode m);

    virtual void keyPressEvent(QKeyEvent& event);
    virtual void keyReleaseEvent(QKeyEvent& event);
    virtual void mouseMoveEvent(RMouseEvent& event);
    virtual void mousePressEvent(RMouseEvent& event);
    virtual void mouseReleaseEvent(RMouseEvent& event);
    virtual void coordinateEvent(RCoordinateEvent& event);
    virtual void coordinateEventPreview(RCoordinateEvent& event);
    virtual void wheelEvent(RWheelEvent& event);
    virtual void tabletEvent(RTabletEvent& event);

    virtual void propertyChangeEvent(RPropertyEvent& event);
    virtual void ucsSetEvent(const QString& ucsName);

    void regenerateScenes();
    void regenerateScenes(QSet<REntity::Id>& entityIds);
    void regenerateScenes(REntity::Id entityId);
    void regenerateViews(bool force=false);
    void repaintViews();

    void registerScene(RGraphicsScene& scene);
    void unregisterScene(RGraphicsScene& scene);

    //void setPen(const QPen& pen);
    void setCursor(const QCursor& cursor);

    bool importUrl(const QUrl& url);
    bool importFile(const QString& fileName);
    bool exportFile(const QString& fileName);

    void undo();
    void redo();

    void setSnap(RSnap* snap);
    RSnap* getSnap();
    RVector snap(RMouseEvent& event);

    REntity::Id getClosestEntity(RMouseEvent& event);
    REntity::Id getClosestEntity(const RVector& position, double range);
    //REntity::Id getClosestEntityInUcs(const RVector& position, double range);
    void highlightEntity(REntity::Id entityId);
    void highlightReferencePoint(const RVector& position);
    void selectEntity(REntity::Id entityId, bool add = false);
    void deselectEntity(REntity::Id entityId);
    //void selectClosestEntity(RMouseEvent& event, const RLine& line, bool add = false);
    void selectBoxXY(const RBox& box, bool add = false);
    //void selectBoxInUcs(const RBox& box, bool add = false);
    void clearSelection();
    bool hasSelection();

    void addZoomBoxToPreview(const RBox& box);
    void addSelectionBoxToPreview(const RBox& box);
    void addAuxShapeToPreview(RShape& shape);
    void addEntityToPreview(REntity& entity);
    void beginPreview();
    void endPreview();
    void clearPreview();
    bool isPreviewEmpty();

    void showPropertiesOf(REntity& entity);
    void clearProperties();

    void zoomIn();
    void zoomOut();
    void autoZoom();
    bool zoomToSelection();
    void zoomTo(const RBox& region, int margin = 0);
    void zoomPrevious();

    void previewOperation(const ROperation& operation);
    RTransaction applyOperation(const ROperation& operation);

    void objectChangeEvent(QSet<RObject::Id>& objectIds);

    RVector getRelativeZero();
    void setRelativeZero(const RVector& p);

    RVector getCursorPosition();
    void setCursorPosition(const RVector& p);

    void deleteTerminatedActions();

    void setCurrentColor(const RColor& color);
    RColor getCurrentColor();

    void setCurrentLineweight(RLineweight::Lineweight lw);
    RLineweight::Lineweight getCurrentLineweight();

    void setCurrentLinetype(RLinetype lt);
    RLinetype getCurrentLinetype();

    void setCurrentLayer(const RLayer& layer);
    void setCurrentLayer(const QString& layerName);

    void setCurrentBlock(const RBlock& block);
    void setCurrentBlock(const QString& blockName);

//    void setCurrentView(const RView& view);
    void setCurrentView(const QString& viewName);

    void setCurrentUcs(const RUcs& ucs);
    void setCurrentUcs(const QString& ucsName);
    RUcs getCurrentUcs();
    QString getCurrentUcsName();

    RGraphicsView* getLastKnownViewWithFocus() {
        return lastKnownViewWithFocus;
    }

    void setLastKnownViewWithFocus(RGraphicsView* view) {
        lastKnownViewWithFocus = view;
    }

    //void copySelectionToClipboard(const RVector& reference=RVector::nullVector);
    //void pasteFromClipboard(const RVector& reference);

    static RDocumentInterface& getClipboard();
    bool isClipboard() {
        return this==clipboard;
    }

private:
    void handleClickEvent(RAction& action, RMouseEvent& event);
    void previewClickEvent(RAction& action, RMouseEvent& event);

private:
    RDocument& document;
    QList<RGraphicsScene*> scenes;
    RGraphicsView* lastKnownViewWithFocus;
    QMap<QString, RScriptHandler*> scriptHandlers;

    RAction* defaultAction;
    QStack<RAction*> currentActions;

	QList<RCoordinateListener*> coordinateListeners;

    RSnap* currentSnap;

    RVector relativeZero;
    RVector cursorPosition;
    RUcs currentUcs;
    QString currentUcsName;
    bool suspended;

    QMutex mutex;
    
    static RDocumentInterface* clipboard;
};

Q_DECLARE_METATYPE(RDocumentInterface*)

#endif
