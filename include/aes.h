/**
 *	@file aes.h
 *	@author computer-spectre
 * */

#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  /**
  * @brief Encrypts a 16 Bytes array using AES-128
  *
  * @param[in]    in            The array to encrypt
  * @param[out]   out           The encrypted array
  * @param        key           The encryption key
  * @param        verbose       Show intermediate states ?
  */
  void aes_encrypt(uint8_t *out, uint8_t *in, uint8_t *key, bool verbose);

  /**
  * @brief Decrypts an AES-128 encrypted 16 Bytes array
  *
  * @param[in]    in    The array to decrypt
  * @param[out]   out   The decrypted array
  * @param        key   The encryption key
  */
  void aes_decrypt(uint8_t *out, uint8_t *in, uint8_t *key);

  /**
  * @brief Prints a 16 Bytes array
  *
  * @param        state    The array to print
  */
  void aes_print_state(uint8_t *state);

#ifdef __cplusplus
}
#endif

#endif
