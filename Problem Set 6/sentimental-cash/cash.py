# TODO

count = 0

while True:
    try:
        money = float(input('Change: '))
        if money > 0:
            break
    except:
        False

money *= 100


while money >= 25:
    money -= 25
    count += 1

while money >= 10:
    money -= 10
    count += 1

while money >= 5:
    money -= 5
    count += 1

while money >= 1:
    money -= 1
    count += 1

print(f'Total coins: {count} ')