// Библиотеки
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <string>
#include "Header.h"
#include <windows.h>

std::string name = "NAME"; // Имя игрока
std::string message = " "; // Краткая информация о бое
int score = 0, cntOfGames = 0; // Данные игрока: счёт и количество игр
int match = 1; // Номер матча
unsigned long long enemyID = 100000000; // ID противника
char difficult = '1'; // Сложность
char board[9] = {}; // Игровое поле

int main() {
    setlocale(0, "");
    std::cout << "Добро пожаловать в игру \"Крестики-нолики!\"\n";

    std::string replay = "y"; // Играть ли?
    int x_wins = 0, o_wins = 0, ties = 0; // Игровые исходы
    char winner; // Победитель

    while (replay == "y") {
        CleanBoard(); // Очистка

        // Выбор вида игры: против друга или против компьютера
        std::cout << "\nВы будете играть с компьютером, с другом или по сети? ('c' - с компьютером, 'f' - с другом, 'o' - по сети)\n Введите 's' если хотите настроить фон \n  -> ";
        char play_with;
        std::cin >> play_with;
        while (play_with != 'f' && play_with != 'c' && play_with != 's' && play_with != 'o') {
            std::cout << "\nПожалуйста, введите одну из букв: 'c' - с компьютером, 'f' - с другом, 'o' - по сети... \n   -> ";
            std::cin >> play_with;
        }
        if (play_with == 'f') {
            winner = PlayWithFriend();
            PrintBoard();
        }
        else if (play_with == 'c') {
            // Выбор сложности
            std::cout << "\nВыберите сложность: \n 1.Нулевой \n 2.Лёгкий \n 3.Нормальный \n 4.Сложный \n (Введите цифру) \n   -> ";
            std::cin >> difficult;
            while (difficult < '1' || difficult > '4') {
                std::cout << "\nПожалуйста, введите корректную цифру... \n   -> ";
                std::cin >> difficult;
            }
            winner = PlayWithComputer(0, 'X', 'O', 1);
            PrintBoard();
            // Счётчик побед
            switch (winner) {
            case 'X':
                x_wins++;
                break;
            case 'O':
                o_wins++;
                break;
            case 'D':
                ties++;
                break;
            }

            // Сообщения в конце
            std::cout << "\n\t*Статистика игры*\nИгрок \"X\": " << x_wins
                << " побед,   Игрок \"O\": " << o_wins << " побед,   Ничьи: " << ties << "\n\n";
        }
        else if (play_with == 'o') {
            char startGame;
            std::ifstream DATA("data.txt");
            if (!DATA.is_open()) std::cout << "\n   Файл с вашими данными не открывается! Если он не создан, то создайте его\n"; // Если не открылся
            else if (DATA.peek() == EOF) /*Если первый символ конец файла*/{ 
                // Создание учётной записи в файле
                DATA.close();
                std::ofstream OUTDATA("data.txt", std::ios_base::out);
                std::cout << "\n   Файл с вашими данными пуст!\n Для начала игры введите своё имя\n  -> ";
                std::cin >> name;
                OUTDATA << name << "\n" << 0 << "\n" << 0;
                std::cout << "     Отлично, учетная запись создана! Теперь вы сможете играть on-line";
                Sleep(2500);
                Clearing();
                OUTDATA.close();
                return main();
            }
            else {
                Clearing();
                DATA >> name >> score >> cntOfGames;
                std::cout << "\tВаша учётная запись:\n   " << name << "\n   Счёт: " << score << "\n   Количество игр on-line: " << cntOfGames;
                std::cout << "\n Начать игру? ('y' - ДА, 'n' - НЕТ)\n   -> ";
                do {
                    std::cin >> startGame;
                    if (startGame == 'y') {
                        // Сообщения о начале
                        std::cout << "\nИдёт подбор противника...\n";
                        Sleep(3000);
                        std::cout << "Противник найден! Начало игры...\n";
                        Sleep(2000);
                        Clearing();
                        // Запуск симуляции игры по сети
                        Online();

                        // Запись результатов в файл для программы
                        std::ofstream OUTDATA("data.txt", std::ios_base::out);
                        OUTDATA << name << std::endl << score << std::endl << cntOfGames;
                        OUTDATA.close();

                        // Запись результатов в файл для игрока
                        std::ofstream file("MyProfile.txt", std::ios_base::out);
                        file << "--------------------------\n Никнейм в игре: " << name << "\n Счёт: " << score
                            << "\n Количество игр on-line: " << cntOfGames << "\n--------------------------";
                    }
                    else if (startGame == 'n') {
                        return main();
                    }
                    else {
                        std::cout << "Пожалуйста, введите одну из букв: 'y' - ДА, 'n' - НЕТ...\n   -> ";
                    }
                } while (startGame != 'y' && startGame != 'n');
            }
        }
        else {
            Clearing();
            Setup();
            Clearing();
            return main();
        }

        // Рестарт
        std::cout << "\nХотите играть снова? ('y' - ДА, 'n' - НЕТ)\n   -> ";
        std::cin >> replay;
        while (replay != "y" && replay != "n") {
            std::cout << "Пожалуйста, введите одну из букв: 'y' - ДА, 'n' - НЕТ...\n   -> ";
            std::cin >> replay;
            Clearing();
        }
    }

    return 0;
}

// Функция симуляции игры онлайн
void Online() {
    cntOfGames++; // Увеличение количества игр
    srand(time(NULL));
    enemyID = ((rand() * rand() / rand()) * rand() % rand()) / 10;
    char player1, player2, winner;
    int firstWins = 0, secondWins = 0, ties = 0, turnRatio;
    int wins[3];

    // За кого играет игрок
    switch (rand() % 2) {
    case 0:
        turnRatio = 0; player1 = 'O'; player2 = 'X';
        break;
    case 1:
        turnRatio = 1; player1 = 'X'; player2 = 'O';
        break;
    }

    // Сортировка сложности по счёту игрока
    if (score <= 50) difficult = rand() % 2 + 49;
    else if (score <= 150) difficult = rand() % 2 + 50;
    else if (score <= 250) difficult = rand() % 2 + 51;
    else difficult = '4';

    // 1-й раунд: запрос хода, проверка, увеличение счётчиков
    message = "\n\t\ Раунды: | ? | - | - | ";
    winner = PlayWithComputer(2000, player1, player2, turnRatio);
    if ((turnRatio && (winner == 'X')) || (!turnRatio && (winner == 'O'))) {
        firstWins++; wins[0] = 1;
    }
    else if ((turnRatio && (winner == 'O')) || (!turnRatio && (winner == 'X')))
    {
        secondWins++; wins[0] = 2;
    }
    else {
        ties++; wins[0] = 0;
    }
    message = "\n\t\ Раунды: | "; message.append(std::to_string(wins[0])); message.append(" | ? | - | ");
    
    // Обмен игроков местами
    if (turnRatio) { turnRatio = 0; player1 = 'O'; player2 = 'X'; }
    else { turnRatio = 1; player1 = 'X'; player2 = 'O'; }

    CleanBoard();
    // 2-й раунд: запрос хода, проверка, увеличение счётчиков
    winner = PlayWithComputer(2000, player1, player2, turnRatio);
    if ((turnRatio && (winner == 'X')) || (!turnRatio && (winner == 'O'))) {
        firstWins++; wins[1] = 1;
    }
    else if ((turnRatio && (winner == 'O')) || (!turnRatio && (winner == 'X')))
    {
        secondWins++; wins[1] = 2;
    }
    else {
        ties++; wins[1] = 0;
    }
    message = "\n\t\ Раунды: | " + std::to_string(wins[0]) + " | " + std::to_string(wins[1]);  message.append(" | - | ");

    // Проверка на победу в матче
    if (firstWins == 2) {
        score += 10;
        std::cout << "\n\tВы победили. Ваш счёт повышен!";
        return;
    }
    else if (secondWins == 2) {
        score -= 10;
        std::cout << "\n\tВы проиграли. Ваш счёт понижен!";
        return;
    }
    
    // Обмен игроков местами
    if (turnRatio) { turnRatio = 0; player1 = 'O'; player2 = 'X'; }
    else { turnRatio = 1; player1 = 'X'; player2 = 'O'; }

    CleanBoard();
    // 3-й раунд: запрос хода, проверка, увеличение счётчиков
    winner = PlayWithComputer(2000, player1, player2, turnRatio);
    if ((turnRatio && (winner == 'X')) || (!turnRatio && (winner == 'O'))) {
        firstWins++;
    }
    else if ((turnRatio && (winner == 'O')) || (!turnRatio && (winner == 'X')))
    {
        secondWins++;
    }
    else {
        ties++;
    }

    // Конечная проверка на победу, сообщения
    if (firstWins == 2 || (firstWins == 1 && ties == 2)) {
        score += 10;
        std::cout << "\n\tВы победили. Ваш счёт повышен!";
    }
    else if (secondWins == 2 || (secondWins == 1 && ties == 2)) {
        score -= 10;
        std::cout << "\n\tВы проиграли. Ваш счёт понижен!";
    }
    else {
        score += 5;
        std::cout << "\n\tНичья. Ваш счёт повышен.";
    }
    return;
}

// Функция настроек
void Setup() {
    char color;
    while (1) {
        std::cout << "  Выберите цвет консоли и текста:\n 1.Черный-белый\n 2.Белый-чёрный\n 3.Синий-белый \n 4.Зелёный-жёлтый \n 5.Красный-жёлтый \n" <<
            " 0.Выход из настроек \n   -> ";
        std::cin >> color;
        while (color > '5' || color < '0') {
            std::cout << "Пожалуйста, введите корректную цифру...\n   -> ";
            std::cin >> color;
        }
        switch (color) {
        case '0':
            return;
        case '1':
            system("color 0F");
            Clearing();
            break;
        case '2':
            system("color F0");
            Clearing();
            break;
        case '3':
            system("color 1F");
            Clearing();
            break;
        case '4':
            system("color 2E");
            Clearing();
            break;
        case '5':
            system("color 4E");
            Clearing();
            break;
        }
    }
}

// Функция очистки поля
void CleanBoard()
{
    for (int i = 0; i < 9; i++)
    {
        board[i] = '-';
    }
}

// Функция проверки победителей
bool Winning(char player)
{
    if (board[0] == 'X' && board[1] == 'X' && board[2] == 'X') return true;
    else if (board[0] == 'O' && board[1] == 'O' && board[2] == 'O') return true;
    else if (board[3] == 'X' && board[4] == 'X' && board[5] == 'X') return true;
    else if (board[3] == 'O' && board[4] == 'O' && board[5] == 'O') return true;
    else if (board[6] == 'X' && board[7] == 'X' && board[8] == 'X') return true;
    else if (board[6] == 'O' && board[7] == 'O' && board[8] == 'O') return true;
    else if (board[0] == 'X' && board[3] == 'X' && board[6] == 'X') return true;
    else if (board[0] == 'O' && board[3] == 'O' && board[6] == 'O') return true;
    else if (board[1] == 'X' && board[4] == 'X' && board[7] == 'X') return true;
    else if (board[1] == 'O' && board[4] == 'O' && board[7] == 'O') return true;
    else if (board[2] == 'X' && board[5] == 'X' && board[8] == 'X') return true;
    else if (board[2] == 'O' && board[5] == 'O' && board[8] == 'O') return true;
    else if (board[0] == 'X' && board[4] == 'X' && board[8] == 'X') return true;
    else if (board[0] == 'O' && board[4] == 'O' && board[8] == 'O') return true;
    else if (board[2] == 'X' && board[4] == 'X' && board[6] == 'X') return true;
    else if (board[2] == 'O' && board[4] == 'O' && board[6] == 'O') return true;
    else return false;
}

// Функция вывода на экран поля
void PrintBoard()
{
    std::cout << std::endl << " id#" << enemyID << std::endl << message << std::endl;
    std::cout << "-" << board[6] << "-|-" << board[7] << "-|-" << board[8] << "-\n";
    std::cout << "-" << board[3] << "-|-" << board[4] << "-|-" << board[5] << "-\n";
    std::cout << "-" << board[0] << "-|-" << board[1] << "-|-" << board[2] << "-\n";
}

// Функция хода игрока
int HumanMove()
{
    // Вывод поля и номеров клеток
    PrintBoard();
    std::cout << "\n\n  Номера позиций: \n  -7-|-8-|-9-\n  -4-|-5-|-6-\n  -1-|-2-|-3-\n\n";
    std::cout << "\n";
    std::cout << "\nВаш ход...   ";

    // Определение хода
    char move;
    std::cin >> move;
    while (move > '9' || move < '1' || board[int(move) - 49] != '-')
    {
        std::cout << "Пожалуйста, введите допустимую цифру (1-9):\n   -> ";
        std::cin >> move;
    }
    return (int(move) - 48);
}

// Функция проверки поля
int CheckBoard(char player) {
    if ((board[3] == player && board[6] == player && board[0] == '-') || (board[4] == player && board[8] == player && board[0] == '-') || (board[1] == player && board[2] == player && board[0] == '-')) return 0;
    else if ((board[0] == player && board[2] == player && board[1] == '-') || (board[7] == player && board[4] == player && board[1] == '-')) return 1;
    else if ((board[0] == player && board[1] == player && board[2] == '-') || (board[6] == player && board[4] == player && board[2] == '-') || (board[8] == player && board[5] == player && board[2] == '-')) return 2;
    else if ((board[5] == player && board[4] == player && board[3] == '-') || (board[0] == player && board[6] == player && board[3] == '-')) return 3;
    else if ((board[3] == player && board[5] == player && board[4] == '-') || (board[2] == player && board[6] == player && board[4] == '-') || (board[0] == player && board[8] == player && board[4] == '-') || (board[1] == player && board[7] == player && board[4] == '-')) return 4;
    else if ((board[3] == player && board[4] == player && board[5] == '-') || (board[2] == player && board[8] == player && board[5] == '-')) return 5;
    else if ((board[8] == player && board[7] == player && board[6] == '-') || (board[0] == player && board[3] == player && board[6] == '-') || (board[2] == player && board[4] == player && board[6] == '-')) return 6;
    else if ((board[6] == player && board[8] == player && board[7] == '-') || (board[1] == player && board[4] == player && board[7] == '-')) return 7;
    else if ((board[6] == player && board[7] == player && board[8] == '-') || (board[0] == player && board[4] == player && board[8] == '-') || (board[2] == player && board[5] == player && board[8] == '-')) return 8;
    else return -1;
}

// Функция игры с нулевым компьютером и функция хода в случайную клетку
int ComputerMoveNull() {
    srand(time(NULL));
    int move;
    do { move = rand() % 10; } while (board[move] != '-');
    return move;
}

// Функция игры с лёгким компьютером
int ComputerMoveEasy() {
    // Рандоиное число (1-2) - ход
    srand(time(NULL));
    int typeOfMove = rand() % 2;

    // Симуляция невнимательности и ход
    if (typeOfMove) { 
        if (CheckBoard('O') > -1) return CheckBoard('O'); 
    }
    else if (CheckBoard('X') > -1) return CheckBoard('X');
    return ComputerMoveNull();
}

// Функция игры с нормальным компьютером
int ComputerMoveNormal() {
    int cntOfSectors = 2, sumOfSectors = 0, typeOfMove;

    // Создание пары раномных чисел для определения того, будет ли компьютер мешать победить
    srand(time(NULL));
    for (int i = 0; i < cntOfSectors; i++) {
        sumOfSectors += (rand() % 2 + 1);
    }

    // Симуляция невнимательности и ход
    if (CheckBoard('O') > -1) return CheckBoard('O');
    if (sumOfSectors > 2) if (CheckBoard('X') > -1) return CheckBoard('X');
    return ComputerMoveNull();
}

// Функция игры со сложным компьютером
int ComputerMoveHard() {
    int fork, typeOfMove; char player;

    srand(time(NULL));
    fork = rand() % 2;

    // Ход
    if (CheckBoard('O') > -1) return CheckBoard('O');
    if (CheckBoard('X') > -1) return CheckBoard('X');
    // Вилка
    if (fork) {
        if (board[1] == 'O' && board[6] == 'O' && board[0] == '-') return 0;
        else if (board[2] == 'O' && board[7] == 'O' && board[8] == '-') return 8;
        else if (board[1] == 'O' && board[8] == 'O' && board[2] == '-') return 2;
        else if (board[3] == 'O' && board[2] == 'O' && board[4] == '-') return 4;
        else if (board[0] == 'O' && board[5] == 'O' && board[4] == '-') return 4;
        else if (board[3] == 'O' && board[8] == 'O' && board[4] == '-') return 3;
        else if (board[6] == 'O' && board[5] == 'O' && board[4] == '-') return 4;
        else if (board[6] == 'O' && board[4] == 'O' && board[8] == '-') return 8;
        else if (board[0] == 'O' && board[4] == 'O' && board[2] == '-') return 2;
        else if (board[4] == 'O' && board[8] == 'O' && board[4] == '-') return 4;
    }
    else return ComputerMoveNull();
}

// Первый ход компьютера
int FirstComputerMove() {
    srand(time(NULL));
    int r;
    switch (difficult)
    {
    case '1':
        return ComputerMoveNull();
    case '2':
        return ComputerMoveNull();
    case '3':
        do {
            r = ComputerMoveNull();
        } while (r % 2 != 0);
        return r;
    case '4':
        if (board[4] == '-') return 4;
        do {
            r = ComputerMoveNull();
        } while (r % 2 != 0);
        return r;
    }
}

// Игра с другом
char PlayWithFriend()
{
    int turn = 1;

    // Пока никто не выиграл
    while (!Winning('X') && !Winning('O'))
    {
        // Очистка и ходы
        Clearing();
        int move = HumanMove();
        Clearing();
        // Проверки на выигрыш и поздравление
        if (turn % 2 == 1)
        {
            board[move - 1] = 'X';
            if (Winning('X'))
            {
                std::cout << "Поздравляю, игрок 'X'! Вы победили!\n";
                return 'X';
            }
        }
        else
        {
            board[move - 1] = 'O';
            if (Winning('O'))
            {
                std::cout << "Поздравляю, игрок 'O'! Вы победили!\n";
                return 'O';
            }
        }
        turn++;
        if (turn == 10)
        {
            std::cout << "\t Ничья!\n";
            return 'D';
        }
    }

}

// Игра с компьютером
char PlayWithComputer(int timeOfSleep, char player1, char player2, int turnRatio)
{
    int turn = 1, move;

    // Пока никто не выиграл
    while (!Winning(player1) && !Winning(player2)) {
        // Очистка, ходы, проверка
        Clearing();
        // Ход игрока
        if (turn % 2 == turnRatio) {
            move = HumanMove();
            Clearing();
            board[move - 1] = player1;
            if (Winning(player1)) {
                std::cout << "\tПобеда!\n";
                Sleep(1000);
                return player1;
            }
        }
        // Ход компьютера
        else {
            std::cout << "\n\n\tХод второго игрока...";
            Sleep(timeOfSleep);
            if (turn == 2 || turn == 1) move = FirstComputerMove();
            else {
                switch (difficult) {
                case '1':
                    move = ComputerMoveNull();
                    break;
                case '2':
                    move = ComputerMoveEasy();
                    break;
                case '3':
                    move = ComputerMoveNormal();
                    break;
                case '4':
                    move = ComputerMoveHard();
                    break;
                }
            }
            Clearing();
            board[move] = player2;
            if (Winning(player2)) {
                std::cout << "\tПоражение!\n";
                Sleep(1000);
                return player2;
            }
        }
        turn++;
        if (turn > 9) {
            std::cout << "\tНичья!\n";
            Sleep(1000);
            return 'D';
        }
    }
}