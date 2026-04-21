#include "hookapi.h"

// --- CONFIGURATION ---
// Treasury Wallet: rDZKzhgNdvBqBeMHGjaimgMmW6DUZeRFRq
uint8_t TREASURY[20] = {0xB4, 0x1E, 0x4B, 0x8D, 0x56, 0x6E, 0x60, 0x9B, 0x0C, 0xF9, 0xC3, 0x6E, 0x82, 0x6E, 0x92, 0x7E, 0x71, 0x6E, 0x5D, 0x4D};

// Pre-calculated XFL for 0.0033 (33 BPS)
#define FEE_RATIO_XFL 5906232930722119680ll 

int64_t hook(uint32_t reserved) {
    
    _g(1, 1);

    // 1. Filter for Payments only
    if (otxn_type() != ttPAYMENT)
        accept(SVAR("Non-payment, passing."), 0);

    // 2. Get the amount
    uint8_t amt_buf[48];
    // Cast to (uint32_t) to satisfy the compiler warnings
    int64_t amt_len = otxn_field((uint32_t)amt_buf, 48, sfAmount);

    // 3. Skip if it's base XRP
    if (amt_len != 48)
        accept(SVAR("Base XRP, skipping."), 0);

    // 4. Calculate 33 BPS (0.33%) Fee
    int64_t full_amt_xfl = float_sto_set((uint32_t)amt_buf, 48);
    int64_t fee_amt_xfl = float_multiply(full_amt_xfl, FEE_RATIO_XFL);

    // 5. Prepare the Fee Emission
    uint8_t tx[256];
    PREPARE_PAYMENT_SIMPLE(tx, fee_amt_xfl, TREASURY, 0, 0);

    // 6. Emit Fee (FIXED: Added missing arguments for hash_out and hash_len)
    uint8_t emithash[32];
    if (emit((uint32_t)tx, 256, (uint32_t)emithash, 32) < 0)
        rollback(SVAR("Emission failed, safety halt."), 1);

    // 7. Success
    accept(SVAR("IAW v1.7: 33 BPS Harvested."), 0);

    return 0;
}
