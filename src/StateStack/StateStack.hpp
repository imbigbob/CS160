#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <functional>
#include <SFML/System/NonCopyable.hpp>
#include "../State/State.hpp"

class StateStack : private sf::NonCopyable {
   public:
    enum Action {
        Push,
        Pop,
        Clear,
    };

    explicit StateStack(const State::Context& context);

    template <typename T, typename... Params>
    void registerState(States::ID stateID, Params... parameters);

    void handleEvent(const sf::Event& event);
    void update(sf::Time deltaTime);
    void draw();

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

   private:
    struct PendingChange {
        explicit PendingChange(
            Action action, States::ID stateID = States::ID::None
        );

        Action action;
        States::ID stateID;
    };

    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;

    State::Context mContext;
    std::map<States::ID, std::function<State::Ptr()>> mFactories;

    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();
};

template <typename T, typename... Params>
void StateStack::registerState(States::ID stateID, Params... parameters) {
    mFactories[stateID] = [this, parameters...]() {
        return State::Ptr(new T(*this, mContext, parameters...));
    };
}

#endif
