#ifndef IDENTIFIER_HPP
#define IDENTIFIER_HPP

namespace States {
enum class ID {
    Menu,
    Transaction,
    RecurringTransaction,
    Statistics,
    Management,

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

    TitleBackground,
    MenuBackground,
    Frame,
    Add,

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
