#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>

// Мьютекс для синхронизации доступа к общим файлам
std::mutex file_mutex;

// Функция для вычисления НОД с использованием алгоритма Евклида
int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

// Функция, которую будет выполнять каждый поток
void process_tasks(const std::string& input_filename, const std::string& output_filename, int buffer_size) {
	std::ifstream input_file(input_filename);
	std::vector < std::pair<int, int>> tasks;
	std::vector<int> results;
	results.reserve(buffer_size);

	// Чтение данных из файла
	{
		std::lock_guard<std::mutex> lock(file_mutex);
		int a, b;
		while (input_file >> a >> b) {
			tasks.emplace_back(a, b);
		}
	}

	// Обработка задач и вычисление НОД
	for (const auto& task : tasks) {
		int result = gcd(task.first, task.second);
		results.push_back(result);

		// Если буфер заполнен, записываем результаты в файл
		if (results.size() >= buffer_size) {
			std::lock_guard<std::mutex> lock(file_mutex);
			std::ofstream output_file(output_filename, std::ios::app);
			for (int res : results) {
				output_file << res << std::endl;
			}
			results.clear();
		}
	}

	// Записываем оставшиеся результаты в файл, если они есть
	if (!results.empty()) {
		std::lock_guard<std::mutex> lock(file_mutex);
		std::ofstream output_file(output_filename, std::ios::app);
		for (int res : results) {
			output_file << res << std::endl;
		}
	}
}

int main() {
	const std::string input_filename = "input.txt";
	const std::string output_filename = "output.txt";
	const int num_threads = 4; // Количество потоков
	const int buffer_size = 10; // Размер буфера для записи

	// Создаем потоки
	std::vector<std::thread> threads;
	for (int i = 0; i < num_threads; ++i) {
		threads.emplace_back(process_tasks, input_filename, output_filename, buffer_size);
	}

	// Дожидаемся завершения всех потоков
	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "Задачи выполнены. Результаты сохранены в файле " << output_filename << std::endl;
	return 0;
}