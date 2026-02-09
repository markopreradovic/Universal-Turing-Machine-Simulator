# Binary incrementer (dodaje 1 binarnom broju na traci)
# Ulaz: niz od 0 i 1 (MSB lijevo ili LSB lijevo – ovdje LSB desno)
# Primer ulaza: 01101   → očekivani izlaz: 01110

blank: _
start: q0
accept: q_accept
reject: q_reject

# q0: idi desno do kraja broja (prvi blank)
q0 0 -> q0 0 R
q0 1 -> q0 1 R
q0 _ -> q1 _ L

# q1: nosi 1 nazad (carry)
q1 0 -> q_accept 1 S          # bilo je 0 → postane 1, gotovo
q1 1 -> q1 0 L                # bilo je 1 → postane 0, nosi dalje
q1 _ -> q_accept 1 S          # ako dođemo do lijeve strane i još ima carry → dodaj 1 na početku