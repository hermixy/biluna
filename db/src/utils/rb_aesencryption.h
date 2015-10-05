/*****************************************************************
 * $Id: rb_aesencryption.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jun 13, 2012 9:43:49 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_AESENCRYPTION_H
#define RB_AESENCRYPTION_H


#include <openssl/evp.h>
#include "db_global.h"
#include "rb_string.h"

#define AES_BLOCK_SIZE 16

/**
 * AES encryption using openSSL library
 * based on: http://saju.net.in/blog/?p=36
 * and http://cdmweb.de/openssl_aes.c.txt
 * more information also in crypto/evp/evp_test.c
 */
class DB_EXPORT RB_AesEncryption {

public:
    RB_AesEncryption();
    virtual ~RB_AesEncryption();

    virtual bool init(const RB_String& pwd);
    QByteArray encrypt(const RB_String& plainText);
    RB_String decrypt(const QByteArray& cipherText);

    int test(const RB_String& str);

private:
    int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt,
                 EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);
    unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
    unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);

    bool mIsInitialized;

    //! Encryption
    EVP_CIPHER_CTX mEn;
    //! Decryption
    EVP_CIPHER_CTX mDe;

};

#endif
