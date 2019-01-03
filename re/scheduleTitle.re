open BsReactNative;

let component = ReasonReact.statelessComponent("ScheduleTitle");

let styles =
  StyleSheet.create(
    Style.(
      {
        "title":
          style([fontFamily("open-sans-bold"), fontSize(Float(16.)), color(String("white")), marginBottom(Pt(4.))])
      }
    )
  );

let make = (~talkTitle, _children) => {
  ...component,
  render: (_self) => <Text style=styles##title value=talkTitle />
};
