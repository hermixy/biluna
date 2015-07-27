#ifndef RGRAPHICSSCENE_H
#define RGRAPHICSSCENE_H

#include <QCursor>
#include <QDebug>

#include "RDebug.h"
#include "REntity.h"
#include "RExporter.h"
#include "RGraphicsView.h"
#include "RMouseEvent.h"
#include "RSpatialIndex.h"
#include "RTabletEvent.h"
#include "RWheelEvent.h"

class RDocumentInterface;



/**
 * Abstract base class for all graphics scenes.
 * A graphics scene is an exporter that exports entities into
 * something that can be shown in a view (painter paths,
 * triangles, OpenGL elements, ...).
 *
 * Graphics scenes store a map of reference points of all entities
 * that are currently selected. This information is stored at
 * the scene level and not at the document interface level because
 * a scene that shows for example a wall from the side may show
 * different reference points than a scene that shows the same
 * wall from the top.
 *
 * \ref RGraphicsView "graphics views".
 *
 * \ingroup core
 * \scriptable
 */
class RGraphicsScene : public RExporter {
public:
    RGraphicsScene(RDocumentInterface& documentInterface);
    virtual ~RGraphicsScene();

    RDocumentInterface& getDocumentInterface();
    QList<RGraphicsView*> getGraphicsViews();

    virtual void regenerate();
    virtual void regenerate(QSet<REntity::Id>& affectedEntities);
    virtual void regenerateViews(bool force=false);
    virtual void repaintViews();
    virtual void setCursor(const QCursor& cursor);

    void handleKeyPressEvent(QKeyEvent& event);
    void handleKeyReleaseEvent(QKeyEvent& event);
    void handleMouseMoveEvent(RMouseEvent& event);
    void handleMousePressEvent(RMouseEvent& event);
    void handleMouseReleaseEvent(RMouseEvent& event);
    void handleWheelEvent(RWheelEvent& event);
    void handleTabletEvent(RTabletEvent& event);

    virtual void registerView(RGraphicsView* view);
    virtual void unregisterView(RGraphicsView* view);

    virtual void beginPreview();
    virtual void endPreview();
    virtual void clearPreview();
    virtual bool isPreviewEmpty();

    RVector getHighlightedReferencePoint();

    /**
     * Highlights the given entity. This is typically used to highlight entities
     * when the mouse hovers over them.
     */
    virtual void highlightEntity(REntity& entity) = 0;

    /**
     * Highlights the reference point at the given position. This is typically
     * used to highlight reference point when the mouse hovers over them.
     */
    virtual void highlightReferencePoint(const RVector& position);

    virtual void exportCurrentEntity();
    virtual void unexportEntity(REntity::Id entityId);

    QMultiMap<REntity::Id, RVector>& getReferencePoints() {
        return referencePoints;
    }

    virtual void dump() {
        qDebug() << *this;
    }

    /**
     * \nonscriptable
     */
    friend QDebug operator<<(QDebug dbg, RGraphicsScene& gs);

protected:
    virtual void exportReferencePoints();

protected:
    RDocumentInterface& documentInterface;
    QList<RGraphicsView*> views;
    bool exportToPreview;
    bool previewIsEmpty;
    RVector highlightedReferencePoint;

    /**
     * Internal map of reference points for every selected entity in the scene.
     * Used for drawing reference points.
     */
    QMultiMap<REntity::Id, RVector> referencePoints;
};

Q_DECLARE_METATYPE(QList<RGraphicsScene*>)
Q_DECLARE_METATYPE(RGraphicsScene*)

#endif
