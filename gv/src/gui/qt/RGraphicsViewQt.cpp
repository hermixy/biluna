#include <QtCore>
#include <QWheelEvent>
#include <QDragMoveEvent>
#include <QScrollBar>
//#include <QtConcurrent>
#include <QPainter>

#include "RDebug.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RGraphicsSceneQt.h"
#include "RGraphicsViewQt.h"
#include "RLine.h"
#include "RMdiChildQt.h"
#include "RMouseEvent.h"
#include "RSettings.h"
#include "RWheelEvent.h"
#include "RGuiActionQt.h"
#include "RMainWindowQt.h"

#include <QGraphicsScene>

RGraphicsViewQt::RGraphicsViewQt(QWidget* parent)
    : QWidget(parent),
      RGraphicsView(),
      sceneQt(NULL),
      lastSize(0,0),
      lastOffset(RVector::invalid),
      lastFactor(-1.0),
      gridPainter(NULL),
      doPaintOrigin(true),
      antialiasing(false) {

    currentScale = 1.0;
    setFocusPolicy(Qt::WheelFocus);
    saveViewport();

    setMouseTracking(true);

    graphicsBufferNeedsUpdate = true;

    lastButtonState = Qt::NoButton;

    numBuffers = 1;
    if (RS::getCPUCores() > 1) {
            numBuffers = 2;
    }
    //qDebug() << "numBuffers: " << numBuffers;

    painter = new QPainter*[numBuffers];
    for (int i = 0; i < numBuffers; ++i) {
        painter[i] = NULL;
    }
    threadBuffer = new QImage[numBuffers];
    mutex = new QMutex[numBuffers];
}



RGraphicsViewQt::~RGraphicsViewQt() {

}

void RGraphicsViewQt::setPaintOrigin(bool val) {
	doPaintOrigin = val;
}

void RGraphicsViewQt::setAntialiasing(bool val) {
	antialiasing = val;
}

bool RGraphicsViewQt::getAntialiasing() {
	return antialiasing;
}

void RGraphicsViewQt::viewportChangeEvent() {
	RGraphicsView::viewportChangeEvent();
	emit viewportChanged();
}

void RGraphicsViewQt::setScene(RGraphicsSceneQt* scene) {
    sceneQt = scene;
    RGraphicsView::setScene(scene);
}


void RGraphicsViewQt::invalidate(bool force) {
    graphicsBufferNeedsUpdate = true;
    if (force) {
        lastFactor = -1;
    }
}

/**
 * Regenerates the view from the underlying scene. 
 */
void RGraphicsViewQt::regenerate(bool force) {
    updateTransformation();
    invalidate(force);
    update();
    viewportChangeEvent();
}

/**
 * Triggers a paintEvent based on a buffered offscreen bitmap (very fast).
 */
void RGraphicsViewQt::repaint() {
    update();
}
    
void RGraphicsViewQt::saveViewport() {
    previousView = transform;
}

void RGraphicsViewQt::restoreViewport() {
    transform = previousView;
}

RVector RGraphicsViewQt::mapToView(const RVector& v) {
    /*
    RVector projected = v.getTransformed(sceneQt.getProjection());
    */
    RVector projected = v;
    updateTransformation();
    QPointF t = transform.map(QPointF(projected.x, projected.y));
    return RVector(t.x(), t.y());
}

RVector RGraphicsViewQt::mapFromView(const RVector& v, double z) {
	updateTransformation();
    QPointF p = transform.inverted().map(QPointF(v.x, v.y));
    //return RVector(p.x(), p.y(), z).getTransformed(sceneQt.getInverseProjection());
    return RVector(p.x(), p.y(), z);
}

double RGraphicsViewQt::mapDistanceToView(double d) {
    return d*factor;
}

double RGraphicsViewQt::mapDistanceFromView(double d) {
    return d/factor;
}

double RGraphicsViewQt::getCurrentScale() {
	return factor;
}

/**
 * Repaints the view. If the view has been invalidated before,
 * the view is redrawn from scratch. Otherwise, only a cached
 * buffer is drawn (very fast).
 *
 * \see invalidate
 */
void RGraphicsViewQt::paintEvent(QPaintEvent* /*e*/) {
    if(sceneQt == NULL) {
        return;
    }

    if (graphicsBufferNeedsUpdate) {
        updateGraphicsBuffer();
        graphicsBufferNeedsUpdate = false;

        // optimization for panning:
        if (lastFactor==factor) {
            QImage lastBuffer = graphicsBuffer.copy();
            QPainter gbPainter(&graphicsBuffer);
            RVector o = mapToView(offset) - mapToView(lastOffset);
            int ox = RMath::mround(o.x);
            int oy = RMath::mround(o.y);
            gbPainter.drawImage(ox, oy, lastBuffer);
            gbPainter.end();

            {
                QRect rect(
                    ox<0 ? ox+width() : 0,
                    qMax(oy, 0),
                    abs(ox),
                    height()-oy
                );
                paintDocument(rect);
                paintGrid(graphicsBuffer, rect);
            }

            {
                QRect rect(
                    0,
                    oy<0 ? oy+height() : 0,
                    width(),
                    abs(oy)+1
                );
                paintDocument(rect);
                paintGrid(graphicsBuffer, rect);
            }
        }
        else {
            paintDocument();
            paintGrid(graphicsBuffer);
        }
        lastOffset = offset;
        lastFactor = factor;
    }
    

    QImage graphicsBufferWithPreview = graphicsBuffer;

    // draws the current preview on top of the buffer:
    QList<RPainterPath> preview = sceneQt->getPreviewPainterPaths();
    if (!preview.isEmpty()) {
        QPainter* painter = initPainter(graphicsBufferWithPreview, false);
        Draw d(painter, this, getBackgroundColor().value());
		d(-1);
        painter->end();
        delete painter;
    }

    // highlighting of closest reference point:
    if (scene->getHighlightedReferencePoint().isValid()) {
        RVector p = mapToView(scene->getHighlightedReferencePoint());
        QPainter gbPainter(&graphicsBufferWithPreview);
        gbPainter.setPen(RColor::getHighlighted(RSettings::getColor(RSettings::ReferencePointColor)));
        gbPainter.drawRect(QRect(p.x - 5, p.y - 5, 10, 10));
        gbPainter.end();
    }

    paintCursor(graphicsBufferWithPreview);
    paintRelativeZero(graphicsBufferWithPreview);

    QPainter wPainter(this);
    wPainter.drawImage(0, 0, graphicsBufferWithPreview, 0, 0, -1, -1, Qt::OrderedAlphaDither);
    wPainter.end();

}

void RGraphicsViewQt::paintGrid(QPaintDevice& device, const QRect& rect) {
    QRect r = rect;
    if (rect.isNull()) {
        r = QRect(0,0,width(),height());
    }

    RVector c1 = mapFromView(RVector(r.left()-1,r.top()-1));
    RVector c2 = mapFromView(RVector(r.right()+1,r.bottom()+1));
    QRectF rf(c1.x, c1.y, c2.x-c1.x, c2.y-c1.y);

    gridPainter = initPainter(device, false);
    if (!rect.isNull()) {
        gridPainter->setClipRect(rf);
    }

//    gridPainter->setPen(
//                QPen(RSettings::getColor(RSettings::OriginColor, RColor(Qt::red))));
    gridPainter->setPen(QPen(RColor(Qt::red)));
    paintOrigin();

    if (grid!=NULL) {
//        gridPainter->setPen(
//                    QPen(RSettings::getColor(RSettings::MetaGridColor,
//                                             RColor(Qt::gray)),
//                         0, Qt::DotLine));
        gridPainter->setPen(QPen(RColor(Qt::gray), 0, Qt::DotLine));
        grid->paintMetaGrid(*this);

//        gridPainter->setPen(
//                    QPen(RSettings::getColor(RSettings::GridColor,
//                                             RColor(Qt::gray))));
        gridPainter->setPen(QPen(RColor(Qt::gray)));
        grid->paint(*this);
    }

    delete gridPainter;
    gridPainter = NULL;
}

void RGraphicsViewQt::paintGridPoint(const RVector& ucsPosition) {
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQt::paintGridPoint: gridPainter is NULL");
        return;
    }
    gridPainter->drawPoint(QPointF(ucsPosition.x, ucsPosition.y));
}

void RGraphicsViewQt::paintMetaGridLine(const RLine& ucsPosition) {
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQt::paintMetaGridLine: gridPainter is NULL");
        return;
    }
    gridPainter->drawLine(
        QPointF(ucsPosition.startPoint.x, ucsPosition.startPoint.y),
        QPointF(ucsPosition.endPoint.x, ucsPosition.endPoint.y)
    );
}

void RGraphicsViewQt::paintOrigin() {
    if (!doPaintOrigin) {
            return;
    }
    if (gridPainter==NULL) {
        RDebug::warning("RGraphicsViewQt::paintGridOrigin: gridPainter is NULL");
        return;
    }
    double r = mapDistanceFromView(20.0);
    gridPainter->drawLine(
        QPointF(-r, 0),
        QPointF(r,0)
    );
    gridPainter->drawLine(
        QPointF(0, -r),
        QPointF(0,r)
    );
}

void RGraphicsViewQt::paintCursor(QPaintDevice& device) {
    RDocumentInterface* di = getDocumentInterface();
    if (di==NULL) {
        return;
    }

    RVector pos = di->getCursorPosition();
    if (!pos.isValid()) {
        return;
    }

    if (!RSettings::getShowCrosshair()) {
        return;
    }

    RVector screenPos = mapToView(pos);

    QPainter painter(&device);
    painter.setPen(QPen(RSettings::getColor(RSettings::CrosshairColor), 0, Qt::DashLine));
    painter.drawLine(screenPos.x, 0, screenPos.x, height());
    painter.drawLine(0, screenPos.y, width(), screenPos.y);
    painter.end();
}

void RGraphicsViewQt::paintRelativeZero(QPaintDevice& device) {
    RDocumentInterface* di = getDocumentInterface();
    if (di==NULL) {
        return;
    }

    RVector relativeZero = di->getRelativeZero();
    if (!relativeZero.isValid()) {
        return;
    }

    RVector screenPos = mapToView(relativeZero);
    //double r = mapDistanceFromView(5.0);
    double r = 5.0;

    QPainter painter(&device);
    painter.setPen(
        QPen(RSettings::getColor(RSettings::RelativeZeroColor), 0)
    );
    painter.drawLine(
        QPointF(screenPos.x-r, screenPos.y),
        QPointF(screenPos.x+r, screenPos.y)
    );
    painter.drawLine(
        QPointF(screenPos.x, screenPos.y-r),
        QPointF(screenPos.x, screenPos.y+r)
    );
    painter.drawEllipse(QPointF(screenPos.x, screenPos.y), r, r);
    painter.end();
}

void RGraphicsViewQt::updateTransformation() {
	transform.reset();
	transform.scale(1, -1);
	transform.translate(0, -height());
	transform.scale(factor, factor);
	transform.translate(offset.x, offset.y);
}

/**
 * Updates the graphics buffer from scratch. 
 * This can be relatively slow and is only called on view
 * port changes or document changes.
 */
void RGraphicsViewQt::updateGraphicsBuffer() {
    QSize newSize = size();
    if (lastSize != newSize) {
        graphicsBuffer = QImage(newSize, QImage::Format_ARGB32);
        for (int i = 0; i < numBuffers; ++i) {
            threadBuffer[i] = QImage(newSize, QImage::Format_ARGB32);
        }
        lastSize = newSize;
        lastFactor = -1;
        return;
    }
}

void RGraphicsViewQt::paintDocument(const QRect& rect) {
    RDocument* document = getDocument();
    if (document == NULL) {
            return;
    }

    QRect r = rect;
    if (rect.isNull()) {
        r = QRect(0,0,width(),height());
    }
    
    // TODO: sort painter paths by Z-level:
    //qSort(p.begin(), p.end());

	QPainter gbPainter(&graphicsBuffer);
    gbPainter.setBackground(getBackgroundColor());
    if (!rect.isNull()) {
        //QRect rc(r.x()-1,r.y()-1,r.width()+2,r.height()+2);
        gbPainter.setClipRect(r);
    }
    gbPainter.eraseRect(r);

	for (int i = 0; i < numBuffers; ++i) {
		painter[i] = initPainter(threadBuffer[i], true, false, r);
	}

    // lookup entity IDs within the visible area:
    RVector c1 = mapFromView(RVector(r.left(),r.bottom()), -1e6);
    RVector c2 = mapFromView(RVector(r.right(),r.top()), 1e6);
    RBox queryBox(c1, c2);
    // TODO: convert units
    queryBox.growXY(document->getMaxLineweight()/100.0);
    QSet<REntity::Id> ids =
        document->queryIntersectedEntitiesXY(queryBox, true);

    // draw painter paths:
    QSet<RObject::Id> selectedIds;
    Draw f(painter, this, mutex, selectedIds, getBackgroundColor().value());
    f.setSelected(false);
    if (RSettings::getConcurrentDrawing()) {
//		QtConcurrent::blockingMap(ids, f);
	} else {
        QList<REntity::Id> list = document->getStorage().orderBackToFront(ids);
		QListIterator<REntity::Id> i(list);
		while (i.hasNext()) {
			f(i.next());
		}
	}

	for (int i = 0; i < numBuffers; ++i) {
        gbPainter.drawImage(0, 0, threadBuffer[i]);
	}

    // paint selected entities:
    if (!selectedIds.isEmpty()) {
        /*
    	for (int i = 0; i < numBuffers; ++i) {
			painter[i] = initPainter(threadBuffer[i], true);
		}
        */

		f.setSelected(true);
	    if (RSettings::getConcurrentDrawing()) {
//			QtConcurrent::blockingMap(selectedIds, f);
        } else {
            QList<REntity::Id> list = document->getStorage().orderBackToFront(selectedIds);
	    	//QList<RObject::Id> list = selectedIds.toList();
			//qSort(list);
			QListIterator<RObject::Id> i(list);
			while (i.hasNext()) {
				f(i.next());
			}
		}
		for (int i = 0; i < numBuffers; ++i) {
            gbPainter.drawImage(0, 0, threadBuffer[i]);
        }
        /*
		for (int i = 0; i < numBuffers; ++i) {
			painter[i]->end();
			delete painter[i];
			gbPainter.drawImage(0, 0, threadBuffer[i]);
		}
        */
	}

    for (int i = 0; i < numBuffers; ++i) {
		painter[i]->end();
		delete painter[i];
    }

    // paint reference points of selected entities:
    QMultiMap<REntity::Id, RVector>& referencePoints =
            scene->getReferencePoints();
    if (!referencePoints.isEmpty()) {
        QMultiMap<REntity::Id, RVector>::iterator it;
        for (it = referencePoints.begin(); it != referencePoints.end(); ++it) {
            RVector p = mapToView(it.value());
            // TODO: configurable size of reference point (app pref):
            gbPainter.fillRect(QRect(p.x - 5, p.y - 5, 10, 10),
                    RSettings::getColor(RSettings::ReferencePointColor));
        }
    }

	gbPainter.end();
}

QPainter* RGraphicsViewQt::initPainter(QPaintDevice& device, bool erase, bool screen, const QRect& rect) {
    QPainter* painter = new QPainter(&device);
    if (antialiasing) {
        painter->setRenderHint(QPainter::Antialiasing);
    }
    if (erase) {
        QRect r = rect;
        if (rect.isNull()) {
            r = QRect(0,0,lastSize.width(),lastSize.height());
        }
    	painter->setCompositionMode(QPainter::CompositionMode_Clear);
        //painter->eraseRect(0, 0, lastSize.width(), lastSize.height());
        painter->eraseRect(r);
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    }
    //painter->setRenderHint(QPainter::NonCosmeticDefaultPen, false);

    if (!screen) {
        painter->setWorldTransform(transform);
    }
    return painter;
}

RGraphicsViewQt::Draw::Draw(QPainter** painter, RGraphicsViewQt* view, QMutex* mutex,
        QSet<RObject::Id>& selectedIds, int bgColorValue) :
	painter(painter), view(view), mutex(mutex), isSelected(false),
            selectedIds(&selectedIds), bgColorValue(bgColorValue) {
}

RGraphicsViewQt::Draw::Draw(QPainter* painter, RGraphicsViewQt* view, int bgColorValue) :
    view(view), mutex(NULL), isSelected(false), selectedIds(NULL), bgColorValue(bgColorValue) {
	this->painter = new QPainter*[1];
	this->painter[0] = painter;
}

void RGraphicsViewQt::Draw::operator()(int id) {
	if (selectedIds != NULL && !isSelected && view->getDocument()->isSelected(
			id)) {
		static QMutex m;
		m.lock();
		selectedIds->insert(id);
		m.unlock();
		return;
	}

	QList<RPainterPath> painterPaths;
	if (id == -1) {
		painterPaths = 
            ((RGraphicsSceneQt*) view->getScene())->getPreviewPainterPaths();
	} else {
		painterPaths = 
            ((RGraphicsSceneQt*) view->getScene())->getPainterPaths(id);
	}

    QListIterator<RPainterPath> i(painterPaths);
	while (i.hasNext()) {
		RPainterPath path = i.next();
		QPen pen = path.getPen();
		QBrush brush = path.getBrush();
		if (pen.style() != Qt::NoPen) {
			if (pen.widthF() * view->getCurrentScale() < 2.0) {
				pen.setWidth(0);
			}
			if (isSelected || path.isSelected()) {
				pen.setColor(RSettings::getColor(RSettings::SelectionColor));
			}

            // prevent black on black / white on white drawing
			if (pen.color().value() == 0 && bgColorValue < 64) {
				pen.setColor(Qt::white);
            } else if (pen.color().value() == 255 && bgColorValue > 192) {
				pen.setColor(Qt::black);
			}

			if(path.isHighlighted()){
				pen.setColor(RColor::getHighlighted(pen.color()));
				pen.setWidthF(pen.widthF() + view->mapDistanceFromView(2.0));
			}
		}

		int i = 0;
		bool mutexFound = false;
		do {
			int a = i % view->getNumBuffers();
			if (mutex == NULL) {
				a = 0;
			}
			if (mutex == NULL || mutex[a].tryLock()) {
				if (brush.style() != Qt::NoBrush) {
					painter[a]->setBrush(brush);
				}
				painter[a]->setPen(pen);
				if (path.elementCount() == 2 && path.elementAt(0).isMoveTo()
						&& path.elementAt(1).isLineTo()) {
					qreal x1 = path.elementAt(0).x;
					qreal y1 = path.elementAt(0).y;
					qreal x2 = path.elementAt(1).x;
					qreal y2 = path.elementAt(1).y;
					painter[a]->drawLine(QPointF(x1, y1), QPointF(x2, y2));
				} else {
					painter[a]->drawPath(path);
				}
                if (path.hasPoints()) {
                    double one = view->mapDistanceFromView(1.0);
                    QList<RVector> points = path.getPoints();
                    QList<RVector>::iterator it;
                    for (it=points.begin(); it<points.end(); it++) {
                        painter[a]->drawLine(
                            QPointF((*it).x-one, (*it).y),
                            QPointF((*it).x+one, (*it).y)
                        );
                        painter[a]->drawLine(
                            QPointF((*it).x, (*it).y-one),
                            QPointF((*it).x, (*it).y+one)
                        );
                    }
                }
				if (mutex != NULL) {
					mutex[a].unlock();
				}
				mutexFound = true;
			}
			++i;
		} while (!mutexFound);
	}
}

void RGraphicsViewQt::Draw::setSelected(bool on) {
	isSelected = on;
}

/**
 * Relays the Qt mouse event to the scene.
 */
void RGraphicsViewQt::mouseMoveEvent(QMouseEvent* event) {
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



void RGraphicsViewQt::mousePressEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RMouseEvent e(*event, *scene, *this);
    RGraphicsView::handleMousePressEvent(e);
    lastButtonState = event->buttons();
}



void RGraphicsViewQt::mouseReleaseEvent(QMouseEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RMouseEvent e(*event, *scene, *this);
    RGraphicsView::handleMouseReleaseEvent(e);
    lastButtonState = event->buttons();
}



void RGraphicsViewQt::wheelEvent(QWheelEvent* event) {
    if (event==NULL || scene==NULL) {
        return;
    }
    RWheelEvent e(*event, *scene, *this);
    RGraphicsView::handleWheelEvent(e);
}

void RGraphicsViewQt::keyPressEvent(QKeyEvent* event) {
    if (event==NULL) {
        return;
    }
    RGraphicsView::handleKeyPressEvent(*event);
}

void RGraphicsViewQt::keyReleaseEvent(QKeyEvent* event) {
    if (event==NULL) {
        return;
    }
    RGraphicsView::handleKeyReleaseEvent(*event);
}

void RGraphicsViewQt::dragEnterEvent(QDragEnterEvent* event) {
	emit dragEnter(event);
}

void RGraphicsViewQt::dropEvent(QDropEvent* event) {
	emit drop(event);
}

void RGraphicsViewQt::resizeEvent(QResizeEvent* ) {
    regenerate();
}

int RGraphicsViewQt::getWidth() {
    return width();
}

int RGraphicsViewQt::getHeight() {
    return height();
}

void RGraphicsViewQt::setCursor(Qt::CursorShape cursorShape) {
    QWidget::setCursor(cursorShape);
}

void RGraphicsViewQt::setCursor(const QCursor& cursor) {
    QWidget::setCursor(cursor);
}

QSize RGraphicsViewQt::sizeHint() const {
    return QSize(200, 200);
}
