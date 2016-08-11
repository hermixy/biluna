/****************************************************************************
** $Id: rs_graphicview.h 9183 2008-02-24 14:07:07Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you. Updated for Biluna 2016-08-11
**
**********************************************************************/


#ifndef GV_GRAPHICVIEW_H
#define GV_GRAPHICVIEW_H

#include "gv_entitycontainer.h"

#include <stdarg.h>

#include "gv.h"
#include "gv_blocklist.h"
#include "gv_color.h"
//#include "rs_keyevent.h"
#include "gv_linetypepattern.h"
//#include "rs_mouseevent.h"
//#include "rs_commandevent.h"

class GV_ActionInterface;
class GV_EventHandler;
class GV_Grid;
class GV_Painter;
class GV_PainterQt;
class GV_Preview;



/**
 * This class is a common GUI interface for the graphic viewer 
 * widget which has to be implementet by real GUI classes such 
 * as the Qt graphical view.
 *
 * Note that this is just an interface used as a slot to 
 * communicate with the qcadlib from a GUI level. 
 */
class GV_GraphicView {
public:
    GV_GraphicView();
    virtual ~GV_GraphicView();

    void cleanUp();

    /**
     * @return Pointer to the graphic entity if the entity container
     * connected to this view is a graphic and valid.
     * NULL otherwise.
     */
    GV_Graphic* getGraphic() {
        if (container!=NULL && container->rtti()==GV2::EntityGraphic) {
            return (GV_Graphic*)container;
        } else {
            return NULL;
        }
    }

    /**
     * Sets the drawing mode.
     */
    void setDrawingMode(GV2::DrawingMode m) {
        drawingMode = m;
    }

    /**
     * @return Current drawing mode.
     */
    GV2::DrawingMode getDrawingMode() {
        return drawingMode;
    }
    
    /**
     * Activates or deactivates the delete mode.
     */
    void setDeleteMode(bool m) {
        deleteMode = m;
    }

    /**
     * @reval true Deleting instead of drawing.
     *        false Normal drawing mode.
     */
    bool getDeleteMode() {
        return deleteMode;
    }

    /** This virtual method must be overwritten to return
      the width of the widget the graphic is shown in */
    virtual int getWidth() = 0;
    /** This virtual method must be overwritten to return
      the height of the widget the graphic is shown in */
    virtual int getHeight() = 0;
    /** This virtual method must be overwritten to redraw
      the widget. */
    virtual void redraw() = 0;
    /** This virtual method must be overwritten to redraw
      the widget including the current preview. */
    virtual void redrawPreview() = 0;
    /** This virtual method must be overwritten to restore
      the widget view to the last known clean state. */
    virtual void restore() = 0;
    /** This virtual method must be overwritten and is then
      called whenever the view changed */
    virtual void adjustOffsetControls() {}
    /** This virtual method must be overwritten and is then
      called whenever the view changed */
    virtual void adjustZoomControls() {}
    /**
     * Sets an external painter device.
     */
    virtual void setPainter(GV_Painter* p) {
        painter = p;
    }
    /** This virtual method must be overwritten and must return
      the painter device for the buffer of this widget */
    virtual GV_Painter* createPainter() = 0;
    /**
     * This virtual method must be overwritten and must return
     *  the direct painter device for this widget 
     */
    virtual GV_Painter* createDirectPainter() = 0;
    /**
     * This virtual method must be overwritten and must return
     *  a painter device for simulations for this widget 
     */
    virtual GV_Painter* createSimPainter() = 0;
    /**
     * This virtual method must be overwritten and must destroy
     * the painter device and point 'painter' to NULL. If the painter 
     * is static during the whole life of the application, it might 
     * just do nothing. 
     */
    virtual void destroyPainter() = 0;

    GV_Painter* getPainter() {
        return painter;
    }

    /**
     * Sets the background color. Note that applying the background
     * color for the widget is up to the implementing class.
     */
    virtual void setBackground(const GV_Color& bg) {
        background = bg;

        // bright background:
        if (bg.red()+bg.green()+bg.blue()>380) {
            foreground = GV_Color(0,0,0);
        } else {
            foreground = GV_Color(255,255,255);
        }
    }
    
    /**
     * @return Current background color.
     */
    GV_Color getBackground() {
        return background;
    }
    
    /**
     * @return Current foreground color.
     */
    GV_Color getForeground() {
        return foreground;
    }

    /**
     * Sets the grid color.
     */
    void setGridColor(const GV_Color& c) {
        gridColor = c;
    }

    /**
     * Sets the meta grid color.
     */
    void setMetaGridColor(const GV_Color& c) {
        metaGridColor = c;
    }
    
    /**
     * Sets the selection color.
     */
    void setSelectedColor(const GV_Color& c) {
        selectedColor = c;
    }

    GV_Color getSelectedColor() {
        return selectedColor;
    }
    
    /**
     * Sets the highlight color.
     */
    void setHighlightedColor(const GV_Color& c) {
        highlightedColor = c;
    }

    /**
     * This virtual method can be overwritten to set the mouse
     * cursor to the given type.
     */
    virtual void setMouseCursor(GV2::CursorType /*c*/) {}

    void setContainer(GV_EntityContainer* container);
    GV_EntityContainer* getContainer() {
        return container;
    }
    bool setFactor(double f) {
        return (setFactorX(f) && setFactorY(f));
    }
    bool setFactorX(double f);
    bool setFactorY(double f);
    GV_Vector getFactor() {
        return factor;
    }
#ifdef GV_ARCH
#ifdef Q_OS_WIN32
    bool checkFactor(double f) {
        return (f>1.0e-2 && f<1.0e3);
    }
#else
    bool checkFactor(double f) {
        return (f>1.0e-2 && f<1.0e6);
    }
#endif
#else
    bool checkFactor(double /*f*/) {
        return true;
    }
#endif
    /**
     * Sets the offset of the graphic.
     */
    void setOffset(int ox, int oy) {
        setOffsetX(ox);
        setOffsetY(oy);
    }
    void setOffsetX(int ox) {
        offsetX = ox;
    }
    void setOffsetY(int oy) {
        offsetY = oy;
    }
    int getOffsetX() {
        return offsetX;
    }
    int getOffsetY() {
        return offsetY;
    }
    void centerOffsetX();
    void centerOffsetY();
    void centerX(double x);
    void centerY(double y);
    //virtual void updateView();
    /**
     * Sets a fixed border in pixel around the graphic. This border
     * specifies how far the user can scroll outside the graphic
     * area.
     */
    void setBorders(int left, int top, int right, int bottom) {
        borderLeft = left;
        borderTop = top;
        borderRight = right;
        borderBottom = bottom;
    }

    int getBorderLeft() {
        return borderLeft;
    }
    int getBorderTop() {
        return borderTop;
    }
    int getBorderRight() {
        return borderRight;
    }
    int getBorderBottom() {
        return borderBottom;
    }

    void disableUpdate() {
        updateEnabled--;
    }
    void enableUpdate() {
        updateEnabled++;
    }
    bool isUpdateEnabled() {
        return (updateEnabled==0);
    }

    void freezeZoom(bool freeze) {
        zoomFrozen=freeze;
    }
    bool isZoomFrozen() {
        return zoomFrozen;
    }

    void setDefaultAction(GV_ActionInterface* action);
    GV_ActionInterface*  getDefaultAction();
    void setCurrentAction(GV_ActionInterface* action);
    GV_ActionInterface* getCurrentAction();
    
    //void killSelectActions();
    void killAllActions();

    /**
     * Must be overwritten to emulate a mouse move event with
     * the last known mouse position.
     *
     * @see mx, my
     */
    virtual void emulateMouseMoveEvent() = 0;
    virtual void emulateMousePressEvent() = 0;
    virtual void emulateMouseReleaseEvent() = 0;

    void back();
    void enter();

//    void mousePressEvent(GV_MouseEvent* e);
//    void mouseReleaseEvent(GV_MouseEvent* e);
//    void mouseMoveEvent(GV_MouseEvent* e);
//    void mouseLeaveEvent();
//    void mouseEnterEvent();
//    void keyPressEvent(GV_KeyEvent* e);
//    void keyReleaseEvent(GV_KeyEvent* e);
//    void commandEvent(GV_CommandEvent* e);
//    void dragEnterEvent(QDragEnterEvent* e);
//    void dragMoveEvent(QDragMoveEvent* e);
//    void dropEvent(QDropEvent* e);

    void enableCoordinateInput();
    void disableCoordinateInput();

    virtual void zoomIn(double f=1.5, const GV_Vector& center=GV_Vector(false));
    virtual void zoomInX(double f=1.5);
    virtual void zoomInY(double f=1.5);
    virtual void zoomOut(double f=1.5, const GV_Vector& center=GV_Vector(false));
    virtual void zoomOutX(double f=1.5);
    virtual void zoomOutY(double f=1.5);
    virtual void zoomAuto(bool axis=true, bool keepAspectRatio=true);
    virtual void zoomSelection();
    virtual void zoomAutoY(bool axis=true);
    virtual void zoomPrevious();
    virtual void saveView();
    virtual void restoreView();
    virtual void zoomWindow(GV_Vector v1, GV_Vector v2,
                            bool keepAspectRatio=true);
    virtual void zoomPan(int dx, int dy);
    virtual void zoomScroll(GV2::Direction direction);
    virtual void zoomPage();

    virtual void drawIt();
    virtual void deleteEntity(GV_Entity* e);
    virtual bool isVisible(GV_Entity* e);
    virtual void drawEntity(GV_Entity* e, double patternOffset=0.0, bool db=false);
    virtual void drawEntityHandles(GV_Entity* e);
    virtual void drawEntityPlain(GV_Entity* e, double patternOffset=0.0);
    virtual void setPenForEntity(GV_Entity* e);
    
    void simulateIt();
    void simulateEntity(GV_Entity* e, const GV_Pen& pen);
    void drawLineSmooth(const GV_Vector& p1, const GV_Vector& p2, const GV_Pen& pen);
    void drawArcSmooth(const GV_Vector& center, double radius,
            double a1, double a2, bool rev, const GV_Pen& pen);
    void prepareSimulationPreview(const GV_Vector& pos);
    void simulationDelay(bool step=false);

    virtual GV_LineTypePattern* getPattern(GV2::LineType t);

    virtual void drawAbsoluteZero();
    virtual void drawRelativeZero();
    virtual void drawPaper();
    virtual void drawGrid();
    virtual void drawMetaGrid();
    virtual void updateGrid();
    GV_Grid* getGrid() {
        return grid;
    }
    virtual void updateGridStatusWidget(const QString& /*text*/) {}

    void setDefaultSnapMode(GV2::ActionType sm);
    GV2::ActionType getDefaultSnapMode() {
        return defaultSnapMode;
    }
    void setSnapRestriction(GV2::ActionType sr);
    GV2::ActionType getSnapRestriction() {
        //return defaultSnapRes;
        return GV2::ActionRestrictNothing;
    }

    bool isGridOn();

    /**
     * Translates a vector in real coordinates to a vector in screen coordinates.
     */
    inline GV_Vector toGui(const GV_Vector& v) {
        return GV_Vector(toGuiX(v.x), toGuiY(v.y), 0.0);
    }
    double toGuiX(double x, bool* visible=NULL);
    double toGuiY(double y);
    double toGuiDX(double d);
    double toGuiDY(double d);

    GV_Vector toGraph(const GV_Vector& v);
    GV_Vector toGraph(int x, int y);
    double toGraphX(int x);
    double toGraphY(int y);
    double toGraphDX(int d);
    double toGraphDY(int d);

    /**
     * (Un-)Locks the position of the relative zero.
     *
     * @param lock true: lock, false: unlock
     */
    void lockRelativeZero(bool lock) {
        relativeZeroLocked=lock;
    }

    /**
     * @return true if the position of the realtive zero point is
     * locked.
     */
    bool isRelativeZeroLocked() {
        return relativeZeroLocked;
    }

    /**
     * @return Relative zero coordinate.
     */
    GV_Vector getRelativeZero() {
        return relativeZero;
    }

    void setRelativeZero(const GV_Vector& pos);
    void moveRelativeZero(const GV_Vector& pos);

    GV_EventHandler* getEventHandler() {
        return eventHandler;
    }

    /**
     * Enables or disables print preview.
     */
    void setPrintPreview(bool pv) {
        printPreview = pv;
    }

    /**
     * @retval true This is a print preview graphic view.
     * @retval false Otherwise.
     */
    bool isPrintPreview() {
        return printPreview;
    }
    
    /**
     * Enables or disables printing.
     */
    void setPrinting(bool p) {
        printing = p;
    }

    /**
     * @retval true This is a a graphic view for printing.
     * @retval false Otherwise.
     */
    bool isPrinting() {
        return printing;
    }
    
    /**
     * @retval true Draft mode is on for this view (all lines with 1 pixel / no style scaling).
     * @retval false Otherwise.
     */
    bool isDraftMode() {
        return draftMode;
    }

    /** 
     * Sets the simulation speed in percentage.
     */
    void setSimulationSpeed(int s) {
        simulationSpeed = s;
    }
    /** 
     * @return the simulation speed in percentage.
     */
    int getSimulationSpeed() {
        return simulationSpeed;
    }
    /** 
     * Sets the simulation smooth mode.
     */
    void setSimulationSmooth(bool s) {
        simulationSmooth = s;
    }
    bool getSimulationSmooth() {
        return simulationSmooth;
    }
    /** 
     * Sets the simulation rapid mode.
     */
    void setSimulationRapid(bool r) {
        simulationRapid = r;
    }
    /** 
     * @return the simulation rapid mode.
     */
    bool getSimulationRapid() {
        return simulationRapid;
    }

    void cancelSimulation() {
        simulationRunning = false;
        redraw();
    }

//    GV_Preview* getPreview() {
//        return preview;
//    }

protected:
    GV_EntityContainer* container;
    GV_EventHandler* eventHandler;

    int mx;   //!< Last known mouse cursor position
    int my;   //!< Last known mouse cursor position

    GV_Painter* painter;
    /** background color (any color) */
    GV_Color background;
    /** foreground color (black or white) */
    GV_Color foreground;
    /** grid color */
    GV_Color gridColor;
    /** meta grid color */
    GV_Color metaGridColor;
    /** selected color */
    GV_Color selectedColor;
    /** highlighted color */
    GV_Color highlightedColor;
    /** Grid */
    GV_Grid* grid;
    /** 
     * Current default snap mode for this graphic view. Used for new
     * actions.
     */
    GV2::ActionType defaultSnapMode;
    /** 
     * Current default snap restriction for this graphic view. Used for new
     * actions.
     */
    GV2::ActionType defaultSnapRes;

    GV2::DrawingMode drawingMode;

    /**
     * Delete mode. If true, all drawing actions will delete in background color
     * instead.
     */
    bool deleteMode;
    //! If true, the simulation is currectly running
    bool simulationRunning;
//    GV_Preview* preview;
    
    int borderLeft;
    int borderTop;
    int borderRight;
    int borderBottom;


private:
    int updateEnabled;
    bool zoomFrozen;
    bool draftMode;

    GV_Vector factor;
    int offsetX;
    int offsetY;
    
    GV_Vector previousFactor;
    int previousOffsetX;
    int previousOffsetY;

    GV_Vector relativeZero;
    bool relativeZeroLocked;
    //! Print preview flag
    bool printPreview;
    //! Active when printing only:
    bool printing;

    //! Simulation speed in percentage
    int simulationSpeed;
     //! If true, the entity is drawn slowly (pixel by pixel).
    bool simulationSmooth;
     //! If true, the way between entities is also shown.
    bool simulationRapid;
    //! Last position (for rapid move)
    GV_Vector simulationLast;
};

#endif
