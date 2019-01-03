open BsReactNative;

let component = ReasonReact.statelessComponent("SpeakerBio");

let styles =
  StyleSheet.create(
    Style.(
      {
        "wrapper": style([padding(Pt(10.)), marginTop(Pt(-10.))]),
        "heading":
          style([color(String("rgb(185,219,111)")), fontSize(Float(16.)), marginBottom(Pt(5.)), marginTop(Pt(5.))]),
        "text": style([fontFamily("open-sans"), fontSize(Float(16.)), color(String("white"))])
      }
    )
  );

let make = (~speakers: array(Item.speaker), _children) => {
  ...component,
  render: (_self) => {
    let speakerCount = Array.length(speakers);
    speakerCount > 1 ?
      <View>
        (
          Array.map(
            ({Item.bio, name}) =>
              <View style=styles##wrapper key=name>
                <Text value=name style=styles##heading />
                <Text style=styles##text value=bio />
              </View>,
            speakers
          )
          |> ReasonReact.array
        )
      </View> :
      <View style=styles##wrapper>
        <Text value="Speaker Bio" style=styles##heading />
        <Text style=styles##text value=speakers[0].bio />
      </View>
  }
};
