blank: _
start: q0
accept: q_accept
reject: q_reject

q0 _ -> q_loop _ S

q_loop _ -> q_loop _ R
q_loop _ -> q_loop _ L