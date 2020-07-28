from random import randint


def password(len: int) -> str:
    s = ""
    for i in range(len):
        if randint(0, 3) == 3:
            s += chr(randint(48, 57))
        else:
            if randint(0, 1) == 1:
                s += chr(randint(65, 90))
            else:
                s += chr(randint(97, 122))
    return s


x = input('Длина пароля: ')
try:
    x = int(x)
except ValueError:
    print('Неверный формат!')
    exit(0)
print(password(x))
