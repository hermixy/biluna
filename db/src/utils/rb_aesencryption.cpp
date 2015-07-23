/*****************************************************************
 * $Id: rb_aesencryption.cpp 1973 2013-08-19 15:00:11Z rutger $
 * Created: Jun 13, 2012 9:43:49 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_aesencryption.h"

#include <QtCore>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rb_debug.h"

/**
 * Constructor
 */
RB_AesEncryption::RB_AesEncryption() {
    mIsInitialized = false;
}

RB_AesEncryption::~RB_AesEncryption() {
    // nothing
}

/**
 * Initialize
 * @param pwd encryption/decrytpion password
 */
bool RB_AesEncryption::init(const RB_String& pwd) {
    unsigned int salt[] = {12345, 54321};
    unsigned char* key_data;
    int key_data_len;

    const QByteArray ba = pwd.toLatin1();
    const char* constChar = ba.constData();
    key_data = (unsigned char*)qstrdup(constChar);
    key_data_len = strlen(constChar);

    if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &mEn, &mDe)) {
        mIsInitialized = false;
        RB_DEBUG->error("RB_AesEncryption::init() ERROR");
        return false;
    }

    mIsInitialized = true;
    return true;
}

QByteArray RB_AesEncryption::encrypt(const RB_String& plainText) {
    if (!mIsInitialized) {
        RB_DEBUG->error("RB_AesEncryption::encrypt() not initialized ERROR");
        return QByteArray();
    }

    int len = plainText.length() + 1;
    const QByteArray ba = plainText.toLatin1();
    const char* constChr = ba.constData();

    unsigned char* input = (unsigned char*)qstrdup(constChr);
    unsigned char* cipherText = aes_encrypt(&mEn, input, &len);
    QByteArray bo((const char*)(cipherText), len);
    bo = bo.toBase64();
    free(input);
    free(cipherText);

    return bo;
}

RB_String RB_AesEncryption::decrypt(const QByteArray& cipherText) {
    if (!mIsInitialized) {
        RB_DEBUG->error("RB_AesEncryption::decrypt() not initialized ERROR");
        return RB_String();
    }

    const QByteArray ba = QByteArray::fromBase64(cipherText);
    int len = ba.length() + 1;
    const char* constChr = ba.constData();

    unsigned char* input = (unsigned char*)qstrdup(constChr);
    char* uncText = (char*)aes_decrypt(&mDe, input, &len);
    QString plainText = QString(uncText); // QString(QLatin1String((const char*)(uncText), len));
    free(input);
    free(uncText);

    return plainText;
}

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int RB_AesEncryption::aes_init(unsigned char *key_data, int key_data_len,
                               unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
                               EVP_CIPHER_CTX *d_ctx) {
  int i, nrounds = 5;
  unsigned char key[32], iv[32];

  /*
   * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
   * nrounds is the number of times the we hash the material. More rounds are more secure but
   * slower.
   */
  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    printf("Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char* RB_AesEncryption::aes_encrypt(EVP_CIPHER_CTX *e,
                                             unsigned char *plaintext,
                                             int *len) {
  /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  unsigned char* ciphertext = (unsigned char*)malloc(c_len);

  /* allows reusing of 'e' for multiple encryption cycles */
  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  /* update ciphertext with the final remaining bytes */
  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;

  return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char* RB_AesEncryption::aes_decrypt(EVP_CIPHER_CTX *e,
                                             unsigned char *ciphertext,
                                             int *len) {
  /* plaintext will always be equal to or lesser than length of ciphertext*/
  int p_len = *len, f_len = 0;
  unsigned char* plaintext = (unsigned char*)malloc(p_len);

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

/**
 *
 *
 * (was main())
 */
int RB_AesEncryption::test(const RB_String& str /*int argc, char **argv*/) {
  /* "opaque" encryption, decryption ctx structures that libcrypto uses to record
     status of enc/dec operations */
  EVP_CIPHER_CTX en, de;

  /* 8 bytes to salt the key_data during key generation. This is an example of
     compiled in salt. We just read the bit pattern created by these two 4 byte
     integers on the stack as 64 bits of contigous salt material -
     ofcourse this only works if sizeof(int) >= 4 */
  unsigned int salt[] = {12345, 54321};
  unsigned char *key_data;
  int key_data_len, i;
  char *input[] = {(char*)"a", (char*)"abcd", (char*)"this is a test", (char*)"this is a bigger test",
                   (char*)"\nWho are you ?\nI am the 'Doctor'.\n'Doctor' who ?\nPrecisely!", (char*)"ï¿½ï¿½ï¿½ï¿½ï¿½dï¿½",
                   NULL};

  /* the key_data is read from the argument list */
//  key_data = (unsigned char *)argv[1];
//  key_data_len = strlen(argv[1]);
  const QByteArray ba = str.toLatin1();
  const char* constChar = ba.constData();
  key_data = (unsigned char*)qstrdup(constChar);
  key_data_len = strlen(constChar);

  /* gen key and iv. init the cipher ctx object */
  if (aes_init(key_data, key_data_len, (unsigned char *)&salt, &en, &de)) {
    printf("Couldn't initialize AES cipher\n");
    return -1;
  }

  /* encrypt and decrypt each input string and compare with the original */
  for (i = 0; input[i]; i++) {
    char *plaintext;
    unsigned char *ciphertext;
    int olen, len;

    /* The enc/dec functions deal with binary data and not C strings. strlen() will
       return length of the string without counting the '\0' string marker. We always
       pass in the marker byte to the encrypt/decrypt functions so that after decryption
       we end up with a legal C string */
    olen = len = strlen(input[i])+1;

    ciphertext = aes_encrypt(&en, (unsigned char *)input[i], &len);
    plaintext = (char *)aes_decrypt(&de, ciphertext, &len);

    if (strncmp(plaintext, input[i], olen))
      printf("FAIL: enc/dec failed for \"%s\"\n", input[i]);
    else
      printf("OK: enc/dec ok for \"%s\"\n", plaintext);

    free(ciphertext);
    free(plaintext);
  }

  EVP_CIPHER_CTX_cleanup(&en);
  EVP_CIPHER_CTX_cleanup(&de);

  return 0;
}

/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/aes.h>

int main(int argc, char** argv) {
    AES_KEY aes;
    unsigned char key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
    unsigned char* input_string;
    unsigned char* encrypt_string;
    unsigned char* decrypt_string;
    unsigned int len;        // encrypt length (in multiple of AES_BLOCK_SIZE)
    unsigned int i;
    int remainder;


    // check usage
    if (argc != 2) {
        fprintf(stderr, "%s <plain text>\n", argv[0]);
        exit(-1);
    }
    // check input
    if (strlen(argv[1])<=0) {
        fprintf(stderr, "input length is zero!\n");
        exit(-1);
    }

    // set the encryption length
    len = strlen(argv[1]) + AES_BLOCK_SIZE;
    remainder =strlen(argv[1])%AES_BLOCK_SIZE;
    if ( 0!=remainder)
        len -= remainder;

    // set the input string
    input_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (input_string == NULL) {
        fprintf(stderr, "Unable to allocate memory for input_string\n");
        exit(-1);
    }
    //padding! Important!
    memset( input_string,len-strlen(argv[1]),len );
    strncpy((char*)input_string, argv[1], strlen(argv[1]));

    // Generate AES 128-bit key
    memset(key, 0x01, AES_BLOCK_SIZE);

    // Set encryption key
    memset(iv, 0x01, AES_BLOCK_SIZE);
    if (AES_set_encrypt_key(key, 128, &aes) < 0) {
        fprintf(stderr, "Unable to set encryption key in AES\n");
        exit(-1);
    }

    // alloc encrypt_string
    encrypt_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (encrypt_string == NULL) {
        fprintf(stderr, "Unable to allocate memory for encrypt_string\n");
        exit(-1);
    }

    // encrypt (iv will change)
    AES_cbc_encrypt(input_string, encrypt_string, len, &aes, iv, AES_ENCRYPT);

    /////////////////////////////////////

    // alloc decrypt_string
    decrypt_string = (unsigned char*)calloc(len, sizeof(unsigned char));
    if (decrypt_string == NULL) {
        fprintf(stderr, "Unable to allocate memory for decrypt_string\n");
        exit(-1);
    }

    // Set decryption key
    memset(iv, 0x01, AES_BLOCK_SIZE);
    if (AES_set_decrypt_key(key, 128, &aes) < 0) {
        fprintf(stderr, "Unable to set decryption key in AES\n");
        exit(-1);
    }

    // decrypt
    AES_cbc_encrypt(encrypt_string, decrypt_string, len, &aes, iv,
            AES_DECRYPT);

    // print
    printf("input_string =%s\n", input_string);
    printf("encrypted string =");
    for (i=0; i<len; ++i) {
        printf("%u ", encrypt_string[i]);
    }
    printf("\n");
    printf("decrypted string =%s\n", decrypt_string);

    return 0;
}
*/
