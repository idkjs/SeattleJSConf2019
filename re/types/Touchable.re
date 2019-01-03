[@bs.module "react-native-platform-touchable"]
external touchable : ReasonReact.reactClass = "default";

let make = (~onPress=?, ~delayPressIn=?, ~style=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=touchable,
    ~props=
      Js.Undefined.(
        {
          "onPress": fromOption(onPress),
          "delayPressIn": fromOption(delayPressIn),
          "style": fromOption(style)
        }
      ),
    children
  );
