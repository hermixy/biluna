/*****************************************************************
 * $Id: peng_pedtable.cpp 1965 2013-08-08 16:33:15Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_pedtable.h"

#include <cmath>
#include <QColor>
#include <QFont>
#include "rb_debug.h"


PENG_PedTable::PENG_PedTable(QWidget *parent, Qt::WindowFlags flags)
					: RB_PlotterWidget(parent, flags) {
	// in case of no input
    RB_PlotText* text = new RB_PlotText(toScale(1.0), toScale(1.0), "(no input)", 
    								QColor(Qt::red));
    std::vector<RB_PlotText*> textVector;
    textVector.push_back(text);
    setTexts(textVector);
}

PENG_PedTable::~PENG_PedTable() {
	
}

/**
 * Draw horizontal line indicating the maximum design pressure in bar
 */
void PENG_PedTable::setHorizontalLine(double pressure) {
	// create a curve with only two points which will be a horizontal line
	std::vector<double> curveData;
	curveData.push_back(0.1); 		// x1
	curveData.push_back(pressure); 	// y1
	curveData.push_back(100000); 	// x2
	curveData.push_back(pressure); 	// y2
	setCurveData(0, curveData);
}

/**
 * Draw vertical line indicating the volume in litres or line size in mm
 */
void PENG_PedTable::setVerticalLine(double sizeVolume) {
	// create a curve with only two points which will be a horizontal line
	std::vector<double> curveData;
	curveData.push_back(sizeVolume); 	// x1
	curveData.push_back(0.1); 			// y1
	curveData.push_back(sizeVolume); 	// x2
	curveData.push_back(100000); 		// y2
	setCurveData(1, curveData);
}


/**
 * translate original value to the plot coordinates, 
 * this case log10 scale for both x-axis and y-axis 
 */
double PENG_PedTable::toScale(double originalValue) {
	return log10(originalValue);
}

/**
 * translate the plot value coordinate to the plot original value
 */
double PENG_PedTable::fromScale(double scaledValue) {
	return pow(10.0, scaledValue);
}

void PENG_PedTable::setTable(int table) {
    QFont smallFont("Helvetica", 9);
	    
	if (table == 1) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("V(litres)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(1000), toScale(1), toScale(1000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(200), toScale(1), toScale(200), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical lines
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(200), toScale(1), toScale(25), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(1000), toScale(2000), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(200), toScale(400), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(50), toScale(100), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(25), toScale(50), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(1050), "PS=1000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(210), "PS=200", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical lines
        textVector.push_back(new RB_PlotText(toScale(0.95), toScale(50), "V=1", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines
        textVector.push_back(new RB_PlotText(toScale(4.75), toScale(210), "PS*V=1000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(4.75), toScale(42), "PS*V=200", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(4.75), toScale(10.5), "PS*V=50", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(4.75), toScale(5.3), "PS*V=25", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(52.5), toScale(0.525), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(142.5), toScale(0.525), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(665), toScale(0.525), "III", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(2850), toScale(0.525), "IV", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 2) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("V(litres)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(3000), toScale(1), toScale(3000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(1000), toScale(1), toScale(1000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(750), toScale(4), toScale(100000), toScale(4), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(1000), toScale(1), toScale(50), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(3000), toScale(750), toScale(4), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(1000), toScale(2000), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(200), toScale(400), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(50), toScale(100), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(3150), "PS=3000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(1050), "PS=1000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(4.2), "PS=4", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(0.95), toScale(100), "V=1", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(5), toScale(600), "PS*V=3000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(5), toScale(200), "PS*V=1000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(5), toScale(40), "PS*V=200", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(5), toScale(10), "PS*V=50", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(150), toScale(0.525), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(700), toScale(0.525), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(3000), toScale(0.525), "III", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(3000), toScale(4.2), "IV", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);

	} else if (table == 3) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("V(litres)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(500), toScale(100000), toScale(500), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(20), toScale(10), toScale(100000), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(200), toScale(1), toScale(100000), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(1), toScale(200), toScale(400), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(525), "PS=500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(10.5), "PS=10", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(0.95), toScale(3000), "V=1", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(5), toScale(40), "PS*V=200", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(700), toScale(0.525), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(700), toScale(10.5), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(700), toScale(525), "III", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(525), "IV", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 4) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("V(litres)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(1000), toScale(10), toScale(1000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(20), toScale(500), toScale(100000), toScale(500), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1000), toScale(10), toScale(100000), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(10), toScale(1000), toScale(10), toScale(100000), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(10), toScale(1000), toScale(1000), toScale(10), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(525), "PS=500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(10.5), "PS=10", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(9.5), toScale(3000), "V=10", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(200), toScale(50), "PS*V=10000", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(1500), toScale(10.5), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(1500), toScale(525), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.125), toScale(1050), "I", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 5) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("V(litres)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(2), toScale(32), toScale(93.75), toScale(32), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(2), toScale(0.5), toScale(2), toScale(100000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(1000), toScale(0.5), toScale(1000), toScale(3), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(93.75), toScale(32), toScale(1000), toScale(3), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(6.25), toScale(32), toScale(400), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(2), toScale(25), toScale(100), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(5), toScale(33.6), "PS=32", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(1.9), toScale(100), "V=2", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(250), toScale(12), "PS*V=3000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(50), toScale(4), "PS*V=200", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(50), toScale(1), "PS*V=50", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(10), toScale(0.525), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(150), toScale(0.525), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(500), toScale(0.525), "III", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(3000), toScale(0.525), "IV", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 6) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("DN(mm)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(25), toScale(0.5), toScale(25), toScale(100000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(100), toScale(0.5), toScale(100), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(100), toScale(35), toScale(100), toScale(100000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(350), toScale(0.5), toScale(350), toScale(10), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(25), toScale(40), toScale(100), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(100), toScale(35), toScale(350), toScale(10), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(23.75), toScale(1), "DN=25", QColor(Qt::black), smallFont, -90.0));
        textVector.push_back(new RB_PlotText(toScale(95), toScale(1), "DN=100", QColor(Qt::black), smallFont, -90.0));
        textVector.push_back(new RB_PlotText(toScale(95), toScale(100), "DN=100", QColor(Qt::black), smallFont, -90.0));
        textVector.push_back(new RB_PlotText(toScale(332.5), toScale(1), "DN=350", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(140), toScale(25), "PS*DN=3500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(80), toScale(12.5), "PS*DN=1000", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(50), toScale(0.525), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(150), toScale(0.525), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(500), toScale(0.525), "III", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 7) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("DN(mm)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(32), toScale(31.25), toScale(32), toScale(100000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(100), toScale(35), toScale(100), toScale(100000), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(250), toScale(20), toScale(250), toScale(100000), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(250), toScale(20), toScale(10000), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(100), toScale(35), toScale(7000), toScale(0.5), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(32), toScale(31.25), toScale(2000), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(13000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(23.75), toScale(100), "DN=32", QColor(Qt::black), smallFont, -90.0));
        textVector.push_back(new RB_PlotText(toScale(95), toScale(100), "DN=100", QColor(Qt::black), smallFont, -90.0));
        textVector.push_back(new RB_PlotText(toScale(237.5), toScale(100), "DN=250", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(400), toScale(12.5), "PS*DN=5000", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(700), toScale(5), "PS*DN=3500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(500), toScale(2), "PS*DN=1000", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(50), toScale(50), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(130), toScale(50), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(500), toScale(50), "III", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 8) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("DN(mm)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(25), toScale(500), toScale(100000), toScale(500), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(200), toScale(10), toScale(100000), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(25), toScale(80), toScale(25), toScale(100000), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(25), toScale(80), toScale(4000), toScale(0.5), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(525), "PS=500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(10.5), "PS=10", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(23.75), toScale(1000), "DN=25", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(50), toScale(40), "PS*DN=2000", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(5000), toScale(0.5), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(4000), toScale(10), "II", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(4000), toScale(500), "III", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else if (table == 9) {
		RB_String str = "PED 97/23/EC Table " + RB_String::number(table);
		setTitle(str);

		setUnitX("PS(bar)");
		setUnitY("DN(mm)");
		
		std::vector<RB_PlotLine*> lineVector;
		std::vector<RB_PlotText*> textVector;
		
		// horizontal lines
		lineVector.push_back(new RB_PlotLine(toScale(200), toScale(500), toScale(100000), toScale(500), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(500), toScale(10), toScale(100000), toScale(10), QColor(Qt::black)));
		lineVector.push_back(new RB_PlotLine(toScale(0.1), toScale(0.5), toScale(100000), toScale(0.5), QColor(Qt::black)));
		// vertical line
		lineVector.push_back(new RB_PlotLine(toScale(200), toScale(25), toScale(200), toScale(100000), QColor(Qt::black)));
		// sloped lines
		lineVector.push_back(new RB_PlotLine(toScale(200), toScale(25), toScale(500), toScale(10), QColor(Qt::black)));
		// text horizontal lines
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(525), "PS=500", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(10.5), "PS=10", QColor(Qt::black), smallFont));
        textVector.push_back(new RB_PlotText(toScale(10000), toScale(0.525), "PS=0.5", QColor(Qt::black), smallFont));
		// text vertical line
        textVector.push_back(new RB_PlotText(toScale(190), toScale(1000), "DN=200", QColor(Qt::black), smallFont, -90.0));
		// text sloped lines		
        textVector.push_back(new RB_PlotText(toScale(250), toScale(20), "PS*DN=5000", QColor(Qt::black), smallFont));
		// category area indication
        textVector.push_back(new RB_PlotText(toScale(2000), toScale(10), "I", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(2000), toScale(500), "II", QColor(Qt::red), smallFont));
		// remaining non-category areas
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(1.2), "Article 3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.7), "par.3", QColor(Qt::red), smallFont));
        textVector.push_back(new RB_PlotText(toScale(0.119), toScale(0.15), "PED not applicable", QColor(Qt::red), smallFont));

		setLines(lineVector);
		setTexts(textVector);
	} else {
        this->clearLines();
        this->clearTexts();

		std::vector<RB_PlotText*> textVector;
		textVector.push_back(new RB_PlotText(toScale(1.0), toScale(1.0), 
                        "(No table number definition or press calculate)", QColor(Qt::red)));
		setTexts(textVector);

        this->clearCurve(0);
        this->clearCurve(1);
        this->clearTitle(); // also refreshes the pixmap
    }
}

