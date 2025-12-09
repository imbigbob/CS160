#include "TransactionState.hpp"

#include <fstream>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
TransactionState::TransactionState(StateStack& stack, Context context)
    : State(stack, context) {
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture& backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y
    );

    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back"
    );
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]() { requestStackPop(); });
    auto addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add"
    );
    addButton->setPosition(400.f, 50.f);
    addButton->setCallback([this]() { /* TODO: Add transaction */ });

    auto toggleButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Show Expense"
    );
    toggleButton->setPosition(550.f, 50.f);

    toggleButton->setCallback([this, toggleButton]() {
        // Switch mode
        if (mMode == Mode::Income) {
            mMode = Mode::Expense;
            toggleButton->setText("Show Income");
        } else {
            mMode = Mode::Income;
            toggleButton->setText("Show Expense");
        }

        reloadTable();  // refresh rows
    });
    // Table position and size
    float tableX = 150.f;
    float tableY = 150.f;
    float colWidth[] = {
        180.f, 180.f, 120.f, 200.f
    };  // date, source, amount, description
    float rowHeight = 40.f;

    //--------- Header Rectangle ----------
    sf::RectangleShape headerRect;
    headerRect.setSize(
        sf::Vector2f(
            colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3], rowHeight
        )
    );
    headerRect.setPosition(tableX, tableY);
    headerRect.setFillColor(sf::Color(230, 230, 230));

    mTableHeader = headerRect;  // store in class

    //--------- Header Texts ----------
    sf::Font& font = context.fontHolder->get(Fonts::ID::Dosis);

    std::vector<std::string> headers = {
        "Date", "Source", "Amount", "Description"
    };

    float currentX = tableX;

    for (int i = 0; i < headers.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(currentX + 10.f, tableY + 8.f);

        mHeaderTexts.push_back(text);
        currentX += colWidth[i];
    }

    //-------------------------------------------------------------
    // ROWS (dummy example, replace with your Income list later)
    //-------------------------------------------------------------

    struct RowData {
        std::string date;
        std::string source;
        std::string amount;
        std::string description;
    };

    DynamicArray<Income>& exampleRows =
        mIncomeManager.getAll();  // Load incomes

    float currentY = tableY + rowHeight;

    for (int i = 0; i < exampleRows.getSize(); i++) {
        Income& r = exampleRows[i];

        // ----------- Row background -----------
        sf::RectangleShape rowRect;
        rowRect.setSize(
            sf::Vector2f(
                colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3], rowHeight
            )
        );
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor(sf::Color(245, 245, 245));

        mRowRects.push_back(rowRect);

        // ----------- Convert row to strings -----------
        std::vector<std::string> items = {
            r.getDate(), r.getName(), std::to_string(r.getAmount()),
            r.getDescription()
        };

        // ----------- Row text -----------
        float cx = tableX;
        for (int col = 0; col < items.size(); col++) {
            sf::Text t;
            t.setFont(font);
            t.setString(items[col]);
            t.setCharacterSize(18);
            t.setFillColor(sf::Color::Black);
            t.setPosition(cx + 10.f, currentY + 8.f);

            mRowTexts.push_back(t);

            cx += colWidth[col];
        }

        currentY += rowHeight;
    }
    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(addButton);
    mGUIContainer.addComponent(toggleButton);
}
void TransactionState::reloadTable() {
    mRowRects.clear();
    mRowTexts.clear();

    float tableX = 150.f;
    float tableY = 150.f;
    float colWidth[] = {180.f, 180.f, 120.f, 200.f};
    float rowHeight = 40.f;
    float currentY = tableY + rowHeight;

    sf::Font& font = getContext().fontHolder->get(Fonts::ID::Dosis);

    // ---------------------- SELECT DATA -------------------------
    DynamicArray<Income>* incomes = nullptr;
    DynamicArray<Expense>* expenses = nullptr;

    if (mMode == Mode::Income) {
        incomes = &mIncomeManager.getAll();
    } else {
        expenses = &mExpenseManager.getAll();
    }

    // ---------------------- BUILD ROWS --------------------------
    if (mMode == Mode::Income) {
        for (int i = 0; i < incomes->getSize(); i++) {
            const Income& r = (*incomes)[i];

            sf::RectangleShape rowRect;
            rowRect.setSize(
                sf::Vector2f(
                    colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3],
                    rowHeight
                )
            );
            rowRect.setPosition(tableX, currentY);
            rowRect.setFillColor(sf::Color(245, 245, 245));
            mRowRects.push_back(rowRect);

            std::vector<std::string> items = {
                r.getDate(), r.getName(), std::to_string(r.getAmount()),
                r.getDescription()
            };

            float cx = tableX;
            for (auto& s : items) {
                sf::Text t;
                t.setFont(font);
                t.setString(s);
                t.setCharacterSize(18);
                t.setFillColor(sf::Color::Black);
                t.setPosition(cx + 10.f, currentY + 8.f);
                mRowTexts.push_back(t);

                cx += colWidth[mRowTexts.size() % 4];
            }

            currentY += rowHeight;
        }
    } else {
        for (int i = 0; i < expenses->getSize(); i++) {
            const Expense& r = (*expenses)[i];

            sf::RectangleShape rowRect;
            rowRect.setSize(
                sf::Vector2f(
                    colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3],
                    rowHeight
                )
            );
            rowRect.setPosition(tableX, currentY);
            rowRect.setFillColor(sf::Color(245, 245, 245));
            mRowRects.push_back(rowRect);

            std::vector<std::string> items = {
                r.getDate(), r.getName(), std::to_string(r.getAmount()),
                r.getDescription()
            };

            float cx = tableX;
            for (auto& s : items) {
                sf::Text t;
                t.setFont(font);
                t.setString(s);
                t.setCharacterSize(18);
                t.setFillColor(sf::Color::Black);
                t.setPosition(cx + 10.f, currentY + 8.f);
                mRowTexts.push_back(t);

                cx += colWidth[mRowTexts.size() % 4];
            }

            currentY += rowHeight;
        }
    }
}
bool TransactionState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}

bool TransactionState::update(sf::Time deltaTime) { return true; }

void TransactionState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mTableHeader);

    for (auto& t : mHeaderTexts) window.draw(t);

    for (auto& r : mRowRects) window.draw(r);

    for (auto& t : mRowTexts) window.draw(t);
    window.draw(mGUIContainer);
}
