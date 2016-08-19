/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
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
** not clear to you.
**
**********************************************************************/

#include "qg_ruler.h"

#include <QPainter>

#include "rs_graphic.h"
#include "rs_grid.h"
#include "rs_settings.h"
#include "rs_vector.h"

/**
 * Constructor
 */
QG_Ruler::QG_Ruler(bool horizontal, QWidget* parent)
        : QFrame(parent) {
    
    //setFrameStyle(QFrame::Panel|QFrame::Raised);
    //setLineWidth(1);

    graphicView = NULL;
    this->horizontal = horizontal;
    /*
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();
    */

    if (horizontal) {
        setMinimumHeight(20);
    }
    else {
        setMinimumWidth(20);
    }
    
    QPalette p = palette();
    QColor bg = p.color(QPalette::Window);
    p.setColor(QPalette::Window, bg.darker(120));
    setPalette(p);
    
    setAutoFillBackground(true);

}


/**
 * Destructor
 */
QG_Ruler::~QG_Ruler() {}


void QG_Ruler::setGraphicView(RS_GraphicView* graphicView) {
    this->graphicView = graphicView;
}

    
    
void QG_Ruler::paintEvent(QPaintEvent* e) {
    QFrame::paintEvent(e);
                
    if (graphicView==NULL) {
        return;
    }

    if (horizontal) {
        RS_DEBUG->print("QG_Ruler::paintEvent: horizontal");
        RS_DEBUG->timestamp("QG_Ruler::paintEvent");
    }
    else {
        RS_DEBUG->print("QG_Ruler::paintEvent: vertical");
        RS_DEBUG->timestamp("QG_Ruler::paintEvent");
    }

    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Helvetica", 8));

    RS_Grid* grid = graphicView->getGrid();
    double spacing = grid->getSpacing();
    if (spacing<1.0e-6) {
        spacing = 1.0e-6;
    }
    bool isMetric = true;

    if (graphicView->getGraphic()!=NULL && 
        !RS_Units::isMetric(graphicView->getGraphic()->getUnit()) &&
        graphicView->getGraphic()->getLinearFormat()!=RS2::Decimal && 
        graphicView->getGraphic()->getLinearFormat()!=RS2::Engineering) {

        isMetric = false;
    }
    
    RS_DEBUG->print("QG_Ruler::paintEvent: isMetric: %d", (int)isMetric);
    
    if (graphicView->toGuiDX(spacing)>=80.0) {
        spacing/=10.0;
    }
    else if (graphicView->toGuiDX(spacing)>=30.0) {
        spacing/=5.0;
    }
    else if (graphicView->toGuiDX(spacing)>=20.0) {
        spacing/=2.0;
    }
    
    while (graphicView->toGuiDX(spacing)<10.0) {
        spacing*=10.0;
    }

    int pixelSpacing = (int)ceil(graphicView->toGuiDX(spacing));
    
    RS_DEBUG->print("QG_Ruler::paintEvent: spacing: %f", spacing);
    RS_DEBUG->print("QG_Ruler::paintEvent: pixelSpacing: %d", pixelSpacing);
        
    // precision of labels
    int prec = 1;
    if (spacing<0.0002) {
        prec = 5;
    }
    else if (spacing<0.002) {
        prec = 4;
    }
    else if (spacing<0.02) {
        prec = 3;
    }
    else if (spacing<0.2) {
        prec = 2;
    }
    
    RS_DEBUG->print("QG_Ruler::paintEvent: prec: %d", prec);
        
    // measure maximum width that is required by the labels:
    QString testLabel1 = QString("%1").arg(grid->getMetaXMin(), 0, 'f', prec);
    QString testLabel2 = QString("%1").arg(grid->getMetaXMax(), 0, 'f', prec);
    int labelWidth = std::max(
        painter.fontMetrics().boundingRect(testLabel1).width(),
        painter.fontMetrics().boundingRect(testLabel2).width()) + 10;

    // show every 'labelStep'th label only:
    int minLabelStepX = labelWidth/pixelSpacing + 1;
    int labelStepX = 1;
    int labelStepY = 1;

    // decide how many ticks we need between labels for non-metric systems:
    if (!isMetric) {
        labelStepX = grid->getMetaFactorX();
        RS_DEBUG->print("QG_Ruler::paintEvent: labelStepX: %d", (int)labelStepX);
        if (labelStepX==1) {
            labelStepX = (int)(1.0/spacing);
            if (labelStepX>=8 && minLabelStepX<=4) {
                labelStepX = 4;
            }
            else if (labelStepX>=16 && minLabelStepX<=8) {
                labelStepX = 8;
            }
        }
        else {
            if (labelStepX>6 && minLabelStepX<=6) {
                labelStepX=6;
            }
        }

        /*
        labelStepY = grid->getMetaFactorY();
        if (labelStepY==1) {
            labelStepY = (int)(1.0/spacing);
            if (labelStepY>=8) {
                labelStepY = 4;
            }
        }
        else {
            if (labelStepY>6) {
                labelStepY=6;
            }
        }
        */
        
        /*
        while (labelStepX>0.0 && labelStepX<minLabelStepX) {
            labelStepX*=2;
        }
        */
        labelStepY = labelStepX;
    }

    // decide how many ticks we need between labels for metric systems:
    else {
        labelStepX = minLabelStepX;
        if (labelStepX>=3 && labelStepX<=4) {
            labelStepX = 5;
        }
        else if (labelStepX>=6 && labelStepX<=9) {
            labelStepX = 10;
        }
        else if (labelStepX>=11 && labelStepX<=19) {
            labelStepX = 20;
        }
        else if (labelStepX>=21 && labelStepX<=99) {
            labelStepX = 100;
        }
        labelStepY = labelStepX;
    }

    if (labelStepX==0) {
        labelStepX = 1;
    }
    if (labelStepY==0) {
        labelStepY = 1;
    }
    
    RS_DEBUG->print("QG_Ruler::paintEvent: labelStep: %d/%d", labelStepX, labelStepY);

    if (horizontal) {
        int c = 0;
        double mx = floor(graphicView->toGraphX(0) / (labelStepX*spacing))
                      * (labelStepX*spacing);
        double maxMx = ceil(graphicView->toGraphX(graphicView->getWidth()) / (labelStepX*spacing))
                      * (labelStepX*spacing);

        RS_DEBUG->print("QG_Ruler::paintEvent: from %f to %f step %f", mx, maxMx, spacing);
        RS_DEBUG->timestamp("QG_Ruler::paintEvent");
        
        while (mx < maxMx) {
            paintHorizontalLabel(painter, mx, c%labelStepX==0, labelWidth, prec);
            mx += spacing;
            c++;
        }
        RS_DEBUG->timestamp("QG_Ruler::paintEvent");
        
        /*
        c = 0;
        mx = 0.0;
        while (mx > grid->getMetaXMin()) {
            paintHorizontalLabel(painter, mx, c%labelStepX==0, labelWidth, prec);
            mx -= spacing;
            c++;
        }
        */
    }
    else {
        int c = 0;
        double my = floor(graphicView->toGraphY(graphicView->getHeight()) / (labelStepY*spacing))
                      * (labelStepY*spacing);
        double maxMy = ceil(graphicView->toGraphY(0) / (labelStepY*spacing))
                      * (labelStepY*spacing);
        while (my < maxMy) {
            paintVerticalLabel(painter, my, c%labelStepY==0, labelWidth, prec);
            my += spacing;
            c++;
        }
        
        /*
        c = 0;
        my = 0.0;
        while (my > grid->getMetaYMin()) {
            paintVerticalLabel(painter, my, c%labelStepY==0, labelWidth, prec);
            my -= spacing;
            c++;
        }
        */
    }
    
    RS_DEBUG->print("QG_Ruler::paintEvent: OK");
    RS_DEBUG->timestamp("QG_Ruler::paintEvent");
}
            
            
void QG_Ruler::paintHorizontalLabel(QPainter& painter, double mx, 
    bool hasLabel, int labelWidth, int prec) {

    int x = RS_Math::mround(graphicView->toGuiX(mx)) - pos().x();

    /*
    if (x<-labelWidth || x>graphicView->getWidth()+labelWidth) {
        return;
    }
    */

    int lineLength;
    painter.setPen(Qt::black);
    if (hasLabel) {
        painter.drawText(
            QRect(x-labelWidth/2, 2, labelWidth, 12), 
            Qt::AlignCenter, 
            RS_Math::doubleToString(mx, prec)
            /*QString("%1").arg(mx, 0, 'f', prec)*/);

        lineLength = 7;
    }
    else {
        lineLength = 3;
    }

    painter.drawLine(x, height()-lineLength, x, height());
    painter.setPen(Qt::white);
    painter.drawLine(x+1, height()-lineLength, x+1, height());
}



void QG_Ruler::paintVerticalLabel(QPainter& painter, double my, 
    bool hasLabel, int labelWidth, int prec) {

    int y = RS_Math::mround(graphicView->toGuiY(my)) - pos().y();
    int lineLength;
    painter.setPen(Qt::black);
    if (hasLabel) {
        painter.save();
        painter.rotate(-90.0);
        painter.translate(-y, 8);
        painter.drawText(
            QRect(-labelWidth/2, -6, labelWidth, 12), 
            Qt::AlignCenter, 
            RS_Math::doubleToString(my, prec)
            /*QString("%1").arg(my, 0, 'f', prec)*/);
        painter.restore();
        lineLength = 7;
    }
    else {
        lineLength = 2;
    }

    painter.drawLine(width()-lineLength, y, width(), y);
    painter.setPen(Qt::white);
    painter.drawLine(width()-lineLength, y+1, width(), y+1);
}
