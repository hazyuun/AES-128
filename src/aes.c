/**
 *	@file aes.c
 *	@author computer-spectre
 * */

#include <aes.h>
#include <lookup.h>
#include <stdio.h>
#include <string.h>

static void aes_add_round_key(uint8_t *state, uint8_t *key){
  for(uint8_t i = 0; i < 16; i++)
    state[i] ^= key[i];
}

static void aes_substitute(uint8_t *state){
  for(uint8_t i = 0; i < 16; i++)
    state[i] = rijndael_sbox[state[i]];
}


/* Shifts a row to the left, by 1 step              */
/* Note : Technically this is a rotate not a shift, */
/*        but I will follow the original naming     */
static void aes_shift_row_step(uint8_t *state, uint8_t row_number){
  uint8_t first = state[row_number];
  for(uint8_t i = 0; i < 3; i++)
    state[4 * i + row_number] = state[4 * i +row_number + 4];
  state[4 * 3 + row_number] = first;
}

/* Shifts a row to the left, by n0 steps */
static void aes_shift_row(uint8_t *state, uint8_t row_number, uint8_t n){
  while(n > 0){
    aes_shift_row_step(state, row_number);
    --n;
  }
}

static void aes_shift_rows(uint8_t *state){
  uint8_t row_number = 1;
  while(row_number < 4){
    aes_shift_row(state, row_number, row_number);
    ++row_number;
  }
}

static void aes_mix_column(uint8_t *mixed, uint8_t *state, uint8_t n){
  mixed[4*n + 0] = (uint8_t)   mul2[state[4*n + 0]] ^ mul3[state[4*n + 1]] ^      state[4*n + 2]  ^      state[4*n + 3];
	mixed[4*n + 1] = (uint8_t)        state[4*n + 0]  ^ mul2[state[4*n + 1]] ^ mul3[state[4*n + 2]] ^      state[4*n + 3];
	mixed[4*n + 2] = (uint8_t)        state[4*n + 0]  ^      state[4*n + 1]  ^ mul2[state[4*n + 2]] ^ mul3[state[4*n + 3]];
	mixed[4*n + 3] = (uint8_t)   mul3[state[4*n + 0]] ^      state[4*n + 1]  ^      state[4*n + 2]  ^ mul2[state[4*n + 3]];
}

static void aes_mix_columns(uint8_t *state){
  uint8_t mixed[16];
  uint8_t i;

	for(i = 0; i < 4; i++)
    aes_mix_column(mixed, state, i);

  memcpy(state, mixed, 16);
}

static void aes_key_expand_first(uint8_t *col, uint8_t rcon_index){
  uint8_t first = col[0];
  for(uint8_t i = 0; i < 3; i++)
    col[i] = col[i+1];
  col[3] = first;

  for(uint8_t i = 0; i < 4; i++)
    col[i] = rijndael_sbox[col[i]];
  col[0] ^= rcon[rcon_index];
}

static void aes_key_expand(uint8_t *expanded, uint8_t *original){
  /* First 16 bytes are same as the original */
  memcpy(expanded, original, 16);

  uint8_t temp[4];
  for (uint8_t i = 16, rcon_index = 1; i < 176;) {

    for (uint8_t j = 0; j < 4; j++)
      temp[j] = expanded[j + i - 4];

    /* The first column is special */
    if ( !(i % 16) )
      aes_key_expand_first(temp, rcon_index++);

    /* The rest of the columns are just sum */
    for (uint8_t j = 0; j < 4; j++) {
      expanded[i] = expanded[i - 16] ^ temp[j];
      ++i;
    }

  }
}

void aes_print_state(uint8_t *state){
  for(uint8_t i = 0; i < 16; i++){
    printf("0x%02X ", state[i]); fflush(stdout);
  }
}

static inline void aes_log(bool verbose, const char *prefix, uint8_t *state){
  if(verbose) {
    printf("\n %10s :  ", prefix);
    aes_print_state(state);
  }
}

void aes_encrypt(uint8_t *out, uint8_t *in, uint8_t *key, bool verbose){

  if(verbose) {
    printf("\n *** AES encrypt ***"); fflush(stdout);
  }

  /* This will hold the current state */
  uint8_t state[16];
  memcpy(state, in, 16);
aes_log(verbose, "input", state);

  uint8_t expanded[176];
  aes_key_expand(expanded, key);

	aes_add_round_key(state, expanded);
  aes_log(verbose, "add", state);

	for (int i = 0; i < 9; i++) {
    aes_substitute(state);
    aes_log(verbose, "subs", state);

  	aes_shift_rows(state);
    aes_log(verbose, "shift", state);

  	aes_mix_columns(state);
    aes_log(verbose, "mix", state);

  	aes_add_round_key(state, expanded + (16 * (i+1)));
    aes_log(verbose, "add", state);
	}

  aes_substitute(state);
  aes_log(verbose, "subs", state);

  aes_shift_rows(state);
  aes_log(verbose, "shift", state);

  aes_add_round_key(state, expanded + 160);
  aes_log(verbose, "add", state);

  aes_log(verbose, "output", state);
  memcpy(out, state, 16);
}

void aes_decrypt(uint8_t *out, uint8_t *in, uint8_t *key){
  (void) out;
  (void) in;
  (void) key;

  printf("*** AES decrypt ***");
  printf("\n Not implemented yet");
}
