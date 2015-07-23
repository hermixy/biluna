/*****************************************************************
 * $Id: rb_printer.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jun 28, 2010 9:04:25 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PRINTER_H
#define RB_PRINTER_H

#include <QPrinter>
#include <QPrinterInfo>
#include "db_global.h"
#include "rb_debug.h"
#include "rb_string.h"
#include "rb_stringlist.h"

#define RB_PRINTER RB_Printer::getInstance()


/**
 * Singleton giving helper functons based on QPrinter and QPrinterInfo
 */
class DB_EXPORT RB_Printer {

public:
    virtual ~RB_Printer();
    static RB_Printer* getInstance();

    RB_StringList getPaperFormats();
    QSizeF getPaperSize(const RB_String& pageFormat);
    QSizeF getPaperSize(QPrinter::PageSize pageSize);

    QPrinter::PaperSize paperFormatToEnum(const RB_String& paperFormat) const;
    QPrinter::PaperSize sequenceNumberToEnum(int seqNum) const;
    RB_String paperEnumToFormat(QPrinter::PaperSize paperEnum) const;

private:
    RB_Printer();

    //! Unique instance
    static RB_Printer* mActivePrinter;
};

#endif // RB_PRINTER_H
