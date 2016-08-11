/****************************************************************************
** $Id: gv_units.cpp 7898 2008-01-15 17:26:19Z andrew $
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


#include "gv_units.h"

#include <stdio.h>
#include <qobject.h>

#include "gv_math.h"
#include "rb_debug.h"

/**
 * Converts a DXF integer to a Unit enum.
 */
GV2::Unit GV_Units::dxfint2unit(int dxfint) {
    return (GV2::Unit)dxfint;

    /*switch(dxfint) {
    default:
    case  0:
        return GV2::None;
    case  1:
        return GV2::Inch;
    case  2:
        return GV2::Foot;
    case  3:
        return GV2::Mile;
    case  4:
        return GV2::Millimeter;
    case  5:
        return GV2::Centimeter;
    case  6:
        return GV2::Meter;
    case  7:
        return GV2::Kilometer;
    case  8:
        return GV2::Microinch;
    case  9:
        return GV2::Mil;
    case 10:
        return GV2::Yard;
    case 11:
        return GV2::Angstrom;
    case 12:
        return GV2::Nanometer;
    case 13:
        return GV2::Micron;
    case 14:
        return GV2::Decimeter;
    case 15:
        return GV2::Decameter;
    case 16:
        return GV2::Hectometer;
    case 17:
        return GV2::Gigameter;
    case 18:
        return GV2::Astro;
    case 19:
        return GV2::Lightyear;
    case 20:
        return GV2::Parsec;
}*/
}


/**
 * @return a short string representing the given unit (e.g. "mm")
 */
QString GV_Units::unitToSign(GV2::Unit u) {
    QString ret = "";

    switch (u) {
    case GV2::None:
        ret = "";
        break;
    case GV2::Inch:
        ret = "\"";
        break;
    case GV2::Foot:
        ret = "'";
        break;
    case GV2::Mile:
        ret = "mi";
        break;
    case GV2::Millimeter:
        ret = "mm";
        break;
    case GV2::Centimeter:
        ret = "cm";
        break;
    case GV2::Meter:
        ret = "m";
        break;
    case GV2::Kilometer:
        ret = "km";
        break;
    case GV2::Microinch:
        ret = "μ\"";
        break;
    case GV2::Mil:
        ret = "mil";
        break;
    case GV2::Yard:
        ret = "yd";
        break;
    case GV2::Angstrom:
        ret = "A";
        break;
    case GV2::Nanometer:
        ret = "nm";
        break;
    case GV2::Micron:
        ret = "μm";
        break;
    case GV2::Decimeter:
        ret = "dm";
        break;
    case GV2::Decameter:
        ret = "dam";
        break;
    case GV2::Hectometer:
        ret = "hm";
        break;
    case GV2::Gigameter:
        ret = "Gm";
        break;
    case GV2::Astro:
        ret = "astro";
        break;
    case GV2::Lightyear:
        ret = "ly";
        break;
    case GV2::Parsec:
        ret = "pc";
        break;
    case GV2::Plu:
        ret = "plu";
        break;

    default:
        ret = "";
        break;
    }

    return ret;
}



/**
 * @return a string representing the given unit (e.g. "Millimeter").
 *      translated if @a t is true (the default).
 */
QString GV_Units::unitToString(GV2::Unit u, bool t) {
    QString ret = "";

    switch (u) {
    case GV2::None:
        ret = t ? QObject::tr("None") : QString("None");
        break;
    case GV2::Inch:
        ret = t ? QObject::tr("Inch") : QString("Inch");
        break;
    case GV2::Foot:
        ret = t ? QObject::tr("Foot") : QString("Foot");
        break;
    case GV2::Mile:
        ret = t ? QObject::tr("Mile") : QString("Mile");
        break;
    case GV2::Millimeter:
        ret = t ? QObject::tr("Millimeter") : QString("Millimeter");
        break;
    case GV2::Centimeter:
        ret = t ? QObject::tr("Centimeter") : QString("Centimeter");
        break;
    case GV2::Meter:
        ret = t ? QObject::tr("Meter") : QString("Meter");
        break;
    case GV2::Kilometer:
        ret = t ? QObject::tr("Kilometer") : QString("Kilometer");
        break;
    case GV2::Microinch:
        ret = t ? QObject::tr("Microinch") : QString("Microinch");
        break;
    case GV2::Mil:
        ret = t ? QObject::tr("Mil") : QString("Mil");
        break;
    case GV2::Yard:
        ret = t ? QObject::tr("Yard") : QString("Yard");
        break;
    case GV2::Angstrom:
        ret = t ? QObject::tr("Angstrom") : QString("Angstrom");
        break;
    case GV2::Nanometer:
        ret = t ? QObject::tr("Nanometer") : QString("Nanometer");
        break;
    case GV2::Micron:
        ret = t ? QObject::tr("Micron") : QString("Micron");
        break;
    case GV2::Decimeter:
        ret = t ? QObject::tr("Decimeter") : QString("Decimeter");
        break;
    case GV2::Decameter:
        ret = t ? QObject::tr("Decameter") : QString("Decameter");
        break;
    case GV2::Hectometer:
        ret = t ? QObject::tr("Hectometer") : QString("Hectometer");
        break;
    case GV2::Gigameter:
        ret = t ? QObject::tr("Gigameter") : QString("Gigameter");
        break;
    case GV2::Astro:
        ret = t ? QObject::tr("Astro") : QString("Astro");
        break;
    case GV2::Lightyear:
        ret = t ? QObject::tr("Lightyear") : QString("Lightyear");
        break;
    case GV2::Parsec:
        ret = t ? QObject::tr("Parsec") : QString("Parsec");
        break;
    case GV2::Plu:
        ret = t ? QObject::tr("Plu") : QString("Plu");
        break;

    default:
        ret = "";
        break;
    }

    return ret;
}



/**
 * Converts a string into a unit enum.
 */
GV2::Unit GV_Units::stringToUnit(const QString& u) {
    GV2::Unit ret = GV2::None;

    if (u=="None") {
        ret = GV2::None;
    } else if (u==QObject::tr("Inch")) {
        ret = GV2::Inch;
    } else if (u==QObject::tr("Foot")) {
        ret = GV2::Foot;
    } else if (u==QObject::tr("Mile")) {
        ret = GV2::Mile;
    } else if (u==QObject::tr("Millimeter")) {
        ret = GV2::Millimeter;
    } else if (u==QObject::tr("Centimeter")) {
        ret = GV2::Centimeter;
    } else if (u==QObject::tr("Meter")) {
        ret = GV2::Meter;
    } else if (u==QObject::tr("Kilometer")) {
        ret = GV2::Kilometer;
    } else if (u==QObject::tr("Microinch")) {
        ret = GV2::Microinch;
    } else if (u==QObject::tr("Mil")) {
        ret = GV2::Mil;
    } else if (u==QObject::tr("Yard")) {
        ret = GV2::Yard;
    } else if (u==QObject::tr("Angstrom")) {
        ret = GV2::Angstrom;
    } else if (u==QObject::tr("Nanometer")) {
        ret = GV2::Nanometer;
    } else if (u==QObject::tr("Micron")) {
        ret = GV2::Micron;
    } else if (u==QObject::tr("Decimeter")) {
        ret = GV2::Decimeter;
    } else if (u==QObject::tr("Decameter")) {
        ret = GV2::Decameter;
    } else if (u==QObject::tr("Hectometer")) {
        ret = GV2::Hectometer;
    } else if (u==QObject::tr("Gigameter")) {
        ret = GV2::Gigameter;
    } else if (u==QObject::tr("Astro")) {
        ret = GV2::Astro;
    } else if (u==QObject::tr("Lightyear")) {
        ret = GV2::Lightyear;
    } else if (u==QObject::tr("Parsec")) {
        ret = GV2::Parsec;
    } else if (u==QObject::tr("Plu")) {
        ret = GV2::Plu;
    }

    return ret;
}




/**
 * @return true: the unit is metric, false: the unit is imperial.
 */
bool GV_Units::isMetric(GV2::Unit u) {
    // 20080114: metric as default
    if (u==GV2::Inch ||
        u==GV2::Foot ||
        u==GV2::Mile ||
        u==GV2::Mil ||
        u==GV2::Yard) {
        return false;
    } else {
        return true;
    }
}



/**
 * @return factor to convert the given unit to Millimeters.
 */
double GV_Units::getFactorToMM(GV2::Unit u) {
    double ret = 1.0;

    switch (u) {
    case GV2::None:
        ret = 1.0;
        break;
    case GV2::Inch:
        ret = 25.4;
        break;
    case GV2::Foot:
        ret = 304.8;
        break;
    case GV2::Mile:
        ret = 1609344;
        break;
    case GV2::Millimeter:
        ret = 1.0;
        break;
    case GV2::Centimeter:
        ret = 10;
        break;
    case GV2::Meter:
        ret = 1000;
        break;
    case GV2::Kilometer:
        ret = 1000000;
        break;
    case GV2::Microinch:
        ret = 0.0000254;
        break;
    case GV2::Mil:
        ret = 0.0254;
        break;
    case GV2::Yard:
        ret = 914.4;
        break;
    case GV2::Angstrom:
        ret = 0.0000001;
        break;
    case GV2::Nanometer:
        ret = 0.000001;
        break;
    case GV2::Micron:
        ret = 0.001;
        break;
    case GV2::Decimeter:
        ret = 100.0;
        break;
    case GV2::Decameter:
        ret = 10000.0;
        break;
    case GV2::Hectometer:
        ret = 100000.0;
        break;
    case GV2::Gigameter:
        ret = 1000000000.0;
        break;
    case GV2::Astro:
        ret = 149600000000000.0;
        break;
    case GV2::Lightyear:
        ret = 9460731798000000000.0;
        break;
    case GV2::Parsec:
        ret = 30857000000000000000.0;
        break;
    case GV2::Plu:
        ret = 0.025;
        break;
    default:
        ret = 1.0;
        break;
    }

    return ret;
}


/**
 * Converts the given value 'val' from unit 'src' to unit 'dest'.
 */
double GV_Units::convert(double val, GV2::Unit src, GV2::Unit dest) {
    if (getFactorToMM(dest)>0.0) {
        return (val*getFactorToMM(src))/getFactorToMM(dest);
    } else {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Units::convert: invalid factor");
        return val;
    }
}



/**
 * Converts the given vector 'val' from unit 'src' to unit 'dest'.
 */
GV_Vector GV_Units::convert(const GV_Vector val, GV2::Unit src, GV2::Unit dest) {
    return GV_Vector(convert(val.x, src, dest),
                     convert(val.y, src, dest),
                     convert(val.z, src, dest));
}



/**
 * Formats the given length in the given format.
 *
 * @param length The length in the current unit of the drawing.
 * @param format Format of the string.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatLinear(double length, GV2::Unit unit,
                                 GV2::LinearFormat format,
                                 int prec, bool showUnit,
                                 bool showLeadingZeroes, 
                                 bool showTrailingZeroes) {
    RB_DEBUG->print("GV_Units::formatLinear");

    QString ret;

    // unit appended to value (e.g. 'mm'):
    /*QString unitString = "";
    if (showUnit) {
        unitString = unitToSign(unit);
}*/

    // barbarian display: show as fraction:
    switch (format) {
    case GV2::Scientific:
        ret = formatScientific(length, unit, prec, showUnit, 
            showLeadingZeroes, showTrailingZeroes);
        break;

    case GV2::Decimal:
        ret = formatDecimal(length, unit, prec, showUnit,
            showLeadingZeroes, showTrailingZeroes);
        break;

    case GV2::Engineering:
        ret = formatEngineering(length, unit, prec, showUnit,
            showLeadingZeroes, showTrailingZeroes);
        break;

    case GV2::Architectural:
        ret = formatArchitectural(length, unit, prec, showUnit,
            showLeadingZeroes, showTrailingZeroes);
        break;

    case GV2::Fractional:
        ret = formatFractional(length, unit, prec, showUnit,
            showLeadingZeroes, showTrailingZeroes);
        break;

    default:
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Units::formatLinear: Unknown format");
        ret = "";
        break;
    }
    
    RB_DEBUG->print("GV_Units::formatLinear: OK");

    return ret;
}



/**
 * Formats the given length in scientific format (e.g. 2.5E7).
 *
 * @param length The length in the current unit of the drawing.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatScientific(double length, GV2::Unit unit,
                                     int prec, bool showUnit,
                                     bool /*showLeadingZeroes*/, bool /*showTrailingZeroes*/) {
    RB_DEBUG->print("GV_Units::formatScientific");

    QString ret;

    // unit appended to value (e.g. 'mm'):
    QString unitString = "";
    if (showUnit) {
        unitString = unitToSign(unit);
    }

    char format[128];
    sprintf(format, "%%.%dE%%s", prec);
    ret.sprintf(format, length, (const char*)unitString.toLatin1());
    
    RB_DEBUG->print("GV_Units::formatScientific: OK");

    return ret;
}



/**
 * Formats the given length in decimal (normal) format (e.g. 2.5).
 *
 * @param length The length in the current unit of the drawing.
 * @param prec Precisision of the value (e.g. 0.001)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatDecimal(double length, GV2::Unit unit,
                                  int prec, bool showUnit,
                                  bool showLeadingZeroes, bool showTrailingZeroes) {

    RB_DEBUG->print("GV_Units::formatDecimal");

    QString ret;
    
    /*
    QString formatString;
    if (showTrailingZeroes) {
        formatString = QString("%.0%1f").arg(prec);
    }
    else {
        formatString = QString("%.%1f").arg(prec);
    }
    */

    // unit appended to value (e.g. 'mm'):

    ret = GV_Math::doubleToString(length, prec, 
            showLeadingZeroes, showTrailingZeroes);
    //ret.sprintf(formatString.toLatin1(), length);
    
    /*
    if (!showTrailingZeroes) {
        ret = trimTrailingZeroes(ret);
    }
    */

    if(showUnit) {
        ret+=unitToSign(unit);
    }
    
    RB_DEBUG->print("GV_Units::formatDecimal: OK");

    return ret;
}



/**
 * Formats the given length in engineering format (e.g. 5' 4.5").
 *
 * @param length The length in the current unit of the drawing.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatEngineering(double length, GV2::Unit unit,
                                      int prec, bool /*showUnit*/,
                                      bool /*showLeadingZeroes*/, bool /*showTrailingZeroes*/) {

    if (unit!=GV2::Inch) {
        RB_DEBUG->print(RB_Debug::D_ERROR, 
            "GV_Units::formatEngineering:"
            "Unit must be set to 'Inch' for engineering format");
        return "";
    }

    QString ret;

    bool sign = (length<0.0);
    int feet = (int)floor(fabs(length)/12);
    double inches = fabs(length) - feet*12;

    QString sInches = GV_Math::doubleToString(inches, prec);

    if (sInches=="12") {
        feet++;
        sInches="0";
    }

    if (feet!=0) {
        ret.sprintf("%d'-%s\"", feet, (const char*)sInches.toLatin1());
    } else {
        ret.sprintf("%s\"", (const char*)sInches.toLatin1());
    }

    if (sign) {
        ret = "-" + ret;
    }

    return ret;
}



/**
 * Formats the given length in architectural format (e.g. 5' 4 1/2").
 *
 * @param length The length in the current unit of the drawing.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatArchitectural(double length, GV2::Unit /*unit*/,
                                        int prec, bool showUnit,
                                      bool /*showLeadingZeroes*/, bool /*showTrailingZeroes*/) {
    QString ret;
    bool neg = (length<0.0);

    int feet = (int)floor(fabs(length)/12);
    double inches = fabs(length) - feet*12;

    QString sInches = formatFractional(inches, GV2::Inch, prec, showUnit);

    if (sInches=="12") {
        feet++;
        sInches = "0";
    }

    if (neg) {
        ret.sprintf("-%d'-%s\"", feet, (const char*)sInches.toLatin1());
    } else {
        ret.sprintf("%d'-%s\"", feet, (const char*)sInches.toLatin1());
    }

    return ret;
}



/**
 * Formats the given length in fractional (barbarian) format (e.g. 5' 3 1/64").
 *
 * @param length The length in the current unit of the drawing.
 * @param unit Should be inches.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 & @param showUnit Append unit to the value.
 */
QString GV_Units::formatFractional(double length, GV2::Unit /*unit*/,
                                     int prec, bool /*showUnit*/,
                                     bool /*showLeadingZeroes*/, bool /*showTrailingZeroes*/) {

    QString ret;

    int num;            // number of complete inches (num' 7/128")
    int nominator;      // number of fractions (nominator/128)
    int denominator;    // (4/denominator)

    // sign:
    QString neg = "";
    if(length < 0) {
        neg = "-";
        length = fabs(length);
    }

    num = (int)floor(length);

    denominator = (int)GV_Math::pow(2, prec);
    nominator = GV_Math::mround((length-num)*denominator);

    // fraction rounds up to 1:
    if (nominator==denominator) {
        nominator=0;
        denominator=0;
        ++num;
    }

    // Simplify the fraction
    if (nominator!=0 && denominator!=0) {
        int gcd = GV_Math::findGCD(nominator, denominator);
        if (gcd!=0) {
            nominator = nominator / gcd;
            denominator = denominator / gcd;
        } else {
            RB_DEBUG->print(RB_Debug::D_WARNING,
                "GV_Units::formatFractional: invalid gcd");
            nominator = 0;
            denominator = 0;
        }
    }

    if( num!=0 && nominator!=0 ) {
        ret.sprintf("%s%d %d/%d",
                    (const char*)neg.toLatin1(), num,
                    nominator, denominator);
    } else if(nominator!=0) {
        ret.sprintf("%s%d/%d",
                    (const char*)neg.toLatin1(),
                    nominator, denominator);
    } else if(num!=0) {
        ret.sprintf("%s%d",
                    (const char*)neg.toLatin1(),
                    num);
    } else {
        ret.sprintf("0");
    }

    return ret;
}





/**
 * Formats the given angle with the given format.
 *
 * @param angle The angle (always in rad).
 * @param format Format of the string.
 * @param prec Precisision of the value (e.g. 0.001 or 1/128 = 0.0078125)
 *
 * @ret String with the formatted angle.
 */
QString GV_Units::formatAngle(double angle, GV2::AngleFormat format,
                                int prec,
                                bool showLeadingZeroes, 
                                bool showTrailingZeroes) {

    QString ret;
    double value;

    switch (format) {
    case GV2::DegreesDecimal:
    case GV2::DegreesMinutesSeconds:
        value = GV_Math::rad2deg(angle);
        break;
    case GV2::Radians:
        value = angle;
        break;
    case GV2::Gradians:
        value = GV_Math::rad2gra(angle);
        break;
    default:
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Units::formatAngle: Unknown Angle Unit");
        return "";
        break;
    }

    switch (format) {
    case GV2::DegreesDecimal:
    case GV2::Radians:
    case GV2::Gradians:
        ret = GV_Math::doubleToString(value, prec, showLeadingZeroes, showTrailingZeroes);
        if (format==GV2::DegreesDecimal)
            ret+=QChar(0xB0);
        if (format==GV2::Radians)
            ret+="r";
        if (format==GV2::Gradians)
            ret+="g";
        break;

    case GV2::DegreesMinutesSeconds: {
            int vDegrees, vMinutes;
            double vSeconds;
            QString degrees, minutes, seconds;

            vDegrees = (int)floor(value);
            vMinutes = (int)floor((value - vDegrees) * 60.0);
            vSeconds = (value - vDegrees - (vMinutes/60.0)) * 3600.0;

            seconds = GV_Math::doubleToString(vSeconds, (prec>1 ? prec-2 : 0));

            if(seconds=="60") {
                seconds="0";
                ++vMinutes;
                if(vMinutes==60) {
                    vMinutes=0;
                    ++vDegrees;
                }
            }

            if (prec==0 && vMinutes>=30.0) {
                vDegrees++;
            } else if (prec==1 && vSeconds>=30.0) {
                vMinutes++;
            }

            degrees.setNum(vDegrees);
            minutes.setNum(vMinutes);

            switch (prec) {
            case 0:
                ret = degrees + QChar(0xB0);
                break;
            case 1:
                ret = degrees + QChar(0xB0) + " " + minutes + "'";
                break;
            default:
                ret = degrees + QChar(0xB0) + " " + minutes + "' "
                      + seconds + "\"";
                break;
            }
        }
        break;

    default:
        break;
    }

    return ret;
}
    
    
    
QString GV_Units::trimTrailingZeroes(const QString& s) {
    QString ret = s;

    bool done = false;
    while (!done) {
        if (ret.size()>0) {
            if (ret.at(ret.size()-1)=='0') {
                ret = ret.left(ret.size()-1);
            }
            else if (ret.at(ret.size()-1)=='.') {
                ret = ret.left(ret.size()-1);
                done = true;
            }
            else {
                done = true;
            }
        }
        else {
            done = true;
        }
    }

    return ret;
}



/**
 * @return Size of the given paper format.
 */
GV_Vector GV_Units::paperFormatToSize(GV2::PaperFormat p) {
    GV_Vector ret(false);

    switch (p) {
    case GV2::Custom:
        ret = GV_Vector(0.0, 0.0);
        break;
    case GV2::Letter:
        ret = GV_Vector(215.9, 279.4);
        break;
    case GV2::Legal:
        ret = GV_Vector(215.9, 355.6);
        break;
    case GV2::Executive:
        ret = GV_Vector(190.5, 254.0);
        break;
    case GV2::A0:
        ret = GV_Vector(841.0, 1189.0);
        break;
    case GV2::A1:
        ret = GV_Vector(594.0, 841.0);
        break;
    case GV2::A2:
        ret = GV_Vector(420.0, 594.0);
        break;
    case GV2::A3:
        ret = GV_Vector(297.0, 420.0);
        break;
    case GV2::A4:
        ret = GV_Vector(210.0, 297.0);
        break;
    case GV2::A5:
        ret = GV_Vector(148.0, 210.0);
        break;
    case GV2::A6:
        ret = GV_Vector(105.0, 148.0);
        break;
    case GV2::A7:
        ret = GV_Vector(74.0, 105.0);
        break;
    case GV2::A8:
        ret = GV_Vector(52.0, 74.0);
        break;
    case GV2::A9:
        ret = GV_Vector(37.0, 52.0);
        break;
        /*case GV2::A10:
            ret = GV_Vector(26.0, 37.0);
            break;*/
    case GV2::B0:
        ret = GV_Vector(1000.0, 1414.0);
        break;
    case GV2::B1:
        ret = GV_Vector(707.0, 1000.0);
        break;
    case GV2::B2:
        ret = GV_Vector(500.0, 707.0);
        break;
    case GV2::B3:
        ret = GV_Vector(353.0, 500.0);
        break;
    case GV2::B4:
        ret = GV_Vector(250.0, 353.0);
        break;
    case GV2::B5:
        ret = GV_Vector(176.0, 250.0);
        break;
    case GV2::B6:
        ret = GV_Vector(125.0, 176.0);
        break;
    case GV2::B7:
        ret = GV_Vector(88.0, 125.0);
        break;
    case GV2::B8:
        ret = GV_Vector(62.0, 88.0);
        break;
    case GV2::B9:
        ret = GV_Vector(44.0, 62.0);
        break;
    case GV2::B10:
        ret = GV_Vector(31.0, 44.0);
        break;
        /*
          case GV2::C0:
              ret = GV_Vector(917.0, 1297.0);
              break;
          case GV2::C1:
              ret = GV_Vector(648.0, 917.0);
              break;
          case GV2::C2:
              ret = GV_Vector(458.0, 648.0);
              break;
          case GV2::C3:
              ret = GV_Vector(324.0, 458.0);
              break;
          case GV2::C4:
              ret = GV_Vector(229.0, 324.0);
              break;
          case GV2::C5:
              ret = GV_Vector(162.0, 229.0);
              break;
          case GV2::C6:
              ret = GV_Vector(114.0, 162.0);
              break;
          case GV2::C7:
              ret = GV_Vector(81.0, 114.0);
              break;
          case GV2::C8:
              ret = GV_Vector(57.0, 81.0);
              break;
          case GV2::C9:
              ret = GV_Vector(40.0, 57.0);
              break;
          case GV2::C10:
              ret = GV_Vector(28.0, 40.0);
              break;
        */
    case GV2::C5E:
        ret = GV_Vector(163.0, 229.0);
        break;
    case GV2::Comm10E:
        ret = GV_Vector(105.0, 241.0);
        break;
    case GV2::DLE:
        ret = GV_Vector(110.0, 220.0);
        break;
    case GV2::Folio:
        ret = GV_Vector(210.0, 330.0);
        break;
    //case GV2::Ledger:
    //    ret = GV_Vector(432.0, 279.0);
    //    break;
    case GV2::Tabloid:
        ret = GV_Vector(279.0, 432.0);
        break;
    case GV2::NPageSize:
        ret = GV_Vector(0.0, 0.0);
        break;
    default:
        break;
    }

    return ret;
}



/**
 * Gets the paper format which matches the given size. If no
 * format matches, GV2::Custom is returned.
 */
GV2::PaperFormat GV_Units::paperSizeToFormat(const GV_Vector s) {
    GV_Vector ts1;
    GV_Vector ts2;

    for (int i=(int)GV2::Custom; i<=(int)GV2::NPageSize; ++i) {
        ts1 = GV_Units::paperFormatToSize((GV2::PaperFormat)i);
        ts2 = GV_Vector(ts1.y, ts1.x);

        if (ts1.distanceTo(s)<1.0e-4 || ts2.distanceTo(s)<1.0e-4) {
            return (GV2::PaperFormat)i;
        }
    }

    return GV2::Custom;
}



/**
 * Converts a paper format to a string (e.g. for a combobox).
 */
QString GV_Units::paperFormatToString(GV2::PaperFormat p) {
    QString ret = "";

    switch (p) {
    case GV2::Custom:
        ret = "Custom";
        break;
    case GV2::Letter:
        ret = "Letter";
        break;
    case GV2::Legal:
        ret = "Legal";
        break;
    case GV2::Executive:
        ret = "Executive";
        break;
    case GV2::A0:
        ret = "A0";
        break;
    case GV2::A1:
        ret = "A1";
        break;
    case GV2::A2:
        ret = "A2";
        break;
    case GV2::A3:
        ret = "A3";
        break;
    case GV2::A4:
        ret = "A4";
        break;
    case GV2::A5:
        ret = "A5";
        break;
    case GV2::A6:
        ret = "A6";
        break;
    case GV2::A7:
        ret = "A7";
        break;
    case GV2::A8:
        ret = "A8";
        break;
    case GV2::A9:
        ret = "A9";
        break;
    case GV2::B0:
        ret = "B0";
        break;
    case GV2::B1:
        ret = "B1";
        break;
    case GV2::B2:
        ret = "B2";
        break;
    case GV2::B3:
        ret = "B3";
        break;
    case GV2::B4:
        ret = "B4";
        break;
    case GV2::B5:
        ret = "B5";
        break;
    case GV2::B6:
        ret = "B6";
        break;
    case GV2::B7:
        ret = "B7";
        break;
    case GV2::B8:
        ret = "B8";
        break;
    case GV2::B9:
        ret = "B9";
        break;
    case GV2::B10:
        ret = "B10";
        break;
        /*
           case GV2::C0:
               ret = "C0";
               break;
           case GV2::C1:
               ret = "C1";
               break;
           case GV2::C2:
               ret = "C2";
               break;
           case GV2::C3:
               ret = "C3";
               break;
           case GV2::C4:
               ret = "C4";
               break;
           case GV2::C5:
               ret = "C5";
               break;
           case GV2::C6:
               ret = "C6";
               break;
           case GV2::C7:
               ret = "C7";
               break;
           case GV2::C8:
               ret = "C8";
               break;
           case GV2::C9:
               ret = "C9";
               break;
           case GV2::C10:
               ret = "C10";
               break;
        */
    case GV2::C5E:
        ret = "C5E";
        break;
    case GV2::Comm10E:
        ret = "Comm10E";
        break;
    case GV2::DLE:
        ret = "DLE";
        break;
    case GV2::Folio:
        ret = "Folio";
        break;
    //case GV2::Ledger:
    //    ret = "Ledger";
    //    break;
    case GV2::Tabloid:
        ret = "Tabloid";
        break;
    case GV2::NPageSize:
        ret = "NPageSize";
        break;
    default:
        break;
    }

    return ret;
}



/**
 * Converts a string to a paper format.
 */
GV2::PaperFormat GV_Units::stringToPaperFormat(const QString& p) {
    QString ls = p.toLower();
    GV2::PaperFormat ret = GV2::Custom;

    if (p=="custom") {
        ret = GV2::Custom;
    } else if (p=="letter") {
        ret = GV2::Letter;
    } else if (p=="legal") {
        ret = GV2::Legal;
    } else if (p=="executive") {
        ret = GV2::Executive;
    } else if (p=="a0") {
        ret = GV2::A0;
    } else if (p=="a1") {
        ret = GV2::A1;
    } else if (p=="a2") {
        ret = GV2::A2;
    } else if (p=="a3") {
        ret = GV2::A3;
    } else if (p=="a4") {
        ret = GV2::A4;
    } else if (p=="a5") {
        ret = GV2::A5;
    } else if (p=="a6") {
        ret = GV2::A6;
    } else if (p=="a7") {
        ret = GV2::A7;
    } else if (p=="a8") {
        ret = GV2::A8;
    } else if (p=="a9") {
        ret = GV2::A9;
    } else if (p=="b0") {
        ret = GV2::B0;
    } else if (p=="b1") {
        ret = GV2::B1;
    } else if (p=="b2") {
        ret = GV2::B2;
    } else if (p=="b3") {
        ret = GV2::B3;
    } else if (p=="b4") {
        ret = GV2::B4;
    } else if (p=="b5") {
        ret = GV2::B5;
    } else if (p=="b6") {
        ret = GV2::B6;
    } else if (p=="b7") {
        ret = GV2::B7;
    } else if (p=="b8") {
        ret = GV2::B8;
    } else if (p=="b9") {
        ret = GV2::B9;
    } else if (p=="b10") {
        ret = GV2::B10;
    }
    /*else if (p=="c0") {
           ret = GV2::C0;
       } else if (p=="c1") {
           ret = GV2::C1;
       } else if (p=="c2") {
           ret = GV2::C2;
       } else if (p=="c3") {
           ret = GV2::C3;
       } else if (p=="c4") {
           ret = GV2::C4;
       } else if (p=="c5") {
           ret = GV2::C5;
       } else if (p=="c6") {
           ret = GV2::C6;
       } else if (p=="c7") {
           ret = GV2::C7;
       } else if (p=="c8") {
           ret = GV2::C8;
       } else if (p=="c9") {
           ret = GV2::C9;
       } else if (p=="c10") {
           ret = GV2::C10;
       }*/
    else if (p=="c5e") {
        ret = GV2::C5E;
    } else if (p=="comm10e") {
        ret = GV2::Comm10E;
    } else if (p=="dle") {
        ret = GV2::DLE;
    } else if (p=="folio") {
        ret = GV2::Folio;
    //} else if (p=="ledger") {
    //    ret = GV2::Ledger;
    } else if (p=="tabloid") {
        ret = GV2::Tabloid;
    } else if (p=="npagesize") {
        ret = GV2::NPageSize;
    }

    return ret;
}

