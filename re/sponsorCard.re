open BsReactNative;

let component = ReasonReact.statelessComponent("SponsorTitle");

let styles =
  StyleSheet.create(
    Style.(
      {
        "card":
          style([
            flex(0.),
            backgroundColor(String("white")),
            padding(Pt(20.)),
            marginHorizontal(Pt(20.)),
            marginVertical(Pt(10.)),
            overflow(Hidden)
          ]),
        "imageContain": style([flex(1.), overflow(Hidden), maxHeight(Pt(100.))]),
        "logo": style([width(Pct(90.)), alignSelf(Center), maxHeight(Pt(100.)), overflow(Hidden)]),
        "description":
          style([
            fontFamily("open-sans"),
            fontSize(Float(16.)),
            color(String("black")),
            textAlign(Left),
            backgroundColor(String("transparent")),
            marginVertical(Pt(10.))
          ])
      }
    )
  );

let make = (~logo, ~url, ~description=?, _children) => {
  ...component,
  render: (_self) =>
    <TouchableOpacity onPress=((_) => Link.openURL(url))>
      <View style=styles##card>
        <View style=styles##imageContain>
          <Image source=logo style=styles##logo resizeMode=`contain />
        </View>
        (
          switch description {
          | None => ReasonReact.null
          | Some(d) => <Text value=d style=styles##description />
          }
        )
      </View>
    </TouchableOpacity>
};
