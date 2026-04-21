#include "hookapi.h"

// 1. IAW TREASURY HEX (Derived from raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c)
static const uint8_t TREASURY[20] = { 
    0x69, 0x18, 0x93, 0x6E, 0x98, 0x0A, 0x5D, 0x64, 0x2A, 0x0D, 
    0x88, 0x4D, 0x41, 0x76, 0x1F, 0x11, 0x93, 0xD4, 0x2C, 0xA1 
};

// 2. RLUSD CURRENCY CODE (Standard 160-bit Hex)
static const uint8_t RLUSD[20] = {0,0,0,0,0,0,0,0,0,0,0,0, 'R', 'L', 'U', 'S', 'D', 0,0,0};

int64_t hook(uint32_t reserved) {
    
    // FETCH THE INCOMING TRANSACTION AMOUNT
    unsigned char amt_buf[48];
    int64_t amt_len = otxn_field(amt_buf, 48, sfAmount);

    // If it's not a token (48 bytes), let it pass
    if (amt_len != 48) accept(SVAR("Passing non-token tx"), 0);

    // INSTITUTIONAL MATH: 0.33% (33 Basis Points)
    // full_amt * (33 / 10000) using XFL for zero rounding error
    int64_t full_amt_xfl = float_set(0, CAST_TO_INT64(amt_buf));
    int64_t fee_ratio = float_divide(float_set(0, 33), float_set(0, 10000));
    int64_t fee_xfl = float_multiply(full_amt_xfl, fee_ratio);

    // RESERVE 1 SLOT FOR EMISSION
    if (etxn_reserve(1) != 1) rollback(SVAR("Reserve failed"), 1);

    // CONSTRUCT THE PAYMENT TO TREASURY
    unsigned char tx[256];
    int64_t tx_len = encode_payment_token(tx, 256, fee_xfl, TREASURY, RLUSD);
    
    // EMIT THE HARVESTED FEE
    if (emit(tx, tx_len) < 0) rollback(SVAR("Emission failed"), 2);

    accept(SVAR("IAW: 33BPS Institutional Settlement Success"), 0);
    return 0;
}
