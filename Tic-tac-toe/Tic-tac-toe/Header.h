#pragma once

// Прототипы функций
bool Winning(char player); // Функция проверки победы
void Setup(); // Функция настройки
void Online(); // Функция игры псевдо-онлайн
void Clearing(); // Функция очитски экрана
void CleanBoard(); // Функция очистки поля
void PrintBoard(); // Функция вывода поля на экран
int HumanMove(); // Функция определения хода игрока
int CheckBoard(char player); // Функция проверки поля для компьютера
int ComputerMoveNull(); // Функция игры с нулевым компьютером
int ComputerMoveEasy(); // Функция игры с легким компьютером
int ComputerMoveNormal(); // Функция игры с средним компьютером
int ComputerMoveHard(); // Функция игры со сложным компьютером
int FirstComputerMove(); // Функция првого хода компьютера
char PlayWithFriend(); // Функция игры с другом
char PlayWithComputer(int timeOfSleep, char player1, char player2, int turnRatio); // Функция игры с компьютером

// Функция очистки экрана
void Clearing()
{
    system("CLS");
}