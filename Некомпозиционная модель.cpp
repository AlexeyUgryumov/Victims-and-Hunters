// для работы необходимо подключение библиотеки SFML

#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

using namespace std;
using namespace sf;

bool is_allowed(int i, int j)
{
    if (i < 0 || j < 0 || i > 99 || j > 99) return false;
    return true;
}

//Описание структуры 
struct entity
{
    int id;
    int x;
    int y;
    int age;
    int max_age;
    int hunger;
    int max_hunger;
    int mult;
    int mult_t;
    int vision;
    bool turn;
    // конструкторы
    entity()
    {
        this->x = 0;
        this->y = 0;
        this->id = 0;
        this->hunger = 0;
        this->max_hunger = 0;
        this->mult_t = 0;
        this->age = 0;
        this->max_age = 0;
        this->turn = 0;
        this->vision = 0;
        this->mult = 0;
    }

    entity(int x, int y)
    {
        this->x = x;
        this->y = y;
        this->id = 0;
        this->hunger = 0;
        this->max_hunger = 0;
        this->mult_t = 0;
        this->age = 0;
        this->max_age = 0;
        this->turn = 0;
        this->vision = 0;
        this->mult = 0;
    }

    entity(int x, int y, int id)
    {
        this->x = x;
        this->y = y;
        this->id = id;
        this->hunger = 0;
        this->mult = 0;
        this->age = 0;
        this->turn = 1;
        if (id == 2)
        {
            this->max_hunger = 40;
            this->mult_t = 25;
            this->vision = 3;
            this->max_age = 1000;
        }
        if (id == 3)
        {
            this->max_hunger = 40;
            this->mult_t = 80;
            this->vision = 20;
            this->max_age = 1000;
        }
    }

    entity(int x, int y, int id, int hunger, int age, int mult)
    {
        this->x = x;
        this->y = y;
        this->id = id;
        this->hunger = hunger;
        this->mult = mult;
        this->age = age;
        this->turn = 1;
        if (id == 2)
        {
            this->max_hunger = 25;
            this->mult_t = 15;
            this->vision = 3;
            this->max_age = 1000;
        }
        if (id == 3)
        {
            this->max_hunger = 40;
            this->mult_t = 80;
            this->vision = 20;
            this->max_age = 1000;
        }

    }

    //методы-проверки
    bool is_allowed_fmult(int i, int j, entity w[100][100])
    {
        if (is_allowed(i, j) && (w[i][j].id == 0 || w[i][j].id == 1)) return true;
        return false;
    }

    bool is_allowed_feat(int i, int j, entity w[100][100])
    {
        int id = this->id;
        if (is_allowed(i, j) && ((id == 3 && (w[i][j].id == 0 || w[i][j].id == 1) || w[i][j].id == 2) || (id == 2 && (w[i][j].id == 0 || w[i][j].id == 1)))) return true;
        return false;
    }

    bool is_alert(int i, int j, entity w[100][100])
    {
        if (is_allowed(i, j) && w[i][j].id == 3) return true;
        else return false;
    }

    bool is_allowed_target(int i, int j, entity w[100][100])
    {
        if (is_allowed(i, j) && ((this->id == 3 && w[i][j].id == 2) || (this->id == 2 && w[i][j].id == 1))) return true;
        return false;
    }

    // методы - действия 
    void death()
    {
        this->id = 0;
        this->age = 0;
        this->max_age = 0;
        this->hunger = 0;
        this->max_hunger = 0;
        this->mult = 0;
        this->mult_t = 0;
        this->vision = 0;
        this->turn = 0;
    }

    void go(int i, int j, entity w[100][100])
    {
        int a, b;
        a = this->x;
        b = this->y;
        if (is_allowed(i, j))
        {
            if ((w[a][b].id == 3 && w[i][j].id == 2) || (w[a][b].id == 2 && w[i][j].id == 1))
            {
                this->hunger = 0;
            }
            this->x = i;
            this->y = j;

            w[i][j] = w[a][b];
            w[a][b] = entity(a, b);
        }
    }

    void do_mult(entity w[100][100])
    {
        this->mult = 0;
        int id = this->id;
        int i, j;
        i = this->x;
        j = this->y;
        if (is_allowed_fmult(i + 1, j, w))
        {
            w[i + 1][j] = entity(i + 1, j, id);
            return;
        }
        if (is_allowed_fmult(i, j + 1, w))
        {
            w[i][j + 1] = entity(i, j + 1, id);
            return;
        }
        if (is_allowed_fmult(i - 1, j, w))
        {
            w[i - 1][j] = entity(i - 1, j, id);
            return;
        }
        if (is_allowed_fmult(i, j - 1, w))
        {
            w[i][j - 1] = entity(i, j - 1, id);
            return;
        }
    }

    void stray(entity w[100][100])
    {
        int i = this->x;
        int j = this->y;
        int a = 0;
        int b = 0;
        int k = 0;
        do
        {
            if (k > 1000) break;
            b = 0;
            a = rand() % 3 - 1;

            if (a == 0) b = rand() % 3 - 1;

            k++;
        } while ((a == 0 && b == 0) || !(is_allowed_fmult(i + a, j + b, w)));

        this->hunger += 1;
        w[i][j].go(i + a, j + b, w);
    }

    //поведение хищника
    void behavior_hunter(entity w[100][100], double season)
    {
        // Шаг 1 : проверка голода и возраста
        if ((this->hunger >= this->max_hunger) || (this->age >= this->max_age))
        {
            this->death();
            return;
        }
        this->age += 1;
        this->hunger += 1;


        //Шаг 2 : проверка готовности размножения
        if (this->mult >= round(this->mult_t * season))
        {
            this->do_mult(w);
            this->hunger += 1;
            this->age += 1;
            return;
        }
        this->mult += 1;

        //Шаг 3 : поиск цели, перемещение
        int k = 1;
        int i, j, v;
        i = this->x;
        j = this->y;
        v = this->vision;
        bool flag;
        while (k < v)
        {
            if (is_allowed_target(i + k, j, w) && is_allowed_feat(i + 1, j, w))
            {

                flag = rand() % 2;
                w[i][j].go(i + 1, j, w);
                if (flag && is_allowed_feat(i + 2, j, w) && (k > 1))
                {
                    w[i + 1][j].go(i + 2, j, w);
                }
                return;

            }
            if (is_allowed_target(i, j + k, w) && is_allowed_feat(i, j + 1, w))
            {
                flag = rand() % 2;
                w[i][j].go(i, j + 1, w);

                if (flag && is_allowed_feat(i, j + 2, w) && (k > 1))
                {
                    w[i][j + 1].go(i, j + 2, w);
                }
                return;
            }
            if (is_allowed_target(i - k, j, w) && is_allowed_feat(i - 1, j, w))
            {
                flag = rand() % 2;
                w[i][j].go(i - 1, j, w);

                if (flag && is_allowed_feat(i - 2, j, w) && (k > 1))
                {
                    w[i - 1][j].go(i - 2, j, w);
                }
                return;
            }
            if (is_allowed_target(i, j - k, w) && is_allowed_feat(i, j - 1, w))
            {
                flag = rand() % 2;
                w[i][j].go(i, j - 1, w);

                if (flag && is_allowed_feat(i, j - 2, w) && (k > 1))
                {
                    w[i][j - 1].go(i, j - 2, w);
                }
                return;
            }
            k++;
        }

        //Шаг 4: если нет цели
        this->stray(w);
    }

    //поведение жертвы
    void behavior_victim(entity w[100][100], double season)
    {
        // Шаг 1 : проверка голода и возраста
        if ((this->hunger >= this->max_hunger) || (this->age >= this->max_age))
        {
            this->death();
            return;
        }
        this->age += 1;
        this->hunger += 1;

        int k = 1;
        int i, j, v;
        i = this->x;
        j = this->y;
        v = this->vision;

        int m = this->mult;
        int mt = this->mult_t;

        //Шаг 2: проверка размножения
        if (m >= round(mt * season))
        {
            this->do_mult(w);
            return;
        }
        this->mult += 1;
        k = 1;

        //Шаг 3: проверка опасности
        while (k < v)
        {
            if (is_alert(i + k, j, w))
            {
                if (is_allowed_fmult(i - 1, j, w))

                {
                    this->mult += 1;
                    w[i][j].go(i - 1, j, w);
                    return;

                }
                else
                {
                    this->mult += 1;
                    w[i][j].stray(w);
                    return;
                }
            }
            if (is_alert(i, j + k, w))
            {
                if (is_allowed_fmult(i, j - 1, w))

                {
                    this->mult += 1;
                    w[i][j].go(i, j - 1, w);
                    return;

                }
                else
                {
                    this->mult += 1;
                    w[i][j].stray(w);
                    return;
                }
            }
            if (is_alert(i - k, j, w))
            {
                if (is_allowed_fmult(i + 1, j, w))

                {
                    this->mult += 1;
                    w[i][j].go(i + 1, j, w);
                    return;

                }
                else
                {
                    this->mult += 1;
                    w[i][j].stray(w);
                    return;
                }
            }
            if (is_alert(i, j - k, w))
            {
                if (is_allowed_fmult(i, j + 1, w))

                {
                    this->mult += 1;
                    w[i][j].go(i, j + 1, w);
                    return;
                }
                else
                {
                    this->mult += 1;
                    w[i][j].stray(w);
                    return;
                }
            }
            k++;
        }

        //Шаг 4: поиск пищи
        while (k < v)
        {
            if (is_allowed_target(i + k, j, w) && is_allowed_feat(i + 1, j, w))
            {
                w[i][j].go(i + 1, j, w);
                return;
            }
            if (is_allowed_target(i, j + k, w) && is_allowed_feat(i, j + 1, w))
            {
                w[i][j].go(i, j + 1, w);
                return;
            }
            if (is_allowed_target(i - k, j, w) && is_allowed_feat(i - 1, j, w))
            {
                w[i][j].go(i - 1, j, w);
                return;
            }
            if (is_allowed_target(i, j - k, w) && is_allowed_feat(i, j - 1, w))
            {
                w[i][j].go(i, j - 1, w);
                return;
            }
            k++;
        }

        //Шаг 5: если нет цели

        this->stray(w);

    }

    // общее поведение
    void behavior(entity w[100][100], int k)
    {
        double season = 1;
        if (k % 1000 < 250) season = 1.25;
        else if (k % 1000 < 500) season = 1;
        else if (k % 1000 < 750) season = 0.75;
        else season = 1;
        if (!(this->turn))
        {
            if (this->id == 2)
            {
                this->turn = 1;
                this->behavior_victim(w, season);
                return;
            }
            if (this->id == 3)
            {
                this->turn = 1;
                this->behavior_hunter(w, season);
                return;
            }
            if (this->id == 0)
            {
                this->turn = 1;
                int flag = rand() % 1000;
                if (flag * season < 10)
                {
                    int a = this->x;
                    int b = this->y;
                    w[a][b] = entity(a, b, 1);
                }
                return;
            }
        }
    }
};

//Создание мира
void create_world(entity w[100][100], int& num_of_hunter, int& num_of_victim, int& res)
{
    srand(time(NULL));
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            w[i][j] = entity(i, j);
        }
    int h = num_of_hunter;
    int v = num_of_victim;
    int r = res;

    int flag = rand() % 100;

    while (h > 0)
    {
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if ((w[i][j].id == 0) && (flag < 1) && (h > 0))
                {
                    w[i][j] = entity(i, j, 3, rand() % 10, rand() % 100, rand() % 50);
                    h -= 1;
                }
                flag = rand() % 100;
            }
    }

    flag = rand() % 100;
    while (v > 0)
    {
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if ((w[i][j].id == 0) && (flag < 1) && (v > 0))
                {
                    w[i][j] = entity(i, j, 2, rand() % 10, rand() % 100, rand() % 50);
                    v -= 1;
                }
                flag = rand() % 100;
            }
    }

    flag = rand() % 100;
    while (r > 0)
    {
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if ((w[i][j].id == 0) && (flag < 1) && (r > 0))
                {
                    w[i][j] = entity(i, j, 1);
                    r -= 1;
                }
                flag = rand() % 100;
            }
    }
    h = 0;
    v = 0;
    r = 0;
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            if (w[i][j].id == 1) r++;
            if (w[i][j].id == 2) v++;
            if (w[i][j].id == 3) h++;
        }

    cout << "number of hunters: " << h << " number of victims: " << v << " res: " << r << endl;
    num_of_hunter = h;
    num_of_victim = v;
    res = r;
}

//изменение матрицы сущностей
void change_world(entity w[100][100], int& num_of_hunter, int& num_of_victim, int& res, int k)
{
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            w[i][j].turn = 0;
        }

    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            w[i][j].behavior(w, k);
        }

    num_of_hunter = 0;
    num_of_victim = 0;
    res = 0;

    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
        {
            if (w[i][j].id == 2) num_of_victim++;
            if (w[i][j].id == 3) num_of_hunter++;
            if (w[i][j].id == 1) res++;
        }
}

int main()
{
    RenderWindow window(VideoMode(1500, 1500), "Model");

    RectangleShape rect(Vector2f(10, 10));

    rect.setFillColor(Color(255, 0, 0));
    int k = 0;
    int num_of_hunter = 50;
    int num_of_victim = 1000;
    int res = 300;
    entity a[100][100];

    create_world(a, num_of_hunter, num_of_victim, res);



    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        change_world(a, num_of_hunter, num_of_victim, res, k);
        k++;
        window.clear(Color::Black);

        for (int i = 0; i < 99; i++)
            for (int j = 0; j < 99; j++)
            {
                switch (a[i][j].id)
                {
                case 0:
                {
                    rect.setFillColor(Color(0, 0, 0));
                    break;
                }
                case 1:
                {
                    rect.setFillColor(Color(255, 255, 0));
                    break;
                }
                case 2:
                {
                    rect.setFillColor(Color(0, 255, 0));
                    break;
                }
                case 3:
                {
                    rect.setFillColor(Color(255, 0, 0));
                    break;
                }
                }
                rect.setPosition(i * 15, j * 15);
                window.draw(rect);
            }

        if (k % 50 == 0)
        {
            cout << k << " hunters: " << num_of_hunter << " victims: " << num_of_victim << endl;
        }
        window.display();
        Sleep(30);
    }
    cout << k;
}