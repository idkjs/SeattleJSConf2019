open BsReactNative;

let component = ReasonReact.statelessComponent("TalkDescription");

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([padding(Pt(10.)), marginTop(Pt(-10.))]),
        "text": style([fontFamily("open-sans"), fontSize(Float(16.)), color(String("white"))])
      }
    )
  );

let make = (~value, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##wrapper>
      <Text style=styles##text> (Utils.str(value)) </Text>
    </View>
};
