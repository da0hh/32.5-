#include "belskiyNik.h"

using json = nlohmann::json;

// Структура для результата поиска
struct RoleInfo {
    std::string movie;
    std::string character;
};

// Приводит строку к нижнему регистру (работает с UTF-8 кириллицей)
std::string toLower(const std::string& str) {
    std::string result;
    for (char c : str) {
        if (c >= 'A' && c <= 'Z') {
            result += c + ('a' - 'A');
        }
        else if (c >= 'А' && c <= 'Я') {  // Кириллица заглавная
            result += c + ('а' - 'А');
        }
        else {
            result += c;
        }
    }
    return result;
}

// Удаляет пробелы и приводит к нижнему регистру
std::string normalize(const std::string& str) {
    std::string result;
    for (char c : toLower(str)) {
        if (!std::isspace(c)) {
            result += c;
        }
    }
    return result;
}

// Поиск: по актёру ИЛИ по персонажу (регистронезависимо, без пробелов)
std::vector<RoleInfo> findPerson(const json& db, const std::string& query) {
    std::vector<RoleInfo> results;
    std::string queryNorm = normalize(query);

    for (auto it = db.begin(); it != db.end(); ++it) {
        std::string movieTitle = it.key();
        const json& movie = it.value();

        if (movie.contains("main_cast")) {
            for (const auto& castMember : movie["main_cast"]) {
                std::string actorName = castMember["actor"];
                std::string characterName = castMember["character"];

                std::string actorNorm = normalize(actorName);
                std::string charNorm = normalize(characterName);

                if (actorNorm.find(queryNorm) != std::string::npos ||
                    charNorm.find(queryNorm) != std::string::npos) {
                    results.push_back({ movieTitle, characterName });
                }
            }
        }
    }
    return results;
}

int main() {
    setlocale(LC_ALL, "RU");
    std::ifstream file("movies.json");
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть movies.json\n";
        return 1;
    }

    json db;
    try {
        file >> db;
    }
    catch (json::parse_error& e) {
        std::cerr << "Ошибка парсинга JSON: " << e.what() << "\n";
        return 1;
    }

    // 2. Ввод запроса
    std::string query;
    std::cout << "Введите имя актёра или персонажа для поиска: ";
    std::getline(std::cin, query);

    // 3. Поиск
    auto results = findPerson(db, query);

    // 4. Вывод результатов
    if (results.empty()) {
        std::cout << "Не найдено.\n";

        // Для отладки: показать всех актёров/персонажей
        std::cout << "\nДоступные актёры и персонажи:\n";
        for (auto it = db.begin(); it != db.end(); ++it) {
            const json& movie = it.value();
            if (movie.contains("main_cast")) {
                for (const auto& castMember : movie["main_cast"]) {
                    std::cout << "- Актёр: " << castMember["actor"]
                        << " | Персонаж: " << castMember["character"]
                        << " (фильм: " << it.key() << ")\n";
                }
            }
        }
    }
    else {
        std::cout << "Найденные совпадения:\n";
        for (const auto& role : results) {
            std::cout << "- Фильм: " << role.movie
                << " | Персонаж: " << role.character << "\n";
        }
    }

    return 0;
}
