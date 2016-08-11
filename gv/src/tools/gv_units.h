/****************************************************************************
** $Id: gv_units.h 7802 2008-01-03 10:30:18Z andrew $
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


#ifndef GV_UNITS_H
#define GV_UNITS_H

#include "gv.h"
#include <QString>
#include "gv_vector.h"

/**
 * Conversion methods for units
 *
 * @author Andrew Mustun
 */
class GV_Units {
public:
    static GV2::Unit dxfint2unit(int dxfint);

    static QString unitToString(GV2::Unit u, bool t = true);
    static GV2::Unit stringToUnit(const QString& u);

    static bool isMetric(GV2::Unit u);
    static double getFactorToMM(GV2::Unit u);
    static double convert(double val, GV2::Unit src, GV2::Unit dest);
    static GV_Vector convert(const GV_Vector val, GV2::Unit src, GV2::Unit dest);
    
    static QString unitToSign(GV2::Unit u);

    static QString formatLinear(double length, GV2::Unit unit,
                                  GV2::LinearFormat format,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString formatScientific(double length, GV2::Unit unit,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString formatDecimal(double length, GV2::Unit unit,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString formatEngineering(double length, GV2::Unit unit,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString formatArchitectural(double length, GV2::Unit unit,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString formatFractional(double length, GV2::Unit unit,
                                  int prec, bool showUnit=false,
                                  bool showLeadingZeroes=true, 
                                  bool showTrailingZeroes=false);
    static QString trimTrailingZeroes(const QString& s);

    static QString formatAngle(double angle, GV2::AngleFormat format,
                                 int prec,
                                 bool showLeadingZeroes=true,
                                 bool showTrailingZeroes=false);

    static GV_Vector paperFormatToSize(GV2::PaperFormat p);
    static GV2::PaperFormat paperSizeToFormat(const GV_Vector s);
    
    static QString paperFormatToString(GV2::PaperFormat p);
    static GV2::PaperFormat stringToPaperFormat(const QString& p);

    static void test();
};


#endif
