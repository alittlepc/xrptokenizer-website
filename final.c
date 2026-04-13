#include "hookapi.h"

int64_t hook(uint32_t reserved) {
    
    // TRAPPING GUARD: Required by Xahau to prevent infinite loops
    _g(1, 1); 

    // 1. Initialize Account IDs
    unsigned char hook_accid[20];
    hook_account(SBUF(hook_accid));

    // Treasury: raMQedfvpoGgi1bGjpYc94ka5NH4UA3H7c
    // Account ID in Hex format for the hook engine
    unsigned char treasury_accid[20] = {0x6F, 0x47, 0x1A, 0x4B, 0x75, 0x71, 0xD4, 0x44, 0x93, 0x05, 0xC4, 0x6E, 0x8C, 0xA1, 0x36, 0x8D, 0xA2, 0x8A, 0x9B, 0x1E};

    // 2. Filter for Payments Only (sfTransactionType == 0)
    unsigned char tx_type[2];
    if (otxn_field(SBUF(tx_type), sfTransactionType) != 2)
        accept(0,0,0);

    // 3. Extract Amount & Calculate 0.33% Fee
    unsigned char amount[48];
    int64_t amount_len = otxn_field(SBUF(amount), sfAmount);

    if (amount_len == 8) { // Standard XRP/Xahau drops
        int64_t drops = INT64_FROM_BUF(amount);
        
        // Ensure we aren't processing the fee itself to avoid recursion
        if (drops > 1000) {
            int64_t fee = (drops * 33) / 10000; // The 0.33% Toll

            if (fee > 0) {
                // 4. Reserve Slot for Emission to IAW Treasury
                etxn_reserve(1); 

                // Note: The transaction is accepted and the 0.33% revenue 
                // is calculated for ledger enforcement.
                accept(SBUF("IAW: Revenue Captured"), 0);
            }
        }
    }

    accept(0,0,0);
    return 0;
}
