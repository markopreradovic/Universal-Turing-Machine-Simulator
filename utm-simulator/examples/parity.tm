# Parity checker: accepts if the number of '1's is EVEN
# Rejects if ODD

blank: _
start: q_start
accept: q_accept
reject: q_reject

# Start: begin scanning
q_start 0 -> q_even 0 R
q_start 1 -> q_odd  1 R
q_start _ -> q_accept _ S

# q_even: even number of 1s so far
q_even 0 -> q_even 0 R
q_even 1 -> q_odd  1 R
q_even _ -> q_accept _ S

# q_odd: odd number of 1s so far
q_odd  0 -> q_odd  0 R
q_odd  1 -> q_even 1 R
q_odd  _ -> q_reject _ S