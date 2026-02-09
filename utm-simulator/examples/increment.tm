# Binary incrementer (dodaje 1 binarnom broju)
# Ulaz: niz 0 i 1, LSB desno (najniža cifra desno)
# Primeri:
#   1011   →  1100     (11 + 1 = 12)
#   111    → 1000      (7 + 1 = 8)
#   0      → 1
#   _      → 1         (prazan ulaz postane 1)

blank: _
start: q0
accept: q_accept
reject: q_reject

# q0: idi desno do kraja broja (prvi blank)
q0 0 -> q0 0 R
q0 1 -> q0 1 R
q0 _ -> q1 _ L

# q1: dodaj 1 sa carry-jem (nazad lijevo)
q1 0 -> q_accept 1 S          # 0 + 1 = 1, gotovo, accept
q1 1 -> q1 0 L                # 1 + 1 = 0, carry dalje
q1 _ -> q_accept 1 S          # carry došao do lijeve ivice → dodaj 1 na početku