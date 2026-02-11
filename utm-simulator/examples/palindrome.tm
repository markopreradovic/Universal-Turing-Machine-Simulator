# Palindrome checker over {0,1} (single-tape TM)
# Strategy: Mark first symbol, scan right, check last symbol matches, mark it, return left, repeat

blank: _
start: q0
accept: q_accept
reject: q_reject

# Find first unmarked symbol
q0 _ -> q_accept _ S
q0 X -> q0 X R
q0 Y -> q0 Y R
q0 0 -> q_got_0 X R
q0 1 -> q_got_1 Y R

# We marked first as 0 (now X), scan right to find last unmarked
q_got_0 0 -> q_got_0 0 R
q_got_0 1 -> q_got_0 1 R
q_got_0 X -> q_got_0 X R
q_got_0 Y -> q_got_0 Y R
q_got_0 _ -> q_check_0 _ L

# Check if last symbol is 0 (to match first)
q_check_0 0 -> q_return X L
q_check_0 1 -> q_reject 1 S
q_check_0 X -> q_accept X S
q_check_0 Y -> q_accept Y S
q_check_0 _ -> q_accept _ S

# We marked first as 1 (now Y), scan right to find last unmarked
q_got_1 0 -> q_got_1 0 R
q_got_1 1 -> q_got_1 1 R
q_got_1 X -> q_got_1 X R
q_got_1 Y -> q_got_1 Y R
q_got_1 _ -> q_check_1 _ L

# Check if last symbol is 1 (to match first)
q_check_1 1 -> q_return Y L
q_check_1 0 -> q_reject 0 S
q_check_1 X -> q_accept X S
q_check_1 Y -> q_accept Y S
q_check_1 _ -> q_accept _ S

# Return to beginning to find next unmarked symbol
q_return 0 -> q_return 0 L
q_return 1 -> q_return 1 L
q_return X -> q_return X L
q_return Y -> q_return Y L
q_return _ -> q0 _ R