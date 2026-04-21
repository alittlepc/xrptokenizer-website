#include "hookapi.h"

// IAW TREASURY HEX (raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c)
static const uint8_t TREASURY[20] = { 
    0x69, 0x18, 0x93, 0x6E, 0x98, 0x0A, 0x5D, 0x64, 0x2A, 0x0D, 
    0x88, 0x4D, 0x41, 0x76, 0x1F, 0x11, 0x93, 0xD4, 0x2C, 0xA1 
};

// Mandatory Guard function for Xahau WASM
// This prevents infinite loops and satisfies the compiler
int64_t cbak(uint32_t reserved) { 
    return 0; 
}

int64_t hook(uint32_t reserved) {
    
    // 1. Initialize the Guard (This fixes the "Guard function _g was not imported" error)
    _g(1, 1);

    unsigned char amt_buf[48];
    int64_t amt_len = otxn_field((uint32_t)amt_buf, 48, sfAmount);

    // If it's not a token (48 bytes), let it pass
    if (amt_len != 48) 
        accept(0, 0, 0); 

    // 2. MATH: 0.33% (33 Basis Points) 
    int64_t full_amt_xfl = float_set(0, (int64_t)amt_buf);
    int64_t fee_ratio = float_divide(float_set(0, 33), float_set(0, 10000));
    int64_t fee_xfl = float_multiply(full_amt_xfl, fee_ratio);

    // 3. RESERVE SLOT
    if (etxn_reserve(1) != 1) 
        rollback(0, 0, 1);

    // 4. PREPARE PAYMENT
    unsigned char tx[256];
    // Note: This macro handles the internal encoding
    PREPARE_PAYMENT_SIMPLE(tx, fee_xfl, TREASURY, 0, 0);

    // 5. EMIT (With explicit length and required 4 arguments)
    if (emit((uint32_t)tx, sizeof(tx), 0, 0) < 0) 
        rollback(0, 0, 2);

    accept(0, 0, 0);
    return 0;
}
