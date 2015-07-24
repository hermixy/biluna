/*****************************************************************
 * $Id: scan.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: 2011-04-17 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_H
#define SCAN_H


/**
 * Class namespace for various enumerators.
 */
class SCAN2 {

public:


    /**
     * Type of scan
     */
    enum ScanType {
        ScanNone,                           /**< Invalid or no scan */

        ScanMCAllValued,                    /**< Multiple choice scan, all answers valued */
        ScanMCOneGood,                      /**< Multiple choice scan, only one good answer */
        ScanOpenQuestion,                   /**< Open questions scan */

        ScanDefault                         /**< Default or all scan */
    };

};

#endif // SCAN_H
