#include "RDebug.h"
#include "RDocumentInterface.h"
#include "RGraphicsSceneGl.h"
#include "RGraphicsViewGl.h"
#include "RLine.h"
#include "RPoint.h"
#include "RCircle.h"
#include "RArc.h"
#include "RTriangle.h"
#include "RSettings.h"


RGraphicsSceneGl::RGraphicsSceneGl(RDocumentInterface& documentInterface)
    : RGraphicsScene(documentInterface) {

    maxPreviewListIndex = 1;
}



RGraphicsSceneGl::~RGraphicsSceneGl() {
}



void RGraphicsSceneGl::beginPreview() {
	RGraphicsScene::beginPreview();

	QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		(*it)->makePreviewCurrent();
		GLuint listId = getListId();
		glNewList(listId, GL_COMPILE);
	}
}



void RGraphicsSceneGl::endPreview() {
	RGraphicsScene::endPreview();

	QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		(*it)->makePreviewCurrent();
		glEndList();
	}
}



void RGraphicsSceneGl::clearPreview() {
    RGraphicsScene::clearPreview();

    QList<RGraphicsViewGl*> glViews = getGlViews();
    QList<RGraphicsViewGl*>::iterator it;
    for (it=glViews.begin(); it!=glViews.end(); it++) {
        (*it)->makePreviewCurrent();
        glDeleteLists(1, maxPreviewListIndex);
    }

    maxPreviewListIndex = 1;
}



void RGraphicsSceneGl::setColor(const RColor& clr) {
    //if (isColorFixed()) {
    //    return;
    //}
    
    glColor4f(clr.redF(), clr.greenF(), clr.blueF(), clr.alphaF());
}



/*
void RGraphicsSceneGl::setEntityColor(const RColor& clr) {
    if (isColorFixed()) {
        return;
    }

    const REntity* entity = getEntity();
    if (entity==NULL) {
        setColor(clr);
        return;
    }

    if (entity->isSelected()) {
    	setStyle(Qt::SolidLine);
        glColor3f(0.64f, 0.27f, 0.27f);
        return;
    }

    setColor(clr);
}
*/



void RGraphicsSceneGl::setPen(const QPen& pen) {
    RExporter::setPen(pen);

    switch(pen.style()) {
    case Qt::DashLine:
        glLineStipple(5, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
        break;

    default:
    case Qt::SolidLine:
        glDisable(GL_LINE_STIPPLE);
        break;
    }
}



/**
 * Creates a display list for the current entity and exports the 
 * entity into that list.
 */
void RGraphicsSceneGl::exportCurrentEntity() {
	QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		setGlContext(**it);
		handleGlErrors();

		if (!exportToPreview) {
			GLuint listId = getListId();
			handleGlErrors();
			glNewList(listId, GL_COMPILE);
			handleGlErrors();
		}

		RGraphicsScene::exportCurrentEntity();
		handleGlErrors();

		if (!exportToPreview) {
			glEndList();
			handleGlErrors();
		}
	}
}


void RGraphicsSceneGl::exportLine(const RLine& line, double offset) {
    REntity* e=getEntity();
    if(e == NULL){
        return;
    }

    QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		setGlContext(**it);

		glLineWidth(e->getLineweight() / 100.0);
		handleGlErrors();

		setGlColor();
		glBegin( GL_LINES);

		RGraphicsScene::exportLine(line, offset);

		glEnd();
		handleGlErrors();
	}
}

void RGraphicsSceneGl::exportArc(const RArc& arc, double offset) {
    // TODO: implement
}

void RGraphicsSceneGl::exportPoint(const RPoint& point) {
    REntity* e = getEntity();
	if (e == NULL) {
		return;
	}

	QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		setGlContext(**it);

		glLineWidth(e->getLineweight() / 100.0);
		handleGlErrors();

		setGlColor();
		glBegin( GL_POINTS);
		glVertex3f(point.position.x, point.position.y, point.position.z);
		glEnd();
		handleGlErrors();
	}
}

void RGraphicsSceneGl::exportCircle(const RCircle& circle) {
    REntity* e = getEntity();
	if (e == NULL) {
		return;
	}

	QList<RGraphicsViewGl*> glViews = getGlViews();
	QList<RGraphicsViewGl*>::iterator it;
	for (it = glViews.begin(); it != glViews.end(); it++) {
		setGlContext(**it);

		glLineWidth(e->getLineweight() / 100.0);
		handleGlErrors();

		setGlColor();
		glBegin( GL_POINTS);
		// TODO
		glVertex3f(circle.center.x, circle.center.y, circle.center.z);
		glEnd();
		handleGlErrors();
	}
}

void RGraphicsSceneGl::exportArcSegment(const RArc& arc) {
}

/**
 * Exports a line with the given geometry to the OpenGL list.
 */
void RGraphicsSceneGl::exportLineSegment(const RLine& line) {
	glVertex3f(line.startPoint.x, line.startPoint.y, line.startPoint.z);
	glVertex3f(line.endPoint.x, line.endPoint.y, line.endPoint.z);
}

void RGraphicsSceneGl::exportTriangle(const RTriangle& triangle) {

    QList<RGraphicsViewGl*> glViews = getGlViews();
    QList<RGraphicsViewGl*>::iterator it;
    for (it=glViews.begin(); it!=glViews.end(); it++) {
        setGlContext(**it);

        glLineWidth(1.0f);

        glBegin(GL_TRIANGLES);
        glVertex3f(triangle.corner[0].x, triangle.corner[0].y, triangle.corner[0].z);
        glVertex3f(triangle.corner[1].x, triangle.corner[1].y, triangle.corner[1].z);
        glVertex3f(triangle.corner[2].x, triangle.corner[2].y, triangle.corner[2].z);
        glEnd();
    }
}



/**
 * Removes the display list that represents the given entity from all
 * views (i.e. the OpenGL contexts of all views).
 */
void RGraphicsSceneGl::unexportEntity(REntity::Id entityId) {
    RGraphicsScene::unexportEntity(entityId);

    QList<RGraphicsViewGl*> glViews = getGlViews();
    QList<RGraphicsViewGl*>::iterator it;
    for (it=glViews.begin(); it!=glViews.end(); it++) {
        (*it)->makeMainCurrent();
        glDeleteLists(entityId, 1);
    }
}
    
    
    
void RGraphicsSceneGl::highlightEntity(REntity& entity) {
    beginPreview();

    // entity highlighting is done with a fixed color that 
    // makes the entity reddish if it is not selected or
    // lighter if it is selected.
    if (entity.isSelected()) {
        //setFixedColor(1.0f,1.0f,1.0f,0.2f);
    }
    else {
        //setFixedColor(0.64f,0.27f,0.27f,0.8f);
    }

    RGraphicsScene::exportEntity(entity);

    //unsetFixedColor();

    endPreview();
}



void RGraphicsSceneGl::highlightReferencePoint(const RVector& position) {
    QList<RGraphicsViewGl*> glViews = getGlViews();
    QList<RGraphicsViewGl*>::iterator it;
    for (it=glViews.begin(); it!=glViews.end(); it++) {
        RVector screenPosition = (*it)->mapToView(position);

        beginPreview();
//		GLuint listId = getListId();
//		handleGlErrors();
//		glNewList(listId, GL_COMPILE);
//		handleGlErrors();

        (*it)->callReferencePoint(screenPosition, true);

//		glEndList();
//		handleGlErrors();
        endPreview();
    }
}



/**
 * \return List of OpenGL views that are attached to this scene.
 */
QList<RGraphicsViewGl*> RGraphicsSceneGl::getGlViews() {
    QList<RGraphicsViewGl*> ret;

    QList<RGraphicsView*>::iterator it;
    for (it=views.begin(); it!=views.end(); it++) {
        RGraphicsViewGl* view = dynamic_cast<RGraphicsViewGl*>(*it);
        if (view==NULL) {
            RDebug::error("RGraphicsSceneGl::getGlViews: "
                "non GL view attached to GL scene");
            continue;
        }

        ret.push_back(view);
    }

    return ret;
}



/**
 * Returns an OpenGL display list ID that is appropriate to use
 * for the export that is in progress. If an entity is currently being
 * exported, the entity ID is returned. If we are currently exporting
 * to the preview, a new auto generated ID is returned.
 */
GLuint RGraphicsSceneGl::getListId() {
    GLuint ret;
    if (exportToPreview) {
		ret = maxPreviewListIndex++;
	} else {
		const REntity* entity = getEntity();
		Q_ASSERT(entity!=NULL);
		ret = entity->getId();
	}
        
    return ret;
}



/**
 * Internal
 */
void RGraphicsSceneGl::setGlContext(RGraphicsViewGl& view) {
    if (exportToPreview) {
        view.makePreviewCurrent();
    }
    else {
        view.makeMainCurrent();
    }
}



/**
 * Sets the color for the current entity.
 */
void RGraphicsSceneGl::setGlColor() {
    const REntity* entity = getEntity();
    if (entity != NULL) {
		setLineweight(RLineweight::Weight000);
		setStyle(Qt::SolidLine);
		if (entity->isSelected()) {
			setColor(RSettings::getColor(RSettings::SelectionColor));
		} else {
            QColor col = entity->getColor();
            // prevent black on black / white on white drawing:
			if (col.value() == 0
					&& RSettings::getColor(RSettings::BackgroundColor).value() < 64) {
				col = Qt::white;
			} else if (col.value() == 255
					&& RSettings::getColor(RSettings::BackgroundColor).value() > 192) {
				col = Qt::black;
			}
			setColor(col);
		}
	}
    glColor4f(currentPen.color().redF(), currentPen.color().greenF(),
			currentPen.color().blueF(), currentPen.color().alphaF());
}



/**
 * Handles OpenGL errors by printing an appropriate warning or
 * error message.
 */
void RGraphicsSceneGl::handleGlErrors() {
    switch (glGetError()) {
    case GL_INVALID_ENUM:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): An unacceptable value is specified for an enumerated argument.");
        break;

    case GL_INVALID_VALUE:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): A numeric argument is out of range.");
        break;

    case GL_INVALID_OPERATION:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): The specified operation is not allowed in the current state.");
        break;

    case GL_STACK_OVERFLOW:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): This command would cause a stack overflow.");
        break;

    case GL_STACK_UNDERFLOW:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): This command would cause a stack underflow.");
        break;

    case GL_OUT_OF_MEMORY:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): There is not enough memory left to execute the command. "
            "The state of the GL is undefined, except for the state of the error "
            "flags, after this error is recorded.");
        break;
/* HACK: Rutger
    case GL_TABLE_TOO_LARGE:
        RDebug::warning("RGraphicsSceneGl::handleGlErrors(): The specified table exceeds "
        	"the implementation's maximum "
            "supported table size.");
        break;
*/
    default:
        break;

    }

}

void RGraphicsSceneGl::registerView(RGraphicsViewGl* view) {
    RGraphicsScene::registerView(view);
}

