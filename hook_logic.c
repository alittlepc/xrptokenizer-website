#include "hookapi.h"

// IAW Global Utility Rail: 0.33% Total Fee
// 0.23% to IAW Vault | 0.1% Referral Pool

int64_t cbak(int64_t reserved) { 
    return 0; 
}

int64_t hook(int64_t reserved) {
    unsigned char amt[48];
    int64_t len = otxn_field(amt, 48, sfAmount);
    if (len < 0) { accept(0,0,0); }

    // 0.33% Fee = amount * 0.0033
    int64_t fee = float_multiply(len, float_set(-4, 33));
    
    // IAW Master Vault - Hex Address
    unsigned char vault[20] = { 
        0x52, 0x44, 0x5A, 0x4B, 0x7A, 0x68, 0x67, 0x4E, 
        0x64, 0x76, 0x42, 0x71, 0x42, 0x65, 0x4D, 0x48, 
        0x47, 0x6A, 0x61, 0x69 
    };

    emit_payment(fee, vault);
    accept(0,0,0);
    return 0;
}
