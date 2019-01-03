open BsReactNative;

let component = ReasonReact.statelessComponent("SpeakerNames");

let styles =
  StyleSheet.create(
    Style.(
      {
        "speaker":
          style([
            fontFamily("montserrat"),
            fontSize(Float(16.)),
            letterSpacing(1.),
            color(String("rgb(185,219,111)")),
            marginBottom(Pt(6.))
          ])
      }
    )
  );

let make = (~talk: Item.talk, _children) => {
  ...component,
  render: (_self) =>
    <Text style=styles##speaker>
      (
        Array.mapi(
          (i, {Item.name}) =>
            i > 0 ?
              Utils.str(String.uppercase(" & " ++ name)) :
              Utils.str(String.uppercase(name)),
          talk.speakers
        )
        |> ReasonReact.array
      )
    </Text>
};
