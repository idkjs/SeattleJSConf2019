open BsReactNative;

type state = {scrollRef: ref(option(ReasonReact.reactRef))};

let setRef = (theRef, {ReasonReact.state}) => state.scrollRef := Js.toOption(theRef);

let styles =
  StyleSheet.create(
    Style.(
      {
        "item": style([flex(1.)]),
        "row": style([flexDirection(Row)]),
        "titleWrap": style([flex(1.), padding(Pt(10.))]),
        "wrapper":
          style([
            position(Relative),
            flex(1.),
            backgroundColor(String("rgb(75, 118, 134)")),
            borderWidth(1.),
            borderColor(String("rgb(137, 167, 177)"))
          ])
      }
    )
  );

let component = ReasonReact.reducerComponent("ItemModal");

let make =
    (
      ~item as {Item.start, talk, title},
      ~contentOpacity,
      ~backgroundOpacity,
      ~expanded,
      ~onClose: unit => unit,
      _children
    ) => {
  ...component,
  reducer: ((), _) => ReasonReact.NoUpdate,
  initialState: () => {scrollRef: ref(None)},
  render: (self) => {
    let fadingContainerStyle = (value) => Style.(style([opacity(value)]));
    let contentContainerOpacity = fadingContainerStyle(contentOpacity);
    <Animated.View
      style=Style.(concat([styles##wrapper, fadingContainerStyle(backgroundOpacity)]))>
      <ScrollView style=styles##item ref=(self.handle(setRef))>
        <View style=styles##row>
          <View style=styles##titleWrap>
            (
              switch talk {
              | None => <ScheduleTitle talkTitle=title />
              | Some({Item.talkTitle}) => <ScheduleTitle talkTitle />
              }
            )
            (
              switch talk {
              | None => ReasonReact.null
              | Some(talk) => <SpeakerNames talk />
              }
            )
          </View>
          (
            switch talk {
            | None => ReasonReact.null
            | Some(talk) => <SpeakerImages talk />
            }
          )
        </View>
        <ScheduleTime start />
        (
          switch expanded {
          | false => ReasonReact.null
          | true =>
            switch talk {
            | None => ReasonReact.null
            | Some({Item.description, speakers}) =>
              <Animated.View style=contentContainerOpacity>
                <TalkDescription value=description />
                <SpeakerBio speakers />
              </Animated.View>
            }
          }
        )
      </ScrollView>
      (
        switch expanded {
        | false => ReasonReact.null
        | true =>
          <Animated.View style=contentContainerOpacity>
            <CloseButton
              onPress=(
                () => {
                  switch self.state.scrollRef^ {
                  | None => ()
                  | Some(s) =>
                    ReasonReact.refToJsObj(s)##scrollTo([%bs.raw "{y: 0, animated: false}"])
                  };
                  onClose()
                }
              )
            />
          </Animated.View>
        }
      )
    </Animated.View>
  }
};