#ifndef _STATISTICS_STATE_HPP
#define _STATISTICS_STATE_HPP

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"
#include "../../core/StatisticManager/StatisticManager.hpp"
#include "../GUI/InputBox/InputBox.hpp"
class StatisticsState : public State
{
private:
    StatisticManager mStatisticManager;
    sf::Sprite mBackgroundSprite;

    GUI::Container mGUIContainer;

    // Inputs
    std::shared_ptr<GUI::InputBox> mStartDateInput;
    std::shared_ptr<GUI::InputBox> mEndDateInput;
    std::shared_ptr<GUI::InputBox> mYearInput;
    std::shared_ptr<GUI::InputBox> mSourceInput;
    std::shared_ptr<GUI::InputBox> mCategoryInput;
    sf::RectangleShape mTableBackground1;
    sf::RectangleShape mTableBackground2;

    // Output Labels (The "Table" values)
    // We map a key (string) to the label so we can find it easily
    std::map<std::string, std::shared_ptr<GUI::Label>> mValueLabels;

private:
    void updateStatistics();                               // The function that does the math
    DynamicArray<int> parseYears(const std::string &text); // Helper

public:
    StatisticsState(StateStack &stack, Context context);

    bool handleEvent(const sf::Event &event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;
};

#endif
