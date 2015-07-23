/*****************************************************************
 * $Id: rb_authentication.h 1680 2012-06-23 15:08:20Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_AUTHENTICATION_H
#define RB_AUTHENTICATION_H

#include "rb_string.h"
#include "rb.h"


/**
 * Very simple hashing of password, this is only to make the password stored
 * on the local disk and in the database not directly readible. This is NOT
 * intended to be a security feature against database hackers.
 */
class RB_Authentication {
	
public:
	RB_Authentication();
	virtual ~RB_Authentication();
	
	bool writeEvalLicense();
	bool requestLicense(RB_String& licenseStr);
	bool activateLocalLicense(RB_String& licenseStr,
								const RB_String& activationKey);

	bool encryptLicenseRequest(const RB_String& licenseStr);
	bool decryptLicenseRequest(const RB_String& filePath, RB_String& licenseStr);
	
	RB2::LicenseType license(RB_String& licenseStr);


private:
	
};

#endif /*RB_AUTHENTICATION_H_*/
