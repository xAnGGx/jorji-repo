# coding=utf-8
from random import randint


class Warrior:
    def __init__(self, name, hp, atk):
        self.name = name
        self.hp = hp
        self.atk = atk

    def attack(self, enemy):
        damage = self.atk + randint(-2, 2)
        enemy.hp -= damage
        print(self.name, "нанес", enemy.name, damage, "урона")


n = int(input("Введите число героев: "))
heroes = []
for i in range(n):
    name = str(input("Имя "+str(i+1)+" героя: "))
    hp = int(input("Жизни "+str(i+1)+" героя: "))
    atk = int(input("Атака "+str(i+1)+" героя: "))
    heroes.append(Warrior(name, hp, atk))
while len(heroes) > 1:
    attacker = randint(0, len(heroes)-1)
    defender = attacker
    while attacker == defender:
        defender = randint(0, len(heroes)-1)
    heroes[attacker].attack(heroes[defender])
    if heroes[defender].hp <= 0:
        print(heroes[defender].name, "убит! Осталось героев:", len(heroes)-1)
        heroes = heroes[:defender] + heroes[defender+1:]
print(heroes[0].name, "- чемпион!")
