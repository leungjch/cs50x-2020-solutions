while True:
    n = input("Height: ")
    if n.isdigit() and int(n) > 0 and int(n) < 9:
        n = int(n)
        break

for i in range(n):
    print((" " * (n-(i+1))) + ("#" * (i+1)))