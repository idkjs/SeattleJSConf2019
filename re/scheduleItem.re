open BsReactNative;

let styles =
  StyleSheet.create(
    Style.(
      {
        "item":
          style([
            flex(1.),
            marginHorizontal(Pt(10.)),
            marginTop(Pt(10.)),
            borderWidth(1.),
            borderColor(String("transparent"))
          ]),
        "row": style([flexDirection(Row)]),
        "titleWrap": style([flex(1.), padding(Pt(10.)),])
      }
    )
  );

let component = ReasonReact.statelessComponent("ScheduleItem");

let make = (~item as {Item.title, start}, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##item>
      <View style=styles##row>
        <View style=styles##titleWrap> <ScheduleTitle talkTitle=title /> </View>
      </View>
      <ScheduleTime start />
    </View>
};
