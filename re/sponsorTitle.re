open BsReactNative;

let component = ReasonReact.statelessComponent("SponsorTitle");

let styles =
  StyleSheet.create(
    Style.(
      {
        "titleWrap": style([paddingVertical(Pt(10.)), marginHorizontal(Pt(10.))]),
        "sponsorTitle":
          style([
            fontFamily("montserrat-bold"),
            fontSize(Float(18.)),
            color(String("white")),
            textAlign(Center),
            backgroundColor(String("transparent"))
          ])
      }
    )
  );

let make = (~value, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##titleWrap> <Text value style=styles##sponsorTitle /> </View>
};
