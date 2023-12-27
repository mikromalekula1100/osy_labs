#include <chrono>
#include <iostream>
#include <thread>
int main() {
    // Запуск секундомера
    auto start = std::chrono::high_resolution_clock::now();

    // Здесь выполняется ваш код
    std::this_thread::sleep_for(std::chrono::seconds(3));
    // Остановка секундомера
    auto stop = std::chrono::high_resolution_clock::now();

    // Вычисление затраченного времени
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Время выполнения: " << duration.count() << " микросекунд" << std::endl;

    return 0;
}