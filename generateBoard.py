from string import ascii_lowercase
from random import choices, randint

kNUM_LINES = 5
kDOUBLE_SYM = "2"
kTRIPLE_SYM = "3"
kMULTI_SYM = "$"


def main():
    fd = open("given.txt", "w")
    lines = []
    for _ in range(kNUM_LINES):
        r = choices(ascii_lowercase, k=kNUM_LINES)
        lines += "".join(r) + "\n"

    placed = set()
    # add double, triple, multi
    for symbol in (kDOUBLE_SYM, kTRIPLE_SYM, kMULTI_SYM):
        rr, rc = (-1, -1)
        while rr == -1 or (rr, rc) in placed:
            rr = randint(0, kNUM_LINES - 1)
            rc = randint(0, kNUM_LINES - 1)
        placed.add((rr, rc))
        idx = rc + rr * (kNUM_LINES + 1)
        lines[idx] = symbol + lines[idx]
    fd.write("".join(lines))


if __name__ == "__main__":
    main()
