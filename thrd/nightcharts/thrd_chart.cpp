/*
 * NightCharts
 * Copyright (C) 2010 by Alexander A. Avdonin, Artem N. Ivanov / ITGears Co.
 * In 2011 Completely rewritten by Rutger Botermans
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact gordos.kund@gmail.com with any questions on this license.
 */
#include "thrd_chart.h"

#include <math.h>



THRD_Chart::THRD_Chart() {
    mFont.setFamily("MS Shell Dlg 2");
    mFont.setPointSize(8);
    mChartType = THRD_Chart::Pie3D;
    mLegendType = THRD_Chart::Vertical;
    mCX = 0;
    mCY = 0;
    mCW = 100;
    mCH = 100;
    mLX = mCX+mCW+20;
    mLY = mCY;
    mIsShadow = true;
    mIsValueInChart = true;
    mStartAngle = 0.0;

    mLengthY = 100.0;
    mNoTicks = 0;
    mStep = 0.0;

    mHasPositiveNumber = false;
    mHasNegativeNumber = false;
    mLabelBefore = "";
    mLabelAfter = "%";
    mBarDist = 15;
    mStackCount = 0;
}

THRD_Chart::~THRD_Chart() {
    for (int i = 0; i < mChartItemList.count(); ++i) {
        delete mChartItemList[i];
    }
    mChartItemList.clear();
    for (int i = 0; i < mStackList.count(); ++i) {
        delete mStackList[i];
    }
    mStackList.clear();
}

void THRD_Chart::addChartItem(const QString& name, const QColor& color,
                              double value) {
    THRD_ChartItem* item = new THRD_ChartItem();
    item->setName(name);
    item->setColor(color);
    item->setValue(value);
    mChartItemList.append(item);

    if (value > 0) {
        mHasPositiveNumber = true;
    } else if (value < 0) {
        mHasNegativeNumber = true;
    }
}

void THRD_Chart::addStackName(const QString& name) {
    THRD_ChartStack* stack = new THRD_ChartStack();
    stack->setName(name);
    stack->setHeight(0.0); // not realy necessary
    mStackList.append(stack);

    mStackCount = mStackList.count();
}

void THRD_Chart::setChartCoords(double x, double y, double w, double h) {
    mCX = x;
    mCY = y;
    mCW = w;
    mCH = h;
}

void THRD_Chart::setLegendCoords(double x, double y) {
    mLX = x;
    mLY = y;
}

void THRD_Chart::setChartType(THRD_Chart::ChartType t) {
    mChartType = t;
}

void THRD_Chart::setLegendType(THRD_Chart::LegendType t) {
    mLegendType = t;
}

void THRD_Chart::setFont(QFont f) {
    mFont = f;
}

void THRD_Chart::setPieStartAngle(double angle) {
    mStartAngle = angle;
}

void THRD_Chart::setShadow(bool ok) {
    mIsShadow = ok;
}

void THRD_Chart::setValueInChart(bool ok) {
    mIsValueInChart = ok;
}

void THRD_Chart::setLabelBefore(const QString& before) {
    mLabelBefore = before;
}

void THRD_Chart::setLabelAfter(const QString& after) {
    mLabelAfter = after;
}

void THRD_Chart::setLengthYaxis(double length) {
    mLengthY = length;
}

void THRD_Chart::draw(QPainter* painter) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setFont(mFont);

    if (mChartType == THRD_Chart::Bar) {
        if (mHasPositiveNumber && mHasNegativeNumber) {
            mCH /= 2;
        } else if (!mHasPositiveNumber && mHasNegativeNumber) {
            mCY -= mCH;
        }
    }

    switch (mChartType) {
        case THRD_Chart::Pie2D : {

            mPW = 0;
            double itemAngle = 0;
            double angle = mStartAngle;

            //Options
            QLinearGradient gradient(mCX+0.5*mCW,mCY,mCX+0.5*mCW,mCY+mCH*2.5);
            gradient.setColorAt(1,Qt::black);

            if (mIsShadow) {
                double sumangle = 0;

                for (int i=0; i < mChartItemList.size(); ++i) {
                  sumangle += 3.6*mChartItemList[i]->getValue();
                }

                painter->setBrush(Qt::darkGray);
                painter->drawPie(mCX,mCY+mPW+5,mCW,mCH,angle*16,sumangle*16);
            }

            QPen pen;
            pen.setWidth(2);

            for (int i = 0; i < mChartItemList.size(); ++i)       {
                gradient.setColorAt(0,mChartItemList[i]->getColor());
                painter->setBrush(gradient);
                pen.setColor(mChartItemList[i]->getColor());
                painter->setPen(pen);
                itemAngle = 3.6*mChartItemList[i]->getValue();
                painter->drawPie(mCX,mCY,mCW,mCH,angle*16,itemAngle*16);
                angle += itemAngle;
            }

            break;
        }
        case THRD_Chart::Pie3D : {

            mPW = 50;
            double itemAngle = 0;
            double angle = mStartAngle;
            QPointF p;

            QLinearGradient gradient(mCX-0.5*mCW,mCY+mCH/2,mCX+1.5*mCW,mCY+mCH/2);
            gradient.setColorAt(0,Qt::black);
            gradient.setColorAt(1,Qt::white);
            QLinearGradient gradient_side(mCX,mCY+mCH,mCX+mCW,mCY+mCH);
            gradient_side.setColorAt(0,Qt::black);

            double sumangle = 0;
            for (int i = 0; i < mChartItemList.size(); ++i) {
                sumangle += 3.6*mChartItemList[i]->getValue();
            }

            if (mIsShadow) {
                painter->setBrush(Qt::darkGray);
                painter->drawPie(mCX,mCY+mPW+5,mCW,mCH,angle*16,sumangle*16);
            }

            int q = getQuarter(angle+sumangle);

            if (q == 2 || q == 3) {
                QPointF p = getPoint(angle+sumangle);
                QPointF points[4] =
                {
                    QPointF(p.x(),p.y()),
                    QPointF(p.x(),p.y()+mPW),
                    QPointF(mCX+mCW/2,mCY+mCH/2+mPW),
                    QPointF(mCX+mCW/2,mCY+mCH/2)
                };

                gradient_side.setColorAt(1,mChartItemList[mChartItemList.size()-1]->getColor());
                painter->setBrush(gradient_side);
                painter->drawPolygon(points,4);
            }

            p = getPoint(angle);
            q = getQuarter(angle);

            if (q == 1 || q == 4) {
                QPointF points[4] =
                {
                    QPointF(p.x(),p.y()),
                    QPointF(p.x(),p.y()+mPW),
                    QPointF(mCX+mCW/2,mCY+mCH/2+mPW),
                    QPointF(mCX+mCW/2,mCY+mCH/2)
                };

                gradient_side.setColorAt(1,mChartItemList[0]->getColor());
                painter->setBrush(gradient_side);
                painter->drawPolygon(points,4);
            }

            for (int i=0;i<mChartItemList.size();i++) {
                gradient.setColorAt(0.5, mChartItemList[i]->getColor());
                painter->setBrush(gradient);
                itemAngle = 3.6*mChartItemList[i]->getValue();
                painter->drawPie(mCX,mCY,mCW,mCH,angle*16,itemAngle*16);

                double a_ = getAngle360(angle);
                int q_ = getQuarter(angle);

                angle += itemAngle;

                double a = getAngle360(angle);
                int q = getQuarter(angle);

                QPainterPath path;
                p = getPoint(angle);

                if((q == 3 || q == 4) && (q_ == 3 || q_ == 4)) {
                    if (a>a_) {
                        // 1)
                        QPointF p_old = getPoint(angle-itemAngle);
                        path.moveTo(p_old.x()-1,p_old.y());
                        path.arcTo(mCX,mCY,mCW,mCH,angle-itemAngle,itemAngle);
                        path.lineTo(p.x(),p.y()+mPW);
                        path.arcTo(mCX,mCY+mPW,mCW,mCH,angle,-itemAngle);
                    } else {
                        // 2)
                        path.moveTo(mCX,mCY+mCH/2);
                        path.arcTo(mCX,mCY,mCW,mCH,180,getAngle360(angle)-180);
                        path.lineTo(p.x(),p.y()+mPW);
                        path.arcTo(mCX,mCY+mPW,mCW,mCH,getAngle360(angle),-getAngle360(angle)+180);
                        path.lineTo(mCX,mCY+mCH/2);

                        path.moveTo(p.x(),p.y());
                        path.arcTo(mCX,mCY,mCW,mCH,angle-itemAngle,360-getAngle360(angle-itemAngle));
                        path.lineTo(mCX+mCW,mCY+mCH/2+mPW);
                        path.arcTo(mCX,mCY+mPW,mCW,mCH,0,-360+getAngle360(angle-itemAngle));

                    }
                } else if((q == 3 || q == 4) && (q_ == 1 || q_ == 2) && a>a_ ) {
                    // 3)
                    path.moveTo(mCX,mCY+mCH/2);
                    path.arcTo(mCX,mCY,mCW,mCH,180,getAngle360(angle)-180);
                    path.lineTo(p.x(),p.y()+mPW);
                    path.arcTo(mCX,mCY+mPW,mCW,mCH,getAngle360(angle),-getAngle360(angle)+180);
                    path.lineTo(mCX,mCY+mCH/2);
                } else if((q == 1 || q == 2) && (q_ == 3 || q_ == 4) && a<a_) {
                    // 4)
                    p = getPoint(angle-itemAngle);
                    path.moveTo(p.x(),p.y());
                    path.arcTo(mCX,mCY,mCW,mCH,angle-itemAngle,360-getAngle360(angle-itemAngle));
                    path.lineTo(mCX+mCW,mCY+mCH/2+mPW);
                    path.arcTo(mCX,mCY+mPW,mCW,mCH,0,-360+getAngle360(angle-itemAngle));
                } else if((q ==1 || q==2) && (q_==1 || q_==2) && a<a_) {
                    // 5)
                    path.moveTo(mCX,mCY+mCH/2);
                    path.arcTo(mCX,mCY,mCW,mCH,180,180);
                    path.lineTo(mCX+mCW,mCY+mCH/2+mPW);
                    path.arcTo(mCX,mCY+mPW,mCW,mCH,0,-180);
                    path.lineTo(mCX,mCY+mCH/2);
                }

                if (!path.isEmpty()) {
                    gradient_side.setColorAt(1,mChartItemList[i]->getColor());
                    painter->setBrush(gradient_side);
                    painter->drawPath(path);
                }

            }
            break;
        }
        case THRD_Chart::Bar :
        case THRD_Chart::StackedBar : {

            if (mChartType == THRD_Chart::Bar) {
                mStackCount = 0;
            } else if (mChartType == THRD_Chart::StackedBar) {
                mStackCount = mStackList.count();
            }

            painter->save();
            painter->translate(mCX, mCY + mCH);
            painter->scale(1.0, -1.0);

            if (mStackCount > 0) {
                mPW = (mCW-(mStackCount)*mBarDist)/mStackCount;
            } else {
                mPW = (mCW-(mChartItemList.size())*mBarDist)/mChartItemList.size();
            }

            QLinearGradient gradient(0.0, 0.0, 0.0, mCW);
            gradient.setColorAt(1, Qt::black);

            QPen pen;
            int penWidth = 2;
            pen.setWidth(penWidth);

            QString label;
            QFontMetrics fontMetr = painter->fontMetrics();

            double val = 0.0;
            double stackHeight = 0.0;
            int stackNo = 0;

            double x = 0.0;
            double y = 0.0;
            double w = 0.0;
            double h = 0.0;

            int itemCount = mChartItemList.size();

            for (int i = 0; i < itemCount; ++i) {
                val = mChartItemList[i]->getValue();
                stackNo = i;

                // Set current stack (of bars) number
                if (mStackCount > 0 && stackNo >= mStackCount) {
                    stackNo = i % mStackCount;
                    stackHeight = mStackList[stackNo]->getHeight();
                }

                // Draw shadow per bar or stack only once
                if (mIsShadow && (mStackCount < 1 || itemCount - i <= mStackCount)) {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Qt::darkGray);

                    x = mBarDist / 2 + stackNo * (mPW + mBarDist);
                    y = penWidth / 2; // compensate for line thickness
                    w = mBarDist / 2; //mPW; only at the leftside
                    h = mCH / mLengthY * val - mBarDist / 2;

                    if (mStackCount > 0) {
                        h += mCH/mLengthY * stackHeight;
                    }

                    // Side shadow
                    if (h > 0 || h < -mBarDist/2) {
                        painter->drawRect(x, y, w, h);
                    }
                    // Bottom shadow only in case of negative values
                    if (h < -mBarDist/2) {
                        painter->drawRect(x+w-penWidth/2, h+penWidth/2,
                                          mPW-mBarDist/2, +mBarDist/2);
                    }
                }

                // Draw bar, gradient 10 times the bar distance otherwise black
                gradient.setStart(0.0, mCH / mLengthY * stackHeight); // color
                gradient.setFinalStop(0.0, mCH / mLengthY * (4 * val + stackHeight)); // black
                gradient.setColorAt(0, mChartItemList[i]->getColor());  // or stackNo instead of i
                painter->setBrush(gradient);
                pen.setColor(mChartItemList[i]->getColor()); // or stackNo instead of i
                painter->setPen(pen);

                x = mBarDist + stackNo * (mPW + mBarDist);
                y = mCH / mLengthY * stackHeight;
                if (mStackCount > 0 && i >= mStackCount) {
                    y += penWidth; // intermediate border compensation
                }
                w = mPW;
                h = mCH / mLengthY * val; // - 5;
                if (mStackCount > 0 && i >= mStackCount) {
                    h -= penWidth; // intermediate border compensation
                }
                painter->drawRect(x, y, w, h);


                // Draw value label
                if (mIsValueInChart && (mStackCount <= 0
                                        || ((mChartItemList.size() - i) <= mStackCount))) {
                    if (!mLabelBefore.isEmpty()) {
                        label = mLabelBefore + " ";
                    } else {
                        label = "";
                    }

                    if (mStackCount <= 0) {
                        // Item value
                        label += QString::number(mChartItemList[i]->getValue());
                    } else {
                        // Stack height
                        label += QString::number(mStackList[(i % mStackCount)]->getHeight()
                                                 + mChartItemList[i]->getValue());
                    }

                    label += mLabelAfter;
                    painter->setPen(Qt::SolidLine);

                    x = mBarDist + mPW / 2 + stackNo * (mPW + mBarDist)
                             - fontMetr.width(label) / 2;

                    if (val >= 0) {
                        // Positive value above the bar
                        y = mCH / mLengthY * (val + stackHeight) + fontMetr.height() / 2;
                    } else {
                        // Negative value below the bar
                        y = mCH / mLengthY * (val + stackHeight) - fontMetr.height() * 1.5;
                    }

                    // Painter scale in order not to write text upside down
                    painter->scale(1.0, -1.0);
                    painter->drawText(x, -y, label);
                    painter->scale(1.0, -1.0);
                }

                if (mStackCount > 0) {
                    mStackList[stackNo]->setHeight(
                            mStackList[stackNo]->getHeight() + val);
                }
            }

            painter->setPen(Qt::SolidLine);

            // Draw Y axis
            if (mHasPositiveNumber) {
                painter->drawLine(0.0, 0.0, 0.0, mCH);
                painter->drawLine(0.0, mCH, 4.0, mCH - 10.0);
                painter->drawLine(0.0, mCH, -4.0, mCH - 10.0);
            }

            if (mHasNegativeNumber) {
                painter->drawLine(0.0, 0.0, 0.0, -mCH);
                painter->drawLine(0.0, -mCH, 4.0, -mCH + 10.0);
                painter->drawLine(0.0, -mCH, -4.0, -mCH + 10.0);
            }

            // Draw X axis
            painter->drawLine(0.0, 0.0, mCW, 0.0);

            painter->restore();
            break;
        }
        case THRD_Chart::Line : {
//            if (mChartType == THRD_Chart::Bar) {
//                mStackCount = 0;
//            } else if (mChartType == THRD_Chart::StackedBar) {
//                mStackCount = mStackList.count();
//            }

            painter->save();
            painter->translate(mCX, mCY + mCH);
            painter->scale(1.0, -1.0);

            if (mStackCount > 0) {
                mPW = (mCW-(mStackCount)*mBarDist)/mStackCount;
            } else {
                mPW = (mCW-(mChartItemList.size())*mBarDist)/mChartItemList.size();
            }

            QLinearGradient gradient(0.0, 0.0, 0.0, mCW);
            gradient.setColorAt(1, Qt::black);

            QPen pen;
            int penWidth = 2;
            pen.setWidth(penWidth);

            QString label;
            QFontMetrics fontMetr = painter->fontMetrics();

            double val = 0.0;
            double val1 = 0.0;
            double stackHeight = 0.0;
            double stackHeight1 = 0.0;
            int stackNo = 0;

            double x = 0.0; // label
            double y = 0.0;
            double x0 = 0.0;
            double y0 = 0.0;
            double x1 = 0.0;
            double y1 = 0.0;

            QPointF pointList[4];

            int itemCount = mChartItemList.size();

            for (int i = 0; i < itemCount; ++i) {
                val = mChartItemList[i]->getValue();
                stackNo = i;

                // Set current stack (of bars) number
                if (mStackCount > 0 && stackNo >= mStackCount) {
                    stackNo = i % mStackCount;
                    stackHeight = mStackList[stackNo]->getHeight();
                }

                QColor color = mChartItemList[i]->getColor();
                pen.setColor(color);
                painter->setPen(pen);

                // Test lines
                if ((stackNo + 1 < mStackCount || mStackCount <= 1)
                        && i + 1 < itemCount) {
                    x0 = mBarDist/2 + (stackNo+0.5) * (mPW + mBarDist);
                    y0 = mCH / mLengthY * (stackHeight + val);
                    x1 = mBarDist/2 + (stackNo+1.5) * (mPW + mBarDist);
                    stackHeight1 = 0.0;

                    if (mStackCount > 1) {
                        stackHeight1 = mStackList[stackNo+1]->getHeight();
                    }

                    val1 = mChartItemList[i+1]->getValue();
                    y1 = mCH / mLengthY * (stackHeight1 + val1);

                    painter->drawLine(x0, y0, x1, y1);

                    // Draw shadow per bar or stack only once
                    if (mIsShadow) {
                        color.setAlpha(84);
                        painter->setBrush(QBrush(color));
                        painter->setPen(Qt::NoPen);

                        pointList[0] = QPointF(x0, mCH / mLengthY * stackHeight + penWidth/2);
                        pointList[1] = QPointF(x1, mCH / mLengthY * stackHeight1 + penWidth/2);
                        pointList[2] = QPointF(x1, y1 - penWidth/2);
                        pointList[3] = QPointF(x0, y0 - penWidth/2);

                        painter->drawPolygon(pointList, 4);
                    }
                }

                // Draw value label
                if (mIsValueInChart && (mStackCount <= 0
                                        || ((mChartItemList.size() - i) <= mStackCount))) {
                    if (!mLabelBefore.isEmpty()) {
                        label = mLabelBefore + " ";
                    } else {
                        label = "";
                    }

                    if (mStackCount <= 0) {
                        // Item value
                        label += QString::number(mChartItemList[i]->getValue(), 'g', 2);
                    } else {
                        // Stack height
                        label += QString::number(mStackList[(i % mStackCount)]->getHeight()
                                                 + mChartItemList[i]->getValue(), 'g', 2);
                    }

                    label += mLabelAfter;
                    painter->setPen(Qt::SolidLine);

                    x = mBarDist + mPW / 2 + stackNo * (mPW + mBarDist)
                             - fontMetr.width(label) / 2;

                    if (val >= 0) {
                        // Positive value above the bar
                        y = mCH / mLengthY * (val + stackHeight) + fontMetr.height() / 2;
                    } else {
                        // Negative value below the bar
                        y = mCH / mLengthY * (val + stackHeight) - fontMetr.height() * 1.5;
                    }

                    // Painter scale in order not to write text upside down
                    painter->scale(1.0, -1.0);
                    painter->drawText(x, -y, label);
                    painter->scale(1.0, -1.0);
                }

                if (mStackCount > 0) {
                    mStackList[stackNo]->setHeight(
                            mStackList[stackNo]->getHeight() + val);
                }
            }

            painter->setPen(Qt::SolidLine);

            // Draw Y axis
            if (mHasPositiveNumber) {
                painter->drawLine(0.0, 0.0, 0.0, mCH);
                painter->drawLine(0.0, mCH, 4.0, mCH - 10.0);
                painter->drawLine(0.0, mCH, -4.0, mCH - 10.0);
            }

            if (mHasNegativeNumber) {
                painter->drawLine(0.0, 0.0, 0.0, -mCH);
                painter->drawLine(0.0, -mCH, 4.0, -mCH + 10.0);
                painter->drawLine(0.0, -mCH, -4.0, -mCH + 10.0);
            }

            // Draw X axis
            painter->drawLine(0.0, 0.0, mCW, 0.0);

            painter->restore();
            break;
        }
    }
}

void THRD_Chart::drawLegend(QPainter* painter) {
    double angle = mStartAngle;
    painter->setPen(Qt::SolidLine);
    painter->setFont(mFont);
    QFontMetrics fontMetr = painter->fontMetrics();

    switch(mLegendType) {
    case THRD_Chart::AxesAngled :
    case THRD_Chart::AxesHoriz : {
        painter->save();
        painter->translate(0.0, mCH + mCY);
        QString label;

        // Draw label Y unit at tip of Y axis
        if (!mLabelBefore.isEmpty() || !mLabelAfter.isEmpty()) {
            label = mLabelBefore.isEmpty() ? "" : mLabelBefore + " ";
            label += mLabelAfter.isEmpty() ? "" : mLabelAfter + " ";
            int textWidth = fontMetr.width(label);
            int textHeight = fontMetr.height();
            int textX = mCX - 5 - textWidth;
            int textY = -mCH + textHeight / 4;

            if (mHasPositiveNumber) {
                painter->drawText(textX, textY, label);
            }

            if (mHasNegativeNumber) {
                textY = -mCH - textHeight / 4;
                painter->drawText(textX, -textY, label);
            }
        }

        // Calculation step and number of ticks on Y axis
        setYaxisDim();

        // Y axis lines and labels
        double stepY = mCH / mLengthY * mStep;

        for (int i = 1; i < mNoTicks; ++i) {
            label = QString::number(mStep * i);
            int textWidth = fontMetr.width(label);
            int textHeight = fontMetr.height();
            int lineX0 = mCX - 3;
            int lineY0 = -stepY * i;
            int lineX1 = mCX + 3;
            int lineY1 = -stepY * i;
            int textX = mCX - 5 - textWidth;
            int textY = -stepY * i + textHeight / 4;

            if (mHasPositiveNumber) {
                painter->drawLine(lineX0, lineY0, lineX1, lineY1);
                painter->drawText(textX, textY, label);
            }

            if (mHasNegativeNumber) {
                painter->drawLine(lineX0, -lineY0, lineX1, -lineY1);
                label = QString::number(-i*mStep);
                textWidth = fontMetr.width(label);
                textX = mCX - 5 - textWidth;
                textY = -stepY * i - textHeight / 4;
                painter->drawText(textX, -textY, label);
            }
        }

        // X axis labels
        int count = mChartItemList.size();
        if (mStackCount > 0) count = mStackCount;

        for (int i = 0; i < count; ++i) {
            if (mStackCount <= 0) {
                label = mChartItemList[i]->getName();
            } else {
                if (mStackCount > i) {
                    label = mStackList[i]->getName();
                }
            }

            if (mLegendType == THRD_Chart::AxesHoriz) {
                // Label X normal
                int val = mChartItemList[i]->getValue();
                int x = mCX+mBarDist+i*(mPW+mBarDist)+mPW/2-fontMetr.width(label)/2;
                int y = 0;

                if (val >= 0) {
                    // Positive value below the X axis
                    y = fontMetr.height() + 5;
                } else {
                    // Negative value above the X axis
                    y = -9;
                }

                if (mStackCount <= 0) {
                    painter->drawText(x, y, mChartItemList[i]->getName());
                } else {
                    if (mStackCount > i) {
                        painter->drawText(x, y, mStackList[i]->getName());
                    }
                }
            } else if (mLegendType == THRD_Chart::AxesAngled) {
                // Label X angled
                int val = mChartItemList[i]->getValue();
                int transX = mCX+mBarDist+i*(mPW + mBarDist)+mPW/2;
                int transY = 0;

                if (val >= 0) {
                    // Positive value below the X axis
                    transY = fontMetr.height() + 5;
                } else {
                    // Negative value above the X axis
                    transY = -5;
                }

                painter->save();
                painter->translate(transX, transY);

                if (val >= 0) {
                    painter->rotate(+45);
                } else {
                    painter->rotate(-45);
                }

                if (mStackCount <= 0) {
                    painter->drawText(0, 0, mChartItemList[i]->getName());
                } else {
                    if (mStackCount > i) {
                        painter->drawText(0, 0, mStackList[i]->getName());
                    }
                }
                painter->restore();
            }
        }

        painter->restore();
        break;
    }
    case THRD_Chart::Horizontal:
    {
        int dist = 5;
        painter->setBrush(Qt::white);
        double x = mLX+dist; // mCX;
        double y = mLY+dist; // mCY+mCH+20+dist;
        //painter->drawRoundRect(mCX+mCW+20,mCY,dist*2+200,mChartItemList.size()*(mFontmetr.height()+2*dist)+dist,15,15);

        for (int i=0; i < mChartItemList.size(); ++i) {
            painter->setBrush(mChartItemList[i]->getColor());
            x += fontMetr.height() + 2*dist;

            if (i%3 == 0) {
                x = mLX;
                y += dist+fontMetr.height();
            }

            painter->drawRect(x,y,fontMetr.height(),fontMetr.height());

            QString label = mChartItemList[i]->getName() + " ";

            if (!mLabelBefore.isEmpty()) {
                label += mLabelBefore + " ";
            }

            label += QString::number(mChartItemList[i]->getValue()) + mLabelAfter;
            painter->drawText(x+fontMetr.height()+dist,y+fontMetr.height()/2+dist,label);
            x += fontMetr.width(label);
        }
        break;
    }
    case THRD_Chart::Vertical:
    {
        int dist = 5;
        painter->setBrush(Qt::white);
        //painter->drawRoundRect(mCX+mCW+20,mCY,dist*2+200,mChartItemList.size()*(fontMetr.height()+2*dist)+dist,15,15);
        for (int i=mChartItemList.size()-1;i>=0;i--)
        {
            painter->setBrush(mChartItemList[i]->getColor());
            float x = mLX+dist;
            float y = mLY+dist+i*(fontMetr.height()+2*dist);
            painter->drawRect(x,y,fontMetr.height(),fontMetr.height());
            QString label = mChartItemList[i]->getName() + " ";

            if (!mLabelBefore.isEmpty()) {
                label += mLabelBefore + " ";
            }

            label += QString::number(mChartItemList[i]->getValue()) + mLabelAfter;
            painter->drawText(x+fontMetr.height()+dist,
                              y+fontMetr.height()/2+dist,
                              label);
        }
        break;
    }
    case THRD_Chart::Round:
        for (int i = 0; i < mChartItemList.size(); ++i) {
            double len = mLengthY;
            double itemAngle = 3.6 * mChartItemList[i]->getValue();
            angle += itemAngle / 2;

            QPointF p0 = getPoint(angle);
            QPointF p1 = getPoint(angle, mCW+len,mCH+len);
            int q = getQuarter(angle);

            if (q == 3 || q == 4) {
                p0.setY(p0.y() + mPW / 2);
                p1.setY(p1.y() + mPW / 2);
            }

            painter->drawLine(p0.x(), p0.y(), p1.x(), p1.y());
            QString label = mChartItemList[i]->getName() + " ";

            if (!mLabelBefore.isEmpty()) {
                label += mLabelBefore + " ";
            }

            label += QString::number(mChartItemList[i]->getValue()) + mLabelAfter;

            double recW = fontMetr.width(label) + 10;
            double recH = fontMetr.height() + 10;
            p1.setX(p1.x() - recW / 2 + recW / 2 * cos(angle * M_PI / 180));
            p1.setY(p1.y() + recH / 2 + recH / 2 * sin(angle * M_PI / 180));

            painter->setBrush(Qt::white);
            painter->drawRoundRect(p1.x() ,p1.y(), recW, -recH);
            painter->drawText(p1.x() + 5, p1.y() - recH / 2 + 5, label);

            angle += itemAngle / 2;
         }
        break;
    }
}

QPointF THRD_Chart::getPoint(double angle, double R1, double R2) {
    if (R1 == 0 && R2 == 0) {
        R1 = mCW;
        R2 = mCH;
    }

    QPointF point;
    double x = R1/2*cos(angle*M_PI/180);
    x+=mCW/2+mCX;
    double y = -R2/2*sin(angle*M_PI/180);
    y+=mCH/2+mCY;
    point.setX(x);
    point.setY(y);
    return point;
}

int THRD_Chart::getQuarter(double angle) {
    angle = getAngle360(angle);

    if(angle>=0 && angle<90)
        return 1;
    if(angle>=90 && angle<180)
        return 2;
    if(angle>=180 && angle<270)
        return 3;
    if(angle>=270 && angle<360)
        return 4;

    return 0;
}

double THRD_Chart::getAngle360(double angle) {
    int i = (int)angle;
    double delta = angle - i;
    return (i%360 + delta);
}

/**
 * Set step and number of ticks, number of ticks between 5 and 10
 */
void THRD_Chart::setYaxisDim() {
    if (mLengthY <= 0) return; // Error

    // Order of magnitude, largest power of 10 smaller mLengthY
    double logOfLength = log10(mLengthY);           // log(240) = 2.38
    int powOfLength = floor(logOfLength);           // int = 2
    double magnitude = pow(10.0,  powOfLength);     // magnitude = 100
    double fraction10 = mLengthY / magnitude;       // fraction10 = 2.4 < 1

    // Step, ends with 5 or 0
    if (fraction10 == 1.0) {
        mStep = magnitude / 10;
        mNoTicks = int(mLengthY / mStep);
    } else if (1.0 < fraction10 && fraction10 <= 2.5) {
        mStep = magnitude / 5;
        mNoTicks = int(mLengthY / mStep);
    } else if (2.5 < fraction10 && fraction10 <= 5.0) {
        mStep = magnitude / 2;
        mNoTicks = int(mLengthY / mStep);
    } else if (5.0 < fraction10 && fraction10 < 10.0) {
        mStep = magnitude / 1;
        mNoTicks = int(mLengthY / mStep);
    }
}

THRD_ChartItem::THRD_ChartItem() {
    mName = "<Empty>";
    mColor = QColor();
    mValue = 0.0;
}

void THRD_ChartItem::setName(const QString& name) {
    mName = name;
}

QString THRD_ChartItem::getName() const {
    return mName;
}

void THRD_ChartItem::setColor(Qt::GlobalColor color) {
    mColor = color;
}

void THRD_ChartItem::setColor(const QColor& color) {
    mColor = color;
}

QColor THRD_ChartItem::getColor() const {
    return mColor;
}

void THRD_ChartItem::setValue(double value) {
    mValue = value;
}

double THRD_ChartItem::getValue() const {
    return mValue;
}



THRD_ChartStack::THRD_ChartStack() {
    mName = "<Empty>";
    mHeight = 0.0;
}

void THRD_ChartStack::setName(const QString& name) {
    mName = name;
}

QString THRD_ChartStack::getName() const {
    return mName;
}

void THRD_ChartStack::setHeight(double height) {
    mHeight = height;
}

double THRD_ChartStack::getHeight() const {
    return mHeight;
}
