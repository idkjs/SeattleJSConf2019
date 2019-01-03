open BsReactNative;

let component = ReasonReact.statelessComponent("CloseButton");

let styles =
  StyleSheet.create(
    Style.(
      {
        "text":
          style([
            fontFamily("open-sans"),
            fontSize(Float(18.)),
            color(String("white")),
            textAlign(Center),
            backgroundColor(String("transparent"))
          ]),
        "button":
          style([
            backgroundColor(String("rgb(140, 170, 29)")),
            padding(Pt(10.)),
            margin(Pt(10.)),
            borderRadius(24.),
            flex(0.)
          ])
      }
    )
  );

let make = (~onPress, _children) => {
  ...component,
  render: (_self) =>
    <TouchableOpacity onPress>
      <View style=styles##button> <Text style=styles##text value="Close" /> </View>
    </TouchableOpacity>
};