# Binary incrementer (adds +1 to a binary number)
# Convention: LSB is on the right (least significant bit desno)
# Examples:
#   1011  →  1100   (11 + 1 = 12)
#   111   → 1000    (7 + 1 = 8)
#   0     → 1
#   (empty) → 1

blank: _
start: q0
accept: q_accept
reject: q_reject

# Phase 1: move right to the end of the number (find first blank)
q0 0 -> q0 0 R
q0 1 -> q0 1 R
q0 _ -> q1 _ L

# Phase 2: add 1 with carry (move left, flip bits)
q1 0 -> q_accept 1 S     # found 0 → write 1, accept, no more carry
q1 1 -> q1     0 L       # found 1 → write 0, carry continues left
q1 _ -> q_accept 1 S     # carry reached left edge → write 1 at beginning, accept