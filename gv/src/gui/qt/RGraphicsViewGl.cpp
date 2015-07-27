#include "RGraphicsViewGl.h"


#include <QDragMoveEvent>
#include <QScrollBar>
#include <QGLPixelBuffer>
#include <QStyle>

#include "RDebug.h"
#include "RDocumentInterface.h"
#include "REntity.h"
#include "RGraphicsSceneGl.h"
#include "RLine.h"
#include "RMath.h"
#include "RMouseEvent.h"
#include "RWheelEvent.h"
#include "RTabletEvent.h"
#include "RSettings.h"


RGraphicsViewGl::RGraphicsViewGl(QWidget* parent)
    : QWidget(parent), 
      RGraphicsView(),
      mainBuffer(NULL),
      previewBuffer(NULL),
      sceneGl(NULL) {
    
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL overlays: %d", 
        QGLFormat::hasOpenGLOverlays());
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL FBO: %d", 
        QGLFramebufferObject::hasOpenGLFramebufferObjects());
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL pbuffer: %d", 
        QGLPixelBuffer::hasOpenGLPbuffers());

    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version None: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_None));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 1.1: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_1_1));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 1.2: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_1_2));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 1.3: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_1_3));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 1.4: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_1_4));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 1.5: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_1_5));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 2.0: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_2_0));
    RDebug::debug("RGraphicsViewGl::RGraphicsViewGl: OpenGL version 2.1: %d", 
        (bool)(QGLFormat::openGLVersionFlags()&QGLFormat::OpenGL_Version_2_1));
    
    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);

    setAutoFillBackground(true);
    QPalette p = palette();
    p.setColor(QPalette::Window, RSettings::getColor(RSettings::BackgroundColor));
    setPalette(p);

    invalidate();

    lastButtonState = Qt::NoButton;
}



RGraphicsViewGl::~RGraphicsViewGl() {
    if (mainBuffer!=NULL) {
        delete mainBuffer;
    }
    if (previewBuffer!=NULL) {
        delete previewBuffer;
    }
}

void RGraphicsViewGl::setScene(RGraphicsSceneGl* scene) {
	sceneGl = scene;
	RGraphicsView::setScene(scene);
}


/**
 * Invalidates this view to make sure that the main graphics buffer (bitmap)
 * is regenerated on the next \ref paintEvent.
 */
void RGraphicsViewGl::invalidate() {
    graphicsBufferNeedsUpdate = true;
}



void RGraphicsViewGl::regenerate(bool) {
    invalidate();
    update();
}



void RGraphicsViewGl::repaint() {
    update();
}



/**
 * Makes the main pbuffer the current OpenGL context.
 */
void RGraphicsViewGl::makeMainCurrent() {
    if (mainBuffer==NULL) {
        RDebug::error("RGraphicsViewGl::makeMainCurrent: mainBuffer is NULL");
        return;
    }

    if (mainBuffer->isValid()==false || mainBuffer->makeCurrent()==false) {
        RDebug::error("RGraphicsViewGl::makeMainCurrent: invalid rendering context");
    }
}



/**
 * Makes the preview pbuffer the current OpenGL context.
 * This pbuffer is overlayed over the main pbuffer.
 */
void RGraphicsViewGl::makePreviewCurrent() {
    if (previewBuffer==NULL) {
        RDebug::error("RGraphicsViewGl::makePreviewCurrent: previewBuffer is NULL");
        return;
    }

    if (previewBuffer->makeCurrent()==false) {
        RDebug::error("RGraphicsViewGl::makePreviewCurrent: invalid rendering context");
    }
}
    
    

/**
 * Called when this view gets the focus. Highlights the view to show to the user
 * that events go to this view.
 */
void RGraphicsViewGl::focusInEvent(QFocusEvent* /*event*/) {
    /*
    QPalette p = palette();
    p.setColor(QPalette::Window, QColor(backgroundColor).lighter(120));
    setPalette(p);
    update();
    */
    if (getDocumentInterface()!=NULL) {
        getDocumentInterface()->setLastKnownViewWithFocus(this);
    }
}



/**
 * Called when this view lost the focus. Resets the focus highlighting.
 */
void RGraphicsViewGl::focusOutEvent(QFocusEvent* /*event*/) {
    /*
    QPalette p = palette();
    p.setColor(QPalette::Window, QColor(backgroundColor));
    setPalette(p);
    */
    update();
}



/**
 * Called for every window resize. Resizes the OpenGL pbuffers.
 */
void RGraphicsViewGl::resizeEvent(QResizeEvent* /*event*/) {
	if (mainBuffer != NULL) {
		delete mainBuffer;
	}
	if (previewBuffer != NULL) {
		delete previewBuffer;
	}

	int w = getWidth();
	int h = getHeight();

	mainBuffer = new QGLPixelBuffer(w, h, QGLFormat(QGL::AlphaChannel));
	//assert(mainBuffer->format().alpha());
	previewBuffer = new QGLPixelBuffer(w, h, QGLFormat(QGL::AlphaChannel));
	//assert(previewBuffer->format().alpha());

	makeMainCurrent();
	setupViewport();

	makePreviewCurrent();
	setupViewport();

	if (scene != NULL) {
		scene->exportEntities();
		regenerate();
	}
}



/**
 * Called when the viewport needs to be repainted.
 * If the current main graphics buffer is valid, the buffer (bitmap) is drawn (very fast).
 * If the current main graphics buffer is not valid, all display lists are called to 
 * regenerate the buffer.
 */
void RGraphicsViewGl::paintEvent(QPaintEvent* /*event*/) {
    if (graphicsBufferNeedsUpdate) {
        updateMainBuffer();
    }
    
    paintMainBuffer();
    paintPreviewBuffer();
    
    // draw border around view with focus:
    if (hasFocus() || 
        (getDocumentInterface()!=NULL && 
         this==getDocumentInterface()->getLastKnownViewWithFocus())) {

    	QPainter painter(this);
		QStyleOptionFrame opt2;
		opt2.initFrom(this);
		style()->drawPrimitive(QStyle::PE_FrameLineEdit, &opt2, &painter,
				this);
    }
}



/**
 * Paints the graphics scene into a pbuffer and copies it into the
 * member variable \c buffer for later use by \ref RGraphicsViewGl::paintMainBuffer.
 */
void RGraphicsViewGl::updateMainBuffer() {
    if (scene == NULL) {
		return;
	}

    makeMainCurrent();
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    setupViewport();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    RDocument& document = scene->getDocument();
    QSet<REntity::Id> ids;

    /*
    if (displayOnlyCurrentUcs) {
        RDocumentInterface& documentInterface = scene->getDocumentInterface();
        RUcs ucs = documentInterface.getCurrentUcs();

        // A UCS does not have a height, however, we can use the Z-vector component
        // of the UCS as height indication for UCS based floors.
        RBox box(
            RVector(
                RMINDOUBLE, 
                RMINDOUBLE, 
                ucs.origin.z + 1.0e-6
            ),
            RVector(
                RMAXDOUBLE, 
                RMAXDOUBLE, 
                ucs.origin.z + ucs.getZAxisDirection().getMagnitude() - 1.0e-6
            )
        );

        ids = document.queryIntersectedEntities(box, true);
    }
    else {
    */
        ids = document.queryAllEntities();
    //}

    // For every entity a display list is called.
    // The display list has the same ID as the entity.
    // TODO: potential performance improvement with glCallLists()
    QSet<REntity::Id>::iterator it;
    for (it=ids.begin(); it!=ids.end(); it++) {
        //RDebug::debug("RGraphicsViewGl::updateMainBuffer: glCallList(%d)", *it);
        if (glIsList(*it)==false) {
            RDebug::error("RGraphicsViewGl::updateMainBuffer(): OpenGL error: no such list: %d", *it);
            continue;
        }
        glCallList(*it);
    }

    callZero();
    callGrid();
    
    callReferencePoints();
    
    if (mainBuffer!=NULL) {
        buffer = mainBuffer->toImage();
    }
    
    graphicsBufferNeedsUpdate = false;
}



/**
 * Redraws the view by simply drawing the cached \c buffer (very fast).
 */
void RGraphicsViewGl::paintMainBuffer() {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), buffer);
    painter.end();
}



/**
 * Paints the current preview as overlayed OpenGL pbuffer. This is called
 * by \ref paintEvent after the main buffer has been regenerated and drawn.
 * In other words: the preview is always on top of the main document and
 * always visible, regardless of the Z-levels in the preview.
 *
 * Note that this function also draws the cursor into the same OpenGL 
 * context as the preview.
 */
void RGraphicsViewGl::paintPreviewBuffer() {
	if (sceneGl == NULL) {
		return;
	}
    makePreviewCurrent();
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    setupViewport();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    for (uint i = 1; i < sceneGl->getMaxPreviewListIndex(); i++) {
		glCallList(i);
	}
    
    callCursor();

    if (previewBuffer!=NULL) {
        // keep this before QPainter instantiation. QPainter does
        //   some magic that prevents this from working:
        QImage image = previewBuffer->toImage();

        QPainter painter(this);
        painter.drawImage(QPoint(0,0), image);
        painter.end();
    }
    else {
        RDebug::error("RGraphicsViewGl::paintPreviewBuffer: previewBuffer is NULL");
    }
}



/**
 * Calls the OpenGL commands to paint the grid based on the information 
 * from \ref RGrid.
 * 
 * Which grid is painted depends on the current projection
 * rendering hint (\ref RExporter::setProjectionRenderingHint()).
 */
void RGraphicsViewGl::callGrid() {
    if (grid==NULL) {
        return;
    }

    grid->update(*this);

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POINTS);

    grid->paint(*this);

    glEnd();

    /*
    
    RDocumentInterface& documentInterface = getDocumentInterface();
    RUcs ucs = documentInterface.getCurrentUcs();
    
    RBox gridBox = grid->getRegion();
    RVector gridBoxC1 = gridBox.getDefiningCorner1();
    RVector gridBoxC2 = gridBox.getDefiningCorner2();

    RS::ProjectionRenderingHint projection = 
        scene.getProjectionRenderingHint();
    
    RVector min;
    RVector max;
    RVector spacing;

    // The grid is always displayed orthogonal to the model coordinate system.
    // The projection rendering hint defines which of the three possible grids
    // we show.
    switch (projection) {
    case RS::ThreeD:
    case RS::Top:
        min.x = gridBoxC1.x;
        max.x = gridBoxC2.x;
        min.y = gridBoxC1.y;
        max.y = gridBoxC2.y;
        spacing.x = grid->getSpacing().x;
        spacing.y = grid->getSpacing().y;
        break;

    / *
    case RS::Front:
        min.x = gridBoxC1.x;
        max.x = gridBoxC2.x;
        min.y = gridBoxC1.z;
        max.y = gridBoxC2.z;
        spacing.x = grid.getSpacing().x;
        spacing.y = grid.getSpacing().z;
        break;

    case RS::Side:
        min.x = gridBoxC1.y;
        max.x = gridBoxC2.y;
        min.y = gridBoxC1.z;
        max.y = gridBoxC2.z;
        spacing.x = grid.getSpacing().y;
        spacing.y = grid.getSpacing().z;
        break;
    * /
    
    default:
        RDebug::warning("RGraphicsViewGl::callGrid: "
            "invalid projection: %d", (int)projection);
        break;
    }

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_POINTS);

    RVector gridPointUcs;
    RVector gridPointWcs;
    for (gridPointUcs.x=min.x; gridPointUcs.x<max.x; gridPointUcs.x+=spacing.x) {
        for (gridPointUcs.y=min.y; gridPointUcs.y<max.y; gridPointUcs.y+=spacing.y) {
            gridPointWcs = ucs.mapFromUcs(gridPointUcs);
            glVertex3f(gridPointWcs.x, gridPointWcs.y, gridPointWcs.z);
        }
    }
            
    glEnd();
    */
}



/**
 *
 */
void RGraphicsViewGl::paintGridPoint(const RVector& ucsPosition) {
	if (scene == NULL) {
		return;
	}
    RDocumentInterface& documentInterface = scene->getDocumentInterface();
    RUcs ucs = documentInterface.getCurrentUcs();
    RVector gridPointWcs = ucs.mapFromUcs(ucsPosition);
    glVertex3f(gridPointWcs.x, gridPointWcs.y, gridPointWcs.z);
}



/**
 * Calls the OpenGL commands to draw the cross hair for the absolute zero point.
 */
void RGraphicsViewGl::callZero() {
    double size = mapDistanceFromView(20);

	if (scene == NULL) {
		return;
	}
    if (scene->getProjectionRenderingHint()==RS::ThreeD) {
        size = 1000.0;
    }

    glColor3f(1.0f,0.0f,0.0f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
    glVertex3f(-size,0,0);
    glVertex3f(size,0,0);
    glVertex3f(0,-size,0);
    glVertex3f(0,size,0);
    glVertex3f(0,0,-size);
    glVertex3f(0,0,size);
    glEnd();
}



/**
 * Calls the OpenGL commands to draw the cross hair for the mouse cursor.
 */
void RGraphicsViewGl::callCursor() {
	if (scene == NULL) {
		return;
	}
    RVector cursorPosition = scene->getDocumentInterface().getCursorPosition();
    RVector position = mapToView(cursorPosition);

    if (position.valid) {
        setupScreenViewport();

        // cursor size in pixels:
        double size = 10.0;

        glColor3f(1.0f,0.75f,0.0f);
        glLineWidth(1.0f);
        glDisable(GL_LINE_STIPPLE);

        // draw 3d crosshairs:
        glBegin(GL_LINES);
        glVertex3f(position.x-size, position.y, position.z);
        glVertex3f(position.x+size, position.y, position.z);
        glVertex3f(position.x, position.y-size, position.z);
        glVertex3f(position.x, position.y+size, position.z);
        glVertex3f(position.x, position.y, position.z-size);
        glVertex3f(position.x, position.y, position.z+size);
        glEnd();
    }
}



/**
 * Calls OpenGL commands to paint the reference points of all selected
 * entities. The reference points are stored in the scene and updated
 * whenever selected entities are exported to the scene.
 */
void RGraphicsViewGl::callReferencePoints() {
	if (scene == NULL) {
		return;
	}
    QMultiMap<REntity::Id, RVector>& referencePoints = 
        scene->getReferencePoints();

    QMultiMap<REntity::Id, RVector>::iterator it;
    for (it=referencePoints.begin(); it!=referencePoints.end(); ++it) {
        RVector position = mapToView(it.value());
        callReferencePoint(position, false);
    }
}



/**
 * Calls the OpenGL commands to draw a reference point at the given screen position.
 *
 * \param highlighted True to highlight the reference point (e.g. on mouse over)
 */
void RGraphicsViewGl::callReferencePoint(const RVector& screenPosition, bool highlighted) {
    setupScreenViewport();

    // note that these commands use screen coordiantes
    // to ensure that reference points have a constant size:
    if (highlighted) {
        glColor4f(1.0f,1.0f,1.0f,1.0f);
    }
    else {
        glColor4f(1.0f,0.0f,0.0f,0.5f);
    }
    glLineWidth(1.0f);

    glBegin(GL_LINE_LOOP);
    glVertex3f(screenPosition.x-3, screenPosition.y-3, 0);
    glVertex3f(screenPosition.x+3, screenPosition.y-3, 0);
    glVertex3f(screenPosition.x+3, screenPosition.y+3, 0);
    glVertex3f(screenPosition.x-3, screenPosition.y+3, 0);
    glEnd();
    
    setupViewport();
}



/**
 * Sets up the OpenGL viewport and transformations for the current
 * OpenGL context. This can be the OpenGL view or the pbuffer for
 * the preview.
 */
void RGraphicsViewGl::setupViewport() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);

    glEnable(GL_LINE_SMOOTH);

    glViewport(0, 0, getWidth(), getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        0, getWidth(), 
        0, getHeight(), 
        -1e6, 1e6
    );
    glMatrixMode(GL_MODELVIEW);
    
    // clear the screen and the depth buffer:
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    // reset transformations:
    glLoadIdentity();

    //glRotated(30.0, 0.0,0.0,1.0);
    glScaled(factor,factor,factor);
    glTranslated(offset.x, offset.y, 0.0);
    //glTranslated(offset.x, offset.y, -10.0);
}



/**
 * Sets up the OpenGL viewport and transformations for the current
 * OpenGL context for 2d screen coordinate based rendering. 
 * This is typically used for overlay information such as reference
 * points, fixed size text labels, etc.
 */
void RGraphicsViewGl::setupScreenViewport() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glViewport(0, 0, getWidth(), getHeight());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
        0, getWidth(), 
        0, getHeight(), 
        0, 1e6
    );
    glMatrixMode(GL_MODELVIEW);
    
    // clear the screen and the depth buffer:
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    // reset transformations:
    glLoadIdentity();

    glScaled(1.0, -1.0, 1.0);
    glTranslated(0.0, -getHeight(), 0.0);
}


void RGraphicsViewGl::keyPressEvent(QKeyEvent* event) {
	if (event == NULL) {
		return;
	}
	RGraphicsView::handleKeyPressEvent(*event);
}

void RGraphicsViewGl::keyReleaseEvent(QKeyEvent* event) {
	if (event == NULL) {
		return;
	}
	RGraphicsView::handleKeyReleaseEvent(*event);
}


/**
 * Relays the Qt mouse event to the scene.
 */
void RGraphicsViewGl::mousePressEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RMouseEvent e(*event, *scene, *this);
    RGraphicsView::handleMousePressEvent(e);
    lastButtonState = event->buttons();
}



/**
 * Relays the Qt mouse event to the scene.
 */
void RGraphicsViewGl::mouseReleaseEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RMouseEvent e(*event, *scene, *this);
    RGraphicsView::handleMouseReleaseEvent(e);
    lastButtonState = event->buttons();
}



/**
 * Relays the Qt mouse event to the scene.
 */
void RGraphicsViewGl::mouseMoveEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RMouseEvent e(*event, *scene, *this);
    RGraphicsView::handleMouseMoveEvent(e);

    // workaround for wacom tablets bug: wacom tablets don't fire a mouseReleaseEvent
    // if a mouse button is pressed, the mouse dragged and then the mouse button
    // is released:
    if (lastButtonState!=event->buttons()) {
        if (lastButtonState.testFlag(Qt::LeftButton)==true && 
            event->buttons().testFlag(Qt::LeftButton)==false) {

            QMouseEvent e(
                QEvent::MouseButtonRelease,
                event->pos(),
                Qt::LeftButton,
                event->buttons(),
                event->modifiers()
            );
            mouseReleaseEvent(&e);
        }
        if (lastButtonState.testFlag(Qt::MidButton)==true && 
            event->buttons().testFlag(Qt::MidButton)==false) {

            QMouseEvent e(
                QEvent::MouseButtonRelease,
                event->pos(),
                Qt::MidButton,
                event->buttons(),
                event->modifiers()
            );
            mouseReleaseEvent(&e);
        }
        if (lastButtonState.testFlag(Qt::RightButton)==true && 
            event->buttons().testFlag(Qt::RightButton)==false) {

            QMouseEvent e(
                QEvent::MouseButtonRelease,
                event->pos(),
                Qt::RightButton,
                event->buttons(),
                event->modifiers()
            );
            mouseReleaseEvent(&e);
        }
        lastButtonState = event->buttons();
    }
}



/**
 * Relays the Qt mouse event to the scene.
 */
void RGraphicsViewGl::wheelEvent(QWheelEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RWheelEvent e(*event, *scene, *this);
    RGraphicsView::handleWheelEvent(e);
}

/**
 * Relays the Qt tablet event to the scene.
 */
void RGraphicsViewGl::tabletEvent(QTabletEvent* event) {
	if (event == NULL || scene==NULL) {
		return;
	}
	RTabletEvent e(*event, *scene, *this);
	RGraphicsView::handleTabletEvent(e);
	event->ignore();
}


/**
 * Sets the current zoom factor.
 */
/*
void RGraphicsViewGl::setZoom(float z) {
    if (z!=factor) {
        factor = z;
        update();
    }
}
*/



/**
 * Resets the viewport (factor 1 and no offset.
 */
void RGraphicsViewGl::slotReset() {
    factor = 1.0;
    offset = RVector(0,0);
    update();
}



/**
 * Maps the given absolute (WCS) 3d model coordinate into 2d screen coordinates.
 */
RVector RGraphicsViewGl::mapToView(const RVector& v) {
    // let's not change the context when mapping coordinates:
    //makeMainCurrent();
    setupViewport();

    double modelview[16], projection[16];
    GLint viewport[4];

    // get the projection matrix:
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    // get the modelview matrix:
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    // get the viewport:
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // unproject the window coordinates to find the world coordinates:
    RVector ret;
//    gluProject(v.x, v.y, v.z, modelview,
//        projection, viewport, &ret.x, &ret.y, &ret.z);

    return RVector(ret.x, viewport[3]-ret.y, 0.0, v.valid);
}



/**
 * Maps the given 2d screen (view) coordinates in Pixels to absolute
 * (WCS) model coordinates on the current UCS plane.
 */
RVector RGraphicsViewGl::mapFromView(const RVector& v, double /*z*/) {
    // We're producing a ray here that goes from the mouse position at the
    // front clipping plane to the mouse position at the back clipping plane.
    // The intersection of that ray with the current UCS plane is the 
    // coordinate that interests us.

	if (scene == NULL) {
		return RVector::invalid;
	}

    // let's not change the context when mapping coordinates:
    //makeMainCurrent();
    setupViewport();

    double modelview[16];
    double projection[16];
    GLint viewport[4];
    
    // get the projection matrix     
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    // get the modelview matrix      
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    // get the viewport      
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    int success;

    // unproject the window coordinates on the front clipping plane:
    RVector mouseFront;
//    success = gluUnProject(
//        v.x, viewport[3]-v.y, 0.0,
//        modelview, projection, viewport,
//        &mouseFront.x, &mouseFront.y, &mouseFront.z
//    );
    
    if (success!=GL_TRUE) {
        RDebug::debug("RGraphicsViewGl::mapFromView: "
            "coordinate mapping failed");
    }
    
    // unproject the window coordinates on the back clipping plane:
    RVector mouseBack;
//    success = gluUnProject(
//        v.x, viewport[3]-v.y, 1.0,
//        modelview, projection, viewport,
//        &mouseBack.x, &mouseBack.y, &mouseBack.z
//    );

    if (success!=GL_TRUE) {
        RDebug::debug("RGraphicsViewGl::mapFromView: "
            "coordinate mapping failed");
    }

    RLine ray(mouseFront, mouseBack);

    RUcs ucs = scene->getDocumentInterface().getCurrentUcs();

    // create plane for current UCS:
    RTriangle plane(
        ucs.origin,
        ucs.origin + ucs.xAxisDirection,
        ucs.origin + ucs.yAxisDirection
    );

//    QList<RVector> retList = plane.getIntersectionPoints(ray, false);
    QList<RVector> retList = RShape::getIntersectionPoints(plane, ray, false);
    if (retList.size()==0) {
        RDebug::debug("RGraphicsViewGl::mapFromView: ret: invalid");
        return RVector::invalid;
    }

    RVector ret = retList.front();
    
    /*
    RVector v2 = mouseBack - mouseFront;

    double f = - mouseFront.z / v2.z;

    RVector ret = mouseFront + v2 * f;
    */

    /*
    RDebug::debug("RGraphicsViewGl::mapFromView: front: %f/%f/%f", mouseFront.x, mouseFront.y, mouseFront.z);
    RDebug::debug("RGraphicsViewGl::mapFromView: back: %f/%f/%f", mouseBack.x, mouseBack.y, mouseBack.z);
    RDebug::debug("RGraphicsViewGl::mapFromView: ret: %f/%f/%f", ret.x, ret.y, ret.z);
    */

    return ret;
}



/**
 * \todo Implement
 */
double RGraphicsViewGl::mapDistanceToView(double /*d*/) {
    // todo: refactor into something more straight to the point
    /*
    QPointF point1 = mapToView(RVector(0,0,0));
    QPointF point2 = mapToView(RVector(d,0,0));
    QPointF point3 = mapToView(RVector(0,d,0));

    RVector p1(point1.x(), point1.y());
    RVector p2(point2.x(), point2.y());
    RVector p3(point3.x(), point3.y());

    double d1 = p1.getDistanceTo(p2);
    double d2 = p1.getDistanceTo(p3);

    return std::max(d1, d2);
    */
    return 0.0;
}



double RGraphicsViewGl::mapDistanceFromView(double d) {
    RVector point1 = mapFromView(RVector(0,0));
    RVector point2 = mapFromView(RVector(1,0));
    return d * (point1.getDistanceTo(point2));
}



int RGraphicsViewGl::getWidth() {
    return width();
}



int RGraphicsViewGl::getHeight() {
    return height();
}



void RGraphicsViewGl::setCursor(Qt::CursorShape cursorShape) {
    QWidget::setCursor(cursorShape);
}

void RGraphicsViewGl::setCursor(const QCursor& cursor) {
    QWidget::setCursor(cursor);
}
