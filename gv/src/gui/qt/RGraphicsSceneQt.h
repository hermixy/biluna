#ifndef RGRAPHICSSCENE2D_H
#define RGRAPHICSSCENE2D_H

#include <QPen>
#include <QList>
#include <QMultiMap>

#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RLine.h"
#include "RPoint.h"
#include "RCircle.h"
#include "RArc.h"
#include "RPainterPath.h"
    

/**
 * \brief Qt based 2d graphics scene.
 *
 * This graphics scene is an exporter that exports entities into
 * QPainterPaths (2d). 
 * 
 * \ref RGraphicView "graphics views". 
 *
 * \ingroup qcadguiqt
 * \scriptable
 */
class RGraphicsSceneQt : public RGraphicsScene {
public:
    RGraphicsSceneQt(RDocumentInterface& documentInterface);
    ~RGraphicsSceneQt();

    virtual void clearPreview();
    
    virtual void unexportEntity(REntity::Id entityId);
    virtual void exportLine(const RLine& line, double offset = RNANDOUBLE);
    virtual void exportPoint(const RPoint& point);
    virtual void exportCircle(const RCircle& circle);
    virtual void exportArc(const RArc& arc, double offset = RNANDOUBLE);
    virtual void exportArcSegment(const RArc& arc);
    virtual void exportLineSegment(const RLine& line);
    virtual void exportTriangle(const RTriangle& triangle);
    virtual void exportRectangle(const RVector& p1, const RVector& p2);
    
    virtual void highlightEntity(REntity& entity);

    void deletePainterPaths();
    QList<RPainterPath> getPainterPaths(REntity::Id entityId);
    QList<RPainterPath> getPreviewPainterPaths();

    void addToPreview(const QList<RPainterPath>& painterPaths);
    void addToPreview(const RPainterPath& painterPath);

    virtual void startEntity(bool topLevelEntity);

    virtual void dump() {
        qDebug() << *this;
    }

    /**
     * \nonscriptable
     */
    friend QDebug operator<<(QDebug dbg, RGraphicsSceneQt& gs);

private:
    RPainterPath currentPainterPath;
    QMultiMap<REntity::Id, RPainterPath> painterPaths;
    QList<RPainterPath> previewPainterPaths;
};

Q_DECLARE_METATYPE(RGraphicsSceneQt*)

#endif
