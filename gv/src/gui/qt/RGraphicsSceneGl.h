#ifndef RGRAPHICSSCENEGL_H
#define RGRAPHICSSCENEGL_H

#include <QtOpenGL>

#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RLine.h"
#include "RPoint.h"
#include "RCircle.h"
#include "RArc.h"

class RGraphicsViewGl;
    
    

/**
 * \brief OpenGL based 2d or 2d graphics scene.
 *
 * A graphics scene is an exporter object that exports entities as
 * OpenGL display lists.
 * 
 * If an entity is exported into an OpenGL graphics scene, one display
 * list is added to the each OpenGL context that belongs to an attached
 * view. This works against the design of the graphics view framework
 * of QCAD but is necessary due to the OpenGL design (display lists
 * are owned by views or pbuffers). The ID of the display list is the same 
 * as the entity ID. 
 *
 * Previews and entity highlighting are handled through a pbuffer
 * OpenGL overlays. Everything that is not part of the graphics
 * document is exported to a pbuffer which is drawn after the
 * actual scene. Note that all pbuffers are owned by the \ref RGraphicViewGl 
 * "OpenGL graphics view". 
 *
 * \ingroup gui_qt
 *
 * \scriptable
 */
class RGraphicsSceneGl : public RGraphicsScene {

	friend class RGraphicsViewGl;

public:
    RGraphicsSceneGl(RDocumentInterface& documentInterface);
    virtual ~RGraphicsSceneGl();

    virtual void beginPreview();
    virtual void endPreview();
    virtual void clearPreview();
    
    virtual void setColor(const RColor& clr);
    //virtual void setEntityColor(const RColor& clr);
    virtual void setPen(const QPen& pen);

    virtual void exportCurrentEntity();
    virtual void exportLine(const RLine& line, double offset = RNANDOUBLE);
    virtual void exportPoint(const RPoint& point);
    virtual void exportCircle(const RCircle& circle);
    virtual void exportArc(const RArc& arc, double offset = RNANDOUBLE);
    virtual void exportArcSegment(const RArc& arc);
    virtual void exportLineSegment(const RLine& line);
    virtual void exportTriangle(const RTriangle& triangle);
    virtual void unexportEntity(REntity::Id entityId);
    
    virtual void highlightEntity(REntity& entity);
    virtual void highlightReferencePoint(const RVector& position);

    virtual void registerView(RGraphicsViewGl* view);

protected:
    GLuint getMaxPreviewListIndex() {
        return maxPreviewListIndex;
    }

    static void handleGlErrors();
    void setGlContext(RGraphicsViewGl& view);
    void setGlColor();
    QList<RGraphicsViewGl*> getGlViews();
    GLuint getListId();

private:
    GLuint maxPreviewListIndex;
};

Q_DECLARE_METATYPE(RGraphicsSceneGl*)

#endif
