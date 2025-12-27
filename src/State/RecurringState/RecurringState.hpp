#ifndef _RECURRING_STATE_HPP
#define _RECURRING_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"
#include "../../core/RecurringManager/RecurringManager.hpp"
class RecurringState : public State
{
public:
    RecurringState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;

private:
    void reloadTable();
    void updateScrollView();
    void handleEdit(int index);
    void handleDelete(int index);
    void handleAdd();

private:
    enum class Mode
    {
        Income,
        Expense,
    };

    Mode currentMode;
    sf::Sprite mBackgroundSprite;
    std::vector<sf::Sprite> mEditSprites;
    std::vector<sf::Sprite> mDeleteSprites;
    bool mNeedsRefresh = false;

    // Helper to handle the actual logic

    GUI::Container mGUIContainer;

    // Buttons tracked for text updates
    std::shared_ptr<GUI::Button> modeSwitchButton;
    std::shared_ptr<GUI::Button> addButton;

    // Assuming you have these managers defined elsewhere or locally
    RecurringManager recurringManager;

    sf::RectangleShape mTableHeader;
    std::vector<sf::Text> mHeaderTexts;

    // --- Table Content (Scrollable) ---
    std::vector<sf::RectangleShape> mRowRects; // Backgrounds for rows
    std::vector<sf::Text> mRowTexts;           // Text data for rows

    // --- Scrolling Mechanics ---
    sf::View mTableView;        // Camera for the list
    sf::FloatRect mTableBounds; // Visible area on screen
    float mScrollY;             // Current scroll position
    float mTotalContentHeight;  // Total height of all rows
};
#endif