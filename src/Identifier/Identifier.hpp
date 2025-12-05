#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

namespace States {
enum class ID {
    Title,
    Menu,

    NewSingleGame,
    SelectPlayer1Single,

    NewMultiplayerGame,
    SelectPlayer1Multi,
    SelectPlayer2Multi,

    LoadSingleGame,
    LoadMultiplayerGame,

    HighScore,
    Settings,

    Pause,
    GameOver,
    MultiplayerGameOverDead1,
    MultiplayerGameOverDead2,
    None,
};
}

namespace Directions {
enum class ID {
    Up,
    Down,
    Left,
    Right,
    DirectionCount,
    None,
};
}

namespace Textures {
enum class ID {
    PlayerChoice1,
    PlayerChoice2,
    PlayerChoice3,
    PlayerChoice4,
    PlayerChoice5,
    PlayerChoice6,

    Player1SelectedChoiceSingle,
    Player1SelectedChoiceMulti,
    Player2SelectedChoiceMulti,

    Halo,

    TitleBackground,
    MenuBackground,
    SettingBackground,
    Frame,

    Bus,
    Car,
    PoliceCar,
    Train,
    Van,

    Cat,
    Cow,
    Dog,
    Horse,
    Lion,

    Rock,
    Tree,

    Log,
    LongLog,
    MediumLog,
    ShortLog,

    RailwaySignal,
    TrafficLight,

    ObstacleLane,
    RiverLane,
    TrainLane,
    VehicleLane,

    ButtonBack,
    ButtonNormal,
    ButtonSelected,
    ButtonPressed,
    LeftButtonNormal,
    RightButtonNormal,
    LeftButtonSelected,
    RightButtonSelected,

    None,
};
}

namespace Fonts {
enum class ID {
    Dosis,
    Pacifico,
    VTV323,
    Minecraft,
};
}

#endif
