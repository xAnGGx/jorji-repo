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
bool was[10][10];
char maze[21][21];
int exp_border[101];
int data[73];   // DEPENDS ON ITEMS_ALL
const int items_all = 19;
const int items_battle = 10;
const int enemy_ults = 9;
sf::Music music;
sf::SoundBuffer b0, b1, b2, b3, b4, b5, b6, pt, bss, spp, die, win, nxt, nope, an1, an2, an3, str, skl;
sf::Sound a0, a1, a2, a3, a4, a5, a6, pts, bs, sp, di, wi, nx, no, h1, h2, h3, sq, sk;
class hero
{
    public:
        int perks;
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
        int ult_id;
        int ult_charge;
        int enemy_ult[enemy_ults];
};
hero myHero;
enemy myEnemy;
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
unsigned long hash_33()
{
    unsigned long hash_33 = 5381;
    for (int i = 0; i < 73; i++) hash_33 = ((hash_33 << 5) + hash_33) + data[i];
    return hash_33;
}
void gen(int x, int y)
{
	int i, a, b, k;
	vector<int> go(0);
	was[x][y] = true;
	if (y != 0) go.push_back(0);
	if (x != 9) go.push_back(1);
	if (y != 9) go.push_back(2);
	if (x != 0) go.push_back(3);
	for (i = 0; i < 6; i++)
	{
		a = rand()%(go.size());
		b = rand()%(go.size());
		k = go[a];
		go[a] = go[b];
		go[b] = k;
	}
	for (i = 0; i < int(go.size()); i++)
    {
    	switch(go[i])
    	{
    		case 0:	// UP
    			if (!was[x][y-1])
    			{
    				maze[2*x+1][2*y] = '.';
    				gen(x, y-1);
				}
    			break;
    		case 1:	// RIGHT
    			if (!was[x+1][y])
    			{
    				maze[2*x+2][2*y+1] = '.';
    				gen(x+1, y);
				}
    			break;
    		case 2:	// DOWN
    			if (!was[x][y+1])
    			{
    				maze[2*x+1][2*y+2] = '.';
    				gen(x, y+1);
				}
    			break;
    		case 3:	// LEFT
    			if (!was[x-1][y])
    			{
    				maze[2*x][2*y+1] = '.';
    				gen(x-1, y);
				}
    			break;
		}
    }
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
void bossLine(int s)    // BOSS SPEECH ON ENCOUNTER
{
    int i;
    string v[8];
    v[0] = "Ты пожалеешь, что явился сюда!";
    v[1] = "Насладись своим последним вздохом!";
    v[2] = "Как жаль, что все твои старания тщетны";
    v[3] = "Что ты возомнил о себе, смертный?";
    v[4] = "Тебе конец!";
    v[5] = "Ты упустил возможность вернуться живым";
    v[6] = "Я тебя сокрушу!";
    v[7] = "От тебя и мокрого места не останется!";
    for (i = 0; i < 4; i++)
    {
        cout << ".";
        Sleep(500);
    }
    cout << " ";
    if (unmute) sp.play();
    for (i = 0; i < int(v[s].size()); i++)
    {
        cout << v[s][i];
        Sleep(52);
    }
    sp.stop();
}
void endText()  // CONGRATULATIONS
{
    int i, j;
    string v[4];
    v[0] = "Поздравляем! Вы победили Оверлорда!";
    v[1] = "Вы спасли Эзреаль и весь его народ. Ваш героический поступок не будет забыт,";
    v[2] = "а Вам, как искателю приключений, остается путешествовать по этому прекрасному";
    v[3] = "цветущему миру и быть готовым помочь ему в трудный час!";
    cout << endl << " ";
    for (i = 0; i < int(v[0].size()); i++)
    {
        cout << v[0][i];
        Sleep(52);
    }
    cout << endl << endl;
    for (i = 1; i < 4; i++)
    {
        cout << " ";
        for (j = 0; j < int(v[i].size()); j++)
        {
            cout << v[i][j];
            Sleep(52);
        }
        cout << endl;
    }
    Sleep(1000);
}
void storyPrint(int a, int b, int c, int d) // HERO STORY
{
    int i;
    string v1[5], v2[5], v3[5], v4[5], p;
    v1[0] = "Ты - королевский стражник из княжества Вальгендорф";
    v1[1] = "Ты был обычным крестьянином, но однажды ты освободился от оков рабства";
    v1[2] = "Ты - отшельник, повидавший весь мир вдоль и поперек";
    v1[3] = "Пока тебя не превратили в человека, ты был козлом (в прямом смысле)";
    v1[4] = "Поверь мне, ты не хочешь знать, кем ты был и кто ты есть";
    v2[0] = "Имея довольно заурядную внешность, ты не очень привлекателен";
    v2[1] = "Красивый, статный, по красоте тебе нет равных";
    v2[2] = "Обделенный вниманием в детстве, ты очень скромен и необщителен";
    v2[3] = "Несмотря на свой ум, в детстве ты часто падал в колодец";
    v2[4] = "Будучи крайне хитрым, ты всегда мог найти общий язык с кем угодно";
    v3[0] = "От нечего делать, однажды ты просто решил побродить по Эзреалю";
    v3[1] = "Из-за недопонимания со стороны своих родителей, ты сбежал из дома";
    v3[2] = "Не знаю, лежать на диване тебя тоже устраивало";
    v3[3] = "Экстремал по натуре, ты бродишь по всему Эзреалю в поисках монстров";
    v3[4] = "Насмотрелся всяких этих аниме и теперь подражаешь главному герою";
    v4[0] = "Король приказал тебе убить Оверлорда, желающего уничтожить весь мир";
    v4[1] = "Ты не знал, чем заняться, и пошел искать какого-то там Оверлорда";
    v4[2] = "Вдруг, ты почувствовал нечто внутри себя и возомнил себя избранным";
    v4[3] = "В детстве Оверлорд поставил тебе подножку, и ты больно ударился";
    v4[4] = "Убить Оверлорда было личной целью всей твоей жизни";
    cout << endl << " [============================================================================]" << endl << endl << " ";
    for (i = 0; i < int(v1[a].size()); i++)
    {
        cout << v1[a][i];
        Sleep(52);
    }
    cout << endl << endl << " ";
    for (i = 0; i < int(v2[b].size()); i++)
    {
        cout << v2[b][i];
        Sleep(52);
    }
    cout << endl << endl << " ";
    p = "Как же ты стал путешественником?";
    for (i = 0; i < int(p.size()); i++)
    {
        cout << p[i];
        Sleep(52);
    }
    cout << endl << endl << " ";
    for (i = 0; i < int(v3[c].size()); i++)
    {
        cout << v3[c][i];
        Sleep(52);
    }
    cout << endl << endl << " ";
    for (i = 0; i < int(v4[d].size()); i++)
    {
        cout << v4[d][i];
        Sleep(52);
    }
    cout << endl << endl << " [============================================================================]" << endl << endl << " ";
    Sleep(500);
    cout << ".";
    Sleep(500);
    cout << ".";
    Sleep(500);
    cout << ".";
    Sleep(500);
    p = " Вот и вся твоя история";
    for (i = 0; i < int(p.size()); i++)
    {
        cout << p[i];
        Sleep(52);
    }
    setCursorPosition(1, 23);
    system("pause");
}
void credits(bool endgame)
{
    int i;
    string v[18];
    v[0] = "Программист";
    v[1] = "Георгий 'AnGG' Ангени";
    v[2] = "Художник";
    v[3] = "Георгий 'AnGG' Ангени";
    v[4] = "Писатель";
    v[5] = "Георгий 'AnGG' Ангени";
    v[6] = "Музыка и звуки из игр";
    v[7] = "Earthbound Copyripght 1994 Ape & HAL Laboratory";
    v[8] = "Earthbound Beginnings Copyripght 1989 Ape";
	v[9] = "Tetris Copyright 1989 Nintendo";
	v[10] = "Final Fantasy VII Copyright Square Co Ltd. 1997";
	v[11] = "Тестеры";
	v[12] = "Илья 'dyra4ek' Куликовских";
	v[13] = "Александр 'G.' Ларин";
	v[14] = "Сергей 'Dipodont' Овчаров";
	v[15] = "Отдельное спасибо за помощь в реализации проекта";
	v[16] = "Мария Зубарева";
	v[17] = " [================================== Титры ===================================]";
    if (endgame)
    {
        for (i = 0; i < int(v[17].size()); i++)
        {
            if (i%2 == 0)
            {
                setCursorPosition(40+i/2, 1);
                cout << v[17][40+i/2];
            }
            else
            {
                setCursorPosition(39-i/2, 1);
                cout << v[17][39-i/2];
            }
            Sleep(52);
        }
        Sleep(500);
        setCursorPosition(1, 3);
        for (i = 0; i < int(v[0].size()); i++)
        {
            cout << v[0][i];
            Sleep(52);
        }
        for (i = v[1].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[1].size()+i, 4);
            cout << v[1][i];
            Sleep(52);
        }
        setCursorPosition(1, 5);
        for (i = 0; i < int(v[2].size()); i++)
        {
            cout << v[2][i];
            Sleep(52);
        }
        for (i = v[3].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[3].size()+i, 6);
            cout << v[3][i];
            Sleep(52);
        }
        setCursorPosition(1, 7);
        for (i = 0; i < int(v[4].size()); i++)
        {
            cout << v[4][i];
            Sleep(52);
        }
        for (i = v[5].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[5].size()+i, 8);
            cout << v[5][i];
            Sleep(52);
        }
        setCursorPosition(1, 9);
        for (i = 0; i < int(v[6].size()); i++)
        {
            cout << v[6][i];
            Sleep(52);
        }
        for (i = v[7].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[7].size()+i, 10);
            cout << v[7][i];
            Sleep(52);
        }
        for (i = v[8].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[8].size()+i, 11);
            cout << v[8][i];
            Sleep(52);
        }
        for (i = v[9].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[9].size()+i, 12);
            cout << v[9][i];
            Sleep(52);
        }
        for (i = v[10].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[10].size()+i, 13);
            cout << v[10][i];
            Sleep(52);
        }
        setCursorPosition(1, 14);
        for (i = 0; i < int(v[11].size()); i++)
        {
            cout << v[11][i];
            Sleep(52);
        }
        for (i = v[12].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[12].size()+i, 15);
            cout << v[12][i];
            Sleep(52);
        }
        for (i = v[13].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[13].size()+i, 16);
            cout << v[13][i];
            Sleep(52);
        }
        for (i = v[14].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[14].size()+i, 17);
            cout << v[14][i];
            Sleep(52);
        }
        setCursorPosition(1, 18);
        for (i = 0; i < int(v[15].size()); i++)
        {
            cout << v[15][i];
            Sleep(52);
        }
        for (i = v[16].size()-1; i >= 0; i--)
        {
            setCursorPosition(79-v[16].size()+i, 19);
            cout << v[16][i];
            Sleep(52);
        }
        setCursorPosition(1, 23);
        Sleep(500);
        system("pause");
    }
    else
    {
        system("cls");
        cout << endl << v[17] << endl << endl;
        cout << " " << v[0] << endl;
        for (i = 0; i < int(79-v[1].size()); i++) cout << " ";
        cout << v[1] << endl;
        cout << " " << v[2] << endl;
        for (i = 0; i < int(79-v[3].size()); i++) cout << " ";
        cout << v[3] << endl;
        cout << " " << v[4] << endl;
        for (i = 0; i < int(79-v[5].size()); i++) cout << " ";
        cout << v[5] << endl;
        cout << " " << v[6] << endl;
        for (i = 0; i < int(79-v[7].size()); i++) cout << " ";
        cout << v[7] << endl;
        for (i = 0; i < int(79-v[8].size()); i++) cout << " ";
        cout << v[8] << endl;
        for (i = 0; i < int(79-v[9].size()); i++) cout << " ";
        cout << v[9] << endl;
        for (i = 0; i < int(79-v[10].size()); i++) cout << " ";
        cout << v[10] << endl;
        cout << " " << v[11] << endl;
        for (i = 0; i < int(79-v[12].size()); i++) cout << " ";
        cout << v[12] << endl;
        for (i = 0; i < int(79-v[13].size()); i++) cout << " ";
        cout << v[13] << endl;
        for (i = 0; i < int(79-v[14].size()); i++) cout << " ";
        cout << v[14] << endl;
        cout << " " << v[15] << endl;
        for (i = 0; i < int(79-v[16].size()); i++) cout << " ";
        cout << v[16];
        setCursorPosition(1, 23);
        system("pause");
    }
}
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
void element(int type)
{
    cout << "(";
    switch(type)
    {
        case 0:
            cout << "тьма";
            break;
        case 1:
            cout << "вода";
            break;
        case 2:
            cout << "огонь";
            break;
        case 3:
            cout << "земля";
            break;
        case 4:
            cout << "воздух";
            break;
        case 5:
            cout << "свет";
            break;
    }
    cout << ")";
}
void itemShow(int num, bool backpack)              // SHOW ITEMS (1 - FROM BACKPACK, 0 - IN SHOP)
{
    string v[items_all];
    v[0] = "Анх бессмертия";
    v[1] = "Свиток возврата";
    v[2] = "Временной скачок";
    v[3] = "Загадка тысячелетия";
    v[4] = "Дыхание дракона";
    v[5] = "Древний минерал";
    v[6] = "Стимулятор";
    v[7] = "Кристаллическая призма";
    v[8] = "Молот Тора";
    v[9] = "Энергетический заряд";
    v[10] = "Точильный камень";
    v[11] = "Мифрильная заплатка";
    v[12] = "Потрепанная книга";
    v[13] = "Мифрил";
    v[14] = "Самоцвет";
    v[15] = "Темная сталь";
    v[16] = "Железо";
    v[17] = "Ткань";
    v[18] = "Дерево";
    if (num < 2) setColor(Yellow, Black);
    else if (num < 6 || num == 13) setColor(LightMagenta, Black);
    else if (num >= 16) setColor(White, Black);
    else setColor(LightCyan, Black);
    cout << v[num];
    setColor(LightGrey, Black);
    if (backpack) cout << " (" << myHero.backpack[num] << ")";
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
void itemShowDescription(int num)
{
    string v[items_all];
    v[0] = "Возрождает умершего героя с ОЗ от 10% до 100%";
    v[1] = "Возвращает количество здоровья с предыдущего хода";
    v[2] = "Полностью восстанавливает заряд";
    v[3] = "Наносит случайный МАГ урон до 5% текущего ОЗ врага (усиления не влияют)";
    v[4] = "Полностью восстанавливает ОЗ";
    v[5] = "Полностью восстанавливает ОМ или ЗК";
    v[6] = "Увеличивает АТК урон на этом ходу в 4 раза";
    v[7] = "Увеличивает МАГ урон на этом ходу в 4 раза";
    v[8] = "Оглушает противника на 8 ходов";
    v[9] = "Восстанавливает 50% ОМ или 1 ЗК";
    v[10] = "+10 АТК (пассивно)";
    v[11] = "+10 ЗАЩ (пассивно)";
    v[12] = "+10 МАГ (пассивно)";
    v[13] = "5 единиц ресурса для создания предметов";
    v[14] = "5 единиц ресурса для создания предметов";
    v[15] = "5 единиц ресурса для создания предметов";
    v[16] = "5 единиц ресурса для создания предметов";
    v[17] = "5 единиц ресурса для создания предметов";
    v[18] = "5 единиц ресурса для создания предметов";
    cout << v[num];
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
int buffHero(int level)
{
    setCursorPosition(3, 3);
    int value = level+1;
    switch(myHero.perks)
    {
        case 1:
            cout << "Крик усилен до " << value << " уровня!";
            break;
        case 2:
            cout << "Ловкость усилена до " << value << " уровня!";
            break;
        case 3:
            cout << "Потенциал раскрыт до " << value << " уровня!";
            break;
    }
    return value;
}
void showNewSkill(int perks, int num)
{
    if (unmute)
    {
        wi.stop();
        sk.play();
    }
    system("cls");
    cout << endl << "================================ ";
    setColor(LightGreen, Black);
    cout << "Новое умение!";
    setColor(LightGrey, Black);
    cout << " ================================" << endl;
    setColor(White, Black);
    cout << "                      _________________________________                        " << endl;
    cout << "                     /                               /,\\                       " << endl;
    cout << "                     | ----------    -----    -----  |O|                       " << endl;
    cout << "                     \\_______________________________\\_/                       " << endl;
    cout << "                     /                                /                        " << endl;
    cout << "                    /  ..,,,,,,.. ,,,,..   ,,....,.  /                         " << endl;
    cout << "                    |                                |                         " << endl;
    cout << "                    |  ,,...,,..,,,. ,,,..,, ,. ,..  |                         " << endl;
    cout << "                    \\                                \\                         " << endl;
    cout << "                     \\   ,,,....,. ,,.. ,,.,, ..,,..  \\                        " << endl;
    cout << "                      \\                                \\                       " << endl;
    cout << "                      |  ..,.., ,,. ,,,...,,.,.,. .,   |                       " << endl;
    cout << "                      /________________________________/                       " << endl;
    cout << "                     /                                /.\\                      " << endl;
    cout << "                     | ------    ----     ---------   |0|                      " << endl;
    cout << "                     \\________________________________\\_/                      " << endl << endl;
    setColor(LightGrey, Black);
    cout << "===============================================================================" << endl << endl;
    switch(perks)
    {
        case 1:
            switch(num)
            {
                case 3:
                    cout << "                ";
                    printKey("3");
                    cout << " Двойной замах - наносит двойной АТК урон и                 " << endl;
                    cout << "                            оглушает врага на ход                              ";
                    break;
                case 4:
                    cout << "         ";
                    printKey("4");
                    cout << " Залп молний - наносит двойной МАГ урон, а также оглушает          " << endl;
                    cout << "                          врага на несколько ходов                             ";
                    break;
                case 5:
                    cout << "          ";
                    printKey("5");
                    cout << " Выброс силы - увеличивает АТК в 5 раз на следующий ход           ";
                    break;
                case 6:
                    cout << "      ";
                    printKey("6");
                    cout << " Сила Одина - наносит серьезный АТК урон и оглушает на огромное       " << endl;
                    cout << "                               количество ходов                                ";
                    break;
            }
            break;
        case 2:
            switch(num)
            {
                case 3:
                    cout << "            ";
                    printKey("3");
                    cout << " Серия ударов - наносит случайный серьезный АТК урон            " << endl;
                    break;
                case 4:
                    cout << "         ";
                    printKey("4");
                    cout << " Сокрушительный удар - наносит совмещенный урон и оглушает         " << endl;
                    cout << "                       на несколько ходов и заражает ядом                      ";
                    break;
                case 5:
                    cout << "     ";
                    printKey("5");
                    cout << " Поедание маны - тратит заметное количетсво маны и наносит большой     " << endl;
                    cout << "               урон в зависимости от количества затраченной маны               ";
                    break;
                case 6:
                    cout << "     ";
                    printKey("6");
                    cout << " Концентрация - лечит огромную часть здоровья и утраивает АТК на       " << endl;
                    cout << "                               следующие три хода                              ";
                    break;
            }
            break;
        case 3:
            switch(num)
            {
                case 3:
                    cout << "          ";
                    printKey("3");
                    cout << " Веном - наносит совмещенный урон и заражает противника           " << endl;
                    cout << "                         слабым ядом на несколько ходов                        ";
                    break;
                case 4:
                    cout << "    ";
                    printKey("4");
                    cout << " Землетрясение - наносит преимущественно магический урон и оглушает     " << endl;
                    cout << "                            врага на несколько ходов                           ";
                    break;
                case 5:
                    cout << "    ";
                    printKey("5");
                    cout << " Энергошторм - наносит колоссальный МАГ урон при случайных серьезных    " << endl;
                    cout << "                                  затратах маны                                ";
                    break;
                case 6:
                    cout << "         ";
                    printKey("6");
                    cout << " Эфириальная форма - удваивает МАГ урон на следующем ходу          " << endl;
                    break;
            }
            break;
        case 4:
            switch(num)
            {
                case 3:
                    cout << "       ";
                    printKey("3");
                    cout << " Жертвоприношение - наносит небольшой урон себе и совмещенный        " << endl;
                    cout << "                                урон врагу (+1 ЗК)                             ";
                    break;
                case 4:
                    cout << "    ";
                    printKey("4");
                    cout << " Сфера отчаяния - наносит серьезный МАГ урон и оглушает противника      " << endl;
                    cout << "                        на значительное количество ходов                       ";
                    break;
                case 5:
                    cout << "        ";
                    printKey("5");
                    cout << " Жатва - наносит большой АТК урон и заражает противника на          " << endl;
                    cout << "                                 несколько ходов                               ";
                    break;
                case 6:
                    cout << "     ";
                    printKey("6");
                    cout << " Самообичевание - наносит колоссальный урон себе и МАГ урон врагу      " << endl;
                    cout << "            в зависимости от количества потерянного здоровья (+1 ЗК)           ";
                    break;
            }
            break;
    }
    cout << endl << endl;
    system("pause");
}
void battle(int type, int enemy_level, float diffC) // BATTLE
{
    ofstream fout;
    string name;
    string monsters[] = {"Скелет","Вспыш","Тень","Фея","Кентавр","Виверн","Ледышка","Демон","Гомункул","Люмен","Падший ангел","Аэр","Арес","Летучий змей","Люкс","Ведьма","Голем","Водяной элементаль","Огненный элементаль","Земляной элементаль","Воздушный элементаль","Темный элементаль","Светлый элементаль","Химера","Цербер","Феникс","Валькирия","Циклон","Ехидна","Оверлорд"};
    int element[] = {0,2,0,3,3,4,1,2,3,5,5,4,2,4,5,0,3,1,2,3,4,0,5,4,0,2,5,3,1,-2};
	bool cont, hero_crit, hero_block, hero_insta, play = true, new_lvl, any_item, immune = false;
	char opt, q;
	int i, j, hp_chance, hp_l, max_hp_l, effect_l, item_cnt, pos, item_use, name_n, enemy_type, damage_amount, heal_amount, prev_level, prev_hp, over_ult, stasis = 0, stun = 0, poison = 0, poison_damage = 0, hero_stun = 0, hero_poison = 0, hero_poison_damage = 0, stunAt, dodge = 0, buffLvl = 0, buffTime = 0, damage_buff_turns = 0, damage_buff_lvl = 1, ability_buff_turns = 0, ability_buff_lvl = 1, gold, chargeC, max_hp, pot_max_hp, max_mp, crit_chance, block_chance, insta_chance, damage, armor, ability;
	float damageC, armorC, abilityC, healC, poisonC;
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
            myEnemy.damage = diffC*(0.025*enemy_level*enemy_level+12*enemy_level+30-enemy_level+rand()%(2*enemy_level));
            myEnemy.ult_id = -1;
            name_n = rand()%23;
            hp_chance = rand()%100;
            if (hp_chance >= 95) myEnemy.max_hp *= 3;
            else if (hp_chance >= 80) myEnemy.max_hp *= 2;
            break;
        case 1:
            if (!music.openFromFile("music/boss.ogg")) return;
            myEnemy.max_hp = 5*diffC*(0.1*enemy_level*enemy_level+100*enemy_level+500-6*enemy_level+rand()%(12*enemy_level));
            myEnemy.damage = diffC*(0.025*enemy_level*enemy_level+12*enemy_level+30-enemy_level+rand()%(2*enemy_level));
            myEnemy.ult_id = rand()%7;
            name_n = 23+rand()%6;
            hp_chance = 0;
            break;
        case 2:
            if (!music.openFromFile("music/overlord.ogg")) return;
            myEnemy.max_hp = 20*diffC*(0.1*enemy_level*enemy_level+100*enemy_level+500-6*enemy_level+rand()%(12*enemy_level));
            myEnemy.damage = diffC*(0.025*enemy_level*enemy_level+12*enemy_level+30-enemy_level+rand()%(2*enemy_level));
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
    chargeC = 10;
    if (myHero.weapon_id == 3) chargeC += 2+2*myHero.weapon_rarity;
    if (myHero.wear_id == 3) chargeC += 2+2*myHero.wear_rarity;
    if (myHero.amulet_id == 3) chargeC += 5+5*myHero.amulet_rarity;
    if (myHero.ring_id == 3) chargeC += 5+5*myHero.ring_rarity;
    stunAt = 0;
    if (myHero.weapon_id == 4) stunAt += 4+3*myHero.weapon_rarity;
    if (myHero.wear_id == 4) stunAt += 4+3*myHero.wear_rarity;
    if (myHero.amulet_id == 4) stunAt += 10+5*myHero.amulet_rarity;
    if (myHero.ring_id == 4) stunAt += 10+5*myHero.ring_rarity;
    healC = 1;
    if (myHero.weapon_id == 5) healC += 0.1+0.1*myHero.weapon_rarity;
    if (myHero.wear_id == 5) healC += 0.1+0.1*myHero.wear_rarity;
    if (myHero.amulet_id == 5) healC += 0.2+0.15*myHero.amulet_rarity;
    if (myHero.ring_id == 5) healC += 0.2+0.15*myHero.ring_rarity;
    abilityC = 1;
    if (myHero.weapon_id == 7) abilityC += 0.05+0.05*myHero.weapon_rarity;
    if (myHero.wear_id == 7) abilityC += 0.05+0.05*myHero.wear_rarity;
    if (myHero.amulet_id == 7) abilityC += 0.15+0.05*myHero.amulet_rarity;
    if (myHero.ring_id == 7) abilityC += 0.15+0.05*myHero.ring_rarity;
    poisonC = 1;
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
    showSpells:
    setCursorPosition(0, 15);
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               " << endl;
    cout << "                                                                               ";
    setCursorPosition(59-(name.size())/2, 2);
    if (name_n == 29) setColor(LightRed, Black);    // ENEMY NAME
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
    switch(myHero.perks)    // BATTLE HUD
    {
        case 1:
            printKey("1");
            cout << " Удар топором" << endl;
            printKey("2");
            cout << " Кража жизни (";
            setColor(LightCyan, Black);
            cout << 7;
            setColor(LightGrey, Black);
            cout << " ОМ)" << endl;
            printKey("3");
            if (myHero.level >= 5)
            {
                cout << " Двойной замах (";
                setColor(LightCyan, Black);
                cout << 4;
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 5 уровень>"<< endl;
            printKey("4");
            if (myHero.level >= 10)
            {
                cout << " Залп молний (";
                setColor(LightCyan, Black);
                cout << 10;
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 10 уровень>" << endl;
            printKey("5");
            if (myHero.level >= 25)
            {
                cout << " Выброс силы (";
                setColor(LightCyan, Black);
                cout << int(0.2*max_mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 25 уровень>" << endl;
            printKey("6");
            if (myHero.level >= 50)
            {
                cout << " Сила Одина (";
                setColor(LightCyan, Black);
                cout << int(0.8*max_mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 50 уровень>" << endl;
            printKey("Z");
            cout << " Крик берсерка (";
            setColor(Yellow, Black);
            cout << 25;
            setColor(LightGrey, Black);
            cout << "% заряд)" << endl;
            printKey("X");
            cout << " Открыть рюкзак" << endl;
            break;
		case 2:
            printKey("1");
            cout << " Удар клинка" << endl;
            printKey("2");
            cout << " Укрытие во тьме (";
            setColor(LightCyan, Black);
            cout << 7;
            setColor(LightGrey, Black);
            cout << " ОМ)" << endl;
            printKey("3");
            if (myHero.level >= 5)
            {
                cout << " Серия ударов (";
                setColor(LightCyan, Black);
                cout << 6;
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 5 уровень>"<< endl;
            printKey("4");
            if (myHero.level >= 10)
            {
                cout << " Сокрушительный удар (";
                setColor(LightCyan, Black);
                cout << 14;
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 10 уровень>" << endl;
            printKey("5");
            if (myHero.level >= 25)
            {
                cout << " Поедание маны (";
                setColor(LightCyan, Black);
                cout << int(0.2*myHero.mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 25 уровень>" << endl;
            printKey("6");
            if (myHero.level >= 50)
            {
                cout << " Концентрация (";
                setColor(LightCyan, Black);
                cout << int(0.4*max_mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 50 уровень>" << endl;
            printKey("Z");
            cout << " Аура неведения (";
            setColor(Yellow, Black);
            cout << 25;
            setColor(LightGrey, Black);
            cout << "% заряд)" << endl;
            printKey("X");
            cout << " Открыть рюкзак" << endl;
            break;
        case 3:
            printKey("1");
            cout << " Магическая атака" << endl;
            printKey("2");
            cout << " Невидимость (";
            setColor(LightCyan, Black);
            cout << int(25+0.1*max_mp);
            setColor(LightGrey, Black);
            cout << " ОМ)" << endl;
            printKey("3");
            if (myHero.level >= 5)
            {
                cout << " Веном (";
                setColor(LightCyan, Black);
                cout << int(10+0.08*max_mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 5 уровень>"<< endl;
            printKey("4");
            if (myHero.level >= 10)
            {
                cout << " Землетрясение (";
                setColor(LightCyan, Black);
                cout << int(25+0.15*max_mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 10 уровень>" << endl;
            printKey("5");
            if (myHero.level >= 25)
            {
                cout << " Энергошторм (";
                setColor(LightCyan, Black);
                cout << int(0.4*max_mp+0.1*myHero.mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 25 уровень>" << endl;
            printKey("6");
            if (myHero.level >= 50)
            {
                cout << " Эфириальная форма (";
                setColor(LightCyan, Black);
                cout << int(30+0.1*myHero.mp);
                setColor(LightGrey, Black);
                cout << " ОМ)" << endl;
            }
            else cout << " <Нужен 50 уровень>" << endl;
            printKey("Z");
            cout << " Волшебная аура (";
            setColor(Yellow, Black);
            cout << 25;
            setColor(LightGrey, Black);
            cout << "% заряд)" << endl;
            printKey("X");
            cout << " Открыть рюкзак" << endl;
            break;
        case 4:
            printKey("1");
            cout << " Вытягивание жизни" << endl;
            printKey("2");
            cout << " Стазис (";
            setColor(LightRed, Black);
            cout << 1;
            setColor(LightGrey, Black);
            cout << " ЗК)" << endl;
            printKey("3");
            if (myHero.level >= 5)
            {
                cout << " Жертвоприношение (";
                setColor(LightRed, Black);
                cout << int(0.15*pot_max_hp);
                setColor(LightGrey, Black);
                cout << " ОЗ; ";
                setColor(LightRed, Black);
                cout << "+1";
                setColor(LightGrey, Black);
                cout << " ЗК)" << endl;
            }
            else cout << " <Нужен 5 уровень>"<< endl;
            printKey("4");
            if (myHero.level >= 10)
            {
                cout << " Сфера отчаяния (";
                setColor(LightRed, Black);
                cout << int(500+0.2*myHero.hp);
                setColor(LightGrey, Black);
                cout << " ОЗ)" << endl;
            }
            else cout << " <Нужен 10 уровень>" << endl;
            printKey("5");
            if (myHero.level >= 25)
            {
                cout << " Жатва (";
                setColor(LightRed, Black);
                cout << 1;
                setColor(LightGrey, Black);
                cout << " ЗК)" << endl;
            }
            else cout << " <Нужен 25 уровень>" << endl;
            printKey("6");
            if (myHero.level >= 50)
            {
                cout << " Самообичевание (";
                setColor(LightRed, Black);
                cout << int(2500+0.4*myHero.hp);
                setColor(LightGrey, Black);
                cout << " ОЗ; ";
                setColor(LightRed, Black);
                cout << "+1";
                setColor(LightGrey, Black);
                cout << " ЗК)" << endl;
            }
            else cout << " <Нужен 50 уровень>" << endl;
            printKey("Z");
            cout << " Ускорение кровотока (";
            setColor(Yellow, Black);
            cout << 25;
            setColor(LightGrey, Black);
            cout << "% заряд)" << endl;
            printKey("X");
            cout << " Открыть рюкзак" << endl;
            break;
    }
    do  // HUD UPDATE & PLAYER TURN
    {
        switch(myHero.perks)    // PRINT CHANGEABLE HP & MANACOSTS
        {
            case 2:
                if (myHero.level >= 25)
                {
                    setCursorPosition(19, 19);
                    setColor(LightCyan, Black);
                    cout << int(5+0.2*myHero.mp);
                    setColor(LightGrey, Black);
                    cout << " ОМ)     ";
                }
                break;
            case 3:
                if (myHero.level >= 25)
                {
                    setCursorPosition(17, 19);
                    setColor(LightCyan, Black);
                    cout << int(0.4*max_mp+0.1*myHero.mp);
                    setColor(LightGrey, Black);
                    cout << " ОМ)     ";
                }
                if (myHero.level >= 50)
                {
                    setCursorPosition(23, 20);
                    setColor(LightCyan, Black);
                    cout << int(30+0.1*myHero.mp);
                    setColor(LightGrey, Black);
                    cout << " ОМ)     ";
                }
                break;
            case 4:
                if (myHero.level >= 10)
                {
                    setCursorPosition(20, 18);
                    setColor(LightRed, Black);
                    cout << int(500+0.2*myHero.hp);
                    setColor(LightGrey, Black);
                    cout << " ОЗ)     ";
                }
                if (myHero.level >= 50)
                {
                    setCursorPosition(20, 20);
                    setColor(LightRed, Black);
                    cout << int(2500+0.4*myHero.hp);
                    setColor(LightGrey, Black);
                    cout << " ОЗ; ";
                    setColor(LightRed, Black);
                    cout << "+1";
                    setColor(LightGrey, Black);
                    cout << " ЗК)     " << endl;
                }
                break;
        }
        if (myHero.hp <= 0)    // USE ANCH UPON DEATH
        {
            myHero.hp = 0;
            if (immune)
            {
                immune = false;
                heroHeal((100+rand()%901)*pot_max_hp/1000, pot_max_hp, 0);
                setCursorPosition(3, 2);
                itemShow(0, 0);
                cout << "использован!";
            }
            else play = false;
        }
        if (rand()%1000 < crit_chance) hero_crit = true;
        else hero_crit = false;
        if (rand()%1000 < block_chance) hero_block = true;
        else hero_block = false;
        if (rand()%1000 < insta_chance) hero_insta = true;
        else hero_insta = false;
        if (myEnemy.enemy_ult[2] > 0) max_hp = pot_max_hp/3;
        else max_hp = pot_max_hp;
        if (myHero.hp < 0) myHero.hp = 0;
        if (myEnemy.hp < 0) myEnemy.hp = 0;
        if (myEnemy.hp == 0) play = false;
        if (myHero.hp > max_hp) myHero.hp = max_hp;
        if (myHero.mp > max_mp) myHero.mp = max_mp;
        if (myEnemy.hp > myEnemy.max_hp) myEnemy.hp = myEnemy.max_hp;
        hp_l = 1;
        while (myEnemy.hp >= pow(10, hp_l)) hp_l++;
        setCursorPosition(62-hp_l-max_hp_l, 6);
        cout << "     ";
        if (myEnemy.hp*100/myEnemy.max_hp >= 60) setColor(Green, Black);
        else if (myEnemy.hp*100/myEnemy.max_hp >= 30) setColor(Brown, Black);
        else setColor(Red, Black);
        cout << myEnemy.hp;
        setColor(LightGrey, Black);
        setCursorPosition(4, 12);
        cout << "ОЗ: ";
        if (myHero.hp*100/pot_max_hp >= 60) setColor(LightGreen, Black);
        else if (myHero.hp*100/pot_max_hp >= 30) setColor(Yellow, Black);
        else setColor(LightRed, Black);
        cout << myHero.hp;
        setColor(LightGrey, Black);
        cout << " / " << max_hp << "    ";
        setCursorPosition(22, 12);
        if (myHero.perks == 4)
        {
            cout << "ЗК:  ";
            if (myHero.mp == myHero.max_mp) setColor(LightRed, Black);
            else setColor(Yellow, Black);
        }
        else
        {
            cout << "ОМ: ";
            setColor(LightCyan, Black);
        }
        cout << myHero.mp;
        setColor(LightGrey, Black);
        cout << " / " << max_mp << "    ";
        setCursorPosition(9, 10);
        if (hero_poison > 0) setColor(LightMagenta, Black);
        else if (myHero.hp*100/pot_max_hp >= 60) setColor(LightGreen, Black);
        else if (myHero.hp*100/pot_max_hp >= 30) setColor(Yellow, Black);
        else setColor(LightRed, Black);
        for (i = 1; i <= 25; i++)
        {
            if (i <= myHero.hp*25/pot_max_hp) cout << "#";
            else cout << " ";
        }
        setColor(LightGrey, Black);
        setCursorPosition(4, 8);
        if (myHero.perks == 4) setColor(LightRed, Black);
        else setColor(LightCyan, Black);
        for (i = 1; i <= 10; i++)
        {
            if (i <= myHero.mp*10/max_mp)
            {
                if (myHero.perks == 4) cout << ">";
                else cout << "/";
            }
            else cout << " ";
        }
        setColor(LightGrey, Black);
        setCursorPosition(44, 4);
        if (poison > 0) setColor(Magenta, Black);
        else if (myEnemy.hp*100/myEnemy.max_hp >= 60) setColor(Green, Black);
        else if (myEnemy.hp*100/myEnemy.max_hp >= 30) setColor(Brown, Black);
        else setColor(Red, Black);
        for (i = 25; i >= 1; i--)
        {
            if (i <= myEnemy.hp*25/myEnemy.max_hp) cout << "#";
            else cout << " ";
        }
        setColor(LightGrey, Black);
        setCursorPosition(4, 10);
        if (myHero.charge < 1000) cout << "0";
        if (myHero.charge < 100) cout << "0";
        setColor(Yellow, Black);
        cout << myHero.charge/10;
        setColor(LightGrey, Black);
        setCursorPosition(70, 4);
        if (myEnemy.ult_id != -1)
        {
            if (myEnemy.ult_charge < 1000) cout << "0";
            if (myEnemy.ult_charge < 100) cout << "0";
            setColor(Brown, Black);
            cout << myEnemy.ult_charge/10;
            setColor(LightGrey, Black);
        }
        switch(myHero.perks)    // AURA INFO
        {
            case 1:
                setCursorPosition(30, 21);
                break;
            case 2:
                setCursorPosition(31, 21);
                break;
            case 3:
                setCursorPosition(31, 21);
                break;
            case 4:
                setCursorPosition(36, 21);
                break;
        }
        cout << "Уровень: ";
        setColor(White, Black);
        cout << buffLvl;
        setColor(LightGrey, Black);
        if (buffLvl > 0)
        {
            cout << " Время: ";
            setColor(White, Black);
            cout << buffTime;
            setColor(LightGrey, Black);
            cout << " ход";
            if (buffTime >= 2 && buffTime <= 4) cout << "а";
            else if (buffTime != 0 && buffTime != 1) cout << "ов";
        }
        else cout << "               ";
        setCursorPosition(17, 10);  // 33% HP BARRIER
        if (myEnemy.enemy_ult[2] > 0) cout << "|";
        setCursorPosition(16, 8);
        cout << "                                ";
        setCursorPosition(16, 8);   // HERO BUFFS
        if (immune)
        {
            setColor(Yellow, Black);
            cout << "АНХ ";
            setColor(LightGrey, Black);
        }
        if (damage_buff_turns > 0)
        {
            setColor(LightCyan, Black);
            cout << "АТКх" << damage_buff_lvl << " ";
            setColor(LightGrey, Black);
        }
        if (ability_buff_turns > 0)
        {
            setColor(LightCyan, Black);
            cout << "МАГх " << ability_buff_lvl << " ";
            setColor(LightGrey, Black);
        }
        if (myEnemy.enemy_ult[2] > 0)
        {
            setColor(Yellow, Black);
            cout << "УМНШ<" << myEnemy.enemy_ult[2] << "> ";
            setColor(LightGrey, Black);
        }
        if (hero_stun > 0)
        {
            setColor(LightRed, Black);
            cout << "ОГЛУШ<" << hero_stun << "> ";
            setColor(LightGrey, Black);
        }
        if (hero_poison > 0)
        {
            setColor(LightMagenta, Black);
            cout << "ЯД<" << hero_poison << "> ";
            setColor(LightGrey, Black);
        }
        if (stasis > 0)
        {
            setColor(LightGreen, Black);
            cout << "СТАЗ<" << stasis << "> ";
            setColor(LightGrey, Black);
        }
        setCursorPosition(37-hp_l-max_hp_l, 6);      // ENEMY BUFFS AND DEBUFFS
        cout << "                              ";
        effect_l = 0;
        if (stun > 0) effect_l += 9;
        if (poison > 0) effect_l += 6;
        if (myEnemy.enemy_ult[4] > 0) effect_l += 10;
        if (myEnemy.enemy_ult[5] > 0) effect_l += 10;
        if (myEnemy.enemy_ult[8] > 0) effect_l += 10;
        setCursorPosition(67-hp_l-max_hp_l-effect_l, 6);
        if (stun > 0)
        {
            setColor(Red, Black);
            cout << "ОГЛУШ<" << stun << "> ";
            setColor(LightGrey, Black);
        }
        if (poison > 0)
        {
            setColor(Magenta, Black);
            cout << "ЯД<" << poison << "> ";
            setColor(LightGrey, Black);
        }
        if (myEnemy.enemy_ult[4] > 0)
        {
            setColor(Cyan, Black);
            cout << "ФИЗБАР<" << myEnemy.enemy_ult[4] << "> ";
            setColor(LightGrey, Black);
        }
        if (myEnemy.enemy_ult[5] > 0)
        {
            setColor(Cyan, Black);
            cout << "МАГБАР<" << myEnemy.enemy_ult[5] << "> ";
            setColor(LightGrey, Black);
        }
        if (myEnemy.enemy_ult[8] > 0)
        {
            setColor(Brown, Black);
            cout << "УЛТБАР<" << myEnemy.enemy_ult[8] << "> ";
            setColor(LightGrey, Black);
        }
        setCursorPosition(0, 24);
        cont = false;
        if (hero_stun == 0 && play) do
    	{
        	if (stasis == 0) opt = _getch();
        	else
            {
                cont = true;
                Sleep(500);
            }
    	    setCursorPosition(3, 2);
    	    cout << "                                   ";
    	    setCursorPosition(3, 3);
    	    cout << "                                   ";
    	    setCursorPosition(3, 4);
    	    cout << "                                   ";
    	    setCursorPosition(3, 5);
    	    cout << "                                   ";
    	    setCursorPosition(3, 6);
    	    cout << "                                   ";
    	    setCursorPosition(45, 9);
    	    cout << "                                   ";
    	    setCursorPosition(45, 10);
    	    cout << "                                   ";
    	    setCursorPosition(45, 11);
    	    cout << "                                   ";
    	    setCursorPosition(45, 12);
    	    cout << "                                   ";
    	    setCursorPosition(3, 3);
        	if (stasis == 0) switch(int(opt))
        	{
        		case '1':
        		    if (unmute) a0.play();
        		    if (myHero.perks == 3) myHero.mp += 0.07*max_mp;
        		    if (myHero.perks != 4) myHero.mp += 3;
        		    if (myHero.mp > max_mp) myHero.mp = max_mp;
                    if (myHero.perks == 1) heroInflictDamage((1+0.5*buffLvl)*damage, 0, -2+rand()%5, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                    else heal_amount = 0.15*heroInflictDamage(damage, 0, -2+rand()%5, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                    if (myHero.perks == 4) heroHeal(heal_amount+0.05*buffLvl*pot_max_hp, pot_max_hp, 1);
                    stun = if_stun(1, stunAt, 1+rand()%2, stun);
                    cont = true;
        			break;
        		case '2':
        			switch(myHero.perks)
        			{
        				case 1:
        				    if (manaCheck(7))
                            {
                                if (unmute) a1.play();
                                heal_amount = heroInflictDamage(2*(1+0.5*buffLvl)*damage, 0, -4+rand()%9, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                heroHeal(healC*heal_amount, pot_max_hp, 1);
                                cont = true;
                            }
        					break;
        				case 2:
        				    if (manaCheck(7))
                            {
                                if (unmute) a1.play();
                                if (dodge < 1) dodge = 1;
                                heroHeal(healC*0.14*pot_max_hp, pot_max_hp, 0);
                                cont = true;
                            }
        					break;
        				case 3:
        				    if (manaCheck(25+0.1*max_mp))
                            {
                                if (unmute) a1.play();
                                if (dodge < 1) dodge = 1;
                                heroHeal(healC*0.15*pot_max_hp, pot_max_hp, 0);
                                cont = true;
                            }
        					break;
                        case 4:
                            if (manaCheck(1))
                            {
                                if (unmute) a1.play();
                                stasis = 3;
                                heal_amount = (0.15+0.05*buffLvl)*pot_max_hp;
                                cont = true;
                            }
                            break;
					}
        			break;
        		case '3':
                    if (myHero.level >= 5) switch(myHero.perks)
        			{
        				case 1:
        				    if (manaCheck(4))
                            {
                                if (unmute) a2.play();
                                heroInflictDamage(2*(1+0.5*buffLvl)*damage, 0, -4+rand()%9, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                stun = if_stun(1, 100, 1, stun);
                                cont = true;
                            }
        					break;
        				case 2:
        				    if (manaCheck(6))
                            {
                                if (unmute) a2.play();
                                heroInflictDamage(2*damage, 0, -3+rand()%9+rand()%(2*damage), 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                cont = true;
                            }
        					break;
        				case 3:
        				    if (manaCheck(10+0.08*max_mp))
                            {
                                if (unmute) a2.play();
                                if (poison == 0) poison = 3;
                                poison_damage = if_poison(1, poisonC*0.2*ability, poison_damage, 3);
                                heroInflictDamage(damage, 2*(1+0.35*buffLvl)*ability, -2+rand()%5, -4+rand()%9, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                cont = true;
                            }
        					break;
                        case 4:
                            if (unmute) a2.play();
                            heroInflictDamage(2*damage, 2*ability, -4+rand()%9, -4+rand()%9, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                            heroDamageSelf(0.15*pot_max_hp);
                            myHero.mp++;
                            cont = true;
                            break;
					}
        			break;
        		case '4':
        			if (myHero.level >= 10) switch(myHero.perks)
        			{
        				case 1:
        				    if (manaCheck(8))
                            {
                                if (unmute) a3.play();
                                heroInflictDamage(0, 2*ability, 0, -4+rand()%9, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                stun = if_stun(1, 100, 2+rand()%4, stun);
                                cont = true;
                            }
        					break;
        				case 2:
        				    if (manaCheck(14))
                            {
                                if (unmute) a3.play();
                                if (poison == 0) poison = 6;
                                poison_damage = if_poison(1, poisonC*0.3*damage, poison_damage, 6);
                                heroInflictDamage(2*damage, 2*ability, -2+rand()%5, -2+rand()%5, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                stun = if_stun(1, 100, 4, stun);
                                cont = true;
                            }
        					break;
        				case 3:
        				    if (manaCheck(25+0.15*max_mp))
                            {
                                if (unmute) a3.play();
                                heroInflictDamage(damage, 3*(1+0.35*buffLvl)*ability, -2+rand()%5, -6+rand()%13, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                stun = if_stun(1, 100, 3+rand()%2, stun);
                                cont = true;
                            }
        					break;
                        case 4:
                            if (unmute) a3.play();
                            heroInflictDamage(0, 4*ability, 0, -8+rand()%17, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                            stun = if_stun(1, 100, 3+rand()%5, stun);
                            heroDamageSelf(500+0.2*myHero.hp);
                            cont = true;
                            break;
					}
        			break;
                case '5':
                    if (myHero.level >= 25) switch(myHero.perks)
        			{
                        case 1:
                            if (manaCheck(int(max_mp/5)))
                            {
                                if (unmute) a4.play();
                                damage_buff_turns = 2;
                                damage_buff_lvl = 5;
                                cont = true;
                            }
                            break;
                        case 2:
                            if (manaCheck(5+int(0.2*myHero.mp)))
                            {
                                if (unmute) a4.play();
                                heroInflictDamage(0, (5+int(0.2*myHero.mp))*40, 0, -12+rand()%25, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                cont = true;
                            }
                            break;
                        case 3:
                            if (manaCheck(0.4*max_mp+0.1*myHero.mp))
                            {
                                if (unmute) a4.play();
                                heroInflictDamage(0, 6*(1+0.35*buffLvl)*ability, 0, -12+rand()%25+rand()%int((2*(1+0.35*buffLvl)*ability)), damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                cont = true;
                            }
                            break;
                        case 4:
                            if (manaCheck(1))
                            {
                                if (unmute) a4.play();
                                heroInflictDamage(5*damage, 0, -10+rand()%21, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                if (poison == 0) poison = 5;
                                poison_damage = if_poison(1, poisonC*0.25*ability, poison_damage, 5);
                                cont = true;
                            }
                            break;
        			}
        			break;
                case '6':
                    if (myHero.level >= 50) switch(myHero.perks)
        			{
                        case 1:
                            if (manaCheck(max_mp*4/5))
                            {
                                if (unmute) a5.play();
                                heroInflictDamage(4*damage, 0, -8+rand()%17, 0, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                                stun = if_stun(1, 100, 10, stun);
                                cont = true;
                            }
                            break;
                        case 2:
                            if (manaCheck(max_mp*2/5))
                            {
                                if (unmute) a5.play();
                                heroHeal(0.3*pot_max_hp, pot_max_hp, 0);
                                damage_buff_turns = 4;
                                damage_buff_lvl = 3;
                                cont = true;
                            }
                            break;
                        case 3:
                            if (manaCheck(30+0.10*myHero.mp))
                            {
                                if (unmute) a5.play();
                                ability_buff_turns = 2;
                                ability_buff_lvl = 2;
                                cont = true;
                            }
                            break;
                        case 4:
                            if (unmute) a5.play();
                            damage_amount = heroDamageSelf(2500+0.4*myHero.hp);
                            heroInflictDamage(0, damage_amount, 0, -10+rand()%21, damage_buff_lvl, ability_buff_lvl, hero_crit, hero_insta);
                            myHero.mp++;
                            cont = true;
                            break;
        			}
        			break;
        		case 'z':
        			if (myHero.charge >= 250) switch(myHero.perks)
        			{
        				case 1:
                            if (unmute) a6.play();
        				    myHero.charge -= 250;
                            buffLvl = buffHero(buffLvl);
                            buffTime = 5;
                            cont = true;
        					break;
        				case 2:
                            if (unmute) a6.play();
        				    myHero.charge -= 250;
                            buffLvl = buffHero(buffLvl);
                            buffTime = 5;
                            cont = true;
        					break;
        				case 3:
                            if (unmute) a6.play();
        				    myHero.charge -= 250;
                            buffLvl = buffHero(buffLvl);
                            buffTime = 5;
                            cont = true;
        					break;
                        case 4:
                            if (unmute) a6.play();
                            myHero.charge -= 250;
                            buffLvl = buffHero(buffLvl);
                            buffTime = 5;
                            cont = true;
					}
        			break;
        		case 'x':
        		    // ADD SOUNDS FOR ITEMS
        		    setCursorPosition(0, 15);
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               " << endl;
        		    cout << "                                                                               ";
        		    setCursorPosition(0, 15);
        		    item_cnt = 0;
        		    any_item = false;
        		    for (i = 0; i < items_battle; i++)
                    {
                        setCursorPosition(4+40*int(item_cnt/7), 15+item_cnt%7);
                        if (myHero.backpack[i] > 0)
                        {
                            item_cnt++;
                            any_item = true;
                            itemShow(i, 1);
                        }
                    }
                    setCursorPosition(0, 22);
                    printKey("X");
                    cout << " Закрыть рюкзак      ";
                    printKey("W");
                    printKey("S");
                    cout << " Выбрать предмет     ";
                    printKey("ENTER");
                    cout << " Использовать предмет";
        		    setCursorPosition(0, 24);
                    if (any_item)
                    {
                        pos = 0;
                        item_use = 0;
                        while (myHero.backpack[item_use] == 0) item_use++;
                        setCursorPosition(0, 15);
                        cout << ">>>";
                        setCursorPosition(0, 24);
                        do
                        {
                            q = getch();
                            if (q == 72 && pos > 0)
                            {
                                i = item_use-1;
                                while (myHero.backpack[i] == 0) i--;
                                item_use = i;
                                setCursorPosition(0+40*int(pos/7), 15+pos%7);
                                cout << "   ";
                                pos--;
                                setCursorPosition(0+40*int(pos/7), 15+pos%7);
                                cout << ">>>";
                            }
                            if (q == 80)
                            {
                                i = item_use+1;
                                while (i < items_battle && myHero.backpack[i] == 0) i++;
                                if (i < items_battle)
                                {
                                    item_use = i;
                                    setCursorPosition(0+40*int(pos/7), 15+pos%7);
                                    cout << "   ";
                                    pos++;
                                    setCursorPosition(0+40*int(pos/7), 15+pos%7);
                                    cout << ">>>";
                                }
                            }
                            setCursorPosition(0, 24);
                        } while (q != 120 && q != 13);
                        if (q == 13)
                        {
                            myHero.backpack[item_use]--;
                            switch(item_use)
                            {
                                case 0:
                                    immune = true;
                                case 1:
                                    myHero.hp = prev_hp;
                                    break;
                                case 2:
                                    myHero.charge = 1000;
                                    break;
                                case 3:
                                    heroInflictDamage(0, 0, 0, myEnemy.hp*0.05, 1, 1, 0, 0);
                                    break;
                                case 4:
                                    heroHeal(pot_max_hp, pot_max_hp, 0);
                                    break;
                                case 5:
                                    heroMana(max_mp, max_mp);
                                    break;
                                case 6:
                                    damage_buff_turns = 2;
                                    damage_buff_lvl = 4;
                                    break;
                                case 7:
                                    ability_buff_turns = 2;
                                    ability_buff_lvl = 4;
                                    break;
                                case 8:
                                    stun = if_stun(1, 100, 8, stun);
                                    break;
                                case 9:
                                    heroMana(0.5*max_mp, max_mp);
                                    break;
                            }
                            setCursorPosition(3, 2);
                            itemShow(item_use, 0);
                            cout << " применен";
                            if (item_use == 3 || item_use == 7) cout << "а";
                            else if (item_use == 4) cout << "о";
                            cout << "!";
                        }
                    }
                    else do { q = getch(); } while (q != 120);
                    goto showSpells;
                    break;
			}
        } while (!cont);
        else if (hero_stun > 0)
        {
            Sleep(750);
            setCursorPosition(3, 2);
    	    cout << "                                   ";
    	    setCursorPosition(3, 3);
    	    cout << "                                   ";
    	    setCursorPosition(3, 4);
    	    cout << "                                   ";
    	    setCursorPosition(3, 5);
    	    cout << "                                   ";
    	    setCursorPosition(3, 6);
    	    cout << "                                   ";
    	    setCursorPosition(45, 9);
    	    cout << "                                   ";
    	    setCursorPosition(45, 10);
    	    cout << "                                   ";
    	    setCursorPosition(45, 11);
    	    cout << "                                   ";
    	    setCursorPosition(45, 12);
    	    cout << "                                   ";
        }
        if (stasis > 0) heroHeal(heal_amount, pot_max_hp, 0);
        prev_hp = myHero.hp;
        myEnemy.hp -= poison_damage;
        if (myEnemy.hp > 0 && play && stun == 0) // ENEMY TURN
        {
            if (myEnemy.ult_charge < 1000)
            {
                setCursorPosition(45, 10);
                if (myEnemy.ult_id != -1)
                {
                    myEnemy.ult_charge += 12+rand()%9;
                    if (myEnemy.ult_charge > 1000) myEnemy.ult_charge = 1000;
                }
                if (dodge == 0)
                {
                    if (myHero.perks == 2 && rand()%100 < buffLvl*15) cout << "Успешное уклонение!";
                    else enemyInflictDamage(myEnemy.damage, armor, 50*diffC, -2+rand()%5, hero_block);
                }
                else
                {
                    dodge--;
                    cout << "Успешное уклонение!";
                }
            }
            else
            {
                myEnemy.ult_charge -= 1000;
                setCursorPosition(45, 9);
                cout << "Враг применил ";
                switch(myEnemy.ult_id)
                {
                    case 0:
                        setColor(Red, Black);
                        cout << "Линчевание";
                        setColor(LightGrey, Black);
                        cout << "!";
                        enemyInflictDamage(0.5*max_hp, 0, 0, 0, 0);
                        break;
                    case 1:
                        setColor(Magenta, Black);
                        cout << "Ядовитое дыхание";
                        setColor(LightGrey, Black);
                        cout << "!";
                        if (hero_poison == 0) hero_poison = 5;
                        hero_poison_damage = if_poison(0, 0.12*pot_max_hp, hero_poison_damage, 5);
                        break;
                    case 2:
                        setColor(Brown, Black);
                        cout << "Уменьшение пульса";
                        setColor(LightGrey, Black);
                        cout << "!";
                        myEnemy.enemy_ult[2] = 10;
                        break;
                    case 3:
                        setColor(Green, Black);
                        cout << "Самолечение";
                        setColor(LightGrey, Black);
                        cout << "!";
                        myEnemy.hp = min(myEnemy.hp+int(0.25*myEnemy.max_hp), myEnemy.max_hp);
                        break;
                    case 4:
                        setColor(Cyan, Black);
                        cout << "Физический барьер";
                        setColor(LightGrey, Black);
                        cout << "!";
                        myEnemy.enemy_ult[4] = 8;
                        break;
                    case 5:
                        setColor(Cyan, Black);
                        cout << "Магический барьер";
                        setColor(LightGrey, Black);
                        cout << "!";
                        myEnemy.enemy_ult[5] = 8;
                        break;
                    case 6:
                        setColor(Red, Black);
                        cout << "Оглушительный рык";
                        setColor(LightGrey, Black);
                        cout << "!";
                        hero_stun = if_stun(0, 100, 8, hero_stun);
                        break;
                    case 7:
                        over_ult = rand()%3;
                        switch(over_ult)
                        {
                            case 0:
                                setColor(Red, Black);
                                cout << "Экстерминацию";
                                setColor(LightGrey, Black);
                                cout << "!";
                                enemyInflictDamage(0.9*max_hp, 0, 0, 0, 0);
                                break;
                            case 1:
                                setColor(Brown, Black);
                                cout << "Сверх-барьер";
                                setColor(LightGrey, Black);
                                cout << "!";
                                myEnemy.enemy_ult[8] = 18;
                                break;
                            case 2:
                                setColor(Red, Black);
                                cout << "Инферно";
                                setColor(LightGrey, Black);
                                cout << "!";
                                hero_stun = if_stun(0, 100, 12, hero_stun);
                                Sleep(2000);
                                break;
                        }
                        break;
                }
                setColor(LightGrey, Black);
            }
        }
        stun--;
        if (stun < 0) stun = 0;
        myHero.charge += chargeC;
        if (myHero.charge > 1000) myHero.charge = 1000;
        buffTime--;
        if (buffTime < 0) buffTime = 0;
        if (buffTime == 0) buffLvl = 0;
        poison--;
        if (poison < 0) poison = 0;
        if (poison == 0) poison_damage = 0;
        for (i = 0; i < 9; i++) myEnemy.enemy_ult[i]--;
        for (i = 0; i < 9; i++) if (myEnemy.enemy_ult[i] < 0) myEnemy.enemy_ult[i] = 0;
        hero_stun--;
        if (hero_stun < 0) hero_stun = 0;
        myHero.hp -= hero_poison_damage;
        hero_poison--;
        if (hero_poison < 0) hero_poison = 0;
        if (hero_poison == 0) hero_poison_damage = 0;
        damage_buff_turns--;
        if (damage_buff_turns < 0) damage_buff_turns = 0;
        if (damage_buff_turns == 0) damage_buff_lvl = 1;
        ability_buff_turns--;
        if (ability_buff_turns < 0) ability_buff_turns = 0;
        if (ability_buff_turns == 0) ability_buff_lvl = 1;
        stasis--;
        if (stasis < 0) stasis = 0;
    } while (play);
    music.stop();
    if (myHero.hp > 0)
    {
        if (unmute) wi.play();
        setCursorPosition(3, 2);
        cout << "                              ";
        setCursorPosition(3, 3);
        cout << "                              ";
        setCursorPosition(3, 4);
        cout << "                              ";
        setCursorPosition(3, 5);
        cout << "                              ";
        setCursorPosition(3, 3);
        cout << "Вы победили!";
        setCursorPosition(3, 4);
        cout << "+";
        setColor(LightMagenta, Black);
        if (hp_chance >= 95) cout << 3*enemy_level*enemy_level;
        else if (hp_chance >= 80) cout << 2*enemy_level*enemy_level;
        else cout << enemy_level*enemy_level;
        setColor(LightGrey, Black);
        cout << " опыта";
        if (hp_chance >= 95) myHero.exp += 3*enemy_level*enemy_level;
        else if (hp_chance >= 80) myHero.exp += 2*enemy_level*enemy_level;
        else myHero.exp += enemy_level*enemy_level;
        new_lvl = false;
        prev_level = myHero.level;
        while (myHero.exp >= exp_border[myHero.level-1])
        {
            myHero.level++;
            if (!new_lvl)
            {
                setColor(White, Black);
                cout << " НОВЫЙ УРОВЕНЬ!";
                setColor(LightGrey, Black);
                new_lvl = true;
            }
            switch(myHero.perks)
            {
                case 1:
                    myHero.max_hp += 240;
                    myHero.max_mp += 1;
                    myHero.hp += 240;
                    myHero.damage += 8;
                    myHero.armor += 3;
                    myHero.ability += 5;
                    break;
                case 2:
                    myHero.max_hp += 160;
                    myHero.max_mp += 1;
                    myHero.hp += 160;
                    myHero.damage += 13;
                    myHero.armor += 1;
                    myHero.ability += 5;
                    break;
                case 3:
                    myHero.max_hp += 180;
                    myHero.max_mp += 4;
                    myHero.hp += 180;
                    myHero.damage += 6;
                    myHero.armor += 2;
                    myHero.ability += 10;
                    break;
                case 4:
                    myHero.max_hp += 300;
                    myHero.damage = max_hp/45;
                    myHero.armor = max_hp/250;
                    myHero.ability = max_hp/45;
            }
        }
        setCursorPosition(3, 5);
        gold = enemy_level-4+rand()%9;
        if (gold < 1) gold = 1;
        myHero.gold += gold;
        cout << "+";
        setColor(Yellow, Black);
        cout << gold;
        setColor(LightGrey, Black);
        cout << " шау";
        setCursorPosition(0, 24);
        system("pause");
        if (myHero.level >= 5 && prev_level < 5) showNewSkill(myHero.perks, 3);
        if (myHero.level >= 10 && prev_level < 10) showNewSkill(myHero.perks, 4);
        if (myHero.level >= 25 && prev_level < 25) showNewSkill(myHero.perks, 5);
        if (myHero.level >= 50 && prev_level < 50) showNewSkill(myHero.perks, 6);
    }
    else
    {
        myHero.perks = 0;
        if (unmute) di.play();
        setCursorPosition(3, 2);
        cout << "                              ";
        setCursorPosition(3, 3);
        cout << "                              ";
        setCursorPosition(3, 4);
        cout << "Ваш герой погиб!             ";
        setCursorPosition(3, 5);
        cout << "Весь прогресс утерян!        ";
        setCursorPosition(0, 24);
        system("pause");
    }
    wi.stop();
}
void printEffectFull(int id, int rarity, bool weapon)    // FOUND ITEM
{
    if (!weapon)
    {
        switch(id)
        {
            case 0:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "25";
                else if (rarity == 1) cout << "20";
                else cout << "15";
                setColor(LightGrey, Black);
                cout << "% макс. ОЗ";
                break;
            case 1:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "25";
                else if (rarity == 1) cout << "20";
                else cout << "15";
                setColor(LightGrey, Black);
                cout << "% АТК";
                break;
            case 2:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "25";
                else if (rarity == 1) cout << "20";
                else cout << "15";
                setColor(LightGrey, Black);
                cout << "% ЗАЩ";
                break;
            case 3:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "1.5";
                else if (rarity == 1) cout << "1";
                else cout << "0.5";
                setColor(LightGrey, Black);
                cout << "% восстановлению заряда за ход";
                break;
            case 4:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "20";
                else if (rarity == 1) cout << "15";
                else cout << "10";
                setColor(LightGrey, Black);
                cout << "% шанс оглушения при обычной атаке";
                break;
            case 5:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "50";
                else if (rarity == 1) cout << "35";
                else cout << "20";
                setColor(LightGrey, Black);
                cout << "% усиление лечения";
                break;
            case 6:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "30";
                else if (rarity == 1) cout << "20";
                else cout << "10";
                setColor(LightGrey, Black);
                cout << " макс. ОМ";
                break;
            case 7:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "25";
                else if (rarity == 1) cout << "20";
                else cout << "15";
                setColor(LightGrey, Black);
                cout << "% МАГ";
                break;
            case 8:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "60";
                else if (rarity == 1) cout << "50";
                else cout << "40";
                setColor(LightGrey, Black);
                cout << "% урона от яда";
                break;
        }
    }
    else
    {
        switch(id)
        {
            case 0:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "15";
                else if (rarity == 1) cout << "10";
                else cout << "5";
                setColor(LightGrey, Black);
                cout << "% макс. ОЗ";
                break;
            case 1:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "15";
                else if (rarity == 1) cout << "10";
                else cout << "5";
                setColor(LightGrey, Black);
                cout << "% АТК";
                break;
            case 2:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "15";
                else if (rarity == 1) cout << "10";
                else cout << "5";
                setColor(LightGrey, Black);
                cout << "% ЗАЩ";
                break;
            case 3:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "0.6";
                else if (rarity == 1) cout << "0.4";
                else cout << "0.2";
                setColor(LightGrey, Black);
                cout << "% восстановлению ульта за ход";
                break;
            case 4:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "10";
                else if (rarity == 1) cout << "7";
                else cout << "4";
                setColor(LightGrey, Black);
                cout << "% шанс оглушения при обычной атаке";
                break;
            case 5:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "30";
                else if (rarity == 1) cout << "20";
                else cout << "10";
                setColor(LightGrey, Black);
                cout << "% усиление лечения";
                break;
            case 6:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "15";
                else if (rarity == 1) cout << "10";
                else cout << "5";
                setColor(LightGrey, Black);
                cout << " макс. ОМ";
                break;
            case 7:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "15";
                else if (rarity == 1) cout << "10";
                else cout << "5";
                setColor(LightGrey, Black);
                cout << "% МАГ";
                break;
            case 8:
                cout << "+";
                setColor(White, Black);
                if (rarity == 2) cout << "30";
                else if (rarity == 1) cout << "25";
                else cout << "20";
                setColor(LightGrey, Black);
                cout << "% урона от яда";
                break;
        }
    }
}
void printEffect(int id, int rarity)    // ITEM IN MENU
{
    switch(id)
    {
        case -1:
            cout << "НЕТ";
            break;
        case 0:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Пылающий";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Рубиновый";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Малиновый";
                setColor(LightGrey, Black);
            }
            cout << " кристалл";
            break;
        case 1:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Абсолютный";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Смертоносный";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Темный";
                setColor(LightGrey, Black);
            }
            cout << " фамилиар";
            break;
        case 2:
            cout << "Энерго-щит ";
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Хранителя";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Рыцаря";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Воина";
                setColor(LightGrey, Black);
            }
            break;
        case 3:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Кси";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Пси";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Ро";
                setColor(LightGrey, Black);
            }
            cout << "-ускоритель";
            break;
        case 4:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Инфразвуковой";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Сверхзвуковой";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Звуковой";
                setColor(LightGrey, Black);
            }
            cout << " излучатель";
            break;
        case 5:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Омега";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Бета";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Альфа";
                setColor(LightGrey, Black);
            }
            cout << "-обновитель";
            break;
        case 6:
        	if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Водяной";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Лазурный";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Аквамариновый";
                setColor(LightGrey, Black);
            }
            cout << " кристалл";
            break;
        case 7:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Божественный";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Древний";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Магический";
                setColor(LightGrey, Black);
            }
            cout << " фолиант";
            break;
        case 8:
            if (rarity == 2)
            {
                setColor(Yellow, Black);
                cout << "Инфернальный";
                setColor(LightGrey, Black);
            }
            else if (rarity == 1)
            {
                setColor(LightMagenta, Black);
                cout << "Смертельный";
                setColor(LightGrey, Black);
            }
            else
            {
                setColor(LightCyan, Black);
                cout << "Заразный";
                setColor(LightGrey, Black);
            }
            cout << " патоген";
            break;
    }
}
void craftingProcess(int item, int quality, int cnt)
{
    char q;
    int i, j, d, weapon_damage, weapon_ap, weapon_type, weapon_level, wear_armor, wear_ap, wear_type, wear_level, amulet_id, amulet_rarity, ring_id, ring_rarity, weapon_id, wear_id, weapon_rarity, wear_rarity;
    int max_temp = 100+quality, temp = max_temp, item_quality = 0, progress = 0, danger = 0;
    if (item >= 4) item_quality += 30;
    system("cls");
    if (!music.openFromFile("music/craftprocess.ogg")) return;
    if (unmute) music.play();
    cout << endl;
    cout << "          //-------------------------\\\\" << endl;
    cout << "        ==<|                         ||" << endl;
    cout << "          \\\\-------------------------//" << endl << endl;
    cout << "    Прогресс: [                                                            ]" << endl << endl;
    cout << "===============================================================================" << endl << endl;
    cout << "===============================================================================" << endl << " ";
    printKey("1");
    cout << " Слабый удар  (+прогресс,   -температура,   +качество,   -опасность)" << endl << " ";
    printKey("2");
    cout << " Средний удар (++прогресс,  --температура,  ++качество)" << endl << " ";
    printKey("3");
    cout << " Сильный удар (+++прогресс, ---температура, +++качество, ++опасность)" << endl << " ";
    printKey("4");
    cout << " Подождать    (---прогресс, --температура,  -качество,   --опасность)" << endl;
    cout << "===============================================================================";
    do
    {
        if (danger >= 5) setColor(LightRed, Black);
        else setColor(LightGrey, Black);
        setCursorPosition(0, 7);
        cout << "===============================================================================" << endl;
        cout << "                                                                               " << endl;
        cout << "===============================================================================";
        setColor(LightRed, Black);  // HUD
        for (i = 0; i < 5; i++)
        {
            setCursorPosition(7+15*i, 8);
            if (danger >= 5) cout << "ОПАСНО";
            else for (j = 0; j < danger; j++) cout << ">";
        }
        setColor(LightGrey, Black);
        if (temp*100/max_temp >= 60) setColor(Red, Black);
        else if (temp*100/max_temp >= 30) setColor(Yellow, Black);
        else setColor(Cyan, Black);
        setCursorPosition(12, 2);
        for (i = 1; i <= 25; i++)
        {
            if (temp*25/max_temp >= i) cout << "#";
            else cout << " ";
        }
        setCursorPosition(15, 5);
        setColor(LightGreen, Black);
        for (i = 1; i <= 60; i++)
        {
            if (progress*4 >= i) cout << ">";
            else cout << " ";
        }
        setColor(LightGrey, Black);
        setCursorPosition(46, 1);
        cout << "Запас температуры: " << temp << "°C ";
        setCursorPosition(46, 2);
        cout << "Качество: ";
        if (item%4 == 0 || item%4 == 1)
        {
            if (item_quality >= 130) setColor(Yellow, Black);
            else if (item_quality >= 100) setColor(LightMagenta, Black);
            else if (item_quality >= 70) setColor(LightCyan, Black);
            else if (item_quality >= 30) setColor(White, Black);
        }
        else
        {
            if (item_quality >= 110) setColor(Yellow, Black);
            else if (item_quality >= 80) setColor(LightMagenta, Black);
            else if (item_quality >= 50) setColor(LightCyan, Black);
            else setColor(White, Black);
        }
        cout << item_quality << "   ";
        setColor(LightGrey, Black);
        q = getch();
        switch(q)
        {
            case '1':
                if (unmute) h1.play();
                progress++;
                d = 2+rand()%2;
                item_quality++;
                danger--;
                break;
            case '2':
                if (unmute) h2.play();
                progress += 2;
                d = 4+rand()%2;
                item_quality += 3;
                break;
            case '3':
                if (unmute) h3.play();
                progress += 3;
                d = 7+rand()%2;
                item_quality += 6;
                danger += 2;
                break;
            case '4':
                progress -= 4;
                d = 4+rand()%2;
                item_quality--;
                danger -= 2;
                break;
        }
        if (danger >= 5) d *= 2;
        temp -= d;
        if (temp < 0) temp = 0;
        if (progress < 0) progress = 0;
        if (danger < 0) danger = 0;
    } while (progress < 15 && temp > 0);
    if (temp != 0)  // ADD WEAPON AND WEAR EFFECTS FROM CRAFTING
    {
        switch(item/2)
        {
            case 0:
                quality += 3;
                break;
            case 1:
                quality += 2;
                break;
            case 2:
                quality += 5;
                break;
            case 3:
                quality += 4;
                break;
        }
        if (item%4 == 0 || item%4 == 1)
        {
            if (item_quality >= 130) item_quality = 4;
            else if (item_quality >= 100) item_quality = 3;
            else if (item_quality >= 70) item_quality = 2;
            else if (item_quality >= 30) item_quality = 1;
            else item_quality = 0;
        }
        else
        {
            if (item_quality >= 110) item_quality = 4;
            else if (item_quality >= 80) item_quality = 3;
            else if (item_quality >= 50) item_quality = 2;
            else item_quality = 1;
        }
        switch(item%4)
        {
            case 0:
                switch(item_quality)
                {
                    case 0:
                        weapon_damage = (60+rand()%31)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        weapon_ap = (60+rand()%31)*(cnt*2+rand()%cnt)/100+rand()%2;
                        weapon_type = rand()%6;
                        weapon_level = rand()%2;
                        weapon_id = -1;
                        weapon_rarity = -1;
                        break;
                    case 1:
                        weapon_damage = (80+rand()%41)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        weapon_ap = (80+rand()%41)*(cnt*2+rand()%cnt)/100+rand()%2;
                        weapon_type = rand()%6;
                        weapon_level = 1+rand()%2;
                        weapon_id = -1;
                        weapon_rarity = -1;
                        break;
                    case 2:
                        weapon_damage = (100+rand()%41)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        weapon_ap = (100+rand()%41)*(cnt*2+rand()%cnt)/100+rand()%2;
                        weapon_type = rand()%6;
                        weapon_level = 2+rand()%2;
                        weapon_id = rand()%9;
                        weapon_rarity = 0;
                        break;
                    case 3:
                        weapon_damage = (130+rand()%21)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        weapon_ap = (130+rand()%21)*(cnt*2+rand()%cnt)/100+rand()%2;
                        weapon_type = rand()%6;
                        weapon_level = 3;
                        weapon_id = rand()%9;
                        weapon_rarity = 1;
                        break;
                    case 4:
                        weapon_damage = (140+rand()%21)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        weapon_ap = (140+rand()%21)*(cnt*2+rand()%cnt)/100+rand()%2;
                        weapon_type = rand()%6;
                        weapon_level = 3;
                        weapon_id = rand()%9;
                        weapon_rarity = 2;
                        break;
                }
                break;
            case 1:
                switch(item_quality)
                {
                    case 0:
                        wear_armor = (60+rand()%31)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        wear_ap = (60+rand()%31)*(cnt*2+rand()%cnt)/100+rand()%2;
                        wear_type = rand()%6;
                        wear_level = rand()%2;
                        wear_id = -1;
                        wear_rarity = -1;
                        break;
                    case 1:
                        wear_armor = (80+rand()%41)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        wear_ap = (80+rand()%41)*(cnt*2+rand()%cnt)/100+rand()%2;
                        wear_type = rand()%6;
                        wear_level = 1+rand()%2;
                        wear_id = -1;
                        wear_rarity = -1;
                        break;
                    case 2:
                        wear_armor = (100+rand()%41)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        wear_ap = (100+rand()%41)*(cnt*2+rand()%cnt)/100+rand()%2;
                        wear_type = rand()%6;
                        wear_level = 2+rand()%2;
                        wear_id = rand()%9;
                        wear_rarity = 0;
                        break;
                    case 3:
                        wear_armor = (130+rand()%21)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        wear_ap = (130+rand()%21)*(cnt*2+rand()%cnt)/100+rand()%2;
                        wear_type = rand()%6;
                        wear_level = 3;
                        wear_id = rand()%9;
                        wear_rarity = 1;
                        break;
                    case 4:
                        wear_armor = (140+rand()%21)*(cnt*4+rand()%(cnt*2))/100+rand()%2;
                        wear_ap = (140+rand()%21)*(cnt*2+rand()%cnt)/100+rand()%2;
                        wear_type = rand()%6;
                        wear_level = 3;
                        wear_id = rand()%9;
                        wear_rarity = 2;
                        break;
                }
                break;
            case 2:
                switch(item_quality)
                {
                    case 0:
                    case 1:
                        amulet_id = -1;
                        amulet_rarity = -1;
                        break;
                    case 2:
                        amulet_id = rand()%9;
                        amulet_rarity = 0;
                        break;
                    case 3:
                        amulet_id = rand()%9;
                        amulet_rarity = 1;
                        break;
                    case 4:
                        amulet_id = rand()%9;
                        amulet_rarity = 2;
                        break;
                }
                break;
            case 3:
                switch(item_quality)
                {
                    case 0:
                    case 1:
                        ring_id = -1;
                        ring_rarity = -1;
                        break;
                    case 2:
                        ring_id = rand()%9;
                        ring_rarity = 0;
                        break;
                    case 3:
                        ring_id = rand()%9;
                        ring_rarity = 1;
                        break;
                    case 4:
                        ring_id = rand()%9;
                        ring_rarity = 2;
                        break;
                }
                break;
        }
        setCursorPosition(1, 16);
        switch(item%4)
        {
            case 0:
                switch(item_quality)
                {
                    case 0:
                        cout << "Вы создали некачественное";
                        break;
                    case 1:
                        cout << "Вы создали ";
                        setColor(White, Black);
                        cout << "обычное";
                        setColor(LightGrey, Black);
                        break;
                    case 2:
                        cout << "Вы создали ";
                        setColor(LightCyan, Black);
                        cout << "редкое";
                        setColor(LightGrey, Black);
                        break;
                    case 3:
                        cout << "Вы создали ";
                        setColor(LightMagenta, Black);
                        cout << "эпическое";
                        setColor(LightGrey, Black);
                        break;
                    case 4:
                        cout << "Вы создали ";
                        setColor(Yellow, Black);
                        cout << "легендарное";
                        setColor(LightGrey, Black);
                        break;
                }
                cout << " оружие";
                setCursorPosition(1, 17);
                cout << "Ваше оружие:  +";
                setColor(White, Black);
                cout << myHero.weapon_damage;
                setColor(LightGrey, Black);
                cout << " АТК / +";
                setColor(White, Black);
                cout << myHero.weapon_ap;
                setColor(LightGrey, Black);
                cout << " МАГ / ";
                setColor(White, Black);
                cout << myHero.weapon_level;
                setColor(LightGrey, Black);
                cout << " уровень ";
                element(myHero.weapon_type);
                setCursorPosition(15, 18);
                printEffectFull(myHero.weapon_id, myHero.weapon_rarity, 1);
                setCursorPosition(1, 19);
                cout << "Новое оружие: +";
                if (weapon_damage > myHero.weapon_damage) setColor(LightGreen, Black);
                else if (weapon_damage == myHero.weapon_damage) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << weapon_damage;
                setColor(LightGrey, Black);
                cout << " АТК / +";
                if (weapon_ap > myHero.weapon_ap) setColor(LightGreen, Black);
                else if (weapon_ap == myHero.weapon_ap) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << weapon_ap;
                setColor(LightGrey, Black);
                cout << " МАГ / ";
                setColor(White, Black);
                if (weapon_level > myHero.weapon_level) setColor(LightGreen, Black);
                else if (weapon_level == myHero.weapon_level) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << weapon_level;
                setColor(LightGrey, Black);
                cout << " уровень ";
                element(weapon_type);
                setCursorPosition(15, 20);
                printEffectFull(weapon_id, weapon_rarity, 1);
                break;
            case 1:
                switch(item_quality)
                {
                    case 0:
                        cout << "Вы создали некачественную";
                        break;
                    case 1:
                        cout << "Вы создали ";
                        setColor(White, Black);
                        cout << "обычную";
                        setColor(LightGrey, Black);
                        break;
                    case 2:
                        cout << "Вы создали ";
                        setColor(LightCyan, Black);
                        cout << "редкую";
                        setColor(LightGrey, Black);
                        break;
                    case 3:
                        cout << "Вы создали ";
                        setColor(LightMagenta, Black);
                        cout << "эпическую";
                        setColor(LightGrey, Black);
                        break;
                    case 4:
                        cout << "Вы создали ";
                        setColor(Yellow, Black);
                        cout << "легендарную";
                        setColor(LightGrey, Black);
                        break;
                }
                cout << " броню";
                setCursorPosition(1, 17);
                cout << "Ваша броня:  +";
                setColor(White, Black);
                cout << myHero.wear_armor;
                setColor(LightGrey, Black);
                cout << " ЗАЩ / +";
                setColor(White, Black);
                cout << myHero.wear_ap;
                setColor(LightGrey, Black);
                cout << " МАГ / ";
                setColor(White, Black);
                cout << myHero.wear_level;
                setColor(LightGrey, Black);
                cout << " уровень ";
                element(myHero.wear_type);
                setCursorPosition(14, 18);
                printEffectFull(myHero.wear_id, myHero.wear_rarity, 1);
                setCursorPosition(1, 19);
                cout << "Новая броня: +";
                if (wear_armor > myHero.wear_armor) setColor(LightGreen, Black);
                else if (wear_armor == myHero.wear_armor) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << wear_armor;
                setColor(LightGrey, Black);
                cout << " ЗАЩ / +";
                if (wear_ap > myHero.wear_ap) setColor(LightGreen, Black);
                else if (wear_ap == myHero.wear_ap) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << wear_ap;
                setColor(LightGrey, Black);
                cout << " МАГ / ";
                setColor(White, Black);
                if (wear_level > myHero.wear_level) setColor(LightGreen, Black);
                else if (wear_level == myHero.wear_level) setColor(White, Black);
                else setColor(LightRed, Black);
                cout << wear_level;
                setColor(LightGrey, Black);
                cout << " уровень ";
                element(wear_type);
                setCursorPosition(14, 20);
                printEffectFull(wear_id, wear_rarity, 1);
                break;
            case 2:
                cout << "Вы создали ";
                switch(item_quality)
                {
                    case 1:
                        setColor(White, Black);
                        cout << "обычное";
                        setColor(LightGrey, Black);
                        break;
                    case 2:
                        setColor(LightCyan, Black);
                        cout << "редкое";
                        setColor(LightGrey, Black);
                        break;
                    case 3:
                        setColor(LightMagenta, Black);
                        cout << "эпическое";
                        setColor(LightGrey, Black);
                        break;
                    case 4:
                        setColor(Yellow, Black);
                        cout << "легендарное";
                        setColor(LightGrey, Black);
                        break;
                }
                cout << " кольцо";
                setCursorPosition(1, 17);
                cout << "Ваш амулет:   ";
                printEffect(myHero.amulet_id, myHero.amulet_rarity);
                setCursorPosition(15, 18);
                printEffectFull(myHero.amulet_id, myHero.amulet_rarity, 0);
                setCursorPosition(1, 20);
                cout << "Новый амулет: ";
                printEffect(amulet_id, amulet_rarity);
                setCursorPosition(14, 21);
                printEffectFull(amulet_id, amulet_rarity, 0);
                break;
            case 3:
                cout << "Вы создали ";
                switch(item_quality)
                {
                    case 1:
                        setColor(White, Black);
                        cout << "обычное";
                        setColor(LightGrey, Black);
                        break;
                    case 2:
                        setColor(LightCyan, Black);
                        cout << "редкое";
                        setColor(LightGrey, Black);
                        break;
                    case 3:
                        setColor(LightMagenta, Black);
                        cout << "эпическое";
                        setColor(LightGrey, Black);
                        break;
                    case 4:
                        setColor(Yellow, Black);
                        cout << "легендарное";
                        setColor(LightGrey, Black);
                        break;
                }
                cout << " кольцо";
                setCursorPosition(1, 17);
                cout << "Ваше кольцо:  ";
                printEffect(myHero.ring_id, myHero.ring_rarity);
                setCursorPosition(15, 18);
                printEffectFull(myHero.ring_id, myHero.ring_rarity, 0);
                setCursorPosition(1, 20);
                cout << "Новое кольцо: ";
                printEffect(ring_id, ring_rarity);
                setCursorPosition(14, 21);
                printEffectFull(ring_id, ring_rarity, 0);
                break;
        }
        setCursorPosition(1, 23);
        printKey("E");
        cout << " Надеть / ";
        printKey("Q");
        cout << " Выкинуть";
        do { q = getch(); } while (q != 'e' && q != 'q');
        if (q == 'e')
        {
            switch(item%4)
            {
                case 0:
                    myHero.weapon_damage = weapon_damage;
                    myHero.weapon_ap = weapon_ap;
                    myHero.weapon_type = weapon_type;
                    myHero.weapon_level = weapon_level;
                    myHero.weapon_id = weapon_id;
                    myHero.weapon_rarity = weapon_rarity;
                    break;
                case 1:
                    myHero.wear_armor = wear_armor;
                    myHero.wear_ap = wear_ap;
                    myHero.wear_type = wear_type;
                    myHero.wear_level = wear_level;
                    myHero.wear_id = wear_id;
                    myHero.wear_rarity = wear_rarity;
                    break;
                case 2:
                    myHero.amulet_id = amulet_id;
                    myHero.amulet_rarity = amulet_rarity;
                    break;
                case 3:
                    myHero.ring_id = ring_id;
                    myHero.ring_rarity = ring_rarity;
                    break;
            }
        }
    }
    else
    {
        setCursorPosition(1, 18);
        setColor(LightRed, Black);
        cout << "Предмет слишком сильно остыл, и последним ударом вы его разбили!";
        setColor(LightGrey, Black);
        setCursorPosition(1, 23);
        system("pause");
    }
    return;
}
void setSoundVolume(int sound_volume)
{
    a0.setVolume(sound_volume);
    a1.setVolume(sound_volume);
    a2.setVolume(sound_volume);
    a3.setVolume(sound_volume);
    a4.setVolume(sound_volume);
    a5.setVolume(sound_volume);
    a6.setVolume(sound_volume);
    pts.setVolume(sound_volume);
    bs.setVolume(sound_volume);
    sp.setVolume(sound_volume);
    di.setVolume(sound_volume);
    wi.setVolume(sound_volume);
    nx.setVolume(sound_volume);
    no.setVolume(sound_volume);
    h1.setVolume(sound_volume);
    h2.setVolume(sound_volume);
    h3.setVolume(sound_volume);
    sk.setVolume(sound_volume);
    sq.setVolume(sound_volume);
    return;
}
int main()
{
    HANDLE hOut;
    CONSOLE_CURSOR_INFO ConCurInf;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &ConCurInf);
    system("mode con cols=80 lines=25");
    srand(time(0));
	setlocale(LC_ALL, "RUS");
    if (!pt.loadFromFile("music/sfx/pt.ogg")) return 0;
    if (!bss.loadFromFile("music/sfx/battlestart.ogg")) return 0;
    if (!spp.loadFromFile("music/sfx/speech.ogg")) return 0;
    if (!die.loadFromFile("music/sfx/die.ogg")) return 0;
    if (!win.loadFromFile("music/sfx/win.ogg")) return 0;
    if (!nxt.loadFromFile("music/sfx/next.ogg")) return 0;
    if (!nope.loadFromFile("music/sfx/no.ogg")) return 0;
    if (!an1.loadFromFile("music/sfx/anvil1.ogg")) return 0;
    if (!an2.loadFromFile("music/sfx/anvil2.ogg")) return 0;
    if (!an3.loadFromFile("music/sfx/anvil3.ogg")) return 0;
    if (!skl.loadFromFile("music/sfx/skill.ogg")) return 0;
    if (!str.loadFromFile("music/sfx/startup.ogg")) return 0;
    pts.setBuffer(pt);
    bs.setBuffer(bss);
    sp.setBuffer(spp);
    di.setBuffer(die);
    wi.setBuffer(win);
    nx.setBuffer(nxt);
    no.setBuffer(nope);
    h1.setBuffer(an1);
    h2.setBuffer(an2);
    h3.setBuffer(an3);
    sk.setBuffer(skl);
    sq.setBuffer(str);
	bool debug = false, f, menu, save, ex, shop, buy1, buy2, any_item, show_backpack = false;
	int i, j, in_ar, out_ar, x_s, y_s, x_f, y_f, x_t, y_t, a, b, s, y, music_volume, sound_volume, sel, cnt, diff, item_cnt, sc, item_find, item_find_use, quality, weapon_damage, wear_armor, weapon_ap, wear_ap, weapon_type, wear_type, weapon_level, wear_level, weapon_id, wear_id, amulet_id, weapon_rarity, wear_rarity, amulet_rarity, ring_id, ring_rarity, res_id, ch_wear, item1, item2, price1, price2, imb, price_imb, verse, gold_l;
	unsigned long checksum, checksumN;
	float diffC;
	char q;
	string shop_s, intro[7];
	ifstream fin;
	ofstream fout;
    s = 0;
    for (i = 0; i < 101; i++)   // LEVEL UP REQUIREMENTS
    {
        s += 6*(i+1)*(i+1);
        exp_border[i] = s;
    }
    exp_border[101] = 2100000000;
    cout << endl << " Убедительная просьба перед началом игры сменить раскладку на ";
    setColor(LightRed, Black);
    cout << "АНГЛИЙСКУЮ";
    setColor(LightGrey, Black);
    cout << "!" << endl;
    cout << " В частности, если это не будет сделано, нельзя будет продолжить игру, так как" << endl;
    cout << " клавиши будут считываться с неправильным кодом" << endl << endl;
    cout << " Для пользователей Windows 7 и выше: проверьте, что в консоли выбран" << endl;
    cout << " шрифт 8 x 12 из раздела \"Точечные шрифты\"" << endl << endl;
    cout << " Также прошу заметить, что ни в коем случае ";
    setColor(LightRed, Black);
    cout << "НЕЛЬЗЯ";
    setColor(LightGrey, Black);
    cout << " выходить из игры через" << endl;
    cout << " \"крестик\" в верхнем правом углу консоли, иначе есть риск исчезновения" << endl;
    cout << " сохраненных данных игры" << endl << endl;
    cout << " Команда ";
    setColor(White, Black);
    cout << "200IQ Entertainment";
    setColor(LightGrey, Black);
    cout << " благодарит игрока за понимание!";
    setCursorPosition(34, 23);
    printKey("Y");
    cout << " Готово";
    do { y = getch(); } while (y != 'y');
	fin.open("save.txt");
	if (!fin)   // IF SAVE FILE EXISTS
    {
        fout.open("save.txt");
        fout << "0 50 50 1";
        fout.close();
        fin.open("save.txt");
        fin >> myHero.perks >> music_volume >> sound_volume;
        fin >> unmute;
    }
    else
    {
        fin >> myHero.perks >> music_volume >> sound_volume;
        fin >> unmute;
        if (myHero.perks != 0)
        {
            for (i = 0; i < 21; i++)
            {
                maze[0][i] = '#';
                maze[20][i] = '#';
            }
            for (i = 1; i < 20; i++)
            {
                fin >> in_ar;
                data[i+3] = in_ar;
                for (j = 0; j < 21; j++)
                {
                    if (in_ar >= pow(2, 20-j))
                    {
                        maze[i][j] = '#';
                        in_ar -= pow(2, 20-j);
                    }
                    else maze[i][j] = '.';
                }
            }
            fin >> diff;
            switch(diff)
            {
                case 1:
                    diffC = 0.9;
                    break;
                case 2:
                    diffC = 1;
                    break;
                case 3:
                    diffC = 1.1;
                    break;
                case 4:
                    diffC = 1.25;
                    break;
            }
            fin >> cnt >> x_s >> y_s >> x_f >> y_f >> x_t >> y_t >> myHero.hp >> myHero.mp >> myHero.charge >> myHero.weapon_damage >> myHero.weapon_ap >> myHero.weapon_type >> myHero.weapon_level >> myHero.weapon_id >> myHero.weapon_rarity >> myHero.wear_armor >> myHero.wear_ap >> myHero.wear_type >> myHero.wear_level >> myHero.wear_id >> myHero.wear_rarity >> myHero.amulet_id >> myHero.amulet_rarity >> myHero.ring_id >> myHero.ring_rarity >> myHero.level >> myHero.exp >> myHero.gold >> quality;
            data[0] = myHero.perks;
            data[1] = music_volume;
            data[2] = sound_volume;
            data[3] = unmute;
            data[23] = diff;
            data[24] = cnt;
            data[25] = x_s;
            data[26] = y_s;
            data[27] = x_f;
            data[28] = y_f;
            data[29] = x_t;
            data[30] = y_t;
            data[31] = myHero.hp;
            data[32] = myHero.mp;
            data[33] = myHero.charge;
            data[34] = myHero.weapon_damage;
            data[35] = myHero.weapon_ap;
            data[36] = myHero.weapon_type;
            data[37] = myHero.weapon_level;
            data[38] = myHero.weapon_id;
            data[39] = myHero.weapon_rarity;
            data[40] = myHero.wear_armor;
            data[41] = myHero.wear_ap;
            data[42] = myHero.wear_type;
            data[43] = myHero.wear_level;
            data[44] = myHero.wear_id;
            data[45] = myHero.wear_rarity;
            data[46] = myHero.amulet_id;
            data[47] = myHero.amulet_rarity;
            data[48] = myHero.ring_id;
            data[49] = myHero.ring_rarity;
            data[50] = myHero.level;
            data[51] = myHero.exp;
            data[52] = myHero.gold;
            data[53] = quality;
            for (i = 0; i < items_all; i++)
            {
                fin >> myHero.backpack[i];
                data[54+i] = myHero.backpack[i];
            }
            fin >> checksum;
            checksumN = hash_33();
            if (myHero.perks != 0 && x_s == 1337 && y_s == 7331 && x_f == 0 && y_f == 0)
            {
                x_s = 1;
                y_s = 1;
                x_f = 19;
                y_f = 19;
                debug = true;
            }
            else if (checksum != checksumN)
            {
                system("cls");
                setCursorPosition(28, 9);
                setColor(White, Black);
                cout << "Ошибка файла сохранения!";
                setColor(LightGrey, Black);
                setCursorPosition(21, 11);
                cout << "В целях безопасности файл будет удален";
                setCursorPosition(11, 13);
                setColor(White, Black);
                cout << "НАПОМИНАНИЕ:";
                setColor(LightGrey, Black);
                cout << " ни в коем случае ";
                setColor(LightRed, Black);
                cout << "НЕ РЕДАКТИРУЙТЕ";
                setColor(LightGrey, Black);
                cout << " файл ";
                setColor(White, Black);
                cout << "save.txt";
                setColor(LightGrey, Black);
                setCursorPosition(0, 24);
                fin.close();
                remove("save.txt");
                system("pause");
                return 0;
            }
            switch(myHero.perks)
            {
                case 1:
                    myHero.max_hp = 2640+240*(myHero.level-1);
                    myHero.max_mp = 40+myHero.level-1;
                    myHero.damage = 80+8*(myHero.level-1);
                    myHero.armor = 30+3*(myHero.level-1);
                    myHero.ability = 50+5*(myHero.level-1);
                    break;
                case 2:
                    myHero.max_hp = 1760+160*(myHero.level-1);
                    myHero.max_mp = 40+myHero.level-1;
                    myHero.damage = 130+13*(myHero.level-1);
                    myHero.armor = 10+myHero.level-1;
                    myHero.ability = 50+5*(myHero.level-1);
                    break;
                case 3:
                    myHero.max_hp = 1980+180*(myHero.level-1);
                    myHero.max_mp = 40+4*(myHero.level-1);
                    myHero.damage = 60+6*(myHero.level-1);
                    myHero.armor = 20+2*(myHero.level-1);
                    myHero.ability = 100+10*(myHero.level-1);
                    break;
                case 4:
                    myHero.max_hp = 2300+300*(myHero.level-1);
                    myHero.max_mp = 3;
                    myHero.damage = myHero.max_hp/45;
                    myHero.armor = myHero.max_hp/250;
                    myHero.ability = myHero.max_hp/45;
                    break;
            }
            myHero.damage += 10*myHero.backpack[10];
            myHero.armor += 10*myHero.backpack[11];
            myHero.ability += 10*myHero.backpack[12];
        }
    }
    fin.close();
    remove("save.txt");
    system("cls");  // INTRO
    if (unmute) sq.play();
    intro[0] = "                          ___   ___   ___    ___   ___                          ";
    intro[1] = "                         /   \\ /   \\ /   \\  |   | /   \\                         ";
    intro[2] = "                         |_| | | | | | | |   | |  | _ |                         ";
    intro[3] = "                         / __/ | | | | | |   | |  | \\\\|                         ";
    intro[4] = "                         |___| \\___/ \\___/  |___| \\__\\\\                         ";
    intro[5] = "                                                                                ";
    intro[6] = "                        200IQ ENTERTAINMENT ПРЕДСТАВЛЯЕТ                        ";
    for (i = 12; i < 68; i++)
    {
        if (i != 67) for (j = 0; j < 7; j++)
        {
            setCursorPosition(i-j, 8+j);
            cout << intro[j][i-j];
            setCursorPosition(i-j+1, 8+j);
            cout << ">";
        }
        else for (j = 0; j < 7; j++)
        {
            setCursorPosition(i-j, 8+j);
            cout << " ";
        }
        Sleep(50);
    }
    setCursorPosition(1, 23);
    system("pause");
    sq.stop();
    music.setVolume(music_volume);
    setSoundVolume(sound_volume);
    mainMenu:   // GAME START
    music.setLoop(false);
    system("cls");
    setColor(LightGrey, Black);
    if (!music.openFromFile("music/menu.ogg")) return 0;
    if (unmute) music.play();
	sel = 1;
	cout << endl << "                      Добро пожаловать в Героев Подземелий" << endl << endl;
    cout << "                       Copyright 200IQ Entertainment 2018" << endl << endl;
	cout << "===============================================================================" << endl << endl;
	setColor(White, Black);
	cout << " Начать игру" << endl;
	setColor(LightGrey, Black);
	if (myHero.perks != 0) cout << " Загрузить игру";
	cout << endl;
	cout << " Настройки" << endl;
	cout << " Титры" << endl;
	cout << " Выйти из игры" << endl;
	setCursorPosition(1, 23);
	printKey("W");
	printKey("S");
	cout << " Выбор   ";
	printKey("ENTER");
	cout << " Подтверждение выбора";
	menu = false;
	save = false;
	ex = false;
	do
    {
        y = getch();
        if ((y == 'w' && sel > 1) || (y == 's' && sel < 5))
        {
            switch(sel)
            {
                case 1:
                    setCursorPosition(0, 7);
                    cout << " Начать игру";
                    break;
                case 2:
                    setCursorPosition(0, 8);
                    cout << " Загрузить игру";
                    break;
                case 3:
                    setCursorPosition(0, 9);
                    cout << " Настройки";
                    break;
                case 4:
                    setCursorPosition(0, 10);
                    cout << " Титры";
                    break;
                case 5:
                    setCursorPosition(0, 11);
                    cout << " Выйти из игры";
                    break;
            }
            if (y == 'w')
            {
                sel--;
                if (myHero.perks == 0 && sel == 2) sel--;
            }
            if (y == 's')
            {
                sel++;
                if (myHero.perks == 0 && sel == 2) sel++;
            }
            setColor(White, Black);
            switch(sel)
            {
                case 1:
                    setCursorPosition(0, 7);
                    cout << " Начать игру";
                    break;
                case 2:
                    setCursorPosition(0, 8);
                    cout << " Загрузить игру";
                    break;
                case 3:
                    setCursorPosition(0, 9);
                    cout << " Настройки";
                    break;
                case 4:
                    setCursorPosition(0, 10);
                    cout << " Титры";
                    break;
                case 5:
                    setCursorPosition(0, 11);
                    cout << " Выйти из игры";
                    break;
            }
            setColor(LightGrey, Black);
        }
        else if (y == 13)
        {
            switch(sel)
            {
                case 1:
                    menu = true;
                    save = false;
                    break;
                case 2:
                    menu = true;
                    save = true;
                    break;
                case 3:
                    system("cls");
                    sel = 1;
                    cout << endl << " [================================ Настройки =================================]" << endl << endl;
                    setColor(White, Black);
                    cout << " Громкость звуков";
                    setColor(LightGrey, Black);
                    cout << "                          [---------------------]" << endl;
                    setColor(LightGrey, Black);
                    cout << " Громкость музыки                          [---------------------]" << endl;
                    cout << " Вкл/выкл звуковое сопровождение" << endl;
                    setCursorPosition(44+sound_volume/5, 3);
                    cout << "|";
                    setCursorPosition(44+music_volume/5, 4);
                    cout << "|";
                    setCursorPosition(1, 23);
                    printKey("W");
                    printKey("S");
                    cout << " Выбор  ";
                    printKey("A");
                    printKey("D");
                    cout << " Настройка  ";
                    printKey("ENTER");
                    cout << " Подтверждение  ";
                    printKey("0");
                    cout << " Вернуться в меню";
                    do
                    {
                        y = getch();
                        if ((y == 'w' && sel > 1) || (y == 's' && sel < 3))
                        {
                            switch(sel)
                            {
                                case 1:
                                    setCursorPosition(0, 3);
                                    cout << " Громкость звуков";
                                    break;
                                case 2:
                                    setCursorPosition(0, 4);
                                    cout << " Громкость музыки";
                                    break;
                                case 3:
                                    setCursorPosition(0, 5);
                                    cout << " Вкл/выкл звуковое сопровождение";
                                    break;
                            }
                            if (y == 'w') sel--;
                            if (y == 's') sel++;
                            setColor(White, Black);
                            switch(sel)
                            {
                                case 1:
                                    setCursorPosition(0, 3);
                                    cout << " Громкость звуков";
                                    break;
                                case 2:
                                    setCursorPosition(0, 4);
                                    cout << " Громкость музыки";
                                    break;
                                case 3:
                                    setCursorPosition(0, 5);
                                    cout << " Вкл/выкл звуковое сопровождение";
                                    break;
                            }
                            setColor(LightGrey, Black);
                        }
                        else if (y == 'a')
                        {
                            if (sel == 1 && sound_volume > 0)
                            {
                                setCursorPosition(44+sound_volume/5, 3);
                                cout << "-";
                                sound_volume -= 5;
                                setCursorPosition(44+sound_volume/5, 3);
                                cout << "|";
                                setSoundVolume(sound_volume);
                                if (unmute) h2.play();
                            }
                            if (sel == 2 && music_volume > 0)
                            {
                                setCursorPosition(44+music_volume/5, 4);
                                cout << "-";
                                music_volume -= 5;
                                setCursorPosition(44+music_volume/5, 4);
                                cout << "|";
                                music.setVolume(music_volume);
                                if (unmute) music.play();
                            }
                        }
                        else if (y == 'd')
                        {
                            if (sel == 1 && sound_volume < 100)
                            {
                                setCursorPosition(44+sound_volume/5, 3);
                                cout << "-";
                                sound_volume += 5;
                                setCursorPosition(44+sound_volume/5, 3);
                                cout << "|";
                                setSoundVolume(sound_volume);
                                if (unmute) h2.play();
                            }
                            if (sel == 2 && music_volume < 100)
                            {
                                setCursorPosition(44+music_volume/5, 4);
                                cout << "-";
                                music_volume += 5;
                                setCursorPosition(44+music_volume/5, 4);
                                cout << "|";
                                music.setVolume(music_volume);
                                if (unmute) music.play();
                            }
                        }
                        else if (sel == 3 && y == 13)
                        {
                            unmute = !unmute;
                            if (!music.openFromFile("music/menu.ogg")) return 0;
                            if (unmute) music.play();
                            else music.stop();
                        }
                    } while (y != '0');
                    music.stop();
                    goto mainMenu;
                    break;
                case 4:
                    credits(false);
                    goto mainMenu;
                    break;
                case 5:
                    fout.open("save.txt");
                    fout << myHero.perks << " " << music_volume << " " << sound_volume << " ";
                    fout << unmute << " ";
                    if (myHero.perks != 0)
                    {
                        for (i = 1; i < 20; i++)
                        {
                            out_ar = 0;
                            for (j = 0; j < 21; j++)
                            {
                                if (maze[i][j] == '#') out_ar += pow(2, 20-j);
                            }
                            fout << out_ar << " ";
                            data[i+3] = out_ar;
                        }
                        fout << diff << " " << cnt << " " << x_s << " " << y_s << " " << x_f << " " << y_f << " " << x_t << " " << y_t << " " << myHero.hp << " " << myHero.mp << " " << myHero.charge << " " << myHero.weapon_damage << " " << myHero.weapon_ap << " " << myHero.weapon_type << " " << myHero.weapon_level << " " << myHero.weapon_id << " " << myHero.weapon_rarity << " " << myHero.wear_armor << " " << myHero.wear_ap << " " << myHero.wear_type << " " << myHero.wear_level << " " << myHero.wear_id << " " << myHero.wear_rarity << " " << myHero.amulet_id << " " << myHero.amulet_rarity << " " << myHero.ring_id << " " << myHero.ring_rarity << " " << myHero.level << " " << myHero.exp << " " << myHero.gold << " " << quality << " ";
                        for (i = 0; i < items_all; i++) fout << myHero.backpack[i] << " ";
                        data[0] = myHero.perks;
                        data[1] = music_volume;
                        data[2] = sound_volume;
                        data[3] = unmute;
                        data[23] = diff;
                        data[24] = cnt;
                        data[25] = x_s;
                        data[26] = y_s;
                        data[27] = x_f;
                        data[28] = y_f;
                        data[29] = x_t;
                        data[30] = y_t;
                        data[31] = myHero.hp;
                        data[32] = myHero.mp;
                        data[33] = myHero.charge;
                        data[34] = myHero.weapon_damage;
                        data[35] = myHero.weapon_ap;
                        data[36] = myHero.weapon_type;
                        data[37] = myHero.weapon_level;
                        data[38] = myHero.weapon_id;
                        data[39] = myHero.weapon_rarity;
                        data[40] = myHero.wear_armor;
                        data[41] = myHero.wear_ap;
                        data[42] = myHero.wear_type;
                        data[43] = myHero.wear_level;
                        data[44] = myHero.wear_id;
                        data[45] = myHero.wear_rarity;
                        data[46] = myHero.amulet_id;
                        data[47] = myHero.amulet_rarity;
                        data[48] = myHero.ring_id;
                        data[49] = myHero.ring_rarity;
                        data[50] = myHero.level;
                        data[51] = myHero.exp;
                        data[52] = myHero.gold;
                        data[53] = quality;
                        for (i = 0; i < items_all; i++)
                        {
                            fin >> myHero.backpack[i];
                            data[54+i] = myHero.backpack[i];
                        }
                        checksumN = hash_33();
                        fout << checksumN;
                        fout.close();
                    }
                    return 0;
            }
        }
    } while (!menu);
    if (!save)  // NEW CHARACTER OR LOAD FROM SAVE
    {
        system("cls");
        cout << endl << "                           Выберите класс персонажа" << endl << endl;
        cout << "===============================================================================" << endl << endl;
        setColor(White, Black);
        cout << " Воин-берсерк" << endl;
        setColor(LightGrey, Black);
        cout << " Мастер Теней" << endl;
        cout << " Верховный Маг" << endl;
        cout << " Чародей Крови" << endl << endl;
        cout << "===============================================================================" << endl << endl;
        cout << " Неумолимый боец, размахивающий своим молотом до последней капли крови         " << endl << endl;
        cout << " АТК: [#######     ]" << endl;
        cout << " МАГ: [####        ]" << endl;
        cout << " ЖИВ: [#########   ]" << endl << endl;
        setColor(LightGreen, Black);
        cout << " Имеет множество оглушающих навыков                                            " << endl;
        cout << "                                                                               " << endl << endl << " ";
        printKey("1");
        cout << " Удар топором - наносит обычный урон, зависящий от АТК                     " << endl << " ";
        printKey("2");
        cout << " Кража жизни - наносит двойной АТК урон и лечит героя на количество урона  " << endl << " ";
        printKey("Z");
        cout << " Крик берсерка - аура, увеличивающая АТК урон на 50% за уровень            " << endl;
        setColor(LightGrey, Black);
        menu = false;
        sel = 1;
        do
        {
            y = getch();
            if ((y == 'w' && sel > 1) || (y == 's' && sel < 4))
            {
                switch(sel)
                {
                    case 1:
                        setCursorPosition(0, 5);
                        cout << " Воин-берсерк";
                        break;
                    case 2:
                        setCursorPosition(0, 6);
                        cout << " Мастер Теней";
                        break;
                    case 3:
                        setCursorPosition(0, 7);
                        cout << " Верховный Маг";
                        break;
                    case 4:
                        setCursorPosition(0, 8);
                        cout << " Чародей Крови";
                        break;
                }
                if (y == 'w') sel--;
                if (y == 's') sel++;
                switch(sel)
                {
                    case 1:
                        setCursorPosition(0, 5);
                        setColor(White, Black);
                        cout << " Воин-берсерк";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Неумолимый боец, размахивающий своим молотом до последней капли крови         " << endl << endl;
                        cout << " АТК: [#######     ]" << endl;
                        cout << " МАГ: [####        ]" << endl;
                        cout << " ЖИВ: [#########   ]" << endl << endl;
                        setColor(LightGreen, Black);
                        cout << " Имеет множество оглушающих навыков                                            " << endl;
                        cout << "                                                                               " << endl << endl << " ";
                        printKey("1");
                        cout << " Удар топором - наносит обычный урон, зависящий от АТК                     " << endl << " ";
                        printKey("2");
                        cout << " Кража жизни - наносит двойной АТК урон и лечит героя на количество урона  " << endl << " ";
                        printKey("Z");
                        cout << " Крик берсерка - аура, увеличивающая АТК урон на 50% за уровень            " << endl;
                        setColor(LightGrey, Black);
                        break;
                    case 2:
                        setCursorPosition(0, 6);
                        setColor(White, Black);
                        cout << " Мастер Теней";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Виртуоз изящных убийств, который способен уклониться от любого удара          " << endl << endl;
                        cout << " АТК: [############]" << endl;
                        cout << " МАГ: [####        ]" << endl;
                        cout << " ЖИВ: [####        ]" << endl << endl;
                        setColor(LightGreen, Black);
                        cout << " Множество способностей на уклонение                                           " << endl;
                        cout << " Сильный урон от яда                                                           " << endl << endl << " ";
                        printKey("1");
                        cout << " Удар клинка - наносит обычный урон, зависящий от АТК                      " << endl << " ";
                        printKey("2");
                        cout << " Укрытие во тьме - дает очень малое лечение и 100% уклонение на ход        " << endl << " ";
                        printKey("Z");
                        cout << " Аура неведения - аура, увеличивающая шанс уклонения на 15% за уровень     " << endl;
                        setColor(LightGrey, Black);
                        break;
                    case 3:
                        setCursorPosition(0, 7);
                        setColor(White, Black);
                        cout << " Верховный Маг";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Мастер волшебства, в чертогах разума которого скрыто множество заклинаний     " << endl << endl;
                        cout << " АТК: [###         ]" << endl;
                        cout << " МАГ: [############]" << endl;
                        cout << " ЖИВ: [#######     ]" << endl << endl;
                        setColor(LightGreen, Black);
                        cout << " Разнообразные способности                                                     " << endl;
                        cout << " Сильный прирост ОМ                                                            " << endl << endl << " ";
                        printKey("1");
                        cout << " Магическая атака - наносит обычный урон, зависящий от АТК                 " << endl << " ";
                        printKey("2");
                        cout << " Невидимость - дает небольшое лечение и уклонение на ход                   " << endl << " ";
                        printKey("Z");
                        cout << " Волшебная аура - аура, увеличивающая МАГ урон на 10% за уровень           " << endl;
                        setColor(LightGrey, Black);
                        break;
                    case 4:
                        setCursorPosition(0, 8);
                        setColor(White, Black);
                        cout << " Чародей Крови";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Окультист, не страшащийся пожертвовать собой ради высшей цели                 " << endl << endl;
                        cout << " АТК: [#######     ]" << endl;
                        cout << " МАГ: [#######     ]" << endl;
                        cout << " ЖИВ: [############]" << endl << endl;
                        setColor(LightRed, Black);
                        cout << " Некоторые способности наносят вред как врагу, так и герою                     " << endl;
                        cout << " Ресурс ЗК восполняется только засчет определенных способностей                " << endl << endl << " ";
                        printKey("1");
                        cout << " Вытягивание жизни - наносит обычный АТК урон и дает небольшое лечение     " << endl << " ";
                        printKey("2");
                        cout << " Стазис - пропуск 3 ходов и сильное лечение на каждом ходу                 " << endl << " ";
                        printKey("Z");
                        cout << " Ускорение кровотока - аура, лечащая героя при обычной атаке и стазисе     " << endl;
                        setColor(LightGrey, Black);
                        break;
                }
            }
            else if (y == 13) menu = true;
        } while (!menu);
        myHero.perks = sel;
        switch(myHero.perks)    // HEROES
        {
            case 1:
                myHero.max_hp = 2640;
                myHero.mp = 40;
                myHero.damage = 80;
                myHero.armor = 30;
                myHero.ability = 50;
                break;
            case 2:
                myHero.max_hp = 1760;
                myHero.mp = 40;
                myHero.damage = 130;
                myHero.armor = 10;
                myHero.ability = 50;
                break;
            case 3:
                myHero.max_hp = 1980;
                myHero.mp = 40;
                myHero.damage = 60;
                myHero.armor = 20;
                myHero.ability = 100;
                break;
            case 4:
                myHero.max_hp = 2300;
                myHero.mp = 3;
                myHero.damage = myHero.max_hp/45;
                myHero.armor = myHero.max_hp/250;
                myHero.ability = myHero.max_hp/45;
                break;
        }
        myHero.hp = myHero.max_hp;
        myHero.max_mp = myHero.mp;
        myHero.charge = 0;
        myHero.weapon_damage = 0;
        myHero.weapon_ap = 0;
        myHero.weapon_type = 5;
        myHero.weapon_level = 0;
        myHero.weapon_id = -1;
        myHero.weapon_rarity = -1;
        myHero.wear_armor = 0;
        myHero.wear_ap = 0;
        myHero.wear_type = 5;
        myHero.wear_level = 0;
        myHero.wear_id = -1;
        myHero.wear_rarity = -1;
        myHero.amulet_id = -1;
        myHero.amulet_rarity = -1;
        myHero.ring_id = -1;
        myHero.ring_rarity = -1;
        myHero.level = 1;
        myHero.exp = 0;
        myHero.gold = 0;
        quality = 0;
        for (i = 0; i < items_all; i++) myHero.backpack[i] = 0;
        system("cls");  // DIFFICULTY
        cout << endl << "                                Выберите сложность" << endl << endl;
        cout << "===============================================================================" << endl << endl;
        cout << " Несведущий      (90%)" << endl;
        setColor(White, Black);
        cout << " Адепт";
        setColor(LightGrey, Black);
        cout << "           (100%)" << endl;
        cout << " Посвященный     (110%)" << endl;
        cout << " Магистр         (125%)" << endl << endl;
        cout << "===============================================================================" << endl << endl;
        cout << " Многие хотят не только не лениться, но и не переусердствовать";
        menu = false;
        sel = 2;
        do
        {
            y = getch();
            if ((y == 'w' && sel > 1) || (y == 's' && sel < 4))
            {
                switch(sel)
                {
                    case 1:
                        setCursorPosition(0, 5);
                        cout << " Несведущий";
                        break;
                    case 2:
                        setCursorPosition(0, 6);
                        cout << " Адепт";
                        break;
                    case 3:
                        setCursorPosition(0, 7);
                        cout << " Посвященный";
                        break;
                    case 4:
                        setCursorPosition(0, 8);
                        cout << " Магистр";
                        break;
                }
                if (y == 'w') sel--;
                if (y == 's') sel++;
                switch(sel)
                {
                    case 1:
                        setCursorPosition(0, 5);
                        setColor(White, Black);
                        cout << " Несведущий";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Каждый из нас делает свой первый шаг                                          ";
                        break;
                    case 2:
                        setCursorPosition(0, 6);
                        setColor(White, Black);
                        cout << " Адепт";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Многие хотят не только не лениться, но и не переусердствовать                 ";
                        break;
                    case 3:
                        setCursorPosition(0, 7);
                        setColor(White, Black);
                        cout << " Посвященный";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Для тех, кто готов внести чуть больший вклад в судьбу мира                    ";
                        break;
                    case 4:
                        setCursorPosition(0, 8);
                        setColor(White, Black);
                        cout << " Магистр";
                        setColor(LightGrey, Black);
                        setCursorPosition(0, 12);
                        cout << " Истинные герои готовы биться с кем угодно и сколько угодно                    ";
                        break;
                }
            }
            else if (y == 13) menu = true;
        } while (!menu);
        diff = sel;
        switch(diff)
        {
            case 1:
                diffC = 0.9;
                break;
            case 2:
                diffC = 1;
                break;
            case 3:
                diffC = 1.1;
                break;
            case 4:
                diffC = 1.25;
                break;
        }
        x_s = rand()%10;
        y_s = rand()%10;
        x_s = 2*x_s+1;
        y_s = 2*y_s+1;
        cnt = 0;
        system("cls");
        if (!music.openFromFile("music/story.ogg")) return 0;
        if (unmute) music.play();
        storyPrint(rand()%5, rand()%5, rand()%5, rand()%5);
        system("cls");
    }
    music.stop();   // LOAD SOUNDS
    switch(myHero.perks)
    {
        case 1:
            if (!b0.loadFromFile("music/sfx/10.ogg")) return 0;
            if (!b1.loadFromFile("music/sfx/11.ogg")) return 0;
            if (!b2.loadFromFile("music/sfx/12.ogg")) return 0;
            if (!b3.loadFromFile("music/sfx/13.ogg")) return 0;
            if (!b4.loadFromFile("music/sfx/14.ogg")) return 0;
            if (!b5.loadFromFile("music/sfx/15.ogg")) return 0;
            if (!b6.loadFromFile("music/sfx/16.ogg")) return 0;
            break;
        case 2:
            if (!b0.loadFromFile("music/sfx/20.ogg")) return 0;
            if (!b1.loadFromFile("music/sfx/21.ogg")) return 0;
            if (!b2.loadFromFile("music/sfx/22.ogg")) return 0;
            if (!b3.loadFromFile("music/sfx/23.ogg")) return 0;
            if (!b4.loadFromFile("music/sfx/24.ogg")) return 0;
            if (!b5.loadFromFile("music/sfx/25.ogg")) return 0;
            if (!b6.loadFromFile("music/sfx/26.ogg")) return 0;
            break;
        case 3:
            if (!b0.loadFromFile("music/sfx/30.ogg")) return 0;
            if (!b1.loadFromFile("music/sfx/31.ogg")) return 0;
            if (!b2.loadFromFile("music/sfx/32.ogg")) return 0;
            if (!b3.loadFromFile("music/sfx/33.ogg")) return 0;
            if (!b4.loadFromFile("music/sfx/34.ogg")) return 0;
            if (!b5.loadFromFile("music/sfx/35.ogg")) return 0;
            if (!b6.loadFromFile("music/sfx/36.ogg")) return 0;
            break;
        case 4:
            if (!b0.loadFromFile("music/sfx/40.ogg")) return 0;
            if (!b1.loadFromFile("music/sfx/41.ogg")) return 0;
            if (!b2.loadFromFile("music/sfx/42.ogg")) return 0;
            if (!b3.loadFromFile("music/sfx/43.ogg")) return 0;
            if (!b4.loadFromFile("music/sfx/44.ogg")) return 0;
            if (!b5.loadFromFile("music/sfx/45.ogg")) return 0;
            if (!b6.loadFromFile("music/sfx/46.ogg")) return 0;
            break;
    }
    a0.setBuffer(b0);
    a1.setBuffer(b1);
    a2.setBuffer(b2);
    a3.setBuffer(b3);
    a4.setBuffer(b4);
    a5.setBuffer(b5);
    a6.setBuffer(b6);
    music.setLoop(true);
	while (!ex && cnt < 100)
    {
        if (!save) cnt++;
        if (cnt == 100)
        {
            setCursorPosition(22, 13);
            bossLine(rand()%8);
            battle(2, 130, diffC);  // OVERLORD
            if (myHero.hp == 0) return 0;
        }
        else if (cnt%10 == 0)
        {
            setCursorPosition(22, 13);
            bossLine(rand()%8);
            battle(1, cnt+10, diffC);  // BOSS
            if (myHero.hp == 0) return 0;
        }
        else
        {
            if (!save) // MAZE GEN
            {
                for (i = 0; i < 21; i++)
                {
                    for (j = 0; j < 21; j++)
                    {
                        if (i%2 == 0 || j%2 == 0) maze[i][j] = '#';
                        else maze[i][j] = '.';
                    }
                }
                for (i = 0; i < 10; i++)
                {
                    for (j = 0; j < 10; j++)
                    {
                        was[i][j] = false;
                    }
                }
                gen(x_s/2, y_s/2);
                do
                {
                    x_f = rand()%10;
                    y_f = rand()%10;
                    x_f = 2*x_f+1;
                    y_f = 2*y_f+1;
                } while (abs(x_s-x_f) < 6 || abs(y_s-y_f) < 6);
                if (rand()%100 < 33)
                {
                    do
                    {
                        x_t = rand()%10;
                        y_t = rand()%10;
                        x_t = 2*x_t+1;
                        y_t = 2*y_t+1;
                    } while ((abs(x_s-x_t) < 6 || abs(y_s-y_t) < 6) || (x_t == x_f && y_t == y_f));
                }
                else
                {
                    x_t = 23;
                    y_t = 23;
                }
                i = 0;
                while (i < 8)
                {
                    a = rand()%10;
                    b = rand()%9;
                    a = 2*a+1;
                    b = 2*b+2;
                    if (maze[a][b] == '#')
                    {
                        maze[a][b] = '.';
                        i++;
                    }
                }
                i = 0;
                while (i < 8)
                {
                    a = rand()%9;
                    b = rand()%10;
                    a = 2*a+2;
                    b = 2*b+1;
                    if (maze[a][b] == '#')
                    {
                        maze[a][b] = '.';
                        i++;
                    }
                }
            }
            save = false;
            loadMaze:
            system("cls");
            if (!music.openFromFile("music/maze.ogg")) return 0;
            if (unmute) music.play();
            cout << "Темное Подземелье - " << cnt; // HELPING HUD
            switch(diff)
            {
                case 1:
                    cout << " (Несведущий)";
                    break;
                case 2:
                    cout << " (Адепт)";
                    break;
                case 3:
                    cout << " (Посвященный)";
                    break;
                case 4:
                    cout << " (Магистр)";
                    break;
            }
            if (debug)
            {
                setCursorPosition(38, 0);
                cout << "DEBUG";
            }
            setCursorPosition(69, 0);
            cout << "Beta 0.9.5" << endl;
            cout << "===============================================================================" << endl;
            for (i = 0; i < 21; i++)
            {
                for (j = 0; j < 21; j++)
                {
                    if (i == x_s && j == y_s)
                    {
                        setColor(White, Black);
                        cout << "X";
                        setColor(LightGrey, Black);
                    }
                    else if (i == x_f && j == y_f)
                    {
                        setColor(LightMagenta, Black);
                        cout << "0";
                        setColor(LightGrey, Black);
                    }
                    else if (i == x_t && j == y_t)
                    {
                        setColor(Yellow, Black);
                        cout << "T";
                        setColor(LightGrey, Black);
                    }
                    else
                    {
                        if (maze[i][j] == '#') setColor(LightGrey, LightGrey);
                        else setColor(Black, Black);
                        cout << maze[i][j];
                        setColor(LightGrey, Black);
                    }
                }
                cout << endl;
            }
            setCursorPosition(22, 4);
            cout << "=========================================================";
            setCursorPosition(22, 20);
            printKey("W");
            printKey("A");
            printKey("S");
            printKey("D");
            cout << " Перемещение     ";
            printKey("Z");
            cout << " Обзор персонажа";
            setCursorPosition(22, 21);
            printKey("B");
            cout << " Открыть/закрыть рюкзак   ";
            printKey("C");
            cout << " Создать предметы";
            setCursorPosition(22, 22);
            printKey("M");
            cout << " Включить/выключить звук  ";
            printKey("0");
            cout << " Сохранить и выйти";
            setCursorPosition(0, 23);
            cout << "===============================================================================";
            loadItem:
            if (myHero.weapon_id == 0)
            {
                myHero.weapon_hp = int((0.05+0.05*myHero.weapon_rarity)*myHero.max_hp);
                myHero.weapon_mp = 0;
            }
            else if (myHero.weapon_id == 6 && myHero.perks != 4)
            {
                myHero.weapon_hp = 0;
                myHero.weapon_mp = 5+5*myHero.weapon_rarity;
            }
            else
            {
                myHero.weapon_hp = 0;
                myHero.weapon_mp = 0;
            }
            if (myHero.wear_id == 0)
            {
                myHero.wear_hp = int((0.05+0.05*myHero.wear_rarity)*myHero.max_hp);
                myHero.wear_mp = 0;
            }
            else if (myHero.wear_id == 6 && myHero.perks != 4)
            {
                myHero.wear_hp = 0;
                myHero.wear_mp = 5+5*myHero.wear_rarity;
            }
            else
            {
                myHero.wear_hp = 0;
                myHero.wear_mp = 0;
            }
            if (myHero.amulet_id == 0)
            {
                myHero.amulet_hp = int((0.15+0.05*myHero.amulet_rarity)*myHero.max_hp);
                myHero.amulet_mp = 0;
            }
            else if (myHero.amulet_id == 6 && myHero.perks != 4)
            {
                myHero.amulet_hp = 0;
                myHero.amulet_mp = 10+10*myHero.amulet_rarity;
            }
            else
            {
                myHero.amulet_hp = 0;
                myHero.amulet_mp = 0;
            }
            if (myHero.ring_id == 0)
            {
                myHero.ring_hp = int((0.15+0.05*myHero.ring_rarity)*myHero.max_hp);
                myHero.ring_mp = 0;
            }
            else if (myHero.ring_id == 6 && myHero.perks != 4)
            {
                myHero.ring_hp = 0;
                myHero.ring_mp = 10+10*myHero.ring_rarity;
            }
            else
            {
                myHero.ring_hp = 0;
                myHero.ring_mp = 0;
            }
            if (myHero.perks == 4)
            {
                myHero.damage = (myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp)/45;
                myHero.armor = (myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp)/250;
                myHero.ability = (myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp)/45;
            }
            if (myHero.hp > myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp) myHero.hp = myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp;
            if (myHero.mp > myHero.max_mp+myHero.amulet_mp+myHero.ring_mp+myHero.weapon_mp+myHero.wear_mp) myHero.mp = myHero.max_mp+myHero.amulet_mp+myHero.ring_mp+myHero.weapon_mp+myHero.wear_mp;
            setCursorPosition(22, 3);
            switch(myHero.perks)
            {
                case 1:
                    cout << "Воин-берсерк ";
                    break;
                case 2:
                    cout << "Мастер Теней ";
                    break;
                case 3:
                    cout << "Верховный Маг ";
                    break;
                case 4:
                    cout << "Чародей Крови ";
                    break;
            }
            setColor(LightMagenta, Black);
            if (myHero.level == 100) cout << "MAX";
            else cout << myHero.level;
            setColor(LightGrey, Black);
            cout << " уровня";
            if (myHero.level < 100)
            {
                cout << " (";
                setColor(LightMagenta, Black);
                cout << exp_border[myHero.level-1]-myHero.exp;
                setColor(LightGrey, Black);
                cout << " ОП до след. уровня)  ";
            }
            setCursorPosition(22, 5);
            cout << "                                                         ";
            setCursorPosition(22, 6);
            cout << "                                                         ";
            setCursorPosition(22, 7);
            cout << "                                                         ";
            setCursorPosition(22, 8);
            cout << "                                                         ";
            setCursorPosition(22, 9);
            cout << "                                                         ";
            setCursorPosition(22, 10);
            cout << "                                                         ";
            setCursorPosition(22, 11);
            cout << "                                                         ";
            setCursorPosition(22, 12);
            cout << "                                                         ";
            setCursorPosition(22, 13);
            cout << "                                                         ";
            setCursorPosition(22, 14);
            cout << "                                                         ";
            setCursorPosition(22, 15);
            cout << "                                                         ";
            setCursorPosition(22, 16);
            cout << "                                                         ";
            setCursorPosition(22, 17);
            cout << "                                                         ";
            setCursorPosition(22, 18);
            cout << "                                                         ";
            setCursorPosition(22, 5);
            cout << "ОЗ:  ";
            if (myHero.hp*100/(myHero.max_hp+myHero.amulet_hp+myHero.ring_hp) >= 60) setColor(LightGreen, Black);
            else if (myHero.hp*100/(myHero.max_hp+myHero.amulet_hp+myHero.ring_hp) >= 30) setColor(Yellow, Black);
            else setColor (LightRed, Black);
            cout << myHero.hp;
            setColor(LightGrey, Black);
            cout << " / ";
            setColor(White, Black);
            cout << myHero.max_hp+myHero.amulet_hp+myHero.ring_hp+myHero.weapon_hp+myHero.wear_hp;
            setColor(LightGrey, Black);
            setCursorPosition(22, 6);
            if (myHero.perks == 4)
            {
                cout << "ЗК:  ";
                if (myHero.mp == myHero.max_mp) setColor(LightRed, Black);
                else setColor(Yellow, Black);
            }
            else
            {
                cout << "ОМ:  ";
                setColor(LightCyan, Black);
            }
            cout << myHero.mp;
            setColor(LightGrey, Black);
            cout << " / ";
            setColor(White, Black);
            cout << myHero.max_mp+myHero.amulet_mp+myHero.ring_mp+myHero.weapon_mp+myHero.wear_mp;
            setColor(LightGrey, Black);
            setCursorPosition(22, 7);
            cout << "АТК: ";
            setColor(White, Black);
            cout << myHero.damage+myHero.weapon_damage;
            setColor(LightGrey, Black);
            cout << " ";
            element(myHero.weapon_type);
            setCursorPosition(22, 8);
            cout << "ЗАЩ: ";
            setColor(White, Black);
            cout << myHero.armor+myHero.wear_armor;
            setColor(LightGrey, Black);
            cout << " ";
            element(myHero.wear_type);
            setCursorPosition(22, 9);
            cout << "МАГ: ";
            setColor(White, Black);
            cout << myHero.ability+myHero.weapon_ap+myHero.wear_ap;
            setColor(LightGrey, Black);
            setCursorPosition(22, 10);
            cout << "АМЛ: ";
            printEffect(myHero.amulet_id, myHero.amulet_rarity);
            setCursorPosition(22, 11);
            cout << "КЛЦ: ";
            printEffect(myHero.ring_id, myHero.ring_rarity);
            setCursorPosition(22, 12);
            setColor(Yellow, Black);
            cout << "Шау: ";
            setColor(White, Black);
            cout << myHero.gold;
            setColor(LightGrey, Black);
            loadBackpack:
            if (show_backpack)
            {
                setCursorPosition(22, 5);
                cout << "                                                         ";
                setCursorPosition(22, 6);
                cout << "                                                         ";
                setCursorPosition(22, 7);
                cout << "                                                         ";
                setCursorPosition(22, 8);
                cout << "                                                         ";
                setCursorPosition(22, 9);
                cout << "                                                         ";
                setCursorPosition(22, 10);
                cout << "                                                         ";
                setCursorPosition(22, 11);
                cout << "                                                         ";
                setCursorPosition(22, 12);
                cout << "                                                         ";
                setCursorPosition(22, 13);
                cout << "                                                         ";
                setCursorPosition(22, 14);
                cout << "                                                         ";
                setCursorPosition(22, 16);
                cout << "                                                         ";
                setCursorPosition(22, 17);
                cout << "                                                         ";
                setCursorPosition(22, 18);
                cout << "                                                         ";
                setCursorPosition(22, 5);
                cout << "Рюкзак:";
                item_cnt = 1;
                any_item = false;
                for (i = 0; i < items_battle; i++)
                {
                    setCursorPosition(22, 5+item_cnt);
                    if (myHero.backpack[i] > 0)
                    {
                        itemShow(i, 1);
                        any_item = true;
                        item_cnt++;
                    }
                }
                if (!any_item) cout << "<ПУСТО>";
                setCursorPosition(50, 5);
                cout << "Ресурсы:";
                item_cnt = 1;
                any_item = false;
                for (i = 0; i < 6; i++)
                {
                    setCursorPosition(50, 5+item_cnt);
                    if (myHero.backpack[i+13] > 0)
                    {
                        itemShow(i+13, 1);
                        any_item = true;
                        item_cnt++;
                    }
                }
                if (!any_item) cout << "<ПУСТО>";
            }
            setCursorPosition(0, 24);
            do
            {
                q = 'q';
                f = false;
                while (!f)
                {
                    q = getch();    // MAZE MOVE
                    if (q == 'w' && maze[x_s-1][y_s] != '#')
                    {
                        f = true;
                        setCursorPosition(y_s, x_s+2);
                        cout << " ";
                        x_s--;
                        setCursorPosition(y_s, x_s+2);
                        setColor(White, Black);
                        cout << "X";
                        setColor(LightGrey, Black);
                    }
                    else if (q == 's' && maze[x_s+1][y_s] != '#')
                    {
                        f = true;
                        setCursorPosition(y_s, x_s+2);
                        cout << " ";
                        x_s++;
                        setCursorPosition(y_s, x_s+2);
                        setColor(White, Black);
                        cout << "X";
                        setColor(LightGrey, Black);
                    }
                    else if (q == 'a' && maze[x_s][y_s-1] != '#')
                    {
                        f = true;
                        setCursorPosition(y_s, x_s+2);
                        cout << " ";
                        y_s--;
                        setCursorPosition(y_s, x_s+2);
                        setColor(White, Black);
                        cout << "X";
                        setColor(LightGrey, Black);
                    }
                    else if (q == 'd' && maze[x_s][y_s+1] != '#')
                    {
                        f = true;
                        setCursorPosition(y_s, x_s+2);
                        cout << " ";
                        y_s++;
                        setCursorPosition(y_s, x_s+2);
                        setColor(White, Black);
                        cout << "X";
                        setColor(LightGrey, Black);
                    }
                    else if (q == '0')  // QUIT & SAVE
                    {
                        f = true;
                        ex = true;
                        fout.open("save.txt");
                        fout << myHero.perks << " " << music_volume << " " << sound_volume << " ";
                        fout << unmute << " ";
                        for (i = 1; i < 20; i++)
                        {
                            out_ar = 0;
                            for (j = 0; j < 21; j++)
                            {
                                if (maze[i][j] == '#') out_ar += pow(2, 20-j);
                            }
                            fout << out_ar << " ";
                            data[i+3] = out_ar;
                        }
                        fout << diff << " " << cnt << " " << x_s << " " << y_s << " " << x_f << " " << y_f << " " << x_t << " " << y_t << " " << myHero.hp << " " << myHero.mp << " " << myHero.charge << " " << myHero.weapon_damage << " " << myHero.weapon_ap << " " << myHero.weapon_type << " " << myHero.weapon_level << " " << myHero.weapon_id << " " << myHero.weapon_rarity << " " << myHero.wear_armor << " " << myHero.wear_ap << " " << myHero.wear_type << " " << myHero.wear_level << " " << myHero.wear_id << " " << myHero.wear_rarity << " " << myHero.amulet_id << " " << myHero.amulet_rarity << " " << myHero.ring_id << " " << myHero.ring_rarity << " " << myHero.level << " " << myHero.exp << " " << myHero.gold << " " << quality << " ";
                        for (i = 0; i < items_all; i++) fout << myHero.backpack[i] << " ";
                        data[0] = myHero.perks;
                        data[1] = music_volume;
                        data[2] = sound_volume;
                        data[3] = unmute;
                        data[23] = diff;
                        data[24] = cnt;
                        data[25] = x_s;
                        data[26] = y_s;
                        data[27] = x_f;
                        data[28] = y_f;
                        data[29] = x_t;
                        data[30] = y_t;
                        data[31] = myHero.hp;
                        data[32] = myHero.mp;
                        data[33] = myHero.charge;
                        data[34] = myHero.weapon_damage;
                        data[35] = myHero.weapon_ap;
                        data[36] = myHero.weapon_type;
                        data[37] = myHero.weapon_level;
                        data[38] = myHero.weapon_id;
                        data[39] = myHero.weapon_rarity;
                        data[40] = myHero.wear_armor;
                        data[41] = myHero.wear_ap;
                        data[42] = myHero.wear_type;
                        data[43] = myHero.wear_level;
                        data[44] = myHero.wear_id;
                        data[45] = myHero.wear_rarity;
                        data[46] = myHero.amulet_id;
                        data[47] = myHero.amulet_rarity;
                        data[48] = myHero.ring_id;
                        data[49] = myHero.ring_rarity;
                        data[50] = myHero.level;
                        data[51] = myHero.exp;
                        data[52] = myHero.gold;
                        data[53] = quality;
                        for (i = 0; i < items_all; i++)
                        {
                            fin >> myHero.backpack[i];
                            data[54+i] = myHero.backpack[i];
                        }
                        checksumN = hash_33();
                        fout << checksumN;
                        fout.close();
                    }
                    else if (q == 'b')  // SHOW OR HIDE BACKPACK
                    {
                        show_backpack = !show_backpack;
                        if (show_backpack) goto loadBackpack;
                        else goto loadItem;
                    }
                    else if (q == 'c')  // CRAFTING MENU
                    {
                        showCrafting:
                        system("cls");
                        if (!music.openFromFile("music/craft.ogg")) return 0;
                        if (unmute) music.play();
                        cout << endl;
                        cout << "                                       ___ " << endl;
                        cout << "                                    . |   | ," << endl;
                        cout << "                                 EEEEEX   |  ." << endl;
                        cout << "                                   ,__|___|___" << endl;
                        cout << "                                   / ---   -- |" << endl;
                        cout << "                                  /_____()____|" << endl;
                        cout << "                                     |  /\\ |   " << endl;
                        cout << "                                   __|_____|__" << endl;
                        cout << "                                  |           |" << endl;
                        cout << "===============================================================================" << endl << endl << " ";
                        printKey("Q");
                        cout << " Оружие      (3 ";
                        itemShow(18, 0);
                        cout << " + 5 ";
                        itemShow(16, 0);
                        cout << " + 1 ";
                        itemShow(15, 0);
                        cout << ")" << endl << " ";
                        printKey("W");
                        cout << " Броня       (4 ";
                        itemShow(17, 0);
                        cout << " + 4 ";
                        itemShow(16, 0);
                        cout << " + 1 ";
                        itemShow(15, 0);
                        cout << ")" << endl << " ";
                        printKey("E");
                        cout << " Амулет      (3 ";
                        itemShow(18, 0);
                        cout << " + 1 ";
                        itemShow(16, 0);
                        cout << " + 2 ";
                        itemShow(14, 0);
                        cout << ")" << endl << " ";
                        printKey("R");
                        cout << " Кольцо      (2 ";
                        itemShow(16, 0);
                        cout << " + 1 ";
                        itemShow(15, 0);
                        cout << " + 2 ";
                        itemShow(14, 0);
                        cout << ")" << endl << endl << "     ";
                        setColor(LightMagenta, Black);
                        cout << "Сверхпредметы (+30 качества на старте)" << endl << " ";
                        setColor(LightGrey, Black);
                        printKey("A");
                        setColor(LightMagenta, Black);
                        cout << " Сверх";
                        setColor(LightGrey, Black);
                        cout << "оружие (3 ";
                        itemShow(18, 0);
                        cout << " + 5 ";
                        itemShow(16, 0);
                        cout << " + 1 ";
                        itemShow(14, 0);
                        cout << " + 3 ";
                        itemShow(15, 0);
                        cout << " + 1 ";
                        itemShow(13, 0);
                        cout << ")" << endl << " ";
                        printKey("S");
                        setColor(LightMagenta, Black);
                        cout << " Сверх";
                        setColor(LightGrey, Black);
                        cout << "броня  (4 ";
                        itemShow(17, 0);
                        cout << " + 4 ";
                        itemShow(16, 0);
                        cout << " + 2 ";
                        itemShow(14, 0);
                        cout << " + 2 ";
                        itemShow(15, 0);
                        cout << " + 1 ";
                        itemShow(13, 0);
                        cout << ")" << endl << " ";
                        printKey("D");
                        setColor(LightMagenta, Black);
                        cout << " Сверх";
                        setColor(LightGrey, Black);
                        cout << "амулет (6 ";
                        itemShow(16, 0);
                        cout << " + 2 ";
                        itemShow(15, 0);
                        cout << " + 1 ";
                        itemShow(13, 0);
                        cout << ")" << endl << " ";
                        printKey("F");
                        setColor(LightMagenta, Black);
                        cout << " Сверх";
                        setColor(LightGrey, Black);
                        cout << "кольцо (4 ";
                        itemShow(16, 0);
                        cout << " + 2 ";
                        itemShow(15, 0);
                        cout << " + 1 ";
                        itemShow(14, 0);
                        cout << " + 1 ";
                        itemShow(13, 0);
                        cout << ")" << endl << endl << " ";
                        printKey("0");
                        cout << " Вернуться на карту" << endl;
                        do  // CRAFTING CHOICE
                        {
                            q = getch();
                            switch(q)
                            {
                                case 'q':
                                    if (myHero.backpack[15] >= 1 && myHero.backpack[16] >= 5 && myHero.backpack[18] >= 3)
                                    {
                                        myHero.backpack[15]--;
                                        myHero.backpack[16] -= 5;
                                        myHero.backpack[18] -= 3;
                                        craftingProcess(0, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'w':
                                    if (myHero.backpack[15] >= 1 && myHero.backpack[16] >= 4 && myHero.backpack[17] >= 4)
                                    {
                                        myHero.backpack[15]--;
                                        myHero.backpack[16] -= 4;
                                        myHero.backpack[17] -= 4;
                                        craftingProcess(1, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'e':
                                    if (myHero.backpack[14] >= 2 && myHero.backpack[16] >= 1 && myHero.backpack[18] >= 3)
                                    {
                                        myHero.backpack[14] -= 2;
                                        myHero.backpack[16]--;
                                        myHero.backpack[18] -= 3;
                                        craftingProcess(2, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'r':
                                    if (myHero.backpack[14] >= 2 && myHero.backpack[15] >= 1 && myHero.backpack[16] >= 2)
                                    {
                                        myHero.backpack[14] -= 2;
                                        myHero.backpack[15]--;
                                        myHero.backpack[16] -= 2;
                                        craftingProcess(3, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'a':
                                    if (myHero.backpack[13] >= 1 && myHero.backpack[14] >= 3 && myHero.backpack[15] >= 3 && myHero.backpack[16] >= 5 && myHero.backpack[18] >= 3)
                                    {
                                        myHero.backpack[13]--;
                                        myHero.backpack[14] -= 3;
                                        myHero.backpack[15] -= 3;
                                        myHero.backpack[16] -= 5;
                                        myHero.backpack[18] -= 3;
                                        craftingProcess(4, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 's':
                                    if (myHero.backpack[13] >= 1 && myHero.backpack[14] >= 2 && myHero.backpack[15] >= 2 && myHero.backpack[16] >= 4 && myHero.backpack[17] >= 4)
                                    {
                                        myHero.backpack[13]--;
                                        myHero.backpack[14] -= 2;
                                        myHero.backpack[15] -= 2;
                                        myHero.backpack[16] -= 4;
                                        myHero.backpack[16] -= 4;
                                        craftingProcess(5, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'd':
                                    if (myHero.backpack[13] >= 1 && myHero.backpack[15] >= 2 && myHero.backpack[16] >= 6)
                                    {
                                        myHero.backpack[13]--;
                                        myHero.backpack[15] -= 2;
                                        myHero.backpack[16] -= 6;
                                        craftingProcess(6, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                                case 'f':
                                    if (myHero.backpack[13] >= 1 && myHero.backpack[14] >= 1 && myHero.backpack[15] >= 2 && myHero.backpack[16] >= 4)
                                    {
                                        myHero.backpack[13]--;
                                        myHero.backpack[14]--;
                                        myHero.backpack[15] -= 2;
                                        myHero.backpack[16] -= 4;
                                        craftingProcess(7, quality, cnt);
                                        goto showCrafting;
                                    }
                                    else if (unmute) no.play();
                                    break;
                            }
                        } while (q != '0');
                        goto loadMaze;
                    }
                    else if (q == 'm')  // MUTE & UNMUTE MUSIC
                    {
                        unmute = !unmute;
                        if (!music.openFromFile("music/maze.ogg")) return 0;
                        if (unmute) music.play();
                        else music.stop();
                    }
                    else if (q == 'z')
                    {
                        system("cls");
                        setCursorPosition(0, 1);
                        cout << "======== Герой ================================================================" << endl << endl;
                        switch(myHero.perks)
                        {
                            case 1:
                                cout << " Воин-берсерк ";
                                break;
                            case 2:
                                cout << " Мастер Теней ";
                                break;
                            case 3:
                                cout << " Верховный Маг ";
                                break;
                        }
                        setColor(LightMagenta, Black);
                        cout << myHero.level;
                        setColor(LightGrey, Black);
                        cout << " уровня" << endl << " Очки опыта: ";
                        setColor(LightMagenta, Black);
                        cout << myHero.exp << endl << endl;
                        setColor(LightGrey, Black);
                        cout << "======== Характеристики =======================================================" << endl << endl << " Атака:  ";
                        setColor(White, Black);
                        cout << myHero.damage;
                        setColor(LightGrey, Black);
                        cout << " +";
                        setColor(White, Black);
                        cout << myHero.weapon_damage;
                        setColor(LightGrey, Black);
                        cout << " от оружия ";
                        setColor(White, Black);
                        cout << myHero.weapon_level;
                        setColor(LightGrey, Black);
                        cout << " уровня ";
                        element(myHero.weapon_type);
                        cout << endl << " Защита: ";
                        setColor(White, Black);
                        cout << myHero.armor;
                        setColor(LightGrey, Black);
                        cout << " +";
                        setColor(White, Black);
                        cout << myHero.wear_armor;
                        setColor(LightGrey, Black);
                        cout << " от брони ";
                        setColor(White, Black);
                        cout << myHero.wear_level;
                        setColor(LightGrey, Black);
                        cout << " уровня ";
                        element(myHero.wear_type);
                        cout << endl << " Магика: ";
                        setColor(White, Black);
                        cout << myHero.ability;
                        setColor(LightGrey, Black);
                        cout << " +";
                        setColor(White, Black);
                        cout << myHero.weapon_ap;
                        setColor(LightGrey, Black);
                        cout << " от оружия +";
                        setColor(White, Black);
                        cout << myHero.wear_ap;
                        setColor(LightGrey, Black);
                        cout << " от брони" << endl << endl << "======== Эффекты ==============================================================" << endl << endl << " Оружие: ";
                        if (myHero.weapon_id != -1) printEffectFull(myHero.weapon_id, myHero.weapon_rarity, 1);
                        else cout << "НЕТ";
                        cout << endl << " Броня:  ";
                        if (myHero.wear_id != -1) printEffectFull(myHero.wear_id, myHero.wear_rarity, 1);
                        else cout << "НЕТ";
                        cout << endl << " Амулет: ";
                        if (myHero.amulet_id != -1) printEffectFull(myHero.amulet_id, myHero.amulet_rarity, 0);
                        else cout << "НЕТ";
                        cout << endl << " Кольцо: ";
                        if (myHero.ring_id != -1) printEffectFull(myHero.ring_id, myHero.ring_rarity, 0);
                        else cout << "НЕТ";
                        setCursorPosition(1, 23);
                        system("pause");
                        goto loadMaze;
                    }
                    setCursorPosition(22, 14);
                    cout << "                                        ";
                    setCursorPosition(0, 24);
                }
                if (ex) break;
                else if (x_s == x_t && y_s == y_t)
                {
                    system("cls");
                    if (!music.openFromFile("music/shop.ogg")) return 0;
                    if (unmute) music.play();
                    x_t = 23;
                    y_t = 23;
                    item1 = rand()%13;
                    if (item1 < 2) price1 = 0.05*cnt*cnt+7*cnt+6;
                    else if (item1 < 6) price1 = 0.7*(0.05*cnt*cnt+7*cnt)+5;
                    else price1 = 0.3*(0.05*cnt*cnt+7*cnt)+4;
                    do item2 = rand()%13; while (item1 == item2);
                    if (item2 < 2) price2 = 0.05*cnt*cnt+7*cnt+6;
                    else if (item2 < 6) price2 = 0.7*(0.05*cnt*cnt+7*cnt)+5;
                    else price2 = 0.3*(0.05*cnt*cnt+7*cnt)+4;
                    imb = rand()%2;
                    verse = rand()%3;
                    if (verse == 0) shop_s = "Здравствуй, странник! Что желаешь приобрести?";
                    else if (verse == 1) shop_s = "Здравствуй, странник! Любой каприз за твои Шау!";
                    else shop_s = "Здравствуй, странник! Я продаю разные безделушки со всех концов Эзреаля!";
                    setCursorPosition(40-shop_s.size()/2, 1);
                    cout << shop_s << endl;
                    cout << "                                     _____" << endl;
                    cout << "                                    /____ \\\\_" << endl;
                    cout << "                                   //    \\ \\\\\\" << endl;
                    cout << "                                  //      \\ \\\\\\" << endl;
                    cout << "                                  \\\\      / /'|" << endl;
                    cout << "                                   \\\\____/ / '\\" << endl;
                    cout << "                                ____/     \\____|" << endl;
                    cout << "                               /    \\_   _/    \\" << endl;
                    cout << "                              /___|   \\_/   |___\\" << endl;
                    cout << "                             /,,,,|   ~0~   |,,,,\\" << endl;
                    cout << "===============================================================================" << endl;
                    cout << endl << " [1] ";
                    itemShow(item1, 0);
                    cout << " (цена: ";
                    setColor(Yellow, Black);
                    cout << price1;
                    setColor(LightGrey, Black);
                    cout << ")" << endl << "     ";
                    itemShowDescription(item1);
                    cout << endl << endl << " [2] ";
                    itemShow(item2, 0);
                    cout << " (цена: ";
                    setColor(Yellow, Black);
                    cout << price2;
                    setColor(LightGrey, Black);
                    cout << ")" << endl << "     ";
                    itemShowDescription(item2);
                    cout << endl << endl;
                    if (imb == 0 && myHero.weapon_level < 3)
                    {
                        price_imb = myHero.weapon_damage/5+myHero.weapon_ap/5+1;
                        cout << " [3] Усилить оружие (цена: ";
                        setColor(Yellow, Black);
                        cout << price_imb;
                        setColor(LightGrey, Black);
                        cout << ")" << endl << "     +";
                        setColor(White, Black);
                        cout << int(0.2*myHero.weapon_damage);
                        setColor(LightGrey, Black);
                        cout << " АТК / +";
                        setColor(White, Black);
                        cout << int(0.2*myHero.weapon_ap);
                        setColor(LightGrey, Black);
                        cout << " МАГ / +";
                        setColor(White, Black);
                        cout << "7";
                        setColor(LightGrey, Black);
                        cout << "% шанс крита и +";
                        setColor(White, Black);
                        cout << "5";
                        setColor(LightGrey, Black);
                        cout << "% элем. урона" << endl;
                    }
                    else if (imb == 1 && myHero.wear_level < 3)
                    {
                        price_imb = myHero.wear_armor/5+myHero.wear_ap/5+1;
                        cout << " [3] Усилить доспехи (цена: ";
                        setColor(Yellow, Black);
                        cout << price_imb;
                        setColor(LightGrey, Black);
                        cout << ")" << endl << "     +";
                        setColor(White, Black);
                        cout << int(0.2*myHero.wear_armor);
                        setColor(LightGrey, Black);
                        cout << " ЗАЩ / +";
                        setColor(White, Black);
                        cout << int(0.2*myHero.wear_ap);
                        setColor(LightGrey, Black);
                        cout << " МАГ / +";
                        setColor(White, Black);
                        cout << "7";
                        setColor(LightGrey, Black);
                        cout << "% шанс блока и +";
                        setColor(White, Black);
                        cout << "5";
                        setColor(LightGrey, Black);
                        cout << "% элем. защиты" << endl;
                    }
                    setCursorPosition(0, 23);
                    cout << "===============================================================================";
                    setColor(White, Black);
                    shop = true;
                    buy1 = true;
                    buy2 = true;
                    do // TWO ITEMS AND IMBUE WEAPON/WEAR
                    {
                        gold_l = 1;
                        while (myHero.gold >= pow(10, gold_l)) gold_l++;
                        setCursorPosition(69-gold_l, 20);
                        setColor(White, Black);
                        cout << "     Шау: ";
                        setColor(Yellow, Black);
                        cout << myHero.gold << endl << endl << endl;
                        setColor(LightGrey, Black);
                        setCursorPosition(70, 21);
                        cout << "[0] Выйти";
                        setCursorPosition(1, 22);
                        q = getch();
                        if (q == '0') shop = false; // EXIT
                        if (q == '1' && buy1 && myHero.gold >= price1)  // ITEM 1
                        {
                            buy1 = false;
                            myHero.gold -= price1;
                            setCursorPosition(5, 14);
                            cout << "----------                                     " << endl;
                            cout << "                                                                               ";
                            if (item1 >= 13) myHero.backpack[item1] += 5;
                            else myHero.backpack[item1]++;
                            if (item1 == 10) myHero.damage += 10;
                            else if (item1 == 11) myHero.armor += 10;
                            else if (item1 == 12) myHero.ability += 10;
                        }
                        if (q == '2' && buy2 && myHero.gold >= price2)  // ITEM 2
                        {
                            buy2 = false;
                            myHero.gold -= price2;
                            setCursorPosition(5, 17);
                            cout << "----------                                     " << endl;
                            cout << "                                                                               ";
                            if (item2 >= 13) myHero.backpack[item2] += 5;
                            else myHero.backpack[item2]++;
                            if (item2 == 10) myHero.damage += 10;
                            else if (item2 == 11) myHero.armor += 10;
                            else if (item2 == 12) myHero.ability += 10;
                        }
                        if (q == '3' && myHero.gold >= price_imb && ((imb == 0 && myHero.weapon_level < 3) || (imb == 1 && myHero.wear_level < 3))) // IMBUE
                        {
                            if (imb == 0)
                            {
                                myHero.gold -= price_imb;
                                myHero.weapon_damage += 0.2*myHero.weapon_damage;
                                myHero.weapon_level++;
                                price_imb = myHero.weapon_damage/5+myHero.weapon_ap/5+1;
                                if (myHero.weapon_level == 3)
                                {
                                    setCursorPosition(5, 20);
                                    cout << "----------                                        " << endl;
                                    cout << "                                                                  ";
                                }
                                else
                                {
                                    setCursorPosition(27, 20);
                                    setColor(Yellow, Black);
                                    cout << price_imb;
                                    setColor(LightGrey, Black);
                                    cout << ")" << endl << "     +";
                                    setColor(White, Black);
                                    cout << int(0.2*myHero.weapon_damage);
                                    setColor(LightGrey, Black);
                                    cout << " АТК / +";
                                    setColor(White, Black);
                                    cout << int(0.2*myHero.weapon_ap);
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / +";
                                    setColor(White, Black);
                                    cout << "5";
                                    setColor(LightGrey, Black);
                                    cout << "% элементального урона" << endl;
                                }
                            }
                            if (imb == 1)
                            {
                                myHero.gold -= price_imb;
                                myHero.wear_armor += 0.2*myHero.wear_armor;
                                myHero.wear_level++;
                                price_imb = myHero.wear_armor/5+myHero.wear_ap/5+1;
                                if (myHero.wear_level == 3)
                                {
                                    setCursorPosition(5, 20);
                                    cout << "----------                                        " << endl;
                                    cout << "                                                                  ";
                                }
                                else
                                {
                                    setCursorPosition(28, 20);
                                    setColor(Yellow, Black);
                                    cout << price_imb;
                                    setColor(LightGrey, Black);
                                    cout << ")" << endl << "     +";
                                    setColor(White, Black);
                                    cout << int(0.2*myHero.wear_armor);
                                    setColor(LightGrey, Black);
                                    cout << " ЗАЩ / +";
                                    setColor(White, Black);
                                    cout << int(0.2*myHero.wear_ap);
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / +";
                                    setColor(White, Black);
                                    cout << "5";
                                    setColor(LightGrey, Black);
                                    cout << "% элементальной защиты" << endl;
                                }
                            }
                        }
                    } while (shop);
                    goto loadMaze;
                }
                else if (x_s != x_f || y_s != y_f)
                {
                    sc = rand()%100;
                    if (sc < 10)    // START BATTLE
                    {
                        battle(0, min(100, max(1, cnt-2+rand()%5)), diffC);
                        if (myHero.hp > 0) goto loadMaze;
                        else goto mainMenu;
                    }
                    if (sc >= 90)   // ITEM FOUND
                    {
                        item_find = rand()%47;
                        if (item_find < 12) item_find = 0;   // FIND A WEAPON
                        else if (item_find < 24) item_find = 1; // FIND ARMOR
                        else if (item_find < 28) item_find = 2; // FIND AN AMULET
                        else if (item_find < 32) item_find = 3; // FIND A RING
                        else if (item_find != 46) item_find = 4;    // FIND RESOURCES
                        else item_find = 5; // FIND A USABLE ITEM
                        switch(item_find)
                        {
                            case 0:
                                weapon_damage = (cnt*4+rand()%(cnt*2))+rand()%2;
                                weapon_ap = (cnt*2+rand()%cnt)+rand()%2;
                                weapon_type = rand()%6;
                                weapon_level = rand()%6;
                                if (weapon_level == 5) weapon_level = 2;
                                else if (weapon_level >= 3) weapon_level = 1;
                                else weapon_level = 0;
                                weapon_id = -1;
                                weapon_rarity = -1;
                                break;
                            case 1:
                                wear_armor = (cnt*4+rand()%(cnt*2))+rand()%2;
                                wear_ap = (cnt*2+rand()%cnt)+rand()%2;
                                wear_type = rand()%6;
                                wear_level = rand()%6;
                                if (wear_level == 5) wear_level = 2;
                                else if (wear_level >= 3) wear_level = 1;
                                else wear_level = 0;
                                wear_id = -1;
                                wear_rarity = -1;
                                break;
                            case 2:
                                amulet_id = rand()%9;
                                amulet_rarity = rand()%100;
                                if (amulet_rarity >= 96-(cnt/10)) amulet_rarity = 2;
                                else if (amulet_rarity >= 92-(cnt/4)) amulet_rarity = 1;
                                else amulet_rarity = 0;
                                break;
                            case 3:
                                ring_id = rand()%9;
                                ring_rarity = rand()%100;
                                if (ring_rarity >= 96-(cnt/10)) ring_rarity = 2;
                                else if (ring_rarity >= 92-(cnt/4)) ring_rarity = 1;
                                else ring_rarity = 0;
                                break;
                            case 4:
                                res_id = rand()%14;
                                if (res_id == 0) res_id = 0;
                                else if (res_id < 3) res_id = 1;
                                else if (res_id < 5) res_id = 2;
                                else if (res_id < 8) res_id = 3;
                                else if (res_id < 11) res_id = 4;
                                else res_id = 5;
                                myHero.backpack[13+res_id]++;
                                if (!show_backpack)
                                {
                                    setCursorPosition(22, 14);
                                    cout << "Найден ресурс ";
                                    itemShow(13+res_id, 0);
                                }
                                break;
                            case 5:
                                item_find_use = rand()%items_battle;
                                myHero.backpack[item_find_use]++;
                                if (!show_backpack)
                                {
                                    setCursorPosition(22, 14);
                                    cout << "Найден предмет ";
                                    itemShow(item_find_use, 0);
                                }
                                if (show_backpack) goto loadBackpack;
                                break;
                        }
                        if (item_find < 4)
                        {
                            setCursorPosition(22, 3);
                            cout << "                                                      ";
                            setCursorPosition(22, 5);
                            cout << "                                                      ";
                            setCursorPosition(22, 6);
                            cout << "                                                      ";
                            setCursorPosition(22, 7);
                            cout << "                                                      ";
                            setCursorPosition(22, 8);
                            cout << "                                                      ";
                            setCursorPosition(22, 9);
                            cout << "                                                      ";
                            setCursorPosition(22, 10);
                            cout << "                                                      ";
                            setCursorPosition(22, 11);
                            cout << "                                                      ";
                            setCursorPosition(22, 3);
                            cout << "Вы нашли нов";
                            switch(item_find)
                            {
                                case 0:
                                    cout << "ое оружие";
                                    setCursorPosition(22, 5);
                                    cout << "Ваше оружие:  +";
                                    setColor(White, Black);
                                    cout << myHero.weapon_damage;
                                    setColor(LightGrey, Black);
                                    cout << " АТК / +";
                                    setColor(White, Black);
                                    cout << myHero.weapon_ap;
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / ";
                                    setColor(White, Black);
                                    cout << myHero.weapon_level;
                                    setColor(LightGrey, Black);
                                    cout << " уровень ";
                                    element(myHero.weapon_type);
                                    setCursorPosition(36, 6);
                                    printEffectFull(myHero.weapon_id, myHero.weapon_rarity, 1);
                                    setCursorPosition(22, 7);
                                    cout << "Новое оружие: +";
                                    if (weapon_damage > myHero.weapon_damage) setColor(LightGreen, Black);
                                    else if (weapon_damage == myHero.weapon_damage) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << weapon_damage;
                                    setColor(LightGrey, Black);
                                    cout << " АТК / +";
                                    if (weapon_ap > myHero.weapon_ap) setColor(LightGreen, Black);
                                    else if (weapon_ap == myHero.weapon_ap) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << weapon_ap;
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / ";
                                    if (weapon_level > myHero.weapon_level) setColor(LightGreen, Black);
                                    else if (weapon_level == myHero.weapon_level) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << weapon_level;
                                    setColor(LightGrey, Black);
                                    cout << " уровень ";
                                    element(weapon_type);
                                    setCursorPosition(36, 8);
                                    printEffectFull(weapon_id, weapon_rarity, 1);
                                    break;
                                case 1:
                                    cout << "ую броню";
                                    setCursorPosition(22, 5);
                                    cout << "Ваша броня:  +";
                                    setColor(White, Black);
                                    cout << myHero.wear_armor;
                                    setColor(LightGrey, Black);
                                    cout << " ЗАЩ / +";
                                    setColor(White, Black);
                                    cout << myHero.wear_ap;
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / ";
                                    setColor(White, Black);
                                    cout << myHero.wear_level;
                                    setColor(LightGrey, Black);
                                    cout << " уровень ";
                                    element(myHero.wear_type);
                                    setCursorPosition(36, 6);
                                    printEffectFull(myHero.wear_id, myHero.wear_rarity, 1);
                                    setCursorPosition(22, 7);
                                    cout << "Новая броня: +";
                                    if (wear_armor > myHero.wear_armor) setColor(LightGreen, Black);
                                    else if (wear_armor == myHero.wear_armor) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << wear_armor;
                                    setColor(LightGrey, Black);
                                    cout << " ЗАЩ / +";
                                    if (wear_ap > myHero.wear_ap) setColor(LightGreen, Black);
                                    else if (wear_ap == myHero.wear_ap) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << wear_ap;
                                    setColor(LightGrey, Black);
                                    cout << " МАГ / ";
                                    if (wear_level > myHero.wear_level) setColor(LightGreen, Black);
                                    else if (wear_level == myHero.wear_level) setColor(White, Black);
                                    else setColor(LightRed, Black);
                                    cout << wear_level;
                                    setColor(LightGrey, Black);
                                    cout << " уровень ";
                                    element(wear_type);
                                    setCursorPosition(36, 8);
                                    printEffectFull(myHero.wear_id, myHero.wear_rarity, 1);
                                    break;
                                case 2:
                                    cout << "ый амулет";
                                    setCursorPosition(22, 5);
                                    cout << "Ваш амулет:   ";
                                    printEffect(myHero.amulet_id, myHero.amulet_rarity);
                                    if (myHero.amulet_id != -1)
                                    {
                                        setCursorPosition(36, 6);
                                        printEffectFull(myHero.amulet_id, myHero.amulet_rarity, 0);
                                    }
                                    setCursorPosition(22, 7);
                                    cout << "Новый амулет: ";
                                    printEffect(amulet_id, amulet_rarity);
                                    setCursorPosition(36, 8);
                                    printEffectFull(amulet_id, amulet_rarity, 0);
                                    break;
                                case 3:
                                    cout << "ое кольцо";
                                    setCursorPosition(22, 5);
                                    cout << "Ваше кольцо:  ";
                                    printEffect(myHero.ring_id, myHero.ring_rarity);
                                    if (myHero.ring_id != -1)
                                    {
                                        setCursorPosition(36, 6);
                                        printEffectFull(myHero.ring_id, myHero.ring_rarity, 0);
                                    }
                                    setCursorPosition(22, 7);
                                    cout << "Новое кольцо: ";
                                    printEffect(ring_id, ring_rarity);
                                    setCursorPosition(36, 8);
                                    printEffectFull(ring_id, ring_rarity, 0);
                                    break;
                            }
                            setCursorPosition(22, 9);
                            cout << "=========================================================";
                            setCursorPosition(22, 10);
                            cout << ">>> Надеть или выкинуть?                                 ";
                            setCursorPosition(22, 11);
                            printKey("E");
                            cout << " Надеть                                                ";
                            setCursorPosition(22, 12);
                            printKey("Q");
                            cout << " Выкинуть                                              ";
                            setCursorPosition(22, 13);
                            do
                            {
                                ch_wear = getch();
                            } while (ch_wear != 'e' && ch_wear != 'q');
                            if (ch_wear == 'e')
                            {
                                switch(item_find)
                                {
                                    case 0:
                                        myHero.weapon_damage = weapon_damage;
                                        myHero.weapon_ap = weapon_ap;
                                        myHero.weapon_type = weapon_type;
                                        myHero.weapon_level = weapon_level;
                                        myHero.weapon_id = weapon_id;
                                        myHero.weapon_rarity = weapon_rarity;
                                        break;
                                    case 1:
                                        myHero.wear_armor = wear_armor;
                                        myHero.wear_ap = wear_ap;
                                        myHero.wear_type = wear_type;
                                        myHero.wear_level = wear_level;
                                        myHero.wear_id = wear_id;
                                        myHero.wear_rarity = wear_rarity;
                                        break;
                                    case 2:
                                        myHero.amulet_id = amulet_id;
                                        myHero.amulet_rarity = amulet_rarity;
                                        break;
                                    case 3:
                                        myHero.ring_id = ring_id;
                                        myHero.ring_rarity = ring_rarity;
                                        break;
                                }
                            }
                            goto loadItem;
                        }
                    }
                }
            } while (x_s != x_f || y_s != y_f);
            music.stop();
            if (!ex && unmute) nx.play();
            cout << "Для продолжения нажмите любую клавишу...";
            _getch();
            nx.stop();
        }
    }
    if (!ex)
    {
        system("cls");
        music.setLoop(false);
        if (!music.openFromFile("music/gameend.ogg")) return 0;
        if (unmute) music.play();
        endText();
        system("cls");
        credits(true);
    }
	return 0;
}
