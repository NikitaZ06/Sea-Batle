#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// Шаблонный класс для статистики игроков
template<typename T>
class PlayerStatistics {
private:
    std::vector<T> scores;  // Контейнер для хранения очков
    std::vector<std::string> playerNames;  // Контейнер для хранения имен

public:
    // Конструктор
    PlayerStatistics() = default;

    // Не шаблонный метод
    void addScore(const std::string& playerName, T score) {
        playerNames.push_back(playerName);
        scores.push_back(score);
    }

    // Шаблонный метод для преобразования очков
    template<typename U>
    std::vector<U> getConvertedScores(std::function<U(T)> converter) const {
        std::vector<U> result;
        result.reserve(scores.size());
        for (const auto& score : scores) {
            result.push_back(converter(score));
        }
        return result;
    }

    // Алгоритм сортировки по очкам (по убыванию)
    void sortByScoreDescending() {
        // Создаем пары для сортировки
        std::vector<std::pair<T, std::string>> pairs;
        for (size_t i = 0; i < scores.size(); i++) {
            pairs.emplace_back(scores[i], playerNames[i]);//Добавляет новую пару в конец вектора 
        }

        // Сортируем по убыванию очков
        std::sort(pairs.begin(), pairs.end(),
            [](const auto& a, const auto& b) {
                return a.first > b.first;
            });

        // Обновляем исходные векторы
        scores.clear();
        playerNames.clear();
        for (const auto& pair : pairs) {
            scores.push_back(pair.first);
            playerNames.push_back(pair.second);
        }
    }

    // Алгоритм поиска игрока по имени
    T findScoreByName(const std::string& name) const {
        auto it = std::find(playerNames.begin(), playerNames.end(), name);
        if (it != playerNames.end()) {
            size_t index = std::distance(playerNames.begin(), it);
            return scores[index];
        }
        return T();  // Возвращаем значение по умолчанию
    }

    // Алгоритм поиска лучшего счета
    T getHighestScore() const {
        if (scores.empty()) return T();
        return *std::max_element(scores.begin(), scores.end());
    }

    // Геттеры
    const std::vector<T>& getScores() const { return scores; }
    const std::vector<std::string>& getPlayerNames() const { return playerNames; }

    void displayStats() const {
        std::cout << "\n=== СТАТИСТИКА ИГРОКОВ ===" << std::endl;
        for (size_t i = 0; i < playerNames.size(); i++) {
            std::cout << playerNames[i] << ": " << scores[i] << " очков" << std::endl;
        }
    }
};