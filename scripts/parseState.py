import re

fd = open("scripts/state_input.dat")

raw = fd.read()
board = []
for _ in range(5):
    board.append([None] * 5)

letters_re = r'(?<=display":")\w(?=")'
columns_re = r'(?<=collumn":)\d'
rows_re = r'(?<=row":)\d'
gem_re = r'(?<=letter_mana":)\d'
multi_re = r'(?<=multipliers":\[)\d'

letter_match = re.findall(letters_re, raw)
rows_match = re.findall(rows_re, raw)
col_match = re.findall(columns_re, raw)
gem_match = re.findall(gem_re, raw)
multiplier_match = re.findall(multi_re, raw)
multiplier_match.extend(["0" for _ in range(25 - len(multiplier_match))])

for (letter, row, col, is_gem, multi) in zip(
    letter_match, rows_match, col_match, gem_match, multiplier_match
):
    r = int(row)
    c = int(col)
    is_gem = int(is_gem)
    board[r][c] = letter.upper() if is_gem else letter.lower()
    if multi != "0":
        board[r][c] = multi + board[r][c]


for row in board:
    for c in row:
        print(c, end="")
    print()
