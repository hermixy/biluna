/*****************************************************************
 * $Id: scan_preparejrreport.cpp 1472 2011-10-24 10:41:39Z rutger $
 * Created: May 17, 2011 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_preparejrreport.h"

#include "rb_objectcontainer.h"
#include "scan_modelfactory.h"
#include "scan_preparejrresult.h"
#include "thrd_chart.h"

/**
 * Constructor
 */
SCAN_PrepareJrReport::SCAN_PrepareJrReport() {
    mReportText = "";
}

/**
 * Get, generate report text. Reentrant function
 * @param textBlock scan report or text block
 * @param result operation with calculated result according Jerry Rhodes
 */
RB_String SCAN_PrepareJrReport::getReport(RB_ObjectBase* textBlock,
                                          SCAN_PrepareJrResult* result) {

    RB_ObjectContainer* textList = textBlock->getContainer("SCAN_BlockList");
    RB_ObjectIterator* iter = textList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        addReportText(iter->currentObject(), result);
        getReport(iter->currentObject(), result);
    }

    return mReportText;
}

/**
 * Add text to the report
 * @param textBlock current text of possible result
 * @param result score values of JR scan
 */
void SCAN_PrepareJrReport::addReportText(RB_ObjectBase* textBlock,
                                         SCAN_PrepareJrResult* result) {
    bool result1Valid = false;
    bool result2Valid = false;
    bool result3Valid = false;

    SCAN_PrepareJrResult::JrType resultType =
            static_cast<SCAN_PrepareJrResult::JrType>(textBlock->getValue("var1type").toInt());

    if (resultType < SCAN_PrepareJrResult::JrTotal01
            || resultType > SCAN_PrepareJrResult::JrColorSg) {
        return;
    }

    int resultValue = result->getResult(resultType);
    int andOr1 = textBlock->getValue("andor1").toInt();
    RB_String content = textBlock->getValue("blocktext").toString();
    int type = textBlock->getValue("contenttype").toInt();

    if (resultValue >= textBlock->getValue("var1min").toInt()
            && resultValue <= textBlock->getValue("var1max").toInt()) {
        result1Valid = true;

        if (andOr1 == AndOrNone){
            updateContent(content, type, result);
            mReportText.append(content);
            return;
        }
    } else if (andOr1 == AndOrNone){
        // 0 = none
        // 1 = and
        // 2 = or
        return; // not within range
    }

    resultType = static_cast<SCAN_PrepareJrResult::JrType>(
                textBlock->getValue("var2type").toInt());

    resultValue = result->getResult(resultType);
    int andOr2 = textBlock->getValue("andor2").toInt();

    if (resultValue >= textBlock->getValue("var2min").toInt()
            && resultValue <= textBlock->getValue("var2max").toInt()) {
        result2Valid = true;
    }

    resultType = static_cast<SCAN_PrepareJrResult::JrType>(
                textBlock->getValue("var3type").toInt());
    resultValue = result->getResult(resultType);

    if (resultValue >= textBlock->getValue("var3min").toInt()
            && resultValue <= textBlock->getValue("var3max").toInt()) {
        result3Valid = true;
    }

    // Handle and/or, and takes precedence over or
    bool isValidBlock = false;

    if (andOr1 == AndOrAnd) {
        if (andOr2 == AndOrAnd) {
            if (result1Valid && result2Valid && result3Valid) {
                isValidBlock = true;
            }
        } else if (andOr2 == AndOrOr) {
            if ((result1Valid && result2Valid) || result3Valid) {
                isValidBlock = true;
            }
        } else {
            // None
            if (result1Valid && result2Valid) {
                isValidBlock = true;
            }
        }
    } else if (andOr1 == AndOrOr) {
        if (andOr2 == AndOrAnd) {
            if (result1Valid || (result2Valid && result3Valid)) {
                isValidBlock = true;
            }
        } else if (andOr2 == AndOrOr) {
            if (result1Valid || result2Valid || result3Valid) {
                isValidBlock = true;
            }
        } else {
            // None
            if (result1Valid || result2Valid) {
                isValidBlock = true;
            }
        }
    }

    if (isValidBlock) {
        updateContent(content, type, result);
        mReportText.append(content);
    }
}

/**
 * Update variables in content with the applicable values
 * @param content
 */
void SCAN_PrepareJrReport::updateContent(QString& content, int type,
                                         SCAN_PrepareJrResult* result) {
    switch (type) {
        case SCAN_PrepareJrResult::ContentText : {
            // nothing
            break;
        }
        case SCAN_PrepareJrResult::ContentTextValue : {
            setValue(content, result);
            break;
        }
        case SCAN_PrepareJrResult::ContentChart : {
            setChart(content, result);
            break;
        }
        default : {
            // nothing
            break;
        }
    }
}

void SCAN_PrepareJrReport::setValue(QString& content, SCAN_PrepareJrResult* result) {
    RB_MmProxy* m = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelScanResult);
    if (!m) return;

    RB_String title = m->getCurrentValue("title").toString();
    content.replace("{$SCAN_ScanResult.title}", title);
}

void SCAN_PrepareJrReport::setChart(QString& content, SCAN_PrepareJrResult* result) {
    /*
    QString html = QString(QLatin1String(file.readAll()))
                        .arg(title)
                        .arg(errorString)
                        .arg(reply->url().toString());

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    if (pixmap.save(&imageBuffer, "PNG")) {
        html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE"),
                     QString(QLatin1String(imageBuffer.buffer().toBase64())));
    }

Example 1
    <style type="text/css">
     div.wrapper {
       background-image: url(data:image/gif;base64,R0lGODdhAQAgAMIIABstehsv
          fRwwgR4zhB81iiA3jSE6kvf6/ywAAAAAAQAgAAADDji21U1wSClqDRjow08CADs=);
     }
    </style>

Example 2
    <img src="data:image/png;base64,
    iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAABGdBTUEAALGP
    C/xhBQAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB9YGARc5KB0XV+IA
    AAAddEVYdENvbW1lbnQAQ3JlYXRlZCB3aXRoIFRoZSBHSU1Q72QlbgAAAF1J
    REFUGNO9zL0NglAAxPEfdLTs4BZM4DIO4C7OwQg2JoQ9LE1exdlYvBBeZ7jq
    ch9//q1uH4TLzw4d6+ErXMMcXuHWxId3KOETnnXXV6MJpcq2MLaI97CER3N0
    vr4MkhoXe0rZigAAAABJRU5ErkJggg==" alt="Red dot" />

Example 3
    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = QIcon(":/images/icons/plugin_edit.png");
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    if (pixmap.save(&imageBuffer, "PNG")) {
        content.append("<img src=\"data:image/png;base64,");
        content.append(QString(QLatin1String(imageBuffer.buffer().toBase64())));
        content.append("\" />");
    }

Example 4 as used
    int imgHeight = 400;
    int imgWidth = 400;
    int margin = 20;

    QPixmap pxm(imgWidth,imgHeight);
    QPainter painter(&pxm);

    THRD_Chart chart;
    //{Bar,Line,Pie2D,Pie3D,StackedBar}
    chart.setChartType(THRD_Chart::Pie3D);
    //{AxesAngled,AxesHoriz,Horizontal,Round,Vertical}
    chart.setLegendType(THRD_Chart::Vertical);
    chart.setShadow();

    chart.setChartCoords(margin, margin, pxm.width()/1.5, pxm.height()/3.0);
    chart.setLegendCoords(300, 100);
    chart.addChartItem("Item1",QColor(200,10,50),34);
    chart.addChartItem("Item2",Qt::green,27);
    chart.addChartItem("Item3",Qt::cyan,14);
    chart.addChartItem("Item4",Qt::yellow,7);
    chart.addChartItem("Item5",Qt::red,4);
    chart.draw(&painter);
    chart.drawLegend(&painter);

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);

    if (pxm.save(&imageBuffer, "PNG")) {
        content.append("<img src=\"data:image/png;base64,");
        content.append(QString(QLatin1String(imageBuffer.buffer().toBase64())));
        content.append("\" />");
    }
    */

    // Parse content
    // {$SCAN_Chart;ChartType;LegendType;Shadow;imgWidth;imgHeight;  5 0-4
    //  graphX;graphY;graphWidth;graphHeight;legendX;legendY;        6 5-10
    //  fontName;StartAngle;BeforeLabel;AfterLabel;lengthYAxis;      5 11-15
    //  NumberOfStack;name1;name2; ... ;                             x
    //  NumberOfItem;name1;color1;value1;name2;color2;value2; ... }  x

    if (!content.contains("$SCAN_Chart")) {
        RB_DEBUG->warning("SCAN_PrepareJrReport::setChart() "
                          "content not a chart WARNING");
        return;
    }

    QStringList strList = content.split("{$SCAN_Chart;");
    if (strList.count() != 2) return;

    QString contentBeforeChart = strList.at(0);
    strList = strList.at(1).split("}");
    if (strList.count() != 2) return;

    QString contentAfterChart = strList.at(1);
    strList = strList.at(0).split(";");
    if (strList.count() < 20) return;

    THRD_Chart chart;
    //{Bar,Line,Pie2D,Pie3D,StackedBar}
    chart.setChartType((THRD_Chart::ChartType)strList.at(0).trimmed().toInt());
    //{AxesAngled,AxesHoriz,Horizontal,Round,Vertical}
    chart.setLegendType((THRD_Chart::LegendType)strList.at(1).trimmed().toInt());
    chart.setShadow(strList.at(2).trimmed().toInt());

    QPixmap pxm(strList.at(3).trimmed().toInt(),strList.at(4).trimmed().toInt());
    pxm.fill(); // white
    QPainter painter(&pxm);

    chart.setChartCoords(strList.at(5).trimmed().toInt(), strList.at(6).trimmed().toInt(),
                         strList.at(7).trimmed().toInt(), strList.at(8).trimmed().toInt());
    chart.setLegendCoords(strList.at(9).trimmed().toInt(), strList.at(10).trimmed().toInt());

    chart.setFont(QFont(strList.at(11).trimmed()));
    chart.setPieStartAngle(strList.at(12).trimmed().toDouble());
    chart.setLabelBefore(strList.at(13).trimmed());
    chart.setLabelAfter(strList.at(14).trimmed());
    chart.setLengthYaxis(strList.at(15).trimmed().toDouble());

    int stackCount = 17 + strList.at(16).trimmed().toInt();

    for (int i = 17; i < stackCount; ++i) {
        chart.addStackName(strList.at(i).trimmed());
    }

    int itemCount = stackCount + 1 + strList.at(stackCount).trimmed().toInt() * 3;

    for (int i = stackCount + 1; i < itemCount; /*++i*/) {
        chart.addChartItem(strList.at(i).trimmed(), QColor(strList.at(i+1).trimmed()),
                           result->getResult(strList.at(i+2).trimmed()));
        i += 3;
    }

    chart.draw(&painter);
    chart.drawLegend(&painter);

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);

    if (pxm.save(&imageBuffer, "PNG")) {
        content = contentBeforeChart;
        content.append("<img src=\"data:image/png;base64,");
        content.append(QString(QLatin1String(imageBuffer.buffer().toBase64())));
        content.append("\" />");
        content.append(contentAfterChart);
    }

}
