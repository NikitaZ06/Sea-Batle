#pragma once
#include "GameObject.hpp"
#include <vector>

//  ласс дл€ демонстрации поверхностного и глубокого клонировани€
class ShipInfo : public GameObject {
private:
    int size;
    std::string shipType;
    std::vector<int>* positions; // ”казатель дл€ демонстрации глубокого клонировани€

public:
    ShipInfo(const std::string& name, const std::string& type, int size);
    ShipInfo(const ShipInfo& other); //  онструктор копировани€
    ~ShipInfo();

    void update() override;
    void display() const override;
    std::string getType() const override;
    void performAction() const override;
    GameObject* clone() const override;

    // ћетод дл€ демонстрации работы с указателем
    void addPosition(int pos);
    void showPositions() const;
};

