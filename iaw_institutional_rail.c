/**
 * IAW Institutional Rail - Final Production v2.2
 * Fixed: Mandatory _g Guard Function for Mainnet Compliance
 */

#include "hookapi.h"

// Treasury: raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c
uint8_t treasury[20] = {0x69, 0x93, 0x1A, 0x92, 0x8C, 0x98, 0x05, 0x7E, 0x46, 0x5D, 0x91, 0x86, 0x3F, 0x46, 0x22, 0x8E, 0x91, 0x3C, 0x46, 0x74};

// RLUSD Asset Info
uint8_t rlusd_cur[20] = { 'R', 'L', 'U', 'S', 'D', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t rlusd_iss[20] = {0x2A, 0x77, 0x6E, 0x1F, 0x45, 0x82, 0x82, 0xC6, 0x62, 0x1A, 0xC9, 0x96, 0x9F, 0x4B, 0xE7, 0xA2, 0x6C, 0xF9, 0xE5, 0xC1};

int64_t hook(uint32_t ctx) {
    
    // MANDATORY: Guard function call to prevent loop rejection
    // This satisfies the "Guard function _g was not imported" error
    _g(1, 1); 

    uint8_t amt_buf[48];
    int64_t amt_len = otxn_field(SBUF(amt_buf), sfAmount);
    
    if (amt_len <= 0) accept(SBUF("IAW: Monitoring"), 0);

    // 1. Calculate 0.33% Toll using XFL
    int64_t amount_xfl = (amt_len == 8) ? 
        float_set(0, INT64_FROM_BUF(amt_buf)) : 
        float_sto_set((uint32_t)amt_buf, (uint32_t)amt_len);

    int64_t fee_xfl = float_multiply(amount_xfl, float_set(-4, 33));

    // 2. Prepare for Emission
    if (etxn_reserve(1) != 1) rollback(SBUF("IAW: Reserve Fail"), 1);

    // 3. Prepare Payment Simple
    uint8_t tx[280]; 
    PREPARE_PAYMENT_SIMPLE(tx, fee_xfl, treasury, 0, 0); 
    
    // Inject RLUSD details manually to avoid macro failure
    for (int i = 0; i < 20; i++) {
        tx[164 + i] = rlusd_cur[i]; 
        tx[184 + i] = rlusd_iss[i]; 
    }

    // 4. Final Emit
    if (emit((uint32_t)tx, sizeof(tx), 0, 0) < 0) rollback(SBUF("IAW: Toll Failure"), 2);

    accept(SBUF("IAW: Transaction Secured"), 0);
    return 0;
}

int64_t cbak(uint32_t ctx) { 
    return 0; 
}
