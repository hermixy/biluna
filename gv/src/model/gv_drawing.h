/*****************************************************************
 * $Id: gv_drawing.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 24, 2008 10:01:49 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_DRAWING_H
#define GV_DRAWING_H

#include "gv_graphicsscene.h"
#include "rb_objectcontainer.h"

class GV_Grid;
class GV_GvMdi;



class GV_Drawing : public GV_GraphicsScene, public RB_ObjectContainer  {

    Q_OBJECT

public:
    GV_Drawing(const RB_String& id = "", RB_ObjectBase* p = 0,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    GV_Drawing(GV_Drawing* drawing);
    virtual ~GV_Drawing ();

    virtual void synchronizeModelData(RB_ObjectBase* obj = NULL);
    virtual void synchronizeSceneData(RB_ObjectBase* obj = NULL);

//    virtual bool isDrawingChanged() { return mDrawingIsChanged; }
//    virtual void setDrawingChanged(bool changed = true) {
//        mDrawingIsChanged = changed;
//
//    }
//    virtual bool isSceneChanged() { return mSceneIsChanged; }
//    virtual void setSceneChanged(bool changed = true) { mSceneIsChanged = changed; }

    virtual void initDrawingArea(bool gridOn = false);

    virtual GV_Grid* getGrid();
    virtual bool isGridOn();
    virtual void setGridOn(bool on);
    virtual void updateGrid(const QRectF& rect);
    virtual bool isSelectionOnly();

    virtual double getScaleFactor();
    virtual void setScaleFactor(double sf);

    // TODO: from gv_eventhandler
    virtual RVector getRelativeZero() { return RVector(0.0, 0.0, 0.0); }

    virtual void setDefaultSnapMode(GV2::ActionType sm);
    virtual GV2::ActionType getDefaultSnapMode() {
        return defaultSnapMode;
    }
    virtual void setSnapRestriction(GV2::ActionType sr);
    virtual GV2::ActionType getSnapRestriction() {
        //return defaultSnapRes;
        return GV2::ActionRestrictNothing;
    }

    virtual GV_Component* getSelectedComponent();
    virtual void setPageSize(double width, double height);
    virtual QRectF getPageSize() const;

    virtual void setDrawPageBackground(bool draw) { mDrawPageBackground = draw; }
    virtual bool isDrawPageBackground() { return mDrawPageBackground; }

signals:
    void mousePressed();

protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void drawPageBackGround(QPainter* painter, const QRectF& rect);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e);

    int mx;   //!< Last known mouse cursor position
    int my;   //!< Last known mouse cursor position

    //! Current default snap mode for this graphic view, for new actions.
    GV2::ActionType defaultSnapMode;
    //! Current default snap restriction for this graphic view, for new actions.
    GV2::ActionType defaultSnapRes;

private:
    void createMembers();

//    bool mDrawingIsChanged;
//    bool mSceneIsChanged; // TODO: required?

    // Grid
    GV_Grid* mGrid;
    bool mGridOn;
    QColor mGridColor;
    QColor mMetaGridColor;

    //! Flag draw page outline and background
    bool mDrawPageBackground;

    //! Drawing is for selection only
    bool mSelectionOnly;

    //! Total scaling factor on graphicsview
    double mScaleFactor;

    // Page properties
    double mPageWidth;
    double mPageHeight;
    QColor mPageBorderColor;
    QColor mPageShadowColor;

    double mDotPerMm;

};

#endif /*GV_DRAWING_H*/
