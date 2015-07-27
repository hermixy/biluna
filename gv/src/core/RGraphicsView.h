#ifndef RGRAPHICSVIEW_H
#define RGRAPHICSVIEW_H

#include <QKeyEvent>
#include <QCursor>

#include "RExporter.h"
#include "RGrid.h"
#include "RNonCopyable.h"

class RAction;
class RDocumentInterface;
class RGraphicsScene;
class RLine;
class RMouseEvent;
class RTabletEvent;
class RWheelEvent;


/**
 * \brief Graphics view.
 *
 * A graphics view displays a (part of a) graphics scene.
 *
 * \ingroup core
 * \scriptable
 */
class RGraphicsView : RNonCopyable {
public:
    RGraphicsView(RGraphicsScene* scene = NULL);
    virtual ~RGraphicsView();

    void setDisplayOnlyCurrentUcs(bool on);

    virtual void setScene(RGraphicsScene* scene);
    RGraphicsScene* getScene();
    RDocument* getDocument();
    RDocumentInterface* getDocumentInterface();

    /**
     * Maps the given view coordinate in pixels to a model coordinate.
     *
     * \param v view coordinate in pixels, v.z is always 0
     * \param z known z coordinate in model units
     */
    virtual RVector mapFromView(const RVector& v, double z=0.0) = 0;

    /**
     * Maps the given model coordinate to a view coordinate in pixels.
     */
    virtual RVector mapToView(const RVector& v) = 0;
    virtual RBox mapToView(const RBox& box);

    /**
     * Maps the given view distance in pixels to a model distance.
     * If the view projection is not orthogonal the result is undefined.
     */
    virtual double mapDistanceFromView(double d) = 0;

    /**
     * Maps the given model distance to a screen distance in pixels.
     * If the view projection is not orthogonal the result is undefined.
     */
    virtual double mapDistanceToView(double d) = 0;

    virtual QList<RVector> mapCornersFromView();

    virtual RBox getBox();
    virtual RVector getMinimum();
    virtual RVector getMaximum();

    virtual void handleKeyPressEvent(QKeyEvent& event);
    virtual void handleKeyReleaseEvent(QKeyEvent& event);
    virtual void handleMouseMoveEvent(RMouseEvent& event);
    virtual void handleMousePressEvent(RMouseEvent& event);
    virtual void handleMouseReleaseEvent(RMouseEvent& event);
    virtual void handleWheelEvent(RWheelEvent& event);
    virtual void handleTabletEvent(RTabletEvent& event);
    /**
     * \return True if this view currently has the focus.
     */
    virtual bool hasFocus() = 0;

    virtual void viewportChangeEvent();

    void saveViewport();
    void restoreViewport();
    void zoomIn();
    void zoomOut();
    void zoomIn(const RVector& center);
    void zoomOut(const RVector& center);
    virtual void zoom(const RVector& center, double factor);
    virtual void zoomTo(const RBox& window, int margin = 0);
    void zoomPrevious();
    void autoZoom();
    bool zoomToSelection();

    virtual void pan(const RVector& delta);
    void startPan();

    double getFactor() const {
        return factor;
    }
    void setFactor(double f);

    RVector getOffset() const;
    void setOffset(const RVector& offset);

    /**
     * Regenerates the view to show the visible parts of the underlying scene.
     * This function is called after the entities have been modified or
     * the view port changed after a zoom or pan or after the window
     * has been resized.
     *
     * \param force True if scene has changed, don't use any optimizations, force regeneration.
     */
    virtual void regenerate(bool force=false) = 0;

    /**
     * Repaints the view (widget, etc..). This differs from regenerate in that
     * it does not require the view to iterate through the scene elements. Instead
     * for example a buffered bitmap might be drawn.
     * This is for example called when the preview changes to restore the view
     * before painting the preview on top of it.
     */
    virtual void repaint() = 0;

    /**
     * \return Width of the view in pixels.
     */
    virtual int getWidth() = 0;

    /**
     * \return Height of the view in pixels.
     */
    virtual int getHeight() = 0;

    void setNavigationAction(RAction* action);

    /**
     * \return Current grid or NULL if no grid is set.
     */
    RGrid* getGrid() {
        return grid;
    }

    /**
     * Sets the current grid. This graphics view takes ownership
     * of the grid. The previous grid is deleted. Setting the grid
     * to NULL disables the grid.
     */
    void setGrid(RGrid* g) {
        if (grid!=NULL) {
            delete grid;
            grid = NULL;
        }
        grid = g;
    }

    virtual void paintGridPoint(const RVector& /*ucsPosition*/) {}
    virtual void paintMetaGridLine(const RLine& /*ucsPosition*/) {}

    virtual void setCursor(Qt::CursorShape /*cursorShape*/) {}
    virtual void setCursor(const QCursor& /*cursor*/) {}

    RVector getClosestReferencePoint(const RVector& screenPosition, int range);
    RVector getClosestReferencePoint(REntity::Id entityId, const RVector& screenPosition);
    REntity::Id getClosestEntity(const RVector& screenPosition, int range);
    //REntity::Id getClosestEntityInUcs(const RVector& screenPosition, int range);

    void setBackgroundColor(const QColor& col);
    QColor getBackgroundColor();

    int getMargin();
    void setMargin(int m);

protected:
    virtual void centerTo(const RBox& box);

    /**
     * The scene this view is attached to.
     */
    RGraphicsScene* scene;

    /**
     * Grid that is currently displayed by this view or NULL.
     */
    RGrid* grid;

    /**
     * The navigation action of this view or NULL if no navigation action
     * has been set.
     */
    RAction* navigationAction;

    /**
     * Offset of the zero point from the lower left corner of the view in
     * model coordinates.
     */
    RVector offset;

    /**
     * Scale factor from model coordinates to view coordinates.
     */
    double factor;

    RVector previousOffset;
    double previousFactor;

    /**
     * True to only display entities within the horizontal slice that is defined
     * by the current UCS.
     */
    bool displayOnlyCurrentUcs;

    bool autoScalePatterns;

    QColor backgroundColor;
    int margin;
};

Q_DECLARE_METATYPE(RGraphicsView*)
Q_DECLARE_METATYPE(QList<RGraphicsView*>)

#endif
