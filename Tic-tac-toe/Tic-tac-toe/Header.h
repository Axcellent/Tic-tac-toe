#pragma once

// ��������� �������
bool Winning(char player); // ������� �������� ������
void Setup(); // ������� ���������
void Online(); // ������� ���� ������-������
void Clearing(); // ������� ������� ������
void CleanBoard(); // ������� ������� ����
void PrintBoard(); // ������� ������ ���� �� �����
int HumanMove(); // ������� ����������� ���� ������
int CheckBoard(char player); // ������� �������� ���� ��� ����������
int ComputerMoveNull(); // ������� ���� � ������� �����������
int ComputerMoveEasy(); // ������� ���� � ������ �����������
int ComputerMoveNormal(); // ������� ���� � ������� �����������
int ComputerMoveHard(); // ������� ���� �� ������� �����������
int FirstComputerMove(); // ������� ������ ���� ����������
char PlayWithFriend(); // ������� ���� � ������
char PlayWithComputer(int timeOfSleep, char player1, char player2, int turnRatio); // ������� ���� � �����������

// ������� ������� ������
void Clearing()
{
    system("CLS");
}