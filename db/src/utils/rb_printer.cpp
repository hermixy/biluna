/*****************************************************************
 * $Id: rb_printer.cpp 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 28, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_printer.h"

RB_Printer* RB_Printer::mActivePrinter = NULL;

/**
 * Constructor
 */
RB_Printer::RB_Printer() {
    RB_DEBUG->print("RB_Printer::RB_Printer()");
}

/**
 * Destructor
 */
RB_Printer::~RB_Printer() {
    mActivePrinter = NULL;
    RB_DEBUG->print("RB_Printer::~RB_Printer() OK");
}

/**
 * Create instance of factory
 */
RB_Printer* RB_Printer::getInstance() {
    if (!mActivePrinter) {
        mActivePrinter = new RB_Printer();
    }
    return mActivePrinter;
}

/**
 * Get paper formats of printer or get all paper format list
 */
RB_StringList RB_Printer::getPaperFormats() {
//    QPrinterInfo prInfo;
//    QList<QPrinter::PaperSize> enumList = prInfo.supportedPaperSizes();
    RB_StringList strL;

//    if (enumList.size() > 0) {
//        for (int i = 0; i < enumList.size(); i++) {
//            strL.append(this->paperEnumToFormat(enumList.at(i)));
//        }
//    } else {
        strL << "A0" << "A1" << "A2" << "A3" << "A4" << "A5" << "A6" << "A7" << "A8" << "A9";
        strL << "B0" << "B1" << "B2" << "B3" << "B4" << "B5" << "B6" << "B7" << "B8" << "B9" << "B10";
        strL << "C5E" << "Comm10E" << "DLE"	<< "Executive" << "Folio" << "Ledger";
        strL << "Legal" << "Letter" << "Tabloid" << "Custom";
//    }

    return strL;
}

/**
 * Get QPrinter::PaperSize enum from sequence number as used in this class
 * sequence with getPaperFormats()
 */
QPrinter::PaperSize RB_Printer::sequenceNumberToEnum(int seqNum) const {
    switch (seqNum) {
    case 0:
        return QPrinter::A0;
    case 1:
        return QPrinter::A1;
    case 2:
        return QPrinter::A2;
    case 3:
        return QPrinter::A3;
    case 4:
        return QPrinter::A4;
    case 5:
        return QPrinter::A5;
    case 6:
        return QPrinter::A6;
    case 7:
        return QPrinter::A7;
    case 8:
        return QPrinter::A8;
    case 9:
        return QPrinter::A9;
    case 10:
        return QPrinter::B0;
    case 11:
        return QPrinter::B1;
    case 12:
        return QPrinter::B2;
    case 13:
        return QPrinter::B3;
    case 14:
        return QPrinter::B4;
    case 15:
        return QPrinter::B5;
    case 16:
        return QPrinter::B6;
    case 17:
        return QPrinter::B7;
    case 18:
        return QPrinter::B8;
    case 19:
        return QPrinter::B9;
    case 20:
        return QPrinter::B10;
    case 21:
        return QPrinter::C5E;
    case 22:
        return QPrinter::Comm10E;
    case 23:
        return QPrinter::DLE;
    case 24:
        return QPrinter::Executive;
    case 25:
        return QPrinter::Folio;
    case 26:
        return QPrinter::Ledger;
    case 27:
        return QPrinter::Legal;
    case 28:
        return QPrinter::Letter;
    case 29:
        return QPrinter::Tabloid;
    default:
        return QPrinter::Custom;
    }

    return QPrinter::Custom;
}

/**
 * Get paper size
 * @param pageFormat name of paper format e.g.: A4
 * @returns Paper size, width and height in millimeters
 */
QSizeF RB_Printer::getPaperSize(const RB_String& pageFormat) {
    return getPaperSize(paperFormatToEnum(pageFormat));
}

/**
 * Get paper size
 * @param pageSize QPrinter::PageSize enumerator
 * @returns Paper size, width and height in millimeters
 */
QSizeF RB_Printer::getPaperSize(QPrinter::PageSize pageSize) {
    QSizeF size;

    switch (pageSize) {
    case QPrinter::A0 :
        size = QSizeF(841, 1189);
        break;
    case QPrinter::A1 :
        size = QSizeF(594, 841);
        break;
    case QPrinter::A2 :
        size = QSizeF(420, 594);
        break;
    case QPrinter::A3 :
        size = QSizeF(297, 420);
        break;
    case QPrinter::A4 :
        size = QSizeF(210, 297);
        break;
    case QPrinter::A5 :
        size = QSizeF(148, 210);
        break;
    case QPrinter::A6 :
        size = QSizeF(105, 148);
        break;
    case QPrinter::A7 :
        size = QSizeF(74, 105);
        break;
    case QPrinter::A8 :
        size = QSizeF(52, 74);
        break;
    case QPrinter::A9 :
        size = QSizeF(37, 52);
        break;
    case QPrinter::B0 :
        size = QSizeF(1000, 1414);
        break;
    case QPrinter::B1 :
        size = QSizeF(707, 1000);
        break;
    case QPrinter::B2 :
        size = QSizeF(500, 707);
        break;
    case QPrinter::B3 :
        size = QSizeF(353, 500);
        break;
    case QPrinter::B4 :
        size = QSizeF(250, 353);
        break;
    case QPrinter::B5 :
        size = QSizeF(176, 250);
        break;
    case QPrinter::B6 :
        size = QSizeF(125, 176);
        break;
    case QPrinter::B7 :
        size = QSizeF(88, 125);
        break;
    case QPrinter::B8 :
        size = QSizeF(62, 88);
        break;
    case QPrinter::B9 :
        size = QSizeF(33, 62);
        break;
    case QPrinter::B10 :
        size = QSizeF(31, 44);
        break;
    case QPrinter::C5E :
        size = QSizeF(163, 229);
        break;
    case QPrinter::Comm10E :
        size = QSizeF(105, 241);
        break;
    case QPrinter::DLE :
        size = QSizeF(110, 220);
        break;
    case QPrinter::Executive :
        size = QSizeF(190.5, 254);
        break;
    case QPrinter::Folio :
        size = QSizeF(210, 330);
        break;
    case QPrinter::Ledger :
        size = QSizeF(431.8, 279.4);
        break;
    case QPrinter::Legal :
        size = QSizeF(215.9, 355.6);
        break;
    case QPrinter::Letter :
        size = QSizeF(215.9, 279.4);
        break;
    case QPrinter::Tabloid :
        size = QSizeF(279.4, 431.8);
        break;
    default :
        break;
    }

    // QPrinter::Custom
    return size;
}

/**
 * Paper format name (e.g A4) to Qt QPrinter::PaperSize enumerator
 * @param paperFormat for example A4
 * @return Qt QPrinter::PaperSize enumerator
 */
QPrinter::PaperSize RB_Printer::paperFormatToEnum(const RB_String& paperFormat) const {
    if (paperFormat == "A0") {
        return QPrinter::A0;
    } else if (paperFormat == "A1") {
        return QPrinter::A1;
    } else if (paperFormat == "A2") {
        return QPrinter::A2;
    } else if (paperFormat == "A3") {
        return QPrinter::A3;
    } else if (paperFormat == "A4") {
        return QPrinter::A4;
    } else if (paperFormat == "A5") {
        return QPrinter::A5;
    } else if (paperFormat == "A6") {
        return QPrinter::A6;
    } else if (paperFormat == "A7") {
        return QPrinter::A7;
    } else if (paperFormat == "A8") {
        return QPrinter::A8;
    } else if (paperFormat == "A9") {
        return QPrinter::A9;
    } else if (paperFormat == "B0") {
        return QPrinter::B0;
    } else if (paperFormat == "B1") {
        return QPrinter::B1;
    } else if (paperFormat == "B2") {
        return QPrinter::B2;
    } else if (paperFormat == "B3") {
        return QPrinter::B3;
    } else if (paperFormat == "B4") {
        return QPrinter::B4;
    } else if (paperFormat == "B5") {
        return QPrinter::B5;
    } else if (paperFormat == "B6") {
        return QPrinter::B6;
    } else if (paperFormat == "B7") {
        return QPrinter::B7;
    } else if (paperFormat == "B8") {
        return QPrinter::B8;
    } else if (paperFormat == "B9") {
        return QPrinter::B9;
    } else if (paperFormat == "B10") {
        return QPrinter::B10;
    } else if (paperFormat == "C5E") {
        return QPrinter::C5E;
    } else if (paperFormat == "Comm10E") {
        return QPrinter::Comm10E;
    } else if (paperFormat == "DLE") {
        return QPrinter::DLE;
    } else if (paperFormat == "Executive") {
        return QPrinter::Executive;
    } else if (paperFormat == "Folio") {
        return QPrinter::Folio;
    } else if (paperFormat == "Ledger") {
        return QPrinter::Ledger;
    } else if (paperFormat == "Legal") {
        return QPrinter::Legal;
    } else if (paperFormat == "Letter") {
        return QPrinter::Letter;
    } else if (paperFormat == "Tabloid") {
        return QPrinter::Tabloid;
    }
    return QPrinter::Custom;
}

/**
 * Qt QPrinter::PaperSize enumerator to paper format name (e.g A4)
 */
RB_String RB_Printer::paperEnumToFormat(QPrinter::PaperSize paperEnum) const {
    RB_String str;
    switch (paperEnum) {
    case QPrinter::A0 :
        str = "A0";
        break;
    case QPrinter::A1 :
        str = "A1";
        break;
    case QPrinter::A2 :
        str = "A2";
        break;
    case QPrinter::A3 :
        str = "A3";
        break;
    case QPrinter::A4 :
        str = "A4";
        break;
    case QPrinter::A5 :
        str = "A5";
        break;
    case QPrinter::A6 :
        str = "A6";
        break;
    case QPrinter::A7 :
        str = "A7";
        break;
    case QPrinter::A8 :
        str = "A8";
        break;
    case QPrinter::A9 :
        str = "A9";
        break;
    case QPrinter::B0 :
        str = "B0";
        break;
    case QPrinter::B1 :
        str = "B1";
        break;
    case QPrinter::B2 :
        str = "B2";
        break;
    case QPrinter::B3 :
        str = "B3";
        break;
    case QPrinter::B4 :
        str = "B4";
        break;
    case QPrinter::B5 :
        str = "B5";
        break;
    case QPrinter::B6 :
        str = "B6";
        break;
    case QPrinter::B7 :
        str = "B7";
        break;
    case QPrinter::B8 :
        str = "B8";
        break;
    case QPrinter::B9 :
        str = "B9";
        break;
    case QPrinter::B10 :
        str = "B1";
        break;
    case QPrinter::C5E :
        str = "C5E";
        break;
    case QPrinter::Comm10E :
        str = "Comm10E";
        break;
    case QPrinter::DLE :
        str = "DLE";
        break;
    case QPrinter::Executive :
        str = "Executive";
        break;
    case QPrinter::Folio :
        str = "Folio";
        break;
    case QPrinter::Ledger :
        str = "Ledger";
        break;
    case QPrinter::Legal :
        str = "Legal";
        break;
    case QPrinter::Letter :
        str = "Letter";
        break;
    case QPrinter::Tabloid :
        str = "Tabloid";
        break;
    default  : //QPrinter::Custom	30	Unknown, or a user defined size.
            str = "Custom";
            break;
    }

    return str;
}

/*
QPrinter::A0	5	841 x 1189 mm
QPrinter::A1	6	594 x 841 mm
QPrinter::A2	7	420 x 594 mm
QPrinter::A3	8	297 x 420 mm
QPrinter::A4	0	210 x 297 mm, 8.26 x 11.69 inches
QPrinter::A5	9	148 x 210 mm
QPrinter::A6	10	105 x 148 mm
QPrinter::A7	11	74 x 105 mm
QPrinter::A8	12	52 x 74 mm
QPrinter::A9	13	37 x 52 mm
QPrinter::B0	14	1000 x 1414 mm
QPrinter::B1	15	707 x 1000 mm
QPrinter::B2	17	500 x 707 mm
QPrinter::B3	18	353 x 500 mm
QPrinter::B4	19	250 x 353 mm
QPrinter::B5	1	176 x 250 mm, 6.93 x 9.84 inches
QPrinter::B6	20	125 x 176 mm
QPrinter::B7	21	88 x 125 mm
QPrinter::B8	22	62 x 88 mm
QPrinter::B9	23	33 x 62 mm
QPrinter::B10	16	31 x 44 mm
QPrinter::C5E	24	163 x 229 mm
QPrinter::Comm10E	25	105 x 241 mm, U.S. Common 10 Envelope
QPrinter::DLE	26	110 x 220 mm
QPrinter::Executive	4	7.5 x 10 inches, 190.5 x 254 mm
QPrinter::Folio	27	210 x 330 mm
QPrinter::Ledger	28	431.8 x 279.4 mm
QPrinter::Legal	3	8.5 x 14 inches, 215.9 x 355.6 mm
QPrinter::Letter	2	8.5 x 11 inches, 215.9 x 279.4 mm
QPrinter::Tabloid	29	279.4 x 431.8 mm
QPrinter::Custom	30	Unknown, or a user defined size.
*/
