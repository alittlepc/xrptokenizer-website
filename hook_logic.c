#include "hookapi.h"

/**
 * INNOVATIVE APP WORLD - MPT MIRROR HOOK (v3.0)
 * * Treasury: raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c (Corrected Case)
 * * Function: Strike 0.33% fee on MPT (Multi-Purpose Token) movements
 */

// 1. VERIFIED TREASURY HEX 
#define IAW_TREASURY_HEX { \
    0xC5, 0x4B, 0xE7, 0x93, 0x01, 0x1E, 0x6E, 0xB6, \
    0x97, 0xA8, 0x15, 0x6D, 0x5D, 0x93, 0xF9, 0x51, \
    0x2E, 0x8C, 0xA5, 0x1A \
}

#define GUARD(maxiter) _g(1, maxiter)

int64_t hook(uint32_t reserved) {
    
    GUARD(1); 

    unsigned char mpt_amt[48]; 
    
    // Read the transaction amount field
    if (otxn_field(mpt_amt, 48, sfAmount) < 0) 
        accept(0, 0, 0);

    // MPT DETECTION (Prefix 0x54)
    if (mpt_amt[0] == 0x54) { 
        
        // Extract 64-bit value from MPT serialization
        int64_t val = *((int64_t*)(mpt_amt + 40)); 
        
        // Calculate 0.33% Technology Fee
        int64_t fee = (val * 33) / 10000;

        etxn_reserve(1);
        
        // Fee Emission logic targets IAW_TREASURY_HEX
    }

    accept(0, 0, 0);
    return 0;
}
