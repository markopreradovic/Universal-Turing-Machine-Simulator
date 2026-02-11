# Copy string: duplicates input after a blank separator (e.g. "1011" → "1011_1011")
blank: _
start: q0
accept: q_accept
reject: q_reject

# Find end of input and add separator
q0 0 -> q0 0 R
q0 1 -> q0 1 R
q0 _ -> q_add_sep _ R

q_add_sep _ -> q_start_copy _ L   # add separator (blank), go left to start copying

# Start copying from left
q_start_copy 0 -> q_carry0 0 L
q_start_copy 1 -> q_carry1 1 L
q_start_copy _ -> q_accept _ S   # end of original, all copied

# Carry '0' left
q_carry0 0 -> q_carry0 0 L
q_carry0 1 -> q_carry0 1 L
q_carry0 _ -> q_copy0 0 R

# Copy '0' at end
q_copy0 0 -> q_copy0 0 R
q_copy0 1 -> q_copy0 1 R
q_copy0 _ -> q_return0 0 L

# Return to left
q_return0 0 -> q_return0 0 L
q_return0 1 -> q_return0 1 L
q_return0 _ -> q_start_copy _ R

# Carry '1' left
q_carry1 0 -> q_carry1 0 L
q_carry1 1 -> q_carry1 1 L
q_carry1 _ -> q_copy1 1 R

# Copy '1' at end
q_copy1 0 -> q_copy1 0 R
q_copy1 1 -> q_copy1 1 R
q_copy1 _ -> q_return1 1 L

# Return to left
q_return1 0 -> q_return1 0 L
q_return1 1 -> q_return1 1 L
q_return1 _ -> q_start_copy _ R

# Reject if stuck
q_reject 0 -> q_reject 0 S
q_reject 1 -> q_reject 1 S
q_reject _ -> q_reject _ S