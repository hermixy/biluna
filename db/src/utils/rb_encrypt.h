/*****************************************************************
 * $Id: rb_encrypt.h 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 29, 2008 3:30:49 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ENCRYPT_H
#define RB_ENCRYPT_H

#include "pch.h"
#include "md5.h"
#include "sha.h"
#include "ripemd.h"
#include "files.h"
#include "rng.h"
#include "hex.h"
#include "gzip.h"
#include "default.h"
#include "rsa.h"
#include "randpool.h"
#include "ida.h"
#include "base64.h"
#include "socketft.h"
#include "dsa.h"
#include "rsa.h"
#include "osrng.h"
#include "wait.h"
#include "fips140.h"
#include "factory.h"

#include "bench.h"

#include <iostream>
#include <time.h>

#include "rb_string.h"

USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)


class RB_Encrypt {
	
public:
	RB_Encrypt();
	virtual ~RB_Encrypt();
	
	void generateKeys();
	bool symDecryption(const RB_String& source, const RB_String& target,
						const RB_String& pwd);
	bool symDecrFileToString(const RB_String& source, RB_String& target);
	bool symEncryption(const RB_String& source, const RB_String& target,
						const RB_String& pwd);
	bool symEncrStringToFile(const RB_String& sourceStr, 
						const RB_String& target);

	bool rsaDecryption(const RB_String& source, const RB_String& target);
	bool rsaDecrFileToString(const RB_String& source, RB_String& target);
	bool rsaEncryption(const RB_String& source, const RB_String& target);
	bool rsaEncrStringToFile(const RB_String& source, const RB_String& target);
	bool rsaEncrStringToString(const RB_String& source, RB_String& target);
	RB_String rsaFingerPrint(RB_String& licenseStr);

private:
	void GenerateRSAKey(unsigned int keyLength, const char *privFilename, 
			const char *pubFilename, const char *seed);
	string RSAEncryptString(const char *pubFilename, const char *seed, 
			const char *message);
	string RSADecryptString(const char *privFilename, const char *ciphertext);
	void RSAEncryptFile(const char *pubFilename, const char *seed, 
			const char* inFileName, const char* outFileName);
	void RSADecryptFile(const char *privFilename, const char* inFileName, 
			const char* outFileName);
	RandomNumberGenerator& GlobalRNG();
	
	const char* PRIVATE_KEY_FILE_NAME;
	const char* PUBLIC_KEY_FILE_NAME;
	const char* SEED_KEY;
	const char* SEED_ENCR;
	const char* PASSWORD;

};

#endif /*RB_ENCRYPT_H_*/
