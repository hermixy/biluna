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
#ifndef THRD_CHART_H
#define THRD_CHART_H

#include <QtWidgets>
#include "db_global.h"



class DB_EXPORT THRD_ChartItem {

public:
    explicit THRD_ChartItem();
    virtual ~THRD_ChartItem() { }

    void setName(const QString& name);
    QString getName() const;
    void setColor(Qt::GlobalColor color);
    QColor getColor() const;
    void setColor(const QColor& color);
    double getValue() const;
    void setValue(double value);

protected:
    QString mName;
    QColor mColor;
    double mValue;

};



class DB_EXPORT THRD_ChartStack {

public:
    explicit THRD_ChartStack();
    virtual ~THRD_ChartStack() { }

    void setName(const QString& name);
    QString getName() const;
    void setHeight(double height);
    double getHeight() const;

protected:
    //! Name of the bar stack
    QString mName;
    //! Cummulative height
    double mHeight;

};



class DB_EXPORT THRD_Chart {

public:
    explicit THRD_Chart();
    ~THRD_Chart();

    enum ChartType {
        Bar,
        Line,
        Pie2D,
        Pie3D,
        StackedBar
    };

    enum LegendType{
        AxesHoriz,
        AxesAngled,
        Horizontal,
        Vertical,
        Round
    };

    void addChartItem(const QString& name, const QColor& color, double value);
    void addStackName(const QString& name);

    void setChartCoords(double x, double y, double w, double h);
    void setLegendCoords(double x, double y);
    void setChartType(THRD_Chart::ChartType t);
    void setLegendType(THRD_Chart::LegendType t);

    void setShadow(bool ok = true);
    void setValueInChart(bool ok = true);
    void setFont(QFont f);
    void setPieStartAngle(double angle);
    void setLabelBefore(const QString& before);
    void setLabelAfter(const QString& after);
    void setLengthYaxis(double length);

    void draw(QPainter* painter);
    void drawLegend(QPainter* painter);

private:
    QPointF getPoint(double angle, double R1 = 0, double R2 = 0);
    int getQuarter(double angle);
    double getAngle360(double angle);
    void setYaxisDim();

    double mCX, mCY, mCW, mCH, mPW, mLX, mLY;
    bool mIsShadow;
    bool mIsValueInChart;

    QVector<THRD_ChartItem*> mChartItemList;
    QVector<THRD_ChartStack*> mStackList;

    //! Chart type, bar, stacked bar, pie 2D or pie 3D
    int mChartType;
    //! Legend type such as vertical, on axes or round for pie
    int mLegendType;
    //! Font of legends
    QFont mFont;
    //! Start angle for drawing the pie chart
    double mStartAngle;

    //! Flag for histogram to draw positive Y axis
    bool mHasPositiveNumber;
    //! Flag for histogram to draw negative Y axis
    bool mHasNegativeNumber;
    //! Label before the value such currency EUR
    QString mLabelBefore;
    //! Label after the value such currency %
    QString mLabelAfter;
    //! Bar distance
    double mBarDist;
    //! Stack count in case of a stacked bar chart
    int mStackCount;
    //! Y axis length
    double mLengthY;
    //! Number of ticks on Y axis
    int mNoTicks;
    //! Step length on Y axis
    double mStep;
};

#endif // THRD_CHART_H
