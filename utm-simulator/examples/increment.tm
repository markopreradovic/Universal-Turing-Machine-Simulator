# Palindrome checker over {0,1} (single-tape TM)
# Strategy: Mark leftmost, scan right, match rightmost, repeat

blank: _
start: q0
accept: q_accept
reject: q_reject

# Start: find first unmarked symbol from left
q0 _ -> q_accept _ S
q0 X -> q0 X R
q0 Y -> q0 Y R
q0 0 -> q_mark_left_0 X R
q0 1 -> q_mark_left_1 Y R

# Marked left 0, now scan to rightmost
q_mark_left_0 0 -> q_mark_left_0 0 R
q_mark_left_0 1 -> q_mark_left_0 1 R
q_mark_left_0 X -> q_mark_left_0 X R
q_mark_left_0 Y -> q_mark_left_0 Y R
q_mark_left_0 _ -> q_check_right_0 _ L

# Check if rightmost matches 0
q_check_right_0 0 -> q_return X L
q_check_right_0 1 -> q_reject 1 S
q_check_right_0 X -> q_single_check X S
q_check_right_0 Y -> q_single_check Y S
q_check_right_0 _ -> q_accept _ S

# Marked left 1, now scan to rightmost
q_mark_left_1 0 -> q_mark_left_1 0 R
q_mark_left_1 1 -> q_mark_left_1 1 R
q_mark_left_1 X -> q_mark_left_1 X R
q_mark_left_1 Y -> q_mark_left_1 Y R
q_mark_left_1 _ -> q_check_right_1 _ L

# Check if rightmost matches 1
q_check_right_1 1 -> q_return Y L
q_check_right_1 0 -> q_reject 0 S
q_check_right_1 X -> q_single_check X S
q_check_right_1 Y -> q_single_check Y S
q_check_right_1 _ -> q_accept _ S

# Return to leftmost unmarked position
q_return 0 -> q_return 0 L
q_return 1 -> q_return 1 L
q_return X -> q_return X L
q_return Y -> q_return Y L
q_return _ -> q0 _ R

# Single symbol or all marked - accept
q_single_check X -> q_accept X S
q_single_check Y -> q_accept Y S