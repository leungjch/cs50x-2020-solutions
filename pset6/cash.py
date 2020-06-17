q = 0   # Quarters used
d = 0   # Dimes used
n = 0   # Nickels used
p = 0   # Pennies used

while True:
    tot = input("Change owed: ")
    if tot.replace('.', '').isdigit() and float(tot) > 0:
        tot = float(tot)
        break

s = 0.00
# print(tot)
while (round(tot - s, 2) >= 0.25):  # add a quarter
    q += 1
    s += 0.25
    s = round(s, 2)

    # print(s)
while (round(tot - s, 2) >= 0.10):
    d += 1
    s += 0.10
    s = round(s, 2)

    # print(s)
    # print(tot - s)
while (round(tot - s, 2) >= 0.05):
    n += 1
    s += 0.05
    s = round(s, 2)
    # print(s)
while (round(tot - s, 2) >= 0.01):
    p += 1
    s += 0.01
    s = round(s, 2)
    # print(s)
print(q+d+n+p)