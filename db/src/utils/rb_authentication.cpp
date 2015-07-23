/*****************************************************************
 * $Id: rb_authentication.cpp 1680 2012-06-23 15:08:20Z rutger $
 *
 * Copyright (C) 2006 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_authentication.h"

#include <QDate>
#include <QFile>
#include <QTextStream>
#include "rb_debug.h"
#include "rb_encrypt.h"
#include "rb_hardware.h"
#include "rb_settings.h"
#include "rb_string.h"
#include "rb_uuid.h"


RB_Authentication::RB_Authentication() {

}

RB_Authentication::~RB_Authentication() {
	
	
}

/**
 * User has selected to evaluate the software. A demo license file with
 * the start date and the end date will be written to the data/liceval.d file
 */
bool RB_Authentication::writeEvalLicense() {
	RB_SETTINGS->beginGroup("paths");
    RB_String dataPath = RB_SETTINGS->value("dataPath", "data").toString();
	RB_SETTINGS->endGroup();
	
	dataPath += "/liceval.d";
	
	if (QFile::exists(dataPath)) {
		return false;
	}
	
	QDate dateStart;
	dateStart.setDate(QDate::currentDate().year(), QDate::currentDate().month(),
			QDate::currentDate().day());
	QDate dateEnd = dateStart.addDays(30);

	RB_String licenseStr = dateStart.toString(Qt::ISODate);
	licenseStr += ";";
	licenseStr += dateEnd.toString(Qt::ISODate);
	licenseStr += ";";
	licenseStr += RB_Uuid::createUuid();
	
	RB_Encrypt encr;
	return encr.symEncrStringToFile(licenseStr, dataPath);
}

/**
 * Create license string
 * @param licenseStr string for license with already the application name and
 * the version number separated by semicolon
 */
bool RB_Authentication::requestLicense(RB_String& licenseStr) {
	RB_Hardware hw;
	hw.getEthernetCardNumber();
	
	licenseStr += ";";
	licenseStr += hw.getEthernetCardNumber();
	licenseStr += ";";
	licenseStr += RB_Uuid::createUuid();
	
	return encryptLicenseRequest(licenseStr);
}

/**
 * Activate a local machine bound license
 * @param licenseStr license string containing the application name and version
 * @param activationKey activation or registration key as entered by the user
 * @return success true if correct activation key
 */
bool RB_Authentication::activateLocalLicense(RB_String& licenseStr,
											const RB_String& activationKey) {
	// Read license request file
	RB_SETTINGS->beginGroup("paths");
    RB_String dataPath = RB_SETTINGS->value("dataPath", "data").toString();
	RB_SETTINGS->endGroup();
	
	RB_String fileStr = dataPath + "/licreq.d";

	if (!QFile::exists(fileStr)) {
		return false;
	}
	
	QFile licReqFile(fileStr);

	if (!licReqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&licReqFile);
    RB_String encrLicReqStr;
    
    while (!in.atEnd()) {
    	encrLicReqStr += in.readLine(75); // max 75, no trailing zero
    }

	// Decrypt license request file
    RB_String decrLicReqStr;
	RB_Encrypt encr;
	encr.symDecrFileToString(fileStr, decrLicReqStr);

    // Create activation test license string
	RB_Hardware hw;
	licenseStr += ";";
	licenseStr += hw.getEthernetCardNumber();
	
	RB_String testLicenseStr;
	testLicenseStr = licenseStr;
	testLicenseStr += ";";
	testLicenseStr += activationKey;

	// Check if license strings are the same. If yes, the final license file
	// can be written.
	if (testLicenseStr != decrLicReqStr) {
		return false;
	}
	
	// activation code OK, write license file
	fileStr = dataPath + "/licloc.d";
	encr.symEncrStringToFile(licenseStr, fileStr);
	
	return true;
}

/**
 * Encrypt and write the license request file with password data/licreq.d
 * @param licenseStr license string with software name, version, MAC address 
 * and Uuid separated by semicolons
 * @return success or false if file exists or could not write file 
 */
bool RB_Authentication::encryptLicenseRequest(const RB_String& licenseStr) {
	RB_SETTINGS->beginGroup("paths");
    RB_String dataPath = RB_SETTINGS->value("dataPath", "data").toString();
	RB_SETTINGS->endGroup();
	
	dataPath += "/licreq.d";

	if (QFile::exists(dataPath)) {
		return false;
	}

	RB_Encrypt encr;
	return encr.symEncrStringToFile(licenseStr, dataPath);
	
	// TODO: Did not use rsa encryption because the file written with a public key
	// is always different and needs the private key to decrypt. Therefor the
	// symmetric key is used. However this should be revisited in the future
	// Note: the symmetric file is also different each time.
	// return encr.rsaEncrStringToFile(licenseStr, dataPath);
}

/**
 * Decrypt the license request. This funcion is NOT part of the commercial
 * applications and only to be used by the software company license provider
 * @param licenseStr encrypted license request string with password (Uuid)
 * @return success of false if could not decrypt the data string
 */
bool RB_Authentication::decryptLicenseRequest(const RB_String& filePath, 
												RB_String& licenseStr) {
	if (!QFile::exists(filePath)) {
		return false;
	}
	
	RB_Encrypt encr;
	return encr.symDecrFileToString(filePath, licenseStr);
	
	// TODO: see above rsa comment
	// return encr.rsaDecrFileToString(filePath, licenseStr);
}

/**
 * @param licenseStr license string with already application name and version
 * @return current license invalid, evaluation, in progress, local user or
 * network user.
 */
RB2::LicenseType RB_Authentication::license(RB_String& licenseStr) {
	RB_SETTINGS->beginGroup("paths");
    RB_String dataPath = RB_SETTINGS->value("dataPath", "data").toString();
	RB_SETTINGS->endGroup();
	
	RB_Hardware hw;
	licenseStr += ";";
	licenseStr += hw.getEthernetCardNumber();

	// TODO: RB2::LicenseNetworkUser
	
	// Local user license
	RB_String fileStr = dataPath + "/licloc.d";
	RB_Encrypt encr;
	
	if (QFile::exists(fileStr)) {
		QFile sourceFile(fileStr);
		
	    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
	        return RB2::LicenseNone;;
	    }
	    
	    // content of license file
		RB_String licStr; 
	    QTextStream in(&sourceFile);
	    
	    while (!in.atEnd()) {
	    	licStr += in.readLine(75); // max 75, no trailing zero
	    }

		RB_String decrLicenseFile;
	    encr.symDecrFileToString(fileStr, decrLicenseFile);
	    
	    if (decrLicenseFile == licenseStr) {
	    	return RB2::LicenseLocalUser;
	    } else {
	    	return RB2::LicenseNone;
	    }
	} 
	
	// License request in progress
	fileStr = dataPath + "/licreq.d";
	
	if (QFile::exists(fileStr)) {
		return RB2::LicenseInProgress;
	}
	
	// Evaluation license
	fileStr = dataPath + "/liceval.d";
	
	if (QFile::exists(fileStr)) {
		RB_String licEval = "";
		encr.symDecrFileToString(fileStr, licEval);
		
		RB_StringList strList = licEval.split(";");
		QDate startDate = QDate::fromString(strList.at(0), Qt::ISODate); 
		QDate endDate = QDate::fromString(strList.at(1), Qt::ISODate);
		
		// debug only
		RB_String str = "Start date = ";
		str += startDate.toString();
		RB_DEBUG->print(str);
		str = "End date = ";
		str += endDate.toString();
		RB_DEBUG->print(str);
		
		if (startDate <= QDate::currentDate() && QDate::currentDate() <= endDate) {
			return RB2::LicenseEvaluation;
		} else {
			return RB2::LicenseNone;
		}
	}
	
	return RB2::LicenseNone;
}

