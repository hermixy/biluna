/*****************************************************************
 * $Id: rb_plotterwidget.h 2248 2015-06-21 09:13:00Z rutger $
 *
 * This Plotter and PlotterSetting class is originally from the 
 * Qt3 book written by:
 * - Jasmin Blanchett
 * - Mark Summerfield
 * 
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PLOTTERWIDGET_H
#define RB_PLOTTERWIDGET_H

#include <QtWidgets>

#include <cmath>
#include <map>
#include <vector>

#include "db_global.h"
#include "rb_string.h"

using namespace std;

class QToolButton;
class RB_PlotSettings; // see further down
class RB_PlotLine; // see further down
class RB_PlotText; // see further down

typedef std::vector<RB_PlotLine*> LineData;
typedef std::vector<RB_PlotText*> TextData;
typedef std::vector<double> CurveData;


/**
 * Basic widget for plotting 2D axis with plotted lines, text and curves,
 * with zoom facility
 */
class DB_EXPORT RB_PlotterWidget : public QWidget {
	
    Q_OBJECT
    
public:
    RB_PlotterWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~RB_PlotterWidget();

    void setPlotSettings(const RB_PlotSettings &settings);
    
    void clearTitle();
	void clearUnitX();
	void clearUnitY();
	void clearLines();
	void clearTexts();   
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    
    void setTitle(const QString& t);
	void setUnitX(const QString& ux);
	void setUnitY(const QString& uy);
	void setLines(const LineData& ldata);
	void setTexts(const TextData& tdata);   
    void setCurveData(int id, const CurveData &cdata);
    virtual double toScale(double originalValue);
    virtual double fromScale(double scaledValue);

    virtual RB_String getSvg();
    virtual void renderImage(QImage* img);

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap();
    void drawAxis(QPainter* painter, QImage* img = 0);
    void drawGrid(QPainter* painter, QImage* img = 0);
    void drawCurves(QPainter* painter, QImage* img = 0);
    void drawLinesTexts(QPainter* painter, QImage* img = 0);

    enum { 
    	MarginLeft = 60, 
    	MarginRight = 40, 
    	MarginTop = 30,
    	MarginBottom = 50 
    };

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    std::map<int, CurveData> curveMap;
    
    // containers for text and lines
	std::vector<RB_PlotLine*> lines;
	std::vector<RB_PlotText*> texts;
    RB_String title;
    RB_String unitX;
    RB_String unitY;
    
    std::vector<RB_PlotSettings> zoomStack;
    int curZoom;

    bool rubberBandIsShown;
    QRect rubberBandRect;
	
    QPixmap pixmap;
};


/**
 * Basic plotter settings to calculate the results of the selected zoom area
 */
class DB_EXPORT RB_PlotSettings {
	
public:
    RB_PlotSettings();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    void adjustAxis(double& min, double& max, int& numTicks);
};


/**
 * Plot line data object
 */
class DB_EXPORT RB_PlotLine {

public:
    RB_PlotLine(double _x1, double _y1, double _x2, double _y2,
                                const QColor& _color = QColor(Qt::darkGray)) {
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
        color = _color;
	}
	
	double x1; 
	double y1; 
	double x2; 
	double y2;
	QColor color;
};

/**
 * Plot text data object
 */
class DB_EXPORT RB_PlotText {

public:
    RB_PlotText(double _x1, double _y1, const QString& _text,
                                const QColor& _color = QColor(Qt::darkGray),
                                QFont _font = QFont(),
                                double _angle = 0.0) {
        x1 = _x1;
        y1 = _y1;
        text = _text;
        color = _color;
        font = _font;
        angle = _angle;
	}
	
	double x1;
	double y1;
	RB_String text;
	QColor color;
	QFont font;
	double angle;
	
};
 
#endif
