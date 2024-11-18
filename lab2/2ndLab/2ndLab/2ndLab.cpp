#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <utility>

// Мьютекс для синхронизации записи в общий вектор
std::mutex result_mutex;

// Функция для вычисления НОД с использованием алгоритма Евклида
int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

// Функция, которую выполняет каждый поток
void process_tasks(const std::vector < std::pair<int, int>>& tasks, std::vector<int>& results, int start_index) {
	for (size_t i = 0; i < tasks.size(); ++i) {
		int result = gcd(tasks[i].first, tasks[i].second);

		// Записываем результат в общий вектор с учетом начального индекса
		std::lock_guard<std::mutex> lock(result_mutex);
		results[start_index + i] = result;
	}
}

int main() {
	const std::string input_filename = "input.txt";
	const std::string output_filename = "output.txt";
	const int num_threads = 4; // Количество потоков

	// Чтение всех задач из файла в основной поток
	std::vector < std::pair<int, int>> all_tasks;
	std::ifstream input_file(input_filename);
	int a, b;
	while (input_file >> a >> b) {
		all_tasks.emplace_back(a, b);
	}

	// Инициализируем вектор для хранения результатов
	std::vector<int> results(all_tasks.size());

	// Разбиваем задачи на равные части для каждого потока
	std::vector<std::thread> threads;
	int tasks_per_thread = all_tasks.size() / num_threads;
	for (int i = 0; i < num_threads; ++i) {
		int start_index = i * tasks_per_thread;
		auto start_itr = all_tasks.begin() + start_index;
		auto end_itr = (i == num_threads - 1) ? all_tasks.end() : start_itr + tasks_per_thread;
		std::vector < std::pair<int, int>> thread_tasks(start_itr, end_itr);

		threads.emplace_back(process_tasks, thread_tasks, std::ref(results), start_index);
	}

	// Дожидаемся завершения всех потоков
	for (auto& thread : threads) {
		thread.join();
	}

	// Очищаем файл результатов перед записью
	std::ofstream output_file(output_filename);
	for (const auto& res : results) {
		output_file << res << std::endl;
	}

	// Русский текст в выводе без использования setlocale
	setlocale(LC_ALL, "Russian");
	std::cout << "Задачи выполнены. Результаты сохранены в файле " << output_filename << std::endl;
	return 0;
}
