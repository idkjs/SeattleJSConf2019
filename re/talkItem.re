open BsReactNative;

type state = {elementRef: ref(option(ReasonReact.reactRef))};

let setElementRef = (theRef, {ReasonReact.state}) => state.elementRef := Js.toOption(theRef);

let styles =
  StyleSheet.create(
    Style.(
      {
        "item":
          style([
            flex(1.),
            backgroundColor(String("rgb(75, 118, 134)")),
            marginHorizontal(Pt(10.)),
            marginTop(Pt(10.)),
            borderWidth(1.),
            borderColor(String("rgb(137, 167, 177)"))
          ]),
        "row": style([flexDirection(Row)]),
        "titleWrap": style([flex(1.), padding(Pt(10.)),])
      }
    )
  );

let component = ReasonReact.reducerComponent("TalkItem");

let make =
    (
      ~item as {Item.start} as item,
      ~talk as {Item.talkTitle} as talk,
      ~index: int,
      ~modalOpen: bool,
      ~selectedIndex: int,
      ~onPress,
      _children
    ) => {
  ...component,
  reducer: ((), _) => ReasonReact.NoUpdate,
  initialState: () => {elementRef: ref(None)},
  render: (self) =>
    <TouchableOpacity
      onPress=(
        (_) =>
          switch self.state.elementRef^ {
          | None => ()
          | Some(r) =>
            ReasonReact.refToJsObj(r)##measureInWindow(
              (x, y, width, height) => onPress(x, y, width, height, item, index)
            )
          }
      )>
      <View
        style=Style.(
                concat([
                  styles##item,
                  style([opacity(Float(modalOpen && index === selectedIndex ? 0. : 1.))])
                ])
              )
        ref=(self.handle(setElementRef))>
        <View style=styles##row>
          <View style=styles##titleWrap> <ScheduleTitle talkTitle /> <SpeakerNames talk /> </View>
          <SpeakerImages talk />
        </View>
        <ScheduleTime start />
      </View>
    </TouchableOpacity>
};