/**
 * @file aes.cpp
 * Embodies the aes algorithm to encrypt and decrypt data
 * 1. Substitutes bytes
 * 2. Shifts rows
 * 3. Mixes columns
 * 4. Adds round key
 */

/*
 ---------------------------------------------------------------------------
 Copyright (c) 2013, Igor Saric. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 ---------------------------------------------------------------------------
 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 */

#include "aes.h"

/**
 * Initializes the about object
 */
AES::AES()
{
}

// Encryption with IV
/**
 * Encrypts data based on plaintext and cipher key
 * @param p_input Plaintext
 * @param p_key Cipher key
 * @return Encrypted data
 */
QByteArray AES::encrypt(QByteArray p_input, QByteArray p_key)
{
    if (p_input.isEmpty()) {
        qDebug() << "Error while encryption: Cannot encrypt empty input";
        return QByteArray();
    }

    QByteArray iv = QUuid::createUuid().toRfc4122();
    return encrypt(p_input, p_key, iv);
}

/**
 * Decrypts data based on cipher text and cipher key
 * @param p_input Cipher text
 * @param p_key Cipher key
 * @return Decrypted data
 */
QByteArray AES::decrypt(QByteArray p_input, QByteArray p_key)
{
    if (p_input.isEmpty()) {
        qDebug() << "Error while decryption: Cannot decrypt empty input";
        return QByteArray();
    }
    QByteArray iv = p_input.left(16);
    QByteArray input = p_input.remove(0, 16);
    return decrypt(input, p_key, iv);
}

/**
 * Encrypts data based on ciper text, cipher key and IV
 * @param b_input Plaintext
 * @param p_key Cipher key
 * @param p_iv Initialization vector
 * @return Encrypted data
 */
QByteArray AES::encrypt(QByteArray b_input, QByteArray p_key, QByteArray p_iv)
{
    if (b_input.isEmpty()) {
        qDebug() << "Error while encryption: Cannot encrypt empty input";
        return QByteArray();
    }

    addPadding(&b_input);

    QByteArray result;

    int keySize = p_key.size();
    int ivSize = p_iv.size();

    if (keySize != 16 && keySize != 24 && keySize != 32) {
        qDebug() << "Error while encryption: Invalid keysize";
        return QByteArray();
    }

    if (ivSize != 16) {
        qDebug() << "Error while encryption: Invalid keysize";
        return QByteArray();
    }

    // Chunks have to stay < 1MB (1.050MB to be exactly)
    qint64 chunksCount = (b_input.size()/500000) + 1;

    for (int part = 0; part < chunksCount; part++) {
        // Out of range checks
        if (part*500000 > b_input.size()) {
            qDebug() << "Error while encryption: Skipped: Buffer overflow";
            continue;
        }

        QByteArray p_chunk = b_input.mid(part*500000, 500000);
        int inputSize = p_chunk.size();

        unsigned char key[keySize];
        qByteArrayToUCharArray(p_key, key);

        unsigned char iv[ivSize];
        qByteArrayToUCharArray(p_iv, iv);

        unsigned char decrypted[inputSize];
        qByteArrayToUCharArray(p_chunk, decrypted);

        unsigned char encrypted[inputSize]; // Encrypted text

        aes_context context;
        aes_set_key(key, keySize * 8, &context);
        aes_cbc_encrypt(decrypted, encrypted, inputSize, iv, &context);

        result.append( uCharArrayToQByteArray(encrypted, inputSize));
    }

    result.prepend(p_iv);
    return result;
}

/**
 * Decrypt Decrypts data based on cipher text, cipher key and IV
 * @param b_input Cipher text
 * @param p_key Cipher key
 * @param p_iv Initialization vector
 * @return Decrypted data
 */
QByteArray AES::decrypt(QByteArray b_input, QByteArray p_key, QByteArray p_iv)
{
    if (b_input.isEmpty()) {
        qDebug() << "Error while decryption: Cannot decrypt empty input";
        return QByteArray();
    }

    QByteArray result;

    int keySize = p_key.size();
    int ivSize = p_iv.size();

    if (keySize != 16 && keySize != 24 && keySize != 32) {
        qDebug() << "Error while decryption: Invalid keysize";
        return QByteArray();
    }

    if (ivSize != 16) {
        qDebug() << "Error while decryption: Invalid keysize";
        return QByteArray();
    }

    // 500KB blocks
    qint64 chunksCount = (b_input.size()/500000) + 1;

    for (int part = 0; part < chunksCount; part++) {
        // Out of range checks
        if (part*500000 > b_input.size()) {
            qDebug() << "Error while decryption: Skipped: Buffer overflow";
            continue;
        }

        QByteArray p_chunk = b_input.mid(part*500000, 500000);

        int inputSize = p_chunk.size();

        unsigned char key[keySize];
        qByteArrayToUCharArray(p_key, key);

        unsigned char iv[ivSize];
        qByteArrayToUCharArray(p_iv, iv);

        unsigned char encrypted[inputSize];
        qByteArrayToUCharArray(p_chunk, encrypted);

        unsigned char decrypted[inputSize]; // Decrypted text

        aes_context context;
        aes_set_key(key, keySize * 8, &context);
        aes_cbc_decrypt(encrypted, decrypted, inputSize, iv, &context);

        QByteArray temp_result = uCharArrayToQByteArray(decrypted, inputSize);

        result.append( temp_result);
    }

    removePadding(&result);
    return result;
}


// Helper functions
/**
 * Converts hexadecimal string to a byte array
 * @param key Cipher key
 * @return Byte array
 */
QByteArray AES::hexStringToByte(QString key) {
    return QByteArray::fromHex(QString(key).toLatin1());
}

/**
 * Converts a byte array to a char array
 * @param src Byte array
 * @param dest Char array
 */
void AES::qByteArrayToUCharArray(QByteArray src, unsigned char *dest) {
    for (int i = 0; i < src.size(); i++) {
        dest[i] = src.at(i);
    }
}

/**
 * Converts a char array to a byte array
 * @param src Char array
 * @param p_size Size
 * @return Byte array
 */
QByteArray AES::uCharArrayToQByteArray(unsigned char *src, int p_size) {
    QByteArray array((char*)src, p_size);
    return array;
}

// PKCS#7 padding
/**
 * Removes padding bits from a byte array through PKCS#7
 * @param input Byte array
 */
void AES::removePadding(QByteArray *input) {
    int padding = input->at(input->size() - 1);

    for(int i = 0; i < padding; i++) {
        if (input->at(input->size() - 1) == padding) {
            input->chop(1);
        }
    }
}

/**
 * Adds padding bits to a byte array through PKCS#7
 * @param input Byte array
 */
void AES::addPadding(QByteArray* input) {
    int size = input->size();
    int padding = 16 - (size % 16);

    for(int i = 0; i < padding; i++) {
        input->append(padding);
    }
}

// Algorithm
/**
 * Makes a disjunctive operation
 * @param d Data
 * @param s Source
 */
void AES::xor_block( void *d, const void *s ) {
    ((uint_32t*)d)[ 0] ^= ((uint_32t*)s)[ 0];
    ((uint_32t*)d)[ 1] ^= ((uint_32t*)s)[ 1];
    ((uint_32t*)d)[ 2] ^= ((uint_32t*)s)[ 2];
    ((uint_32t*)d)[ 3] ^= ((uint_32t*)s)[ 3];
}

/**
 * Copies data and refers to the key
 * @param d Data
 * @param s Source
 * @param k Key
 */
void AES::copy_and_key( void *d, const void *s, const void *k )
{
    ((uint_32t*)d)[ 0] = ((uint_32t*)s)[ 0] ^ ((uint_32t*)k)[ 0];
    ((uint_32t*)d)[ 1] = ((uint_32t*)s)[ 1] ^ ((uint_32t*)k)[ 1];
    ((uint_32t*)d)[ 2] = ((uint_32t*)s)[ 2] ^ ((uint_32t*)k)[ 2];
    ((uint_32t*)d)[ 3] = ((uint_32t*)s)[ 3] ^ ((uint_32t*)k)[ 3];
}

/**
 * Adds round key
 * @param d Data
 * @param k Key
 */
void AES::add_round_key( uint_8t d[N_BLOCK], const uint_8t k[N_BLOCK] )
{
    xor_block(d, k);
}

/**
 * Shifts rows
 * @param st Data
 */
void AES::shift_sub_rows( uint_8t st[N_BLOCK] )
{
    uint_8t tt;

    st[ 0] = s_box(st[ 0]); st[ 4] = s_box(st[ 4]);
    st[ 8] = s_box(st[ 8]); st[12] = s_box(st[12]);

    tt = st[1]; st[ 1] = s_box(st[ 5]); st[ 5] = s_box(st[ 9]);
    st[ 9] = s_box(st[13]); st[13] = s_box( tt );

    tt = st[2]; st[ 2] = s_box(st[10]); st[10] = s_box( tt );
    tt = st[6]; st[ 6] = s_box(st[14]); st[14] = s_box( tt );

    tt = st[15]; st[15] = s_box(st[11]); st[11] = s_box(st[ 7]);
    st[ 7] = s_box(st[ 3]); st[ 3] = s_box( tt );
}

/**
 * Inverts shifted rows
 * @param st Data
 */
void AES::inv_shift_sub_rows( uint_8t st[N_BLOCK] )
{
    uint_8t tt;

    st[ 0] = is_box(st[ 0]); st[ 4] = is_box(st[ 4]);
    st[ 8] = is_box(st[ 8]); st[12] = is_box(st[12]);

    tt = st[13]; st[13] = is_box(st[9]); st[ 9] = is_box(st[5]);
    st[ 5] = is_box(st[1]); st[ 1] = is_box( tt );

    tt = st[2]; st[ 2] = is_box(st[10]); st[10] = is_box( tt );
    tt = st[6]; st[ 6] = is_box(st[14]); st[14] = is_box( tt );

    tt = st[3]; st[ 3] = is_box(st[ 7]); st[ 7] = is_box(st[11]);
    st[11] = is_box(st[15]); st[15] = is_box( tt );
}

/**
 * Mixes columns
 * @param dt Data
 */
void AES::mix_sub_columns( uint_8t dt[N_BLOCK] )
{
    uint_8t st[N_BLOCK];
    block_copy(st, dt);

    dt[ 0] = gfm2_sb(st[0]) ^ gfm3_sb(st[5]) ^ s_box(st[10]) ^ s_box(st[15]);
    dt[ 1] = s_box(st[0]) ^ gfm2_sb(st[5]) ^ gfm3_sb(st[10]) ^ s_box(st[15]);
    dt[ 2] = s_box(st[0]) ^ s_box(st[5]) ^ gfm2_sb(st[10]) ^ gfm3_sb(st[15]);
    dt[ 3] = gfm3_sb(st[0]) ^ s_box(st[5]) ^ s_box(st[10]) ^ gfm2_sb(st[15]);

    dt[ 4] = gfm2_sb(st[4]) ^ gfm3_sb(st[9]) ^ s_box(st[14]) ^ s_box(st[3]);
    dt[ 5] = s_box(st[4]) ^ gfm2_sb(st[9]) ^ gfm3_sb(st[14]) ^ s_box(st[3]);
    dt[ 6] = s_box(st[4]) ^ s_box(st[9]) ^ gfm2_sb(st[14]) ^ gfm3_sb(st[3]);
    dt[ 7] = gfm3_sb(st[4]) ^ s_box(st[9]) ^ s_box(st[14]) ^ gfm2_sb(st[3]);

    dt[ 8] = gfm2_sb(st[8]) ^ gfm3_sb(st[13]) ^ s_box(st[2]) ^ s_box(st[7]);
    dt[ 9] = s_box(st[8]) ^ gfm2_sb(st[13]) ^ gfm3_sb(st[2]) ^ s_box(st[7]);
    dt[10] = s_box(st[8]) ^ s_box(st[13]) ^ gfm2_sb(st[2]) ^ gfm3_sb(st[7]);
    dt[11] = gfm3_sb(st[8]) ^ s_box(st[13]) ^ s_box(st[2]) ^ gfm2_sb(st[7]);

    dt[12] = gfm2_sb(st[12]) ^ gfm3_sb(st[1]) ^ s_box(st[6]) ^ s_box(st[11]);
    dt[13] = s_box(st[12]) ^ gfm2_sb(st[1]) ^ gfm3_sb(st[6]) ^ s_box(st[11]);
    dt[14] = s_box(st[12]) ^ s_box(st[1]) ^ gfm2_sb(st[6]) ^ gfm3_sb(st[11]);
    dt[15] = gfm3_sb(st[12]) ^ s_box(st[1]) ^ s_box(st[6]) ^ gfm2_sb(st[11]);
  }

/**
 * Inverses mixed columns
 * @param dt Data
 */
void AES::inv_mix_sub_columns( uint_8t dt[N_BLOCK] )
{
    uint_8t st[N_BLOCK];
    block_copy(st, dt);

    dt[ 0] = is_box(gfm_e(st[ 0]) ^ gfm_b(st[ 1]) ^ gfm_d(st[ 2]) ^ gfm_9(st[ 3]));
    dt[ 5] = is_box(gfm_9(st[ 0]) ^ gfm_e(st[ 1]) ^ gfm_b(st[ 2]) ^ gfm_d(st[ 3]));
    dt[10] = is_box(gfm_d(st[ 0]) ^ gfm_9(st[ 1]) ^ gfm_e(st[ 2]) ^ gfm_b(st[ 3]));
    dt[15] = is_box(gfm_b(st[ 0]) ^ gfm_d(st[ 1]) ^ gfm_9(st[ 2]) ^ gfm_e(st[ 3]));

    dt[ 4] = is_box(gfm_e(st[ 4]) ^ gfm_b(st[ 5]) ^ gfm_d(st[ 6]) ^ gfm_9(st[ 7]));
    dt[ 9] = is_box(gfm_9(st[ 4]) ^ gfm_e(st[ 5]) ^ gfm_b(st[ 6]) ^ gfm_d(st[ 7]));
    dt[14] = is_box(gfm_d(st[ 4]) ^ gfm_9(st[ 5]) ^ gfm_e(st[ 6]) ^ gfm_b(st[ 7]));
    dt[ 3] = is_box(gfm_b(st[ 4]) ^ gfm_d(st[ 5]) ^ gfm_9(st[ 6]) ^ gfm_e(st[ 7]));

    dt[ 8] = is_box(gfm_e(st[ 8]) ^ gfm_b(st[ 9]) ^ gfm_d(st[10]) ^ gfm_9(st[11]));
    dt[13] = is_box(gfm_9(st[ 8]) ^ gfm_e(st[ 9]) ^ gfm_b(st[10]) ^ gfm_d(st[11]));
    dt[ 2] = is_box(gfm_d(st[ 8]) ^ gfm_9(st[ 9]) ^ gfm_e(st[10]) ^ gfm_b(st[11]));
    dt[ 7] = is_box(gfm_b(st[ 8]) ^ gfm_d(st[ 9]) ^ gfm_9(st[10]) ^ gfm_e(st[11]));

    dt[12] = is_box(gfm_e(st[12]) ^ gfm_b(st[13]) ^ gfm_d(st[14]) ^ gfm_9(st[15]));
    dt[ 1] = is_box(gfm_9(st[12]) ^ gfm_e(st[13]) ^ gfm_b(st[14]) ^ gfm_d(st[15]));
    dt[ 6] = is_box(gfm_d(st[12]) ^ gfm_9(st[13]) ^ gfm_e(st[14]) ^ gfm_b(st[15]));
    dt[11] = is_box(gfm_b(st[12]) ^ gfm_d(st[13]) ^ gfm_9(st[14]) ^ gfm_e(st[15]));
  }

/**
 * Sets the cipher key for the pre-keyed version
 * @param key Cipher key
 * @param keylen Length of the cipher key
 * @param ctx AES context
 * @return Result of AES
 */
aes_result AES::aes_set_key( const unsigned char key[], int keylen, aes_context ctx[1] )
{
    uint_8t cc, rc, hi;

    switch( keylen )
    {
    case 128:
        keylen = 16;
        break;
    case 192:
        keylen = 24;
        break;
    case 256:
        keylen = 32;
        break;
    default:
        ctx->rnd = 0;
        return -1;
    }
    block_copy_nn(ctx->ksch, key, keylen);
    hi = (keylen + 28) << 2;
    ctx->rnd = (hi >> 4) - 1;
    for( cc = keylen, rc = 1; cc < hi; cc += 4 )
    {   uint_8t tt, t0, t1, t2, t3;

        t0 = ctx->ksch[cc - 4];
        t1 = ctx->ksch[cc - 3];
        t2 = ctx->ksch[cc - 2];
        t3 = ctx->ksch[cc - 1];
        if( cc % keylen == 0 )
        {
            tt = t0;
            t0 = s_box(t1) ^ rc;
            t1 = s_box(t2);
            t2 = s_box(t3);
            t3 = s_box(tt);
            rc = f2(rc);
        }
        else if( keylen > 24 && cc % keylen == 16 )
        {
            t0 = s_box(t0);
            t1 = s_box(t1);
            t2 = s_box(t2);
            t3 = s_box(t3);
        }
        tt = cc - keylen;
        ctx->ksch[cc + 0] = ctx->ksch[tt + 0] ^ t0;
        ctx->ksch[cc + 1] = ctx->ksch[tt + 1] ^ t1;
        ctx->ksch[cc + 2] = ctx->ksch[tt + 2] ^ t2;
        ctx->ksch[cc + 3] = ctx->ksch[tt + 3] ^ t3;
    }
    return 0;
}

/**
 * Encrypts a single block of 16 bytes
 * @param in Buffer holding the input data
 * @param out Buffer holding the output data
 * @param ctx AES context
 * @return Status from the result
 */
aes_result AES::aes_encrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] )
{
    if( ctx->rnd )
    {
        uint_8t s1[N_BLOCK], r;
        copy_and_key( s1, in, ctx->ksch );

        for( r = 1 ; r < ctx->rnd ; ++r )
        {
            mix_sub_columns( s1 );
            add_round_key( s1, ctx->ksch + r * N_BLOCK);
        }
        shift_sub_rows( s1 );
        copy_and_key( out, s1, ctx->ksch + r * N_BLOCK );
    }
    else
        return -1;
    return 0;
}

/**
 * Encrypts a number of blocks (input and return an IV)
 * @param in Buffer holding the input data
 * @param out Buffer holding the output data
 * @param size Size of the input data
 * @param iv Initialization vector (updated after use)
 * @param ctx AES context
 * @return Status from the result
 */
aes_result AES::aes_cbc_encrypt(const unsigned char *in, unsigned char *out, unsigned long size, unsigned char iv[N_BLOCK], const aes_context ctx[1] )
{
    if (size % 16 != 0)
        return EXIT_FAILURE;

    unsigned long n_block = size / 16;

    while(n_block--)
    {
        xor_block(iv, in);
        if(aes_encrypt(iv, iv, ctx) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        memcpy(out, iv, N_BLOCK);
        in += N_BLOCK;
        out += N_BLOCK;
    }
    return EXIT_SUCCESS;
}

/**
 * Decrypts a single block of 16 bytes
 * @param in Buffer holding the input data
 * @param out Buffer holding the output data
 * @param ctx AES context
 * @return Status from the result
 */
aes_result AES::aes_decrypt( const unsigned char in[N_BLOCK], unsigned char out[N_BLOCK], const aes_context ctx[1] )
{
    if( ctx->rnd )
    {
        uint_8t s1[N_BLOCK], r;
        copy_and_key( s1, in, ctx->ksch + ctx->rnd * N_BLOCK );
        inv_shift_sub_rows( s1 );

        for( r = ctx->rnd ; --r ; )
        {
            add_round_key( s1, ctx->ksch + r * N_BLOCK );
            inv_mix_sub_columns( s1 );
        }
        copy_and_key( out, s1, ctx->ksch );
    }
    else
        return -1;
    return 0;
}

/**
 * Decrypts a number of blocks (input and return an IV)
 * @param in Buffer holding the input data
 * @param out Buffer holding the output data
 * @param size Size of the input data
 * @param iv Initialization vector (updated after use)
 * @param ctx AES context
 * @return Status from the result
 */
aes_result AES::aes_cbc_decrypt( const unsigned char *in, unsigned char *out, unsigned long size, unsigned char iv[N_BLOCK], const aes_context ctx[1] )
{
    if (size % 16 != 0)
        return EXIT_FAILURE;

    unsigned long n_block = size / 16;

    while (n_block--)
    {
        uint_8t tmp[N_BLOCK];

        memcpy(tmp, in, N_BLOCK);
        if(aes_decrypt(in, out, ctx) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        xor_block(out, iv);
        memcpy(iv, tmp, N_BLOCK);
        in += N_BLOCK;
        out += N_BLOCK;
    }
    return EXIT_SUCCESS;
}
