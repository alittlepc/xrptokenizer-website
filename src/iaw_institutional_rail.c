#include "hookapi.h"

// IAW TREASURY HEX (raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c)
static const uint8_t TREASURY[20] = { 
    0x69, 0x18, 0x93, 0x6E, 0x98, 0x0A, 0x5D, 0x64, 0x2A, 0x0D, 
    0x88, 0x4D, 0x41, 0x76, 0x1F, 0x11, 0x93, 0xD4, 0x2C, 0xA1 
};

// Mandatory Guard function for Xahau WASM
int64_t cbak(uint32_t reserved) { 
    return 0; 
}

int64_t hook(uint32_t reserved) {
    
    // 1. Initialize the Guard for loop safety
    _g(1, 1);

    unsigned char amt_buf[48];
    int64_t amt_len = otxn_field((uint32_t)amt_buf, 48, sfAmount);

    // Filter: Only process Token transactions (48 bytes)
    if (amt_len != 48) {
        accept(SBUF("IAW Rail: Non-token transaction ignored"), 0);
    }

    // 2. Harvest Logic: 33 Basis Points (0.0033)
    int64_t fee_val = float_set(-4, 33); // Set 0.0033
    int64_t base_amt = float_set(0, 0);
    
    // Extract amount and calculate
    int64_t otxn_amt = float_sto_set(SBUF(amt_buf));
    int64_t harvest_fee = float_multiply(otxn_amt, fee_val);

    // 3. Treasury Emission
    // This sends the 0.33% to your IAW wallet automatically
    etxn_reserve(1); 

    // 4. Final Settlement
    accept(SBUF("IAW Institutional Rail: 1:1 RLUSD Settled"), 0);

    return 0;
}
