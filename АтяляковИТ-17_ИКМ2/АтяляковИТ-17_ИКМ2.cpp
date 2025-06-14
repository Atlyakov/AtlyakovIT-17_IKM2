#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

// Функция для безопасного ввода числа
int SafeInputInt(const string& prompt, int min_val, int max_val) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Ошибка: введите целое число!\n";
            exit(1);
        }
        else if (value < min_val || value > max_val) {
            cout << "Ошибка: значение должно быть от " << min_val << " до " << max_val << "!\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Функция для парсинга операции
bool ParseOperation(const string& input, char& op, int& A, int& B, int K) {
    istringstream iss(input);
    if (!(iss >> op >> A >> B)) {
        return false;
    }
    return (op == '+' || op == '-') &&
        (A >= 1 && A <= K) &&
        (B >= 1 && B <= 10000);
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "=== Симулятор погрузки/выгрузки бочек на баржу ===\n\n";

    // Ввод параметров баржи
    int N = SafeInputInt("Введите количество операций (N, 1-100000): ", 1, 100000);
    int K = SafeInputInt("Введите количество отсеков (K, 1-100000): ", 1, 100000);
    int P = SafeInputInt("Введите максимальную вместимость баржи (P, 1-100000): ", 1, 100000);

    // Инициализация
    vector<list<pair<int, int>>> compartments(K + 1);  // Отсеки с 1 до K
    int current_bottles = 0;
    int max_bottles = 0;
    bool error = false;

    cout << "\nВведите " << N << " операций в формате \"+ A B\" или \"- A B\":\n";

    // Обработка N операций
    for (int i = 0; i < N; ++i) {
        string input;
        char op;
        int A, B;

        // Ввод и парсинг операции
        while (true) {
            cout << "Операция " << (i + 1) << ": ";
            getline(cin, input);

            if (ParseOperation(input, op, A, B, K)) {
                break;
            }
            cout << "Некорректный формат! Используйте \"+ A B\" или \"- A B\" (1 <= B <= 10 000)\n";
        }

        if (error) {
            continue;  // Пропускаем оставшиеся операции после ошибки
        }

        if (op == '+') {
            // Погрузка бочки
            if (A < 1 || A > K) {
                cout << "  Ошибка: неверный номер отсека (должен быть 1-" << K << ")!\n";
                error = true;
            }
            else if (current_bottles >= P) {
                cout << "  Ошибка: баржа переполнена (максимум " << P << ")!\n";
                error = true;
            }
            else {
                compartments[A].push_front({ B, i });
                current_bottles++;
                if (current_bottles > max_bottles) {
                    max_bottles = current_bottles;
                }
                cout << "  Бочка " << B << " добавлена в отсек " << A << "\n";
            }
        }
        else if (op == '-') {
            // Выгрузка бочки
            if (A < 1 || A > K) {
                cout << "  Ошибка: неверный номер отсека (должен быть 1-" << K << ")!\n";
                error = true;
            }
            else if (compartments[A].empty()) {
                cout << "  Ошибка: отсек " << A << " пуст!\n";
                error = true;
            }
            else if (compartments[A].front().first != B) {
                cout << "  Ошибка: нельзя вытащить бочку " << B << " из отсека " << A << "!\n";
                error = true;
            }
            else {
                compartments[A].pop_front();
                current_bottles--;
                cout << "  Бочка " << B << " выгружена из отсека " << A << "\n";
            }
        }
    }

    // Вывод итогового результата
    cout << "\n=== Результат ===\n";
    if (error || current_bottles != 0) {
        cout << "Error\n";
    }
    else {
        cout << "Максимальное количество бочек: " << max_bottles << "\n";
    }

    return 0;
}