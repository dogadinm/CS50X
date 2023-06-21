# TODO

while True:
    try:
        hight = int(input('Hight: '))
        if hight > 0 and hight <= 8:
            break
    except:
        False

for row in range(hight):
    for colume in range(hight):
        if (row + colume < hight - 1):
            print(' ', end='')
        else:
            print('#', end='')
    print()