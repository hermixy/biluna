/*****************************************************************
 * $Id: rb_plotterwidget.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * This Plotter class is originally from the Qt3 book written by
 * - Jasmin Blanchett
 * - Mark Summerfield
 * 
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include <QtWidgets>
#include <QtSvg>

#include "rb_plotterwidget.h"

#include "rb_debug.h"


RB_PlotterWidget::RB_PlotterWidget(QWidget *parent, Qt::WindowFlags flags)
                    : QWidget(parent, flags) {
    setBackgroundRole(QPalette::Dark);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    rubberBandIsShown = false;

    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(QString::fromUtf8(":/images/zoomin.png")));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(QString::fromUtf8(":/images/zoomout.png")));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    setPlotSettings(RB_PlotSettings());
    
    title = "";
    unitX = "";
    unitY = "";
}

RB_PlotterWidget::~RB_PlotterWidget() {
	clearLines();
	clearTexts();
}

void RB_PlotterWidget::setPlotSettings(const RB_PlotSettings &settings) {
    zoomStack.resize(1);
    zoomStack[0] = settings;
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}

void RB_PlotterWidget::zoomOut() {
    if (curZoom > 0) {
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}

void RB_PlotterWidget::zoomIn() {
    if (curZoom < (int)zoomStack.size() - 1) {
        ++curZoom;
        zoomInButton->setEnabled(
                curZoom < (int)zoomStack.size() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}

void RB_PlotterWidget::setTitle(const QString& t) {
	// RB_DEBUG->print("RB_PlotterWidget::setTitle()");
	title = t;
	// refreshPixmap();
}

void RB_PlotterWidget::setUnitX(const QString& ux) {
	unitX = ux;
	// refreshPixmap();
}

void RB_PlotterWidget::setUnitY(const QString& uy) {
	unitY = uy;
	// refreshPixmap();
}

void RB_PlotterWidget::setLines(const LineData& ldata) {
	if (lines.size() > 0)
		clearLines();
	
	lines = ldata;
	// refreshPixmap();
}

void RB_PlotterWidget::setTexts(const TextData& tdata) {
	if (texts.size() > 0)
		clearTexts();
	
	texts = tdata;
	// refreshPixmap();
}

/**
 * Set curve data, also refreshes the pixmap
 */
void RB_PlotterWidget::setCurveData(int id, const CurveData &cdata) {
    curveMap[id] = cdata;
    refreshPixmap();
}

/**
 * Clear title, also refreshes the pixmap
 */
void RB_PlotterWidget::clearTitle() {
	title = "";
    refreshPixmap();
}
void RB_PlotterWidget::clearUnitX() {
	unitX = "";
}

void RB_PlotterWidget::clearUnitY() {
	unitY = "";
}

void RB_PlotterWidget::clearLines() {
	std::vector<RB_PlotLine*>::iterator iter;
	iter = lines.begin();
	while (iter != lines.end()) {	
		if (*iter != NULL) {
			delete *iter;
			*iter = NULL;
		}
		++iter;
	}
	
	lines.clear();
}

void RB_PlotterWidget::clearTexts() {
	std::vector<RB_PlotText*>::iterator iter;
	iter = texts.begin();
	while (iter != texts.end()) {	
		if (*iter != NULL) {
			delete *iter;
			*iter = NULL;
		}
		++iter;
	}
	
	texts.clear();
}  

void RB_PlotterWidget::clearCurve(int id) {
    curveMap.erase(id);
    refreshPixmap();
}

QSize RB_PlotterWidget::minimumSizeHint() const {
    return QSize(2 * (MarginLeft + MarginRight), 2 * (MarginTop + MarginBottom));
}

QSize RB_PlotterWidget::sizeHint() const {
    return QSize(8 * (MarginLeft + MarginRight), 6 * (MarginTop + MarginBottom));
}

void RB_PlotterWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    QVector<QRect> rects = event->region().rects();
    for (int i = 0; i < (int)rects.size(); ++i)
		painter.drawPixmap(rects[i], pixmap, rects[i]);	

    if (rubberBandIsShown) {
        painter.setPen(QColor(Qt::gray));

        // rect width and height -1 otherwise you do not see the bottom and right
        QRect rect = rubberBandRect.normalized();
        rect.setWidth(rect.width() - 1);
        rect.setHeight(rect.height() - 1);
        painter.drawRect(rect);
    }

    // TODO: this style part is updated but unclear what it does
    QStyleOptionFocusRect option;
 	option.initFrom(this);
    
    if (hasFocus()) {
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, 
        						&option,
        						&painter,
                              	this);
    }
}

/**
 * Translate original value to the plot coordinates,
 * example case log10 scale for both x-axis and y-axis 
 * TODO: implement
 */
double RB_PlotterWidget::toScale(double originalValue) {
	return originalValue;// log10(originalValue);
}

/**
 * Translate the plot value coordinate to the plot original value
 * TODO: implement
 */
double RB_PlotterWidget::fromScale(double scaledValue) {
	return scaledValue; // pow(10, scaledValue);
}

void RB_PlotterWidget::resizeEvent(QResizeEvent *) {
    int x = width() - (zoomInButton->width()
                       + zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();
}

void RB_PlotterWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        rubberBandIsShown = true;
        rubberBandRect.setTopLeft(event->pos());
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
        setCursor(Qt::CrossCursor);
    }
}

void RB_PlotterWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::LeftButton) {
        updateRubberBandRegion();
        rubberBandRect.setBottomRight(event->pos());
        updateRubberBandRegion();
    }
}

void RB_PlotterWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        rubberBandIsShown = false;
        updateRubberBandRegion();
        unsetCursor();

        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4 || rect.height() < 4)
            return;
        rect.translate(-MarginLeft, -MarginTop);

        RB_PlotSettings prevSettings = zoomStack[curZoom];
        RB_PlotSettings settings;
        double dx = prevSettings.spanX() / (width() - (MarginLeft + MarginRight));
        double dy = prevSettings.spanY() / (height() - (MarginTop + MarginBottom));
        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();

        zoomStack.resize(curZoom + 1);
        zoomStack.push_back(settings);
        zoomIn();
    }
}

void RB_PlotterWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void RB_PlotterWidget::wheelEvent(QWheelEvent *event) {
    int numDegrees = event->delta() / 8;
    int numTicks = numDegrees / 15;

    if (event->orientation() == Qt::Horizontal)
        zoomStack[curZoom].scroll(numTicks, 0);
    else
        zoomStack[curZoom].scroll(0, numTicks);
    refreshPixmap();
}

void RB_PlotterWidget::updateRubberBandRegion() {
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

/**
 * @returns SVG
 */
RB_String RB_PlotterWidget::getSvg() {
//    pixmap = QPixmap(size());
//    pixmap.fill(this, 0, 0);
//    QPainter painter(&pixmap); //, this);

    QSvgGenerator svgGen;
    svgGen.setSize(size());
    svgGen.setViewBox(QRect(0, 0, size().width(), size().height()));

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    svgGen.setOutputDevice(&buffer);
    QPainter painter; // &svgGen); //, this);

    // background: lightblue - white - light yellow
//    int viewHeight = size().height();
//    QLinearGradient linearGradient(0, 0, 0, viewHeight);
//    linearGradient.setColorAt(0.0, QColor(225, 255, 255));
//    linearGradient.setColorAt(0.8, QColor(255, 255, 255));
//    linearGradient.setColorAt(1.0, QColor(255, 255, 235));
//    QBrush background(linearGradient);
//    painter.fillRect(QRect(0,0,size().width(),size().height()), background);


    // content of plot
    painter.begin(&svgGen);
    // drawAxis(&painter);

    // border around overall image
    painter.setPen(QColor(Qt::white));
    painter.drawRect(QRect(0, 0, size().width() - 1, size().height() - 1));
    // drawGrid(&painter); // not used
    drawCurves(&painter);
    drawLinesTexts(&painter);
    drawAxis(&painter);
    painter.end();

    return RB_String(buffer.data().data());
}

/**
 * Render plot on image, uses image size
 * @param img image object
 */
void RB_PlotterWidget::renderImage(QImage* img) {
    if (!img) return;

    QSize imgSize = img->size();
    QPainter painter(img);

    // background: lightblue - white - light yellow
    int viewHeight = imgSize.height();
    QLinearGradient linearGradient(0, 0, 0, viewHeight);
    linearGradient.setColorAt(0.0, QColor(225, 255, 255));
    linearGradient.setColorAt(0.8, QColor(255, 255, 255));
    linearGradient.setColorAt(1.0, QColor(255, 255, 235));
    QBrush background(linearGradient);
    painter.fillRect(QRect(0,0,imgSize.width(),imgSize.height()), background);

    // border around overall image
     painter.setPen(QColor(Qt::gray));
     painter.drawRect(QRect(0, 0, imgSize.width() - 1, imgSize.height() - 1));

    // content of plot
    drawAxis(&painter, img);
    // drawGrid(&painter); // not used
    drawCurves(&painter, img);
    drawLinesTexts(&painter, img);
}

/**
 * actual painting of plotter widget
 */
void RB_PlotterWidget::refreshPixmap() {
    pixmap = QPixmap(size());
    pixmap.fill(this, 0, 0);

    QPainter painter(&pixmap); //, this);
    
	// background: lightblue - white - light yellow
 	int viewHeight = size().height();
    QLinearGradient linearGradient(0, 0, 0, viewHeight);
    linearGradient.setColorAt(0.0, QColor(225, 255, 255));
    linearGradient.setColorAt(0.8, QColor(255, 255, 255));
    linearGradient.setColorAt(1.0, QColor(255, 255, 235));
    QBrush background(linearGradient);
    painter.fillRect(QRect(0,0,size().width(),size().height()), background);

	// border around overall image, not used. In PedDialog situated in frame.
    // painter.setPen(QColor(Qt::gray));
    // painter.drawRect(QRect(0, 0, size().width() - 1, size().height() - 1));
    
    // content of plot
    drawAxis(&painter);
	// drawGrid(&painter); // not used
    drawCurves(&painter);
	drawLinesTexts(&painter);
    
    update();
}

void RB_PlotterWidget::drawAxis(QPainter *painter, QImage* img) {
    QRect rect;
    if (img) {
        QRect tmpRect(MarginLeft, MarginTop, img->width() - (MarginLeft + MarginRight),
                                    img->height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    } else {
        QRect tmpRect(MarginLeft, MarginTop, width() - (MarginLeft + MarginRight),
    								height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    }
    RB_PlotSettings settings = zoomStack[curZoom];
    
//    QPalette pal = QApplication::palette();
	QPen mColor = QColor(Qt::black);
	painter->setPen(mColor);

    for (int i = 0; i < settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1) / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(fromScale(label)));
    }
    
    for (int j = 0; j < settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - MarginLeft, y - 10,
                          MarginLeft - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(fromScale(label)));
    }
    
    // draw outline over plot area (limits between axes), instead of axes
    // painter->drawRect(rect);

    // draw axes x, y
    painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
    painter->drawLine(rect.left(), rect.bottom(), rect.left(), rect.top());
    
    // axes units
    painter->drawText(5, 5, MarginRight*3, MarginTop - 5, 
    					Qt::AlignLeft | Qt::AlignTop, unitX);
    painter->drawText(width() - MarginRight*5, height() - MarginBottom, 
    					MarginRight*5 - 5, MarginBottom - 5, 
    					Qt::AlignRight | Qt::AlignBottom, unitY);
    
    // plot title
    painter->drawText(0, 5, width(), MarginTop - 5, 
    					Qt::AlignHCenter | Qt::AlignTop, title);
    
}

void RB_PlotterWidget::drawGrid(QPainter *painter, QImage* img) {
    QRect rect;
    if (img) {
        QRect tmpRect(MarginLeft, MarginTop, img->width() - (MarginLeft + MarginRight),
                                    img->height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    } else {
        QRect tmpRect(MarginLeft, MarginTop, width() - (MarginLeft + MarginRight),
                                    height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    }
    RB_PlotSettings settings = zoomStack[curZoom];
    
//    QPalette pal = QApplication::palette();
	QPen mColor = QColor(Qt::darkGray);
	painter->setPen(mColor);

    for (int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1) / settings.numXTicks);
        painter->setPen(mColor);
        painter->drawLine(x, rect.top(), x, rect.bottom());
    }
    
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.numYTicks);
        painter->setPen(mColor);
        painter->drawLine(rect.left(), y, rect.right(), y);
    }
}


/**
 * First two blue lines are used for the PED table
 */
void RB_PlotterWidget::drawCurves(QPainter *painter, QImage* img) {
    static const QColor colorForIds[6] = {
        Qt::blue, Qt::blue, Qt::red, Qt::green, Qt::cyan, Qt::magenta
    };
    QRect rect;
    if (img) {
        QRect tmpRect(MarginLeft, MarginTop, img->width() - (MarginLeft + MarginRight),
                                    img->height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    } else {
        QRect tmpRect(MarginLeft, MarginTop, width() - (MarginLeft + MarginRight),
                                    height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    }

    RB_PlotSettings settings = zoomStack[curZoom];

    painter->setClipRect(rect.x() + 1, rect.y() + 1,
                         rect.width() - 2, rect.height() - 2);

    map<int, CurveData>::const_iterator it = curveMap.begin();
    while (it != curveMap.end()) {
        int id = (*it).first;
        const CurveData &data = (*it).second;
        int numPoints = 0;
        int maxPoints = data.size() / 2;
        QPolygon points(maxPoints);

        for (int i = 0; i < maxPoints; ++i) {
            double dx = toScale(data[2 * i]) - settings.minX;
            double dy = toScale(data[2 * i + 1]) - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)
                                         / settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                           / settings.spanY());
            if (fabs(x) < 32768 && fabs(y) < 32768) {
                points.setPoint(numPoints, QPoint((int)x, (int)y));
                ++numPoints;
            }
        }
        
        points.resize(numPoints);
        
        painter->setPen(colorForIds[(uint)id % 6]);
        painter->drawPolyline(points);
        ++it;
    }
}

void RB_PlotterWidget::drawLinesTexts(QPainter *painter, QImage* img) {
    QRect rect;
    if (img) {
        QRect tmpRect(MarginLeft, MarginTop, img->width() - (MarginLeft + MarginRight),
                                    img->height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    } else {
        QRect tmpRect(MarginLeft, MarginTop, width() - (MarginLeft + MarginRight),
                                    height() - (MarginTop + MarginBottom));
        rect.setRect(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height());
    }
    RB_PlotSettings settings = zoomStack[curZoom];

    painter->setClipRect(rect.x() + 1, rect.y() + 1,
                         rect.width() - 2, rect.height() - 2);
	QPen stdPen = QColor(Qt::darkGray);
	painter->setPen(stdPen);
	
	// draw lines
	for (int i = 0; i < (int)lines.size(); ++i) {
        double dx1 = lines[i]->x1 - settings.minX;
        double dy1 = lines[i]->y1 - settings.minY;
        double x1 = rect.left() + (dx1 * (rect.width() - 1)
                                     / settings.spanX());
        double y1 = rect.bottom() - (dy1 * (rect.height() - 1)
                                       / settings.spanY());
        double dx2 = lines[i]->x2 - settings.minX;
        double dy2 = lines[i]->y2 - settings.minY;
        double x2 = rect.left() + (dx2 * (rect.width() - 1)
                                     / settings.spanX());
        double y2 = rect.bottom() - (dy2 * (rect.height() - 1)
                                       / settings.spanY());
        
        if (fabs(x1) < 32768 && fabs(y1) < 32768 && fabs(x2) < 32768 
        											&& fabs(y2) < 32768) {
			painter->setPen(lines[i]->color);
			painter->drawLine(QPoint((int)x1, (int)y1), QPoint((int)x2, (int)y2));
        }
	}	
	painter->setPen(QPen());
	painter->setFont(QFont());
	
	// draw texts, left bottom corner of text is positioned to scale
	for (int i = 0; i < (int)texts.size(); ++i) {
        double dx1 = texts[i]->x1 - settings.minX;
        double dy1 = texts[i]->y1 - settings.minY;
        double x1 = rect.left() + (dx1 * (rect.width() - 1)
                                     / settings.spanX());
        double y1 = rect.bottom() - (dy1 * (rect.height() - 1)
                                       / settings.spanY());
        
        if (fabs(x1) < 32768 && fabs(y1) < 32768) {
			painter->save();
			painter->setPen(texts[i]->color);
			painter->setFont(texts[i]->font);
			painter->rotate(texts[i]->angle);
			if (texts[i]->angle > 0.1 || texts[i]->angle < -0.1) {
				double phi = texts[i]->angle/180 * 3.1415926536;

				int tmpX = (int)(x1* cos(phi) + y1 * sin(phi));
				int tmpY = (int)(- x1 * sin(phi) + y1 * cos(phi));

				painter->drawText(QPoint(tmpX, tmpY), texts[i]->text);
			} else {
				painter->drawText(QPoint((int)x1, (int)y1), texts[i]->text);
			}
			painter->restore();
        }
	}	
}


RB_PlotSettings::RB_PlotSettings() {
    // initial plot coordinates
    minX = -1; 		// 0.0;
    maxX = 5; 		// 10.0;
    numXTicks = 6;	// 5;

    minY = -1; 		// 0.0;
    maxY = 5; 		// 10.0;
    numYTicks = 6;	// 5;
}

void RB_PlotSettings::scroll(int dx, int dy) {
    double stepX = spanX() / numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;

    double stepY = spanY() / numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

void RB_PlotSettings::adjust() {
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

void RB_PlotSettings::adjustAxis(double &min, double &max, int &numTicks) {
    const int MinTicks = 4;
    double grossStep = (max - min) / MinTicks;
    double step = pow(10.0, floor(log10(grossStep)));

    if (5 * step < grossStep)
        step *= 5;
    else if (2 * step < grossStep)
        step *= 2;

    numTicks = (int)(ceil(max / step) - floor(min / step));
    min = floor(min / step) * step;
    max = ceil(max / step) * step;
}

