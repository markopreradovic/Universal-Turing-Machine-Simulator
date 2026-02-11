blank: _
start: q0
accept: q_accept
reject: q_reject

q0 0 -> q1 1 R
q0 1 -> q0 1 R
q0 _ -> q_loop _ S

q_loop 0 -> q_loop 0 R
q_loop 1 -> q_loop 1 L
q_loop _ -> q_loop _ R