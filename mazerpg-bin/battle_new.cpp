#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <cmath>
#include <vector>
#include <SFML/Audio.hpp>
using namespace std;
bool unmute;
const int items_all = 19;
const int items_battle = 10;
const int enemy_ults = 9;
enum ConsoleColor
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGrey = 7,
    DarkGrey = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};
class hero
{
    public:
        int aura;
        int grade1;
        int grade2;
        int grade3;
        int hp;
        int max_hp;
        int mp;
        int max_mp;
        int charge;
        int damage;
        int armor;
        int ability;
        int weapon_damage;
        int weapon_ap;
        int weapon_type;
        int weapon_level;
        int weapon_id;
        int weapon_rarity;
        int weapon_hp;
        int weapon_mp;
        int wear_armor;
        int wear_ap;
        int wear_type;
        int wear_level;
        int wear_id;
        int wear_rarity;
        int wear_hp;
        int wear_mp;
        int amulet_hp;
        int amulet_mp;
        int amulet_id;
        int amulet_rarity;
        int ring_hp;
        int ring_mp;
        int ring_id;
        int ring_rarity;
        int level;
        int exp;
        int gold;
        int backpack[items_all];
};
class enemy
{
    public:
        int hp;
        int max_hp;
        int damage;
        int armor;
        int ult_id;
        int ult_charge;
        int enemy_ult[enemy_ults];
};
class skill
{
    public:
        bool exist;
        string name;
        bool deal_damage;
        bool deal_poison;
        bool deal_fire;
        bool deal_blood;
        bool deal_stun;
        bool deal_sleep;
        bool give_dodge;
        bool give_rage;
        bool give_enlight;
        bool need_ready;
        int damage_multi;
        int ability_multi;
        int poison;
        int poison_damage;
        int fire;
        int blood;
        int stun;
        int sleep;
        int dodge;
        int rage;
        int enlight;
        int ready;
        int mana;
        int mana_full;
        int mana_dynamic;
};
class aura
{
    bool give_damage;
    int damage_k;
    bool give_ability;
    int ability_k;
    bool give_dodge;
    int dodge_k;
    bool give_heal;
    int heal_k;
};
hero myHero;
enemy myEnemy;
aura myAura;
skill skills[5];    // SKILLS WITHOUT AURA
void printKey(string key)
{
    setColor(LightGrey, Black);
    cout << "[";
    setColor(White, Black);
    cout << key;
    setColor(LightGrey, Black);
    cout << "]";
    return;
}
void setColor(ConsoleColor text, ConsoleColor background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}
bool manaCheck(int cost)
{
    setCursorPosition(3, 3);
    if (myHero.mp >= cost)
    {
        myHero.mp -= cost;
        return true;
    }
    else
    {
        cout << "Недостаточно ОМ!";
        return false;
    }
}
void manaCost(int mana)
{
    cout << "(";
    setColor(LightCyan, Black);
    cout << mana;
    setColor(LightCyan, Black);
    cout << "ОМ )";
}
int heroInflictDamage(int damage, int ability, int random_damage, int random_ability, int damage_buff_lvl, int ability_buff_lvl, bool crit, bool insta)
{
    int value;
    setCursorPosition(3, 3);
    if (myEnemy.enemy_ult[4] > 0 || myEnemy.enemy_ult[8] > 0)
    {
        damage = 0;
        random_damage = 0;
    }
    if (myEnemy.enemy_ult[5] > 0 || myEnemy.enemy_ult[8] > 0)
    {
        ability = 0;
        random_ability = 0;
    }
    if (insta) value = myEnemy.hp;
    else value = (damage+random_damage)*damage_buff_lvl+(ability+random_ability)*ability_buff_lvl;
    if (!insta && crit)
    {
        setColor(White, Black);
        cout << "КРИТ! ";
        value *= 2;
    }
    myEnemy.hp -= value;
    setColor(LightGrey, Black);
    cout << "Герой нанес ";
    setColor(White, Black);
    cout << value;
    setColor(LightGrey, Black);
    cout << " урона";
    return value;
}
int heroDamageSelf(int damage)
{
    setCursorPosition(3, 4);
    cout << "Нанесено ";
    setColor(LightRed, Black);
    cout << damage;
    setColor(LightGrey, Black);
    cout << " урона себе";
    myHero.hp -= damage;
    if (myHero.hp < 0) myHero.hp = 0;
    return damage;
}
int enemyInflictDamage(int damage, int armor, int crit_chance, int random_damage, bool block)
{
    int value;
    setCursorPosition(45, 10);
    int crit = rand()%1000;
    if (block)
    {
        value = 0;
        setColor(White, Black);
        cout << "БЛОК! ";
        setColor(LightGrey, Black);
        cout << "Получено ";
        setColor(White, Black);
        cout << value;
        setColor(LightGrey, Black);
        cout << " урона";
    }
    else if (crit < crit_chance)
    {
        value = max(0, damage+random_damage);
        myHero.hp -= value;
        setColor(Red, Black);
        cout << "КРИТ! ";
        setColor(LightGrey, Black);
        cout << "Получено ";
        setColor(Red, Black);
        cout << value;
        setColor(LightGrey, Black);
        cout << " урона";
    }
    else
    {
        value = max(0, damage-armor+random_damage);
        myHero.hp -= value;
        cout << "Получено ";
        if (value == 0) setColor(White, Black);
        else setColor(Red, Black);
        cout << value;
        setColor(LightGrey, Black);
        cout << " урона";
    }
    return value;
}
int heroHeal(int value, int max_hp, bool healFromDamage)
{
    if (healFromDamage) setCursorPosition(3, 4);
    else setCursorPosition(3, 3);
    myHero.hp += value;
    if (myHero.hp >= max_hp)
    {
        myHero.hp = max_hp;
        cout << "ОЗ ";
        setColor(LightGreen, Black);
        cout << "полностью восстановлены";
        setColor(LightGrey, Black);
        cout << "!";
    }
    else
    {
        cout << "Восстановлено ";
        setColor(LightGreen, Black);
        cout << value;
        setColor(LightGrey, Black);
        cout << " ОЗ";
    }
    return value;
}
int heroMana(int value, int max_mp)
{
    setCursorPosition(3, 3);
    myHero.mp += value;
    if (myHero.mp >= max_mp)
    {
        myHero.mp = max_mp;
        cout << "ОМ ";
        setColor(LightCyan, Black);
        cout << "полностью восстановлены";
        setColor(LightGrey, Black);
        cout << "!";
    }
    else
    {
        cout << "Восстановлено ";
        setColor(LightBlue, Black);
        cout << value;
        setColor(LightGrey, Black);
        cout << " ОМ";
    }
    return value;
}
int if_stun(bool hero, int chance, int turns, int cur_turn)
{
    if (hero)
    {
        setCursorPosition(3, 4);
        if (rand()%100 < chance && turns > cur_turn)
        {
            if (turns > 1)
            {
                cout << "Враг оглушен на " << turns << " ход";
                if (turns >= 5) cout << "ов";
                else if (turns >= 2) cout << "а";
            }
            else cout << "Враг был оглушен на этом ходу";
            return turns;
        }
        else return cur_turn;
    }
    else
    {
        setCursorPosition(45, 11);
        if (rand()%100 < chance && turns > cur_turn)
        {
            if (turns > 1)
            {
                cout << "Враг оглушил героя на " << turns << " ход";
                if (turns >= 5) cout << "ов";
                else if (turns >= 2) cout << "а";
            }
            return turns;
        }
        else return cur_turn;
    }
}
int if_poison(bool hero, int poison_damage, int cur_poison_damage, int turns)
{
    if (hero)
    {
        setCursorPosition(3, 4);
        if (cur_poison_damage == 0)
        {
            cout << "Враг заражен ядом на ";
            setColor(White, Black);
            cout << turns;
            setColor(LightGrey, Black);
            cout << " ход";
            if (turns >= 5) cout << "ов";
            else if (turns >= 2) cout << "а";
            setCursorPosition(3, 5);
            cout << "и получает от него ";
            setColor(LightMagenta, Black);
            cout << poison_damage;
            setColor(LightGrey, Black);
            cout << " урона!";
            return poison_damage;
        }
        else
        {
            cout << "Враг уже заражен ядом!";
            return cur_poison_damage;
        }
    }
    else
    {
        setCursorPosition(45, 11);
        if (cur_poison_damage == 0)
        {
            cout << "Герой заражен ядом на ";
            setColor(Red, Black);
            cout << turns;
            setColor(LightGrey, Black);
            cout << " ход";
            if (turns >= 5) cout << "ов";
            else if (turns >= 2) cout << "а";
            setCursorPosition(45, 12);
            cout << "и получает от него ";
            setColor(Magenta, Black);
            cout << poison_damage;
            setColor(LightGrey, Black);
            cout << " урона!";
            return poison_damage;
        }
        else
        {
            cout << "Враг уже заражен ядом!";
            return cur_poison_damage;
        }
    }
}
void useSkill(int id)
{

}
void battle(int type, int enemy_level, float diffC)
{
    ofstream fout;
    string name;
    string monsters[] = {"Скелет","Вспыш","Тень","Фея","Кентавр","Виверн","Ледышка","Демон","Гомункул","Люмен","Падший ангел","Аэр","Арес","Летучий змей","Люкс","Ведьма","Голем","Водяной элементаль","Огненный элементаль","Земляной элементаль","Воздушный элементаль","Темный элементаль","Светлый элементаль","Химера","Цербер","Феникс","Валькирия","Циклон","Ехидна","Оверлорд"};
    int element[] = {0,2,0,3,3,4,1,2,3,5,5,4,2,4,5,0,3,1,2,3,4,0,5,4,0,2,5,3,1,-2};
    int stun, dodge, ready, sleep, blood, rage, enlight, poison, fire;
    int poison_damage, fire_damage, fire_damage;
    int stun_chance, crit_chance, block_chance, insta_chance, dodge_chance;
    float damageC, abilityC, healC, armorC, chargeC;
    for (i = 0; i < enemy_ults; i++) myEnemy.enemy_ult[i] = 0;
    music.stop();
    if (unmute) bs.play();
    for (i = 0; i < 79; i++)    // LOAD SCREEN
    {
        for (j = 0; j < 25; j++)
        {
            if (i%2 == 0) setCursorPosition(i/2, j);
            else setCursorPosition(78-(i/2), j);
            cout << "#";
        }
        Sleep(15);
    }
    bs.stop();
    system("cls");
    switch(type)    // ENEMY GEN
    {
        case 0:
            if (!music.openFromFile("music/battle.ogg")) return;
            myEnemy.max_hp = diffC*(0.1*enemy_level*enemy_level+100*enemy_level+500-6*enemy_level+rand()%(12*enemy_level));
            myEnemy.damage = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.armor = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.ult_id = -1;
            name_n = rand()%23;
            hp_chance = rand()%100;
            if (hp_chance >= 95) myEnemy.max_hp *= 3;
            else if (hp_chance >= 80) myEnemy.max_hp *= 2;
            break;
        case 1:
            if (!music.openFromFile("music/boss.ogg")) return;
            myEnemy.max_hp = 8*diffC*(0.1*enemy_level*enemy_level+100*enemy_level+500-6*enemy_level+rand()%(12*enemy_level));
            myEnemy.damage = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.armor = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.ult_id = rand()%7;
            name_n = 23+rand()%6;
            hp_chance = 0;
            break;
        case 2:
            if (!music.openFromFile("music/overlord.ogg")) return;
            myEnemy.max_hp = 30*diffC*(0.1*enemy_level*enemy_level+100*enemy_level+500-6*enemy_level+rand()%(12*enemy_level));
            myEnemy.damage = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.armor = diffC*(0.01*enemy_level*enemy_level+6*enemy_level+15-enemy_level+rand()%(2*enemy_level));
            myEnemy.ult_id = 7;
            name_n = 29;
            hp_chance = 0;
            break;
    }
    enemy_type = element[name_n];
    name = monsters[name_n];
    if (unmute) music.play();
    myEnemy.hp = myEnemy.max_hp;
    myEnemy.ult_charge = 0;
    damageC = 1;    // COEFF INIT
    if (myHero.weapon_id == 1) damageC += 0.05+0.05*myHero.weapon_rarity;
    if (myHero.wear_id == 1) damageC += 0.05+0.05*myHero.wear_rarity;
    if (myHero.amulet_id == 1) damageC += 0.15+0.05*myHero.amulet_rarity;
    if (myHero.ring_id == 1) damageC += 0.15+0.05*myHero.ring_rarity;
    if (enemy_type == -2) damageC += 0.2+0.05*myHero.weapon_level;
    else if (myHero.weapon_type - enemy_type == -1 || myHero.weapon_type - enemy_type == 5) damageC += 0.2+0.05*myHero.weapon_level;
    else if (myHero.weapon_type - enemy_type == 1 || myHero.weapon_type - enemy_type == -5) damageC -= 0.2-0.05*myHero.weapon_level;
    armorC = 1;
    if (myHero.weapon_id == 2) armorC += 0.05+0.05*myHero.weapon_rarity;
    if (myHero.wear_id == 2) armorC += 0.05+0.05*myHero.wear_rarity;
    if (myHero.amulet_id == 2) armorC += 0.15+0.05*myHero.amulet_rarity;
    if (myHero.ring_id == 2) armorC += 0.15+0.05*myHero.ring_rarity;
    if (enemy_type == -2) armorC += 0.2+0.05*myHero.wear_level;
    else if (myHero.wear_type - enemy_type == -1 || myHero.wear_type - enemy_type == 5) armorC += 0.2+0.05*myHero.wear_level;
    else if (myHero.wear_type - enemy_type == 1 || myHero.wear_type - enemy_type == -5) armorC -= 0.2-0.05*myHero.wear_level;
    chargeC = 10;   // MAX = 1000
    if (myHero.weapon_id == 3) chargeC += 2+2*myHero.weapon_rarity;
    if (myHero.wear_id == 3) chargeC += 2+2*myHero.wear_rarity;
    if (myHero.amulet_id == 3) chargeC += 5+5*myHero.amulet_rarity;
    if (myHero.ring_id == 3) chargeC += 5+5*myHero.ring_rarity;
    stun_chance = 0;    // 0-99
    if (myHero.weapon_id == 4) stun_chance += 4+3*myHero.weapon_rarity;
    if (myHero.wear_id == 4) stun_chance += 4+3*myHero.wear_rarity;
    if (myHero.amulet_id == 4) stun_chance += 10+5*myHero.amulet_rarity;
    if (myHero.ring_id == 4) stun_chance += 10+5*myHero.ring_rarity;
    healC = 1;  // float
    if (myHero.weapon_id == 5) healC += 0.1+0.1*myHero.weapon_rarity;
    if (myHero.wear_id == 5) healC += 0.1+0.1*myHero.wear_rarity;
    if (myHero.amulet_id == 5) healC += 0.2+0.15*myHero.amulet_rarity;
    if (myHero.ring_id == 5) healC += 0.2+0.15*myHero.ring_rarity;
    abilityC = 1;   //float
    if (myHero.weapon_id == 7) abilityC += 0.05+0.05*myHero.weapon_rarity;
    if (myHero.wear_id == 7) abilityC += 0.05+0.05*myHero.wear_rarity;
    if (myHero.amulet_id == 7) abilityC += 0.15+0.05*myHero.amulet_rarity;
    if (myHero.ring_id == 7) abilityC += 0.15+0.05*myHero.ring_rarity;
    poisonC = 1;    //float
    if (myHero.weapon_id == 8) poisonC += 0.2+0.05*myHero.weapon_rarity;
    if (myHero.wear_id == 8) poisonC += 0.2+0.05*myHero.wear_rarity;
    if (myHero.amulet_id == 8) poisonC += 0.4+0.1*myHero.amulet_rarity;
    if (myHero.ring_id == 8) poisonC += 0.4+0.1*myHero.ring_rarity;
    pot_max_hp = myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp; // HELPING FORMULAE
    max_mp = myHero.max_mp+myHero.amulet_mp+myHero.ring_mp+myHero.weapon_mp+myHero.wear_mp;
    damage = damageC*(myHero.damage+myHero.weapon_damage);
    armor = armorC*(myHero.armor+myHero.wear_armor);
    ability = abilityC*(myHero.ability+myHero.weapon_ap+myHero.wear_ap);
    crit_chance = 70*myHero.weapon_level;
    block_chance = 70*myHero.wear_level;
    if ((myHero.weapon_type - enemy_type == -1 || myHero.weapon_type - enemy_type == 5) && name_n < 23) insta_chance = 5*myHero.weapon_level;
    else insta_chance = 0;
    prev_hp = myHero.hp;
    max_hp_l = 1;   // HUD LOADING
    while (myEnemy.max_hp >= pow(10, max_hp_l)) max_hp_l++;
    setCursorPosition(67-max_hp_l, 6);
    cout << " / " << myEnemy.max_hp << " :ОЗ";
    setCursorPosition(42, 3);
    cout << "=================================";
    setCursorPosition(43, 4);
    if (type == 0) cout << "=                         ======";
    else cout << "=                         =   %=";
    setCursorPosition(44, 5);
    cout << "===============================";
    setCursorPosition(3, 7);
    cout << "===========";
    setCursorPosition(3, 8);
    cout << "=          =";
    setCursorPosition(3, 9);
    cout << "===============================";
    setCursorPosition(3, 10);
    cout << "=   %=                         =";
    setCursorPosition(3, 11);
    cout << "=================================";
    setCursorPosition(0, 14);
    cout << "===============================================================================";
    setCursorPosition(0, 23);
    cout << "===============================================================================";
    setCursorPosition(0, 15);
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               ";
    setCursorPosition(59-(name.size())/2, 2);   // ENEMY NAME
    if (name_n == 29) setColor(LightRed, Black);
    switch(enemy_type)
    {
        case 0:
            setColor(DarkGrey, Black);
            break;
        case 1:
            setColor(Blue, Black);
            break;
        case 2:
            setColor(Red, Black);
            break;
        case 3:
            setColor(Green, Black);
            break;
        case 4:
            setColor(Cyan, Black);
            break;
        case 5:
            setColor(White, Black);
            break;
    }
    cout << name;
    setColor(LightGrey, Black);
    setCursorPosition(0, 15);
    for (i = 0; i < 5; i++) // OUTPUT SPELLS
    {
        key_string = "";
        key_string += char('1'+i);
        printKey(key_string);
        cout << " ";
        if (skills[0].exist) cout << skills[0].name << endl;
        else switch(i)
        {
            case 1:
                cout << "<Нужен 5 уровень>";
                break;
            case 2:
                cout << "<Нужен 5 уровень>";
                break;
            case 3:
                cout << "<Нужен 25 уровень>";
                break;
            case 4:
                cout << "<Нужен 50 уровень>";
                break;
        }
    }
    do
    {
        for (i = 0; i < 5; i++)
        {
            setCursorPosition(5+skills[i].name, 16+i);
            manaCost(skills[i].mana+int(skills[i].mana_full*myHero.max_mp/100)+int(skills[i].mana_dynamic*myHero.mp/100));
        }
        // BATTLE
    }
    // AFTER BATTLE
}
