/*****************************************************************
 * $Id: rb_encrypt.cpp 1680 2012-06-23 15:08:20Z rutger $
 * Created: Jun 29, 2008 3:31:27 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_encrypt.h"

#include <QFile>
#include <QTextStream>
#include "rb_debug.h"
#include "rb_settings.h"
#include "rb_uuid.h"


RB_Encrypt::RB_Encrypt() {
	// Hack to get around implicit sharing of QString
	RB_SETTINGS->beginGroup("paths");
    std::string dataPathPrk = 
    	RB_SETTINGS->value("dataPath", "data").toString().toStdString();
    std::string dataPathPuk = 
    	RB_SETTINGS->value("dataPath", "data").toString().toStdString();
	RB_SETTINGS->endGroup();

	dataPathPrk += "/prk.d";
	dataPathPuk += "/puk.d";
	
	PRIVATE_KEY_FILE_NAME = dataPathPrk.c_str();
	PUBLIC_KEY_FILE_NAME = dataPathPuk.c_str();
	SEED_KEY = "{d6cd3c73-c222-4d0c-a641-77073bcea05d}";
	SEED_ENCR = "{1ede1d41-488b-4c1c-8e84-195950e10456}"; 
	//SEED_ENCR = RB_Uuid::createUuid().toString().toStdString().c_str(); gave problems
	PASSWORD = "{13b8a715-c6d9-46ba-afdb-16336d541b43}";
}

RB_Encrypt::~RB_Encrypt() {
	
}

/**
 * Generate public and private key. Function is for initial key generation only.
 * @param str string to be shown on screen
 */
void RB_Encrypt::generateKeys() {
	try {
		int keyLength = 1024;
		GenerateRSAKey(keyLength, PRIVATE_KEY_FILE_NAME, 
				PUBLIC_KEY_FILE_NAME, SEED_KEY);
	}

	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		// return -1;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		// return -2;
	}
}

/**
 * RSA decryption using public key, based on crypto-eg example
 * @param pwd password
 * @source source file name (max. 1024 characters)
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::rsaDecryption(const RB_String& source,	const RB_String& target) {
	std::cout << "private key: " << PRIVATE_KEY_FILE_NAME << std::endl;
	
	QFile sourceFile(source);
	QFile targetFile(target);
	QFile prkFile(PRIVATE_KEY_FILE_NAME);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)
    		|| !prkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::rsaDecryption() "
    			"source-, target- or puk file ERROR");
        return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		targetStr = RSADecryptString(PRIVATE_KEY_FILE_NAME, 
				sourceStr.toStdString().c_str());
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * RSA decryption using public key, based on crypto-eg example
 * @param pwd password
 * @source source file name (max. 1024 characters)
 * @target target string
 */
bool RB_Encrypt::rsaDecrFileToString(const RB_String& source, RB_String& target) {
	std::cout << "private key: " << PRIVATE_KEY_FILE_NAME << std::endl;
	
	QFile sourceFile(source);
//	QFile targetFile(target);
	QFile prkFile(PRIVATE_KEY_FILE_NAME);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		/* || !targetFile.open(QIODevice::WriteOnly | QIODevice::Text) */
    		|| !prkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::rsaDecrFileToString() "
    			"source- or prk file ERROR");
        return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		targetStr = RSADecryptString(PRIVATE_KEY_FILE_NAME, 
				sourceStr.toStdString().c_str());
		target = RB_String(targetStr.c_str());
		
//		QTextStream out(&targetFile);
//		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * RSA encryption using public key (file to file), based on crypto-eg example
 * @source source file name (max. 1024 characters)
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::rsaEncryption(const RB_String& source,	const RB_String& target) {
	// std::cout << "public key: " << PUBLIC_KEY_FILE_NAME << std::endl; 
	
	QFile sourceFile(source);
	QFile targetFile(target);
	QFile pukFile(PUBLIC_KEY_FILE_NAME);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)
    		|| !pukFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::rsaEncryption() "
    			"source-, target- or puk file ERROR");
    	return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		targetStr = RSAEncryptString(PUBLIC_KEY_FILE_NAME, SEED_ENCR, 
				sourceStr.toStdString().c_str());
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * RSA encryption using public key (string to file), based on crypto-eg example
 * @source source string
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::rsaEncrStringToFile(const RB_String& source, 
										const RB_String& target) {
	// std::cout << "public key: " << PUBLIC_KEY_FILE_NAME << std::endl; 
	
	// QFile sourceFile(source);
	QFile targetFile(target);
	QFile pukFile(PUBLIC_KEY_FILE_NAME);
	RB_String sourceStr = source; // = "";
	std::string targetStr = "";
	
    if (/* !sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| */ !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)
    		|| !pukFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::rsaEncrStringToFile() "
    			"target file or puk file ERROR");
        return false;
    }

//    QTextStream in(&sourceFile);
//    
//    while (!in.atEnd()) {
//    	sourceStr += in.readLine(75); // max 75, no trailing zero
//    }
	     
	try {	
		targetStr = RSAEncryptString(PUBLIC_KEY_FILE_NAME, SEED_ENCR, 
				sourceStr.toStdString().c_str());
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * RSA encryption using public key (string to string), based on crypto-eg example
 * @source source string
 * @target target string
 */
bool RB_Encrypt::rsaEncrStringToString(const RB_String& source, 
										RB_String& target) {
	// std::cout << "public key: " << PUBLIC_KEY_FILE_NAME << std::endl; 
	
	// QFile sourceFile(source);
	// QFile targetFile(target);
	QFile pukFile(PUBLIC_KEY_FILE_NAME);
	RB_String sourceStr = source; // = "";
	std::string targetStr = "";
	
    if (/* !sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)
    		|| */ !pukFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::rsaEncrStringToString() "
    			"puk file ERROR");
        return false;
    }

//    QTextStream in(&sourceFile);
//    
//    while (!in.atEnd()) {
//    	sourceStr += in.readLine(75); // max 75, no trailing zero
//    }
	     
	try {	
		targetStr = RSAEncryptString(PUBLIC_KEY_FILE_NAME, SEED_ENCR, 
				sourceStr.toStdString().c_str());
		target = RB_String(targetStr.c_str());
		
//		QTextStream out(&targetFile);
//		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * Function is used only in case of errors, to send user information to 
 * application provider
 * @param licenseStr license string with application name, version and MAC address
 * @return Finger print of this computer and software
 */
RB_String RB_Encrypt::rsaFingerPrint(RB_String& licenseStr) {
	std::string targetStr = RSAEncryptString(PUBLIC_KEY_FILE_NAME, SEED_ENCR, 
			licenseStr.toStdString().c_str());
	return RB_String(licenseStr); 
}

/**
 * Symmetric decryption using password/passphrase, based on crypto-eg example
 * @param pwd password
 * @source source file name (max. 1024 characters)
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::symDecryption(const RB_String& source,	const RB_String& target,
								const RB_String& pwd) {
	QFile sourceFile(source);
	QFile targetFile(target);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::symDecryption() "
    			"source- or target file ERROR");
    	return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		StringSource s(sourceStr.toStdString(), true, new Base64Decoder(
						new DefaultDecryptorWithMAC(pwd.toStdString().c_str(), 
						new StringSink(targetStr))));
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * Symmetric decryption using password/passphrase, based on crypto-eg example
 * @param pwd password
 * @source source file name (max. 1024 characters)
 * @target target string
 */
bool RB_Encrypt::symDecrFileToString(const RB_String& source, 
											RB_String& target) {
	QFile sourceFile(source);
	// QFile targetFile(target);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    	/*	|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text) */ ) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::symDecrFileToString() "
    			"source file ERROR");
        return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		RB_String pwd = PASSWORD;
		StringSource s(sourceStr.toStdString(), true, new Base64Decoder(
						new DefaultDecryptorWithMAC(pwd.toStdString().c_str(), 
						new StringSink(targetStr))));
		
		// QTextStream out(&targetFile);
		// out << RB_String(targetStr.c_str());
		target = RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * Symmetric encryption using password/passphrase, based on crypto-eg example
 * @param pwd password
 * @source source file name (max. 1024 characters)
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::symEncryption(const RB_String& source,	const RB_String& target,
								const RB_String& pwd) {
	QFile sourceFile(source);
	QFile targetFile(target);
	RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::symEncryption() "
    			"source- or target file ERROR");
    	return false;
    }

    QTextStream in(&sourceFile);
    
    while (!in.atEnd()) {
    	sourceStr += in.readLine(75); // max 75, no trailing zero
    }
	     
	try {	
		StringSource(sourceStr.toStdString(), true, new DefaultEncryptorWithMAC(
				pwd.toStdString().c_str(), 
				new Base64Encoder(new StringSink(targetStr))));
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * Symmetric encryption using password/passphrase, based on crypto-eg example
 * string to file
 * @param pwd password
 * @sourceStr source string
 * @target target file name (max. 1024 characters)
 */
bool RB_Encrypt::symEncrStringToFile(const RB_String& sourceStr, 
										const RB_String& target) {
	// QFile sourceFile(source);
	QFile targetFile(target);
	// RB_String sourceStr = "";
	std::string targetStr = "";
	
    if (/* !sourceFile.open(QIODevice::ReadOnly | QIODevice::Text) 
    		|| */ !targetFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    	RB_DEBUG->print(RB_Debug::D_ERROR, "RB_Encrypt::symEncrStringToFile() "
    			"target file ERROR");
    	return false;
    }

//    QTextStream in(&sourceFile);
//    
//    while (!in.atEnd()) {
//    	sourceStr += in.readLine(75); // max 75, no trailing zero
//    }
	     
	try {	
		RB_String pwd = PASSWORD;
		StringSource(sourceStr.toStdString(), true, new DefaultEncryptorWithMAC(
				pwd.toStdString().c_str(), 
				new Base64Encoder(new StringSink(targetStr))));
		
		QTextStream out(&targetFile);
		out << RB_String(targetStr.c_str());
	}
	catch(CryptoPP::Exception &e) {
		cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		return false;
	} 
	catch(std::exception &e) {
		cout << "\nstd::exception caught: " << e.what() << endl;
		return false;
	}
	
	return true;
}

/**
 * Refer to cryptopp552
 */
RandomNumberGenerator& RB_Encrypt::GlobalRNG() {
	static RandomPool randomPool;
	return randomPool;
}

/**
 * Refer to cryptopp552
 */
void RB_Encrypt::GenerateRSAKey(unsigned int keyLength, const char *privFilename, 
						const char *pubFilename, const char *seed) {
	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed, strlen(seed));

	RSAES_OAEP_SHA_Decryptor priv(randPool, keyLength);
	HexEncoder privFile(new FileSink(privFilename));
	priv.DEREncode(privFile);
	privFile.MessageEnd();

	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pubFilename));
	pub.DEREncode(pubFile);
	pubFile.MessageEnd();
}

/**
 * Refer to cryptopp552
 */
string RB_Encrypt::RSAEncryptString(const char *pubFilename, const char *seed, 
												const char *message) {
	FileSource pubFile(pubFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed, strlen(seed));

	string result;
	StringSource(message, true, new PK_EncryptorFilter(randPool, pub, 
			new HexEncoder(new StringSink(result))));
	return result;
}

/**
 * Refer to cryptopp552
 */
string RB_Encrypt::RSADecryptString(const char *privFilename, 
												const char *ciphertext) {
	FileSource privFile(privFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	string result;
	StringSource(ciphertext, true, new HexDecoder(
			new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(result))));
	return result;
}

/**
 * Refer to cryptopp552
 */
void RB_Encrypt::RSAEncryptFile(const char *pubFilename, const char *seed, 
						const char* inFileName, const char* outFileName) {
	FileSource pubFile(pubFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed, strlen(seed));

	FileSource(inFileName, true, new PK_EncryptorFilter(randPool, pub, 
			new HexEncoder(new FileSink(outFileName))));
}

/**
 * Refer to cryptopp552
 */
void RB_Encrypt::RSADecryptFile(const char *privFilename, const char* inFileName, 
												const char* outFileName) {
	FileSource privFile(privFilename, true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	FileSource(inFileName, true, new HexDecoder(new PK_DecryptorFilter(
			GlobalRNG(), priv, new FileSink(outFileName, false))));
}
