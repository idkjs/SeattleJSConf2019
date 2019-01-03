open BsReactNative;

let component = ReasonReact.statelessComponent("ScheduleTime");

let styles =
  StyleSheet.create(
    Style.(
      {
        "timeWrapper": style([paddingLeft(Pt(10.)), paddingBottom(Pt(10.)), marginTop(Pt(-5.))]),
        "time": style([fontFamily("open-sans-bold"), fontSize(Float(12.)), color(String("white"))])
      }
    )
  );

let make = (~start, _children) => {
  ...component,
  render: (_self) =>
    <View style=styles##timeWrapper>
      <Text style=styles##time>
        (
          Utils.str(
            Moment.(moment(start) |> Moment.utcOffset("-04:00") |> Moment.format("hh:mm a"))
          )
        )
      </Text>
    </View>
};
