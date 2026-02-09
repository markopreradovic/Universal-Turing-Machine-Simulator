# Jednostavna mašina sa brzom petljom
blank: _
start: q0
accept: q_accept
reject: q_reject

# Počinje normalno
q0 0 -> q1 1 R
q0 1 -> q0 1 R
q0 _ -> q_loop _ S

# Ulaz u petlju
q_loop 0 -> q_loop 0 R
q_loop 1 -> q_loop 1 L
q_loop _ -> q_loop _ R     # ← ovdje se vrtimo lijevo-desno po blankovima