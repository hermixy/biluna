/*****************************************************************
 * $Id: rb_hardware.h 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 4, 2008 7:57:35 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_HARDWARE_H
#define RB_HARDWARE_H

#include "rb_stringlist.h"
#include "rb_string.h"


/**
 * Class to get hardware information
 */
class RB_Hardware {
	
public:
	RB_Hardware();
	virtual ~RB_Hardware();
	
	virtual RB_String getEthernetCardNumber();
	virtual const char* getIpConfig(RB_String& ipStr);
};

#endif /*RB_HARDWARE_H*/
