#include "hookapi.h"

int64_t hook(uint32_t reserved) {
    // 1. Only act on MPT Transfers (where the fee lives)
    if (otxn_type() != 49) // 49 is the code for MPT transactions
        accept(0,0,0);

    // 2. Identify the Fee
    int64_t fee_paid = otxn_amount(); 

    // 3. The Math (2/3 to Family, 1/3 to Partner/Self-Rebate)
    int64_t family_share = (fee_paid * 2) / 3;
    int64_t partner_share = fee_paid - family_share;

    // 4. Send the shares (Emit transactions)
    // NOTE: You will replace the "REPLACE_WITH..." below with your actual wallet addresses
    // etxn_reserve(2);
    // emit(family_share, "REPLACE_WITH_YOUR_FAMILY_VAULT_ADDRESS");
    // emit(partner_share, "REPLACE_WITH_PARTNER_OR_REBATE_ADDRESS");

    accept(0,0,0);
    return 0;
}
