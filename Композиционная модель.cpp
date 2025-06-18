// для запуска кода необходимо подключение библиотеки SFML 

#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// проверка соответствия координат (i; j) области моделирования 

bool is_allowed(int i, int j)
{
    if (i < 0 || j < 0 || i > 99 || j > 99) return false;
    return true;
}

// структура

struct field
{
    int hunters;
    int victims;
    float delta;

    // локальный оператор изменения численности 

    void population(int i, int j, int k, field a[100][100], field b[100][100])
    {
        int hunters = this->hunters;
        int victims = this->victims;
        int Sum_vict = victims;
        int Sum_hunt = hunters;
        int hunters1 = hunters;
        int victims1 = victims;

        double toxin = 0.9;
        double season = 1;
        k %= 1000;


        if (k < 250) season = 1.0;
            else if (k < 500) season = 1.03;
                else if (k < 750) season = 1.0;
                     else if (k < 1000) season = 0.97;

        if (is_allowed(i + 1, j))
        {
            Sum_vict += a[i + 1][j].victims;
            Sum_hunt += a[i + 1][j].hunters;
        }

        if (is_allowed(i - 1, j))
        {
            Sum_vict += a[i - 1][j].victims;
            Sum_hunt += a[i - 1][j].hunters;
        }
        if (is_allowed(i, j + 1))
        {
            Sum_vict += a[i][j + 1].victims;
            Sum_hunt += a[i][j + 1].hunters;
        }
        if (is_allowed(i, j - 1))
        {
            Sum_vict += a[i][j - 1].victims;
            Sum_hunt += a[i][j - 1].hunters;
        }

        if (hunters == 0) hunters1 = 0;
        else
        {
            if (Sum_vict == 0 or Sum_vict < Sum_hunt) hunters1 = floor(0.5 * hunters);
            else
            {
                if (i < 20) hunters1 = ceil(season * toxin * 1.0007 * hunters + 0.003 * victims);               
                else
                    hunters1 = ceil(season * 1.0007 * hunters + 0.003 * victims);
            }
        }

        if (hunters1 < 0) hunters1 = 0;

        if (i < 20) victims1 = ceil(season * toxin * 1.015 * victims - 1 * hunters);
        else victims1 = ceil(season * 1.015 * victims - 1 * hunters);

        if (victims1 < 0) victims1 = 0;




        b[i][j].hunters = hunters1;
        b[i][j].victims = victims1;

    }

    // локальный оператор перемещения жертв

    void victims_migration(int i, int j, field a[100][100])
    {
        int hunters = this->hunters;
        int victims = this->victims;

        if (victims == 0) return;

        int Sum_vict = victims;
        int Sum_hunt = hunters;
        int temp;
        double delta = this->delta;
        int k = 0;
        srand(time(0));


        // сбор данных о популяциях в соседних клетках
        if (is_allowed(i + 1, j))
        {
            Sum_vict += a[i + 1][j].victims;
            k++;
        }
        if (is_allowed(i - 1, j))
        {
            Sum_vict += a[i - 1][j].victims;
            k++;
        }
        if (is_allowed(i, j + 1))
        {
            Sum_vict += a[i][j + 1].victims;
            k++;
        }
        if (is_allowed(i, j - 1))
        {
            Sum_vict += a[i][j - 1].victims;
            k++;
        }

        int k1 = k;

        // выбор перемещения
        if (is_allowed(i + 1, j))
        {
            temp = rand() % (25 * k1);
            if (temp < 25 or k == 1)
            {
                int v0 = a[i][j].victims;
                int v1 = a[i + 1][j].victims;

                int dv0 = floor(v0 * delta);
                int dv1 = floor(v1 * delta);

                a[i][j].victims = a[i][j].victims - dv0 + dv1;
                a[i + 1][j].victims = a[i + 1][j].victims + dv0 - dv1;
                return;
            }
            k--;
        }
        if (is_allowed(i - 1, j))
        {
            temp = rand() % (25 * k1);
            if (temp < 25 or k == 1)
            {
                int v0 = a[i][j].victims;
                int v1 = a[i - 1][j].victims;

                int dv0 = floor(v0 * delta);
                int dv1 = floor(v1 * delta);

                a[i][j].victims = a[i][j].victims - dv0 + dv1;
                a[i - 1][j].victims = a[i - 1][j].victims + dv0 - dv1;
                return;
            }
            k--;

        }
        if (is_allowed(i, j + 1))
        {
            temp = rand() % (25 * k1);
            if (temp < 25 or k == 1)
            {
                int v0 = a[i][j].victims;
                int v1 = a[i][j + 1].victims;

                int dv0 = floor(v0 * delta);
                int dv1 = floor(v1 * delta);

                a[i][j].victims = a[i][j].victims - dv0 + dv1;
                a[i][j + 1].victims = a[i][j + 1].victims + dv0 - dv1;
                return;
            }
            k--;
        }
        if (is_allowed(i, j - 1))
        {

            int v0 = a[i][j].victims;
            int v1 = a[i][j - 1].victims;

            int dv0 = floor(v0 * delta);
            int dv1 = floor(v1 * delta);

            a[i][j].victims = a[i][j].victims - dv0 + dv1;
            a[i][j - 1].victims = a[i][j - 1].victims + dv0 - dv1;
            return;


        }



    }

    // локальный оператор перемещения хищников

    void hunters_migration(int i, int j, field a[100][100])
    {
        int hunters = this->hunters;
        if (hunters == 0) return;
        int Sum_vict = 0;
        int temp, temp_vict;
        double delta = this->delta;
        int k = 0;
        srand(time(0));

        // сбор данных о популяциях в соседних клетках

        if (is_allowed(i + 1, j))
        {
            Sum_vict += a[i + 1][j].victims;
            k++;
        }
        if (is_allowed(i - 1, j))
        {
            Sum_vict += a[i - 1][j].victims;
            k++;
        }
        if (is_allowed(i, j + 1))
        {
            Sum_vict += a[i][j + 1].victims;
            k++;
        }
        if (is_allowed(i, j - 1))
        {
            Sum_vict += a[i][j - 1].victims;
            k++;
        }
        int k1 = k;


        // выбор перемещения

        if (Sum_vict == 0)
        {   
            // в округе нет жертв

            if (is_allowed(i + 1, j))
            {
                temp = rand() % (25 * k1);
                if (temp < 25 or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i + 1][j].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i + 1][j].hunters = a[i + 1][j].hunters + dh0 - dh1;
                    return;
                }
                k--;
            }
            if (is_allowed(i - 1, j))
            {
                temp = rand() % (25 * k1);
                if (temp < 25 or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i - 1][j].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i - 1][j].hunters = a[i - 1][j].hunters + dh0 - dh1;
                    return;
                }
                k--;

            }
            if (is_allowed(i, j + 1))
            {
                temp = rand() % (25 * k1);
                if (temp < 25 or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i][j + 1].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i][j + 1].hunters = a[i][j + 1].hunters + dh0 - dh1;
                    return;
                }
                k--;
            }
            if (is_allowed(i, j - 1))
            {

                int h0 = a[i][j].hunters;
                int h1 = a[i][j - 1].hunters;

                int dh0 = floor(h0 * delta);
                int dh1 = floor(h1 * delta);

                a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                a[i][j - 1].hunters = a[i][j - 1].hunters + dh0 - dh1;
                return;


            }
        }
        else
        { 
            // жертвы есть

            if (is_allowed(i + 1, j))
            {
                temp_vict = a[i + 1][j].victims;
                temp = rand() % (Sum_vict);
                if (temp < temp_vict or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i + 1][j].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i + 1][j].hunters = a[i + 1][j].hunters + dh0 - dh1;
                    return;
                }
                k--;
            }
            if (is_allowed(i - 1, j))
            {
                temp_vict = a[i - 1][j].victims;
                temp = rand() % (Sum_vict);
                if (temp < temp_vict or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i - 1][j].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i - 1][j].hunters = a[i - 1][j].hunters + dh0 - dh1;
                    return;
                }
                k--;

            }
            if (is_allowed(i, j + 1))
            {
                temp_vict = a[i][j + 1].victims;
                temp = rand() % (Sum_vict);
                if (temp < temp_vict or k == 1)
                {
                    int h0 = a[i][j].hunters;
                    int h1 = a[i][j + 1].hunters;

                    int dh0 = floor(h0 * delta);
                    int dh1 = floor(h1 * delta);

                    a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                    a[i][j + 1].hunters = a[i][j + 1].hunters + dh0 - dh1;
                    return;
                }
                k--;
            }
            if (is_allowed(i, j - 1))
            {

                int h0 = a[i][j].hunters;
                int h1 = a[i][j - 1].hunters;

                int dh0 = floor(h0 * delta);
                int dh1 = floor(h1 * delta);

                a[i][j].hunters = a[i][j].hunters - dh0 + dh1;
                a[i][j - 1].hunters = a[i][j - 1].hunters + dh0 - dh1;
                return;


            }

        }

    }

};


// вывод количества жертв в клетках

void print_victims(field a[100][100])
{
    cout << endl;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            cout << a[i][j].victims << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// вывод количества хищников в клетках

void print_hunters(field a[100][100])
{
    cout << endl;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            cout << a[i][j].hunters << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// присваивание a:=b 

void assignment(field a[100][100], field b[100][100])
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j].hunters = b[i][j].hunters;
            a[i][j].victims = b[i][j].victims;
        }
    }
}

// заполнение полей структуры случайными данными

void create_world(field a[100][100], int  max_hunters, int max_victims)
{
    srand(time(0));
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j].hunters = rand() % max_hunters + 1;
            a[i][j].victims = rand() % max_victims + 1;
            a[i][j].delta = 0.3;



        }
    }
}

// глобальный оператор перехода

void change_the_world(field a[100][100], field b[100][100], int k)
{
    // оператор F1
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j].victims_migration(i, j, a);
            a[i][j].hunters_migration(i, j, a);
        }
    }


    //оператор F2
    assignment(b, a);
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j].population(i, j, k, a, b);

        }
    }
    assignment(a, b);
}

// подсчет особей

int count_hunters(field a[100][100])
{
    int Sum = 0;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Sum += a[i][j].hunters;
        }
    }
    return Sum;
}

int count_victims(field a[100][100])
{
    int Sum = 0;

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Sum += a[i][j].victims;
        }
    }
    return Sum;
}

int main()
{
    double mid_vict = 0; double mid_hunt = 0;
    RenderWindow window(VideoMode(3150, 1500), "Victims and Hunters");
    RectangleShape rect(Vector2f(10, 10));
    rect.setFillColor(Color(255, 0, 0));
    srand(time(0));
    field a[100][100];

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            a[i][j].hunters = 0;
            a[i][j].victims = 0;
            a[i][j].delta = 0.5;
        }
    }

    field b[100][100];
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            b[i][j].hunters = 0;
            b[i][j].victims = 0;
            b[i][j].delta = 0.5;
        }
    }

    int k = 0;
    create_world(a, 5, 100);
    mid_vict = count_victims(a);
    mid_hunt = count_hunters(a);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }


        window.clear(Color::Black);
        for (int i = 0; i < 100; i++)
        {
            rect.setFillColor(Color(0, 0, 0));
            rect.setPosition(900, i * 9);
            window.draw(rect);
        }

        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if (a[i][j].victims == 0) rect.setFillColor(Color(255, 255, 255));
                else
                    if (a[i][j].victims <= 10) rect.setFillColor(Color(100, 100, 100));
                    else
                        if (a[i][j].victims <= 15) rect.setFillColor(Color(50, 50, 50));
                        else
                            if (a[i][j].victims >= 15) rect.setFillColor(Color(0, 0, 0));


                rect.setPosition(i * 15, j * 15);
                window.draw(rect);
            }

        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if (a[i][j].hunters == 0) rect.setFillColor(Color(255, 255, 255));
                else
                    if (a[i][j].hunters <= 5) rect.setFillColor(Color(100, 100, 100));
                    else

                        if (a[i][j].hunters <= 10) rect.setFillColor(Color(50, 50, 50));
                        else
                            if (a[i][j].hunters > 10) rect.setFillColor(Color(0, 0, 0));

                rect.setPosition(i * 15 + 1515, j * 15);
                window.draw(rect);
            }

        // Цветная визуализация

        /*for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if (a[i][j].victims == 0) rect.setFillColor(Color(0, 0, 0));
                else
                    if (a[i][j].victims <= 10) rect.setFillColor(Color(50, 0, 0));
                    else
                        if (a[i][j].victims <= 15) rect.setFillColor(Color(150, 0, 0));
                        else
                            if (a[i][j].victims <= 20) rect.setFillColor(Color(200, 200, 0));
                            else
                                if (a[i][j].victims <= 30) rect.setFillColor(Color(0, 255, 0));
                                else
                                    if (a[i][j].victims > 30) rect.setFillColor(Color(0, 255, 255));


                rect.setPosition(i * 9, j * 9);
                window.draw(rect);
            }



        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
            {
                if (a[i][j].hunters == 0) rect.setFillColor(Color(0, 0, 0));
                else
                    if (a[i][j].hunters <= 1) rect.setFillColor(Color(50, 0, 0));
                    else

                        if (a[i][j].hunters <= 5) rect.setFillColor(Color(150, 0, 0));
                        else
                            if (a[i][j].hunters <= 10) rect.setFillColor(Color(200, 200, 0));
                            else
                                if (a[i][j].hunters > 10) rect.setFillColor(Color(0, 255, 0));

                rect.setPosition(i * 9 + 909, j * 9);
                window.draw(rect);
            }*/



        /*if (k % 50 == 0)
        {
            cout << k << " hunters: " << num_of_hunter << " victims: " << num_of_victim << endl;
        }*/

        window.display();

        change_the_world(a, b, k);
        mid_vict += count_victims(a);
        mid_hunt += count_hunters(a);
        if (k % 50 == 0)
        {
            cout << "Iteration: " << k << " Hunters: " << count_hunters(a) << "  Victims: " << count_victims(a) << endl;
        }
        k++;
    }

    cout << " mid_ vict = " << mid_vict / k << " mid hunt = " << mid_hunt / k << endl;

}

