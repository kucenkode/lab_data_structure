#include <iostream>
#include <vector>
#include <stack>
#include <Windows.h>
#include <chrono>

using namespace std;

// Клетка
struct Cell {
    int row;
    int col;
};

// Функция для проверки, является ли клетка доступной (не закончились ли клетки)
bool isValidCell(const Cell& cell, int m, int n) {
    return cell.row >= 0 && cell.row < m && cell.col >= 0 && cell.col < n;
}

// Функция для обхода графа в глубину
void dfs(vector<vector<bool>>& grid, vector<vector<bool>>& visited, Cell cell, int m, int n) {
    visited[cell.row][cell.col] = true;

    // Проверяем всех соседей
    vector<Cell> neighbors = {
        {cell.row - 1, cell.col},
        {cell.row + 1, cell.col},
        {cell.row, cell.col - 1},
        {cell.row, cell.col + 1}
    };

    for (const Cell& neighbor : neighbors) {
        if (isValidCell(neighbor, m, n) && !grid[neighbor.row][neighbor.col] && !visited[neighbor.row][neighbor.col]) {
            dfs(grid, visited, neighbor, m, n);
        }
    }
}

// Функция для подсчета количества кусков
int countPieces(vector<vector<bool>>& grid, int m, int n) {
    int pieces = 0;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    // Проходим по всем клеткам
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!grid[i][j] && !visited[i][j]) {
                pieces++;
                Cell startCell = { i, j };
                dfs(grid, visited, startCell, m, n);
            }
        }
    }

    return pieces;
}

// Функция для проверки, является ли клетка допустимой (с учетом цилиндра)
bool isValidCellCylinder(const Cell& cell, int m, int n) {
    return (cell.row >= 0 && cell.row < m && cell.col >= 0 && cell.col < n) ||
        (cell.row == -1 && cell.col >= 0 && cell.col < n) ||
        (cell.row == m && cell.col >= 0 && cell.col < n);
}

// Функция для обхода графа в глубину (с учетом цилиндра)
void dfsCylinder(vector<vector<bool>>& grid, vector<vector<bool>>& visited, Cell cell, int m, int n) {
    // Корректируем индекс строки для цилиндрического обхода
    cell.row = (cell.row + m) % m;
    visited[cell.row][cell.col] = true;

    // Проверяем всех соседей (с учетом цилиндра)
    vector<Cell> neighbors = {
        {cell.row - 1, cell.col},
        {cell.row + 1, cell.col},
        {cell.row, cell.col - 1},
        {cell.row, cell.col + 1}
    };

    for (const Cell& neighbor : neighbors) {
        // Корректируем индекс строки для каждого соседа
        int wrappedRow = (neighbor.row + m) % m;
        if (isValidCell({ wrappedRow, neighbor.col }, m, n) && !grid[wrappedRow][neighbor.col] && !visited[wrappedRow][neighbor.col]) {
            dfsCylinder(grid, visited, { wrappedRow, neighbor.col }, m, n);
        }
    }
}

// Функция для подсчета количества кусков (с учетом цилиндра)
int countPiecesCylinder(vector<vector<bool>>& grid, int m, int n) {
    int pieces = 0;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    // Проходим по всем клеткам
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!grid[i][j] && !visited[i][j]) {
                pieces++;
                Cell startCell = { i, j };
                dfsCylinder(grid, visited, startCell, m, n);
            }
        }
    }

    return pieces;
}

// Узел связанного списка
struct Node {
    Cell cell;
    Node* next;
};

// Функция для обхода графа в глубину с использованием связанного списка
void dfsList(vector<vector<bool>>& grid, vector<vector<bool>>& visited, Cell cell, int m, int n, Node*& head) {
    visited[cell.row][cell.col] = true;

    // Создаем новый узел и добавляем его в начало списка
    Node* newNode = new Node{ cell, head };
    head = newNode;

    // Проверяем всех соседей
    vector<Cell> neighbors = {
        {cell.row - 1, cell.col},
        {cell.row + 1, cell.col},
        {cell.row, cell.col - 1},
        {cell.row, cell.col + 1}
    };

    for (const Cell& neighbor : neighbors) {
        if (isValidCell(neighbor, m, n) && !grid[neighbor.row][neighbor.col] && !visited[neighbor.row][neighbor.col]) {
            dfsList(grid, visited, neighbor, m, n, head);
        }
    }

    // Удаляем текущую клетку из списка после завершения обхода всех соседей
    Node* temp = head;
    head = head->next;
    delete temp;
}

// Функция для подсчета количества кусков с использованием связанного списка
int countPiecesList(vector<vector<bool>>& grid, int m, int n) {
    int pieces = 0;
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    Node* head = nullptr;

    // Проходим по всем клеткам
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!grid[i][j] && !visited[i][j]) {
                pieces++;
                Cell startCell = { i, j };
                dfsList(grid, visited, startCell, m, n, head);
            }
        }
    }

    return pieces;
}

// Функция для обхода графа в глубину с использованием стека
void dfsStack(vector<vector<bool>>& grid, vector<vector<bool>>& visited, Cell cell, int m, int n, stack<Cell>& stack) {
    visited[cell.row][cell.col] = true;

    // Добавляем текущую клетку в стек
    stack.push(cell);

    // Проверяем всех соседей
    vector<Cell> neighbors = {
        {cell.row - 1, cell.col},
        {cell.row + 1, cell.col},
        {cell.row, cell.col - 1},
        {cell.row, cell.col + 1}
    };

    for (const Cell& neighbor : neighbors) {
        if (isValidCell(neighbor, m, n) && !grid[neighbor.row][neighbor.col] && !visited[neighbor.row][neighbor.col]) {
            dfsStack(grid, visited, neighbor, m, n, stack);
        }
    }

    // Извлекаем клетку из стека
    stack.pop();
}

// Функция для подсчета количества кусков с использованием стека
int countPiecesStack(vector<vector<bool>>& grid, int m, int n) {
    int pieces = 0;
    vector<vector<bool>> visited(m, vector<bool>(n, false));
    stack<Cell> stack;

    // Проходим по всем клеткам
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!grid[i][j] && !visited[i][j]) {
                // Находим новый кусок
                pieces++;
                Cell startCell = { i, j };
                dfsStack(grid, visited, startCell, m, n, stack);
            }
        }
    }

    return pieces;
}
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int m, n;
    cout << "Введите размеры листа (M N): ";
    cin >> m >> n;

    // Создаем матрицу, представляющую лист
    vector<vector<bool>> grid(m, vector<bool>(n, false));
    cout << "Введите клетки для удаления (строка столбец) (0 - клетка удалена, 1 - клетка осталась):" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int cellValue;
            cin >> cellValue;
            grid[i][j] = cellValue == 0;
        }
    }

    // Замеряем время для countPieces
    auto startCountPieces = chrono::high_resolution_clock::now();
    int pieces = countPieces(grid, m, n);
    auto endCountPieces = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedCountPieces = endCountPieces - startCountPieces;
    cout << "Количество кусков (обычный): " << pieces << endl;
    cout << "Время выполнения countPieces: " << elapsedCountPieces.count() << " секунд." << endl;

    // Замеряем время для countPiecesCylinder
    auto startCountPiecesCylinder = chrono::high_resolution_clock::now();
    int piecesCylinder = countPiecesCylinder(grid, m, n);
    auto endCountPiecesCylinder = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedCountPiecesCylinder = endCountPiecesCylinder - startCountPiecesCylinder;
    cout << "Количество кусков (цилиндр): " << piecesCylinder << endl;
    cout << "Время выполнения countPiecesCylinder: " << elapsedCountPiecesCylinder.count() << " секунд." << endl;

    // Замеряем время для countPiecesList
    auto startCountPiecesList = chrono::high_resolution_clock::now();
    int piecesList = countPiecesList(grid, m, n);
    auto endCountPiecesList = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedCountPiecesList = endCountPiecesList - startCountPiecesList;
    cout << "Количество кусков (список): " << piecesList << endl;
    cout << "Время выполнения countPiecesList: " << elapsedCountPiecesList.count() << " секунд." << endl;

    // Замеряем время для countPiecesStack
    auto startCountPiecesStack = chrono::high_resolution_clock::now();
    int piecesStack = countPiecesStack(grid, m, n);
    auto endCountPiecesStack = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedCountPiecesStack = endCountPiecesStack - startCountPiecesStack;
    cout << "Количество кусков (стек): " << piecesStack << endl;
    cout << "Время выполнения countPiecesStack: " << elapsedCountPiecesStack.count() << " секунд." << endl;

    // Сравниваем времена выполнения
    double minTime = elapsedCountPieces.count();
    string fastestMethod = "countPieces";

    if (minTime > elapsedCountPiecesCylinder.count()) {
        minTime = elapsedCountPiecesCylinder.count();
        fastestMethod = "countPiecesCylinder";
    }

    if (minTime > elapsedCountPiecesList.count()) {
        minTime = elapsedCountPiecesList.count();
        fastestMethod = "countPiecesLinkedList";
    }

    if (minTime > elapsedCountPiecesStack.count()) {
        minTime = elapsedCountPiecesStack.count();
        fastestMethod = "countPiecesStack";
    }

    // Выводим самую быструю реализацию
    cout << "Самая быстрая реализация: " << fastestMethod << " с временем " << minTime << " секунд." << endl;

    return 0;
}