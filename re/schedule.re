open BsReactNative;

/* Types */
type sourceItemDimensions = {
  x: float,
  y: float,
  width: float,
  height: float
};

type modalAnimationState = {
  top: Animated.Value.t,
  left: Animated.Value.t,
  width: Animated.Value.t,
  height: Animated.Value.t,
  contentOpacity: Animated.Value.t,
  backgroundOpacity: Animated.Value.t
};

type modalState =
  | Collapsing(Item.t, sourceItemDimensions, modalAnimationState)
  | Expanding(Item.t, sourceItemDimensions, modalAnimationState)
  | Expanded(Item.t, sourceItemDimensions, modalAnimationState)
  | Closed;

type state = {
  modalState,
  selectedIndex: int,
  listOpacity: Animated.Value.t,
  mutable listRef: option(ReasonReact.reactRef)
};

/* Variables */
let animationDuration = 300.;

let statusOffset = Utils.switchPlatform(0., float_of_int(StatBar.constants##currentHeight));

let listPadding = 10;

let iosBarHeight = 20.;

let deferDuration = 0;

let windowWidth = Dimensions.get(`window)##width;

let easeIn = (x) => x *. x *. x;

/* Methods */
let closeModal = (_, {ReasonReact.state}) => ReasonReact.Update({...state, modalState: Closed});

let setRef = (theRef, {ReasonReact.state}) => state.listRef = Js.toOption(theRef);

/* Component */
type action =
  | Close
  | Expand(int, int, int, int, Item.t, int)
  | CollapseFinish
  | ExpandFinish(Item.t, sourceItemDimensions, modalAnimationState);

let startExpand = (x, y, width, height, item, index) => Expand(x, y, width, height, item, index);

let startClose = (_event) => Close;

let component = ReasonReact.reducerComponent("Schedule");

/* Styles */
let styles =
  StyleSheet.create(
    Style.(
      {
        "container":
          style([
            flex(1.),
            alignItems(Stretch),
            justifyContent(Center),
            backgroundColor(String("rgb(54, 97, 115)")),
            paddingTop(Pt(Utils.switchPlatform(20., float_of_int(StatBar.constants##currentHeight))))
          ]),
        "scrollView": style([flex(1.)]),
        "contentContainer":
          style([paddingBottom(Pt(10.)), flexGrow(1.), width(Pt(float_of_int(windowWidth)))]),
        "modal": style([position(Absolute), bottom(Pt(5.)), right(Pt(5.)), width(Pt(25.)), height(Pt(25.))]),
        "date":
          style([
            fontFamily("open-sans-bold"),
            letterSpacing(1.),
            color(String("rgb(185,219,111)")),
            backgroundColor(String("rgb(54, 97, 115)")),
            textAlign(Center),
            fontSize(Float(18.)),
            padding(Pt(10.))
          ]),
        "dateWrap": style([borderBottomWidth(1.), borderBottomColor(String("rgba(255,255,255,0.75)"))]),
        "banner":
          style([
            width(Pt(float_of_int(windowWidth))),
            height(Pt(float_of_int(windowWidth) *. 0.656)),
            marginBottom(Pt(0.))
          ])
      }
    )
  );

let onExpandAnimationFinish = ((item, sourceItemDimensions, modalAnimationState)) =>
  ExpandFinish(item, sourceItemDimensions, modalAnimationState);

let onCollapseAnimationFinish = (_) => CollapseFinish;

let animateExpandModal =
    (~listRef, ~listOpacity, ~modalAnimationState, ~item, ~sourceItemDimensions, ~onFinish) => {
  let {top, width, height, contentOpacity} = modalAnimationState;
  let windowWidth = Dimensions.get(`window)##width - listPadding * 2;
  switch listRef {
  | None => ()
  | Some(r) =>
    ReasonReact.refToJsObj(r)##measure(
      (_x, _y, _width, _height) => {
        let wh = _height - listPadding * 2;
        /* Define animations */
        let animationBatch =
          Animated.parallel(
            [|
              Animated.timing(
                ~value=listOpacity,
                ~toValue=`raw(0.),
                ~easing=easeIn,
                ~duration=animationDuration,
                ()
              ),
              Animated.timing(
                ~value=top,
                ~toValue=
                  `raw(
                    Utils.switchPlatform(
                      iosBarHeight +. float_of_int(listPadding),
                      statusOffset +. float_of_int(listPadding)
                      )
                  ),
                ~easing=easeIn,
                ~duration=animationDuration,
                ()
              ),
              Animated.timing(
                ~value=width,
                ~toValue=`raw(float_of_int(windowWidth)),
                ~easing=easeIn,
                ~duration=animationDuration,
                ()
              ),
              Animated.timing(
                ~value=height,
                ~toValue=`raw(float_of_int(wh)),
                ~easing=easeIn,
                ~duration=animationDuration,
                ()
              )
            |],
            [%bs.raw "{stopTogether: true}"]
          );
        /* Start animations */
        Animated.start(
          animationBatch,
          ~callback=
            (_) => {
              let _id =
                Js.Global.setTimeout(
                  (_) => {
                    onFinish((item, sourceItemDimensions, modalAnimationState));
                    let _id =
                      Js.Global.setTimeout(
                        (_) => {
                          let animation =
                            Animated.timing(
                              ~value=contentOpacity,
                              ~toValue=`raw(1.),
                              ~easing=easeIn,
                              ~duration=animationDuration,
                              ()
                            );
                          Animated.start(animation, ())
                        },
                        deferDuration
                      );
                    ()
                  },
                  deferDuration
                );
              ()
            },
          ()
        )
      }
    )
  }
};

let animateCollapseModal =
    (
      ~listOpacity,
      ~sourceItemDimensions as {width: sourceWidth, y: sourceY, height: sourceHeight},
      ~modalAnimationState as {top, width, height, contentOpacity, backgroundOpacity},
      ~onFinish
    ) => {
  let animationBatch =
    Animated.parallel(
      [|
        Animated.timing(
          ~value=listOpacity,
          ~toValue=`raw(1.),
          ~easing=easeIn,
          ~duration=animationDuration,
          ()
        ),
        Animated.timing(
          ~value=contentOpacity,
          ~toValue=`raw(0.),
          ~easing=easeIn,
          ~duration=animationDuration,
          ()
        ),
        Animated.timing(
          ~delay=animationDuration *. 0.8,
          ~value=backgroundOpacity,
          ~toValue=`raw(0.),
          ~easing=easeIn,
          ~duration=animationDuration *. 0.2,
          ()
        ),
        Animated.timing(
          ~value=top,
          ~toValue=`raw(sourceY),
          ~easing=easeIn,
          ~duration=animationDuration,
          ()
        ),
        Animated.timing(
          ~value=width,
          ~toValue=`raw(sourceWidth),
          ~easing=easeIn,
          ~duration=animationDuration,
          ()
        ),
        Animated.timing(
          ~value=height,
          ~toValue=`raw(sourceHeight),
          ~easing=easeIn,
          ~duration=animationDuration,
          ()
        )
      |],
      {"stopTogether": true}
    );
  /* Start animations */
  Animated.start(
    animationBatch,
    ~callback=
      (_finished) => {
        let _ =
          Js.Global.setTimeout(
            () => {
              onFinish();
              Animated.start(
                Animated.timing(
                  ~value=listOpacity,
                  ~toValue=`raw(1.),
                  ~easing=easeIn,
                  ~duration=animationDuration,
                  ()
                ),
                ()
              )
            },
            deferDuration
          );
        ()
      },
    ()
  )
};

/* Make */
let make = (~data: Item.data, _children) => {
  let loading = (data##loading);
  let error = data##error;
  let schedule =
    (
      switch (Js.Null_undefined.toOption(data##allSchedules)) {
      | None => []
      | Some(arr) => Array.to_list(arr)
      }
    )
    |> List.map(Item.convert_from_js);
  {
    ...component,
    reducer: (action, state) =>
      switch action {
      | Close =>
        switch state.modalState {
        | Expanded(item, sourceItemDimensions, modalState) =>
          ReasonReact.Update({
            ...state,
            modalState: Collapsing(item, sourceItemDimensions, modalState)
          })
        | _ => failwith("TODO: this is impossible state")
        }
      | Expand(x, y, width, height, item, selectedIndex) =>
        switch state.modalState {
        | Closed =>
          /* Handle target dimensions */
          let fx = float_of_int(x);
          let fy = float_of_int(y);
          let fw = float_of_int(width);
          let fh = float_of_int(height);
          let targetDimensions = {x: fx, y: fy +. statusOffset, width: fw, height: fh};
          let newAnimatedValue = (x) => Animated.Value.create(x);
          ReasonReact.Update({
            ...state,
            modalState:
              Expanding(
                item,
                targetDimensions,
                {
                  top: newAnimatedValue(fy +. statusOffset),
                  left: newAnimatedValue(fx),
                  width: newAnimatedValue(fw),
                  height: newAnimatedValue(fh),
                  contentOpacity: newAnimatedValue(0.),
                  backgroundOpacity: newAnimatedValue(1.)
                }
              ),
            selectedIndex
          })
        | _ => ReasonReact.NoUpdate
        }
      | ExpandFinish(item, sourceItemDimensions, modalAnimationState) =>
        ReasonReact.Update({
          ...state,
          modalState: Expanded(item, sourceItemDimensions, modalAnimationState)
        })
      | CollapseFinish => ReasonReact.Update({...state, modalState: Closed})
      },
    initialState: () => {
      listOpacity: Animated.Value.create(1.),
      modalState: Closed,
      selectedIndex: 0,
      listRef: None
    },
    /* let onExpandAnimationFinish = ((item, sourceItemDimensions, modalAnimationState)) =>
  ExpandFinish(item, sourceItemDimensions, modalAnimationState); */
    didUpdate: ({oldSelf, newSelf}) =>
      switch (oldSelf.state.modalState, newSelf.state.modalState) {
      | (Closed, Expanding(item, sourceItemDimensions, modalAnimationState)) =>
        animateExpandModal(
          ~listRef=newSelf.state.listRef,
          ~listOpacity=newSelf.state.listOpacity,
          ~modalAnimationState,
          ~item,
          ~sourceItemDimensions,
          ~onFinish={((item, sourceItemDimensions, modalAnimationState)) => newSelf.send(onExpandAnimationFinish((item, sourceItemDimensions, modalAnimationState)))}
        )
      | (Expanded(_, _, _), Collapsing(_, sourceItemDimensions, modalAnimationState)) =>
        animateCollapseModal(
          ~listOpacity=newSelf.state.listOpacity,
          ~sourceItemDimensions,
          ~modalAnimationState,
          /* let onCollapseAnimationFinish = (_) => CollapseFinish;
 */
          ~onFinish={() => newSelf.send(onCollapseAnimationFinish())}
        )
      | (_, _) => ()
      },
    render: (self) =>
      <View style=styles##container>
        <StatusBar backgroundColor="#ffffff" barStyle=`lightContent />
        <Animated.View style=Style.(style([opacity(Animated(self.state.listOpacity)), flex(1.)]))>
          <View ref=(self.handle(setRef)) style=styles##scrollView collapsable=false>
            <ScrollView
              scrollEnabled=(
                switch self.state.modalState {
                | Closed => true
                | _ => false
                }
              )
              style=styles##scrollView
              stickyHeaderIndices=(
                loading == false ? [1, Utils.getIndexFromSchedule(schedule)] : []
              )
              contentContainerStyle=styles##contentContainer>
              <Image
                source={`Required(Packager.require("../../../static/banner.png"))}
                style=styles##banner
                resizeMode=`cover
              />
              (
                loading == true ?
                  <View
                    style=Style.(style([flex(1.), alignItems(Center), justifyContent(Center)]))>
                    <ActivityIndicator />
                  </View> :
                  (
                    switch (Js.Null_undefined.toOption(error)) {
                    | None =>
                      Utils.getUniqueDates(schedule)
                      |> List.map(
                           (date) =>
                             List.concat([
                               [
                                 <View key=date style=styles##dateWrap>
                                   <Text
                                     style=styles##date
                                     value=(
                                       String.uppercase(
                                         Moment.(moment(date) |> Moment.format("dddd, MMMM Do"))
                                       )
                                     )
                                   />
                                 </View>
                               ],
                               List.mapi(
                                 (index, item: Item.t) =>
                                   switch item.talk {
                                   | None => <ScheduleItem item key=item.id />
                                   | Some(talk) =>
                                     <TalkItem
                                       item
                                       talk
                                       index
                                       selectedIndex=self.state.selectedIndex
                                       modalOpen=(
                                         switch self.state.modalState {
                                         | Expanded(_, _, _) => true
                                         | _ => false
                                         }
                                       )
                                       key=item.id
                                       /*  ~onFinish={((item, sourceItemDimensions, modalAnimationState)) => newSelf.send(onExpandAnimationFinish((item, sourceItemDimensions, modalAnimationState)))} */
                                       onPress={
                                         (x, y, width, height, item, index) =>
                                           self.send(startExpand(x, y, width, height, item, index))
                                       }
                                     />
                                   },
                                 Utils.getScheduleForDate(schedule, date)
                               )
                             ])
                         )
                      |> List.concat
                      |> Array.of_list
                      |> ReasonReact.array
                    | Some(e) =>
                      <View
                        style=Style.(
                                style([flex(1.), alignItems(Center), justifyContent(Center)])
                              )>
                        <Text value=e##message />
                      </View>
                    }
                  )
              )
            </ScrollView>
          </View>
        </Animated.View>
        /* </LinearGradient> */
        (
          switch self.state.modalState {
          | Closed => ReasonReact.null
          /* The fields from the modalAnimationState record
                are bound to names with trailing apostrophes
                because top, left, height, and width were
                shadowed by values from Style module
             */
          | Collapsing(item, _, modalAnimationState)
          | Expanding(item, _, modalAnimationState)
          | Expanded(item, _, modalAnimationState) =>
            let {
              top: top',
              left: left',
              height: height',
              width: width',
              contentOpacity,
              backgroundOpacity
            } = modalAnimationState;
            <Animated.View
              style=Style.(
                      concat([
                        styles##modal,
                        style([
                          position(Absolute),
                          top(Animated(top')),
                          left(Animated(left')),
                          width(Animated(width')),
                          height(Animated(height')),
                          overflow(Hidden)
                        ])
                      ])
                    )>
              <ItemModal
              contentOpacity=Animated(contentOpacity)
              backgroundOpacity=Animated(backgroundOpacity)
                /* contentOpacity */
                /* backgroundOpacity */
                item
                expanded=(
                  switch self.state.modalState {
                  | Expanded(_, _, _) => true
                  | _ => false
                  }
                )
                onClose={() => self.send(startClose())}
              />
            </Animated.View>
          }
        )
      </View>
  }
};

let wrappedComponent = {
  module M = ReactApollo.CreateWrapper(Item);
  M.wrapComponent(~component, ~make)
};