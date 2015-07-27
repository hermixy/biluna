/*****************************************************************
 * $Id: gv_drawing.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 24, 2008 10:05:07 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_drawing.h"

#include "gv_actiondrawsymbol.h"
#include "gv_component.h"
#include "gv_dwgscenesync.h"
#include "gv_eventhandler.h"
#include "gv_grid.h"
//#include "gv_gvmdi_old.h"
#include "gv_objectfactory.h"
#include "RVector.h"


GV_Drawing::GV_Drawing(const RB_String& id, RB_ObjectBase* p,
                        const RB_String& n, RB_ObjectFactory* f)
                        : GV_GraphicsScene(), RB_ObjectContainer (id, p, n, f) {
    createMembers();

    eventHandler = new GV_EventHandler(this);

    mx = my = 0; // last known mouse position
    defaultSnapMode = GV2::ActionSnapGrid; // GV2::ActionSnapFree;
    defaultSnapRes = GV2::ActionRestrictNothing;

//    mDrawingIsChanged = false;
//    mSceneIsChanged = false; // TODO: required?

    mGrid = NULL;
    mGridOn = false;
    mGridColor = QColor(Qt::black);
    mMetaGridColor = QColor(Qt::lightGray);

    mSelectionOnly = true;
    mDotPerMm = 2.8346; // = 72 / 25.4 dot per inch divided by mm per inch
    mScaleFactor = mDotPerMm; // 1.0; // scalefactor is set by graphicsview

    mPageWidth = 210.0;
    mPageHeight = 297.0;
    mPageBorderColor = QColor(Qt::gray);
    mPageShadowColor = QColor(Qt::lightGray);

    mDrawPageBackground = true;
}


GV_Drawing::GV_Drawing(GV_Drawing* drawing) : RB_ObjectContainer(drawing) {
    createMembers();
    // *this = *drawing; TODO: replace with alternative operator= implementation

}


GV_Drawing::~GV_Drawing() {
    RB_DEBUG->print("GV_Drawing::~GV_Drawing()");

//	delete mScene;
//	mScene = NULL;
    // deletion of children and members is done in RB_ObjectBase
    // and RB_ObjectContainer

    if (eventHandler) {
        delete eventHandler;
        eventHandler = NULL;
    }

    if (mGrid) {
        delete mGrid;
        mGrid = NULL;
    }

    RB_DEBUG->print("GV_Drawing::~GV_Drawing() OK");
}


void GV_Drawing::createMembers() {
    addMember(this, "number", "-", "New ...", RB2::MemberChar125);	// 0
    addMember(this, "revision", "-", 0, RB2::MemberInteger);			// 1
    addMember(this, "description", "-", "", RB2::MemberChar125);

    // TODO: old revisions to be a child container
    // TODO: further title block members
}

/**
 * Initialize the drawing area with a background, border and drawing paper color
 * TODO: passed drawing border object and store for redrawing during drawBackground
 */
void GV_Drawing::initDrawingArea(bool gridOn) {
    RB_DEBUG->print("GV_Scene::initDrawingArea()");

    if (gridOn) {
        mGrid = new GV_Grid(this);
        mGridOn = true;
    }

    mSelectionOnly = false;

    int margin = 20;
    QRectF sceneRect = QRectF(0, 0, mPageWidth, mPageHeight);
    sceneRect.adjust(-margin, -margin, margin, margin);
    setSceneRect(sceneRect);
}

/**
 * @return current grid
 */
GV_Grid* GV_Drawing::getGrid() {
    return mGrid;
}

/**
 * @return true if grid is on
 */
bool GV_Drawing::isGridOn() {
    return mGridOn;
}

/**
 * @param on is true if grid is to be shown
 */
void GV_Drawing::setGridOn(bool on) {
    mGridOn = on;
}

/**
 * Update grid based on viewport dimensions
 * @param rect rectangle in scene coordinates
 */
void GV_Drawing::updateGrid(const QRectF& rect) {
    if (isGridOn())
        mGrid->update(rect, mScaleFactor);
}

/**
 * @return true when components can be selected only
 */
bool GV_Drawing::isSelectionOnly() {
    return mSelectionOnly;
}

/**
 * Get current overall scale factor
 */
double GV_Drawing::getScaleFactor() {
    return mScaleFactor;
}

/**
 * Set current scale factor and redraw grid if applicable
 * @param sf overall scale factor
 */
void GV_Drawing::setScaleFactor(double sf) {
    mScaleFactor = sf;

    // test 1 grid drawing alternative
//    double lineThickness = 1.0 / mScaleFactor;
//
//    QPixmap pixmap(10, 10);
//    pixmap.fill(QColor(Qt::white));
//    QPainter painter(&pixmap);
//    painter.setPen(QPen(QBrush(mGridColor), lineThickness, Qt::SolidLine));
//    painter.drawPoint(0,0);
//    setBackgroundBrush(QBrush(pixmap));

}

/**
 * Sets the default snap mode used by newly created actions.
 */
void GV_Drawing::setDefaultSnapMode(GV2::ActionType sm) {
    defaultSnapMode = sm;

    if (eventHandler!=NULL) {
        eventHandler->setSnapMode(sm);
    }
}

/**
 * Sets a snap restriction (e.g. orthogonal).
 */
void GV_Drawing::setSnapRestriction(GV2::ActionType sr) {
    defaultSnapRes = sr;

    if (eventHandler!=NULL) {
        eventHandler->setSnapRestriction(sr);
    }
}

/**
 * Get the MDI graphicsview
 */
//GV_GvMdi* GV_Drawing::getMdiGraphicsView() {
//	QList<QGraphicsView *> viewList = this->views();
//	GV_GvMdi* gvMdi = NULL;
//
//	for (int i = 0; i < viewList.size(); ++i) {
//		GV_GvMdi* tmpGvMdi = dynamic_cast<GV_GvMdi*>(viewList.at(i));
//
//		if (tmpGvMdi) {
//			gvMdi = tmpGvMdi;
//		}
//	}
//
//	return gvMdi;
//}


/**
 * Reimplementation of drawing background with grid and drawing page background
 * Calls drawPageBackground().
 */
void GV_Drawing::drawBackground(QPainter* painter, const QRectF& rect) {
    // Border of page background
    drawPageBackGround(painter, rect);

    if (mGrid == NULL || isGridOn() == false) {
        return;
    }

    // draw meta grid lines:
    painter->setPen(QPen(QBrush(mMetaGridColor), 0.0, Qt::SolidLine));
    double* mx = mGrid->getMetaX();
    double* my = mGrid->getMetaY();
    double* myIso = mGrid->getMetaYIso();

    if (mx != NULL) {
        for (int i = 0; i < mGrid->countMetaX(); ++i) {
            painter->drawLine(QPointF(mx[i], my[0]),
                    QPointF(mx[i], my[mGrid->countMetaY()-1]));
        }
    }

    // false for isometric grid test
    bool gridIsStandard = true; // false for isometric grid

    if (my != NULL && gridIsStandard) {
        for (int i=0; i < mGrid->countMetaY(); ++i) {
            painter->drawLine(QPointF(mx[0], my[i]),
                    QPointF(mx[mGrid->countMetaX()-1], my[i]));
        }
    }

    // test isometric grid only
//    const double isoFactor = 1 / pow(3, 0.5);
//    if (!gridIsStandard) {
//        for (int i=0; i < mGrid->countMetaY(); ++i) {
//        	painter->drawLine(QPointF(mx[0], my[i]),
//        			QPointF(mx[mGrid->countMetaX()-1],
//        			my[i]+(mx[mGrid->countMetaX()-1] - mx[0]) * isoFactor));
//        }
//    }
//    if (!gridIsStandard) {
//        for (int i=0; i < mGrid->countMetaY(); ++i) {
//        	painter->drawLine(QPointF(mx[0], my[i]),
//        			QPointF(mx[mGrid->countMetaX()-1],
//        			my[i]-(mx[mGrid->countMetaX()-1] - mx[0]) * isoFactor));
//        }
//    }

    // grid points
    painter->setPen(QPen(QBrush(mGridColor), 0.0, Qt::SolidLine));
    RVector* pts = mGrid->getPoints();

    if (pts!=NULL) {
        for (int i=0; i < mGrid->count(); ++i) {
            // paint only if in rectangle, improves performance
            if (rect.contains(QPointF(pts[i].x, pts[i].y))) {
                painter->drawPoint(pts[i].x, pts[i].y);
            }
        }
    }
}

/**
 * Draw page background, i.e. page border, margin, watermark and background
 * image. Override this function for custom implementation. Default draws
 * a portait A4 size page without watermark or background image. All in mm.
 * @param painter
 * @param rect rectangle to be painted
 */
void GV_Drawing::drawPageBackGround(QPainter* painter, const QRectF& /* rect */) {
    if (!mDrawPageBackground) return;

    double lineThickness = 2.0;
    double delta = lineThickness / 2;

    // page shadow
    painter->setPen(QPen(QBrush(mPageShadowColor), lineThickness, Qt::SolidLine));
    painter->drawLine(QPointF(mPageWidth + delta, 0 + lineThickness),
                      QPointF(mPageWidth + delta, mPageHeight + delta));
    painter->drawLine(QPointF(mPageWidth + delta, mPageHeight + delta),
                      QPointF(0 + lineThickness, mPageHeight + delta));

    // page border
    lineThickness = 1.0; // / mScaleFactor;
    painter->setPen(QPen(QBrush(mPageBorderColor), lineThickness, Qt::SolidLine));
    painter->drawLine(QPointF(0, 0), QPointF(mPageWidth, 0));
    painter->drawLine(QPointF(mPageWidth, 0), QPointF(mPageWidth, mPageHeight));
    painter->drawLine(QPointF(mPageWidth, mPageHeight), QPointF(0, mPageHeight));
    painter->drawLine(QPointF(0, mPageHeight), QPointF(0, 0));
}

/**
 * Only to emit mousePressed() signal indicating that drawing might be changed
 */
void GV_Drawing::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    GV_GraphicsScene::mousePressEvent(e);
    emit mousePressed();
}

/**
 * Synchronize the Qt graphic item data with the GV object data before saving
 */
void GV_Drawing::synchronizeModelData(RB_ObjectBase* obj) {
    RB_DEBUG->print("GV_Drawing::synchronizeModelData()");

    if (!obj) {
        obj = this;
    }

    GV_DwgSceneSync sync;
    sync.synchronizeModelData(obj);
}

/**
 * Synchronize the scene data after for example opening of an XML model file.
 * The scene has to be passed since the (sub)windows are not open during opening
 * of an XML file. The function is re-entrant that is why the object is passed
 * @param scene scene of current graphics view
 * @param obj object to be handled for synchronization of data
 */
void GV_Drawing::synchronizeSceneData(RB_ObjectBase* obj) {
    RB_DEBUG->print("GV_Drawing::synchronizeSceneData()");

    GV_GraphicsScene::clear();
    GV_DwgSceneSync sync;
    sync.synchronizeSceneData(this, obj);
}

/**
 * Get selected component. If more components are selected only one is returned,
 * the returned component is undefined
 * @return selected component
 */
GV_Component* GV_Drawing::getSelectedComponent() {
    RB_DEBUG->print("GV_Drawing::getSelectedComponent()");

    GV_Component* compItem = NULL; // not a port or node
    QList<QGraphicsItem*> itemList = items();
    int count = itemList.size();

    for (int i = 0; i < count && !compItem; i++) {
        QGraphicsItem *item = itemList.at(i);

        if (item->isSelected()) {
            if (item->type() != QGraphicsItem::UserType + 1
                && item->type() != QGraphicsItem::UserType + 2
                && item->type() != QGraphicsItem::UserType + 5) {
                // item was a port, resize handle or rotation lever
                // nodes cannot be selected
                item = item->parentItem();
            }

            if (item && item->type() == QGraphicsItem::UserType + 1) {
                compItem = dynamic_cast<GV_Symbol*>(item);
            } else if (item && item->type() == QGraphicsItem::UserType + 2) {
                compItem = dynamic_cast<GV_Line*>(item);
            } else if (item && item->type() == QGraphicsItem::UserType + 5) {
                compItem = dynamic_cast<GV_Text*>(item);
            }
        }
    }

    return compItem;
}

void GV_Drawing::setPageSize(double width, double height) {
    mPageWidth = width;
    mPageHeight = height;
}

QRectF GV_Drawing::getPageSize() const {
    return QRectF(0.0, 0.0, mPageWidth, mPageHeight);
}
