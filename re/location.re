open BsReactNative;

/* Variables */
let windowWidth = Dimensions.get(`window)##width;

let initialRegion = {
  "latitude": 47.651857,
  "longitude": (-122.3552919),
  "latitudeDelta": 0.001,
  "longitudeDelta": 0.001
};

let coordinate = {"latitude": 47.651857, "longitude": (-122.3552919)};

let iosDirections = "http://maps.apple.com/?daddr=47.651857,-122.3552919";

let androidDirections = "http://maps.google.com/maps?daddr=47.651857,-122.3552919";

/* Methods */
/* let getDirections = () =>
  if (Platform.os === Platform.IOS(_)) {
    Link.openURL(iosDirections)
  } else {
    Link.openURL(androidDirections)
  }; */
let getDirections = () => Utils.switchPlatform(Link.openURL(iosDirections),Link.openURL(androidDirections));


/* Component */
let component = ReasonReact.statelessComponent("Location");

let paddingTopString = switch(Platform.os()){
  | Platform.IOS(_) => 20.
  | Platform.Android => float_of_int(StatBar.constants##currentHeight)
};
/* Styles */
let styles =
  StyleSheet.create(
    Style.(
      {
        "container": style([flex(1.), backgroundColor(String("rgb(54, 97, 115)"))]),
        "contentContainer":
          style([
            alignItems(Stretch),
            justifyContent(Center),
            paddingTop(Pt(paddingTopString))
          ]),
        "mapview":
          style([
            width(Pt(float_of_int(windowWidth))),
            height(Pt(float_of_int(windowWidth) *. 0.656)),
            marginBottom(Pt(10.))
          ]),
        "foundry":
          style([
            width(Pt(float_of_int(windowWidth) -. 20.)),
            height(Pt(float_of_int(windowWidth) *. 0.569))
          ]),
        "row": style([flex(1.), flexDirection(Column)]),
        "contentWrapper": style([padding(Pt(10.)), marginTop(Pt(10.))]),
        "text":
          style([
            fontFamily("open-sans"),
            fontSize(Float(18.)),
            color(String("white")),
            textAlign(Center),
            backgroundColor(String("transparent"))
          ]),
        "button":
          style([
            backgroundColor(String("rgb(140, 170, 29)")),
            padding(Pt(10.)),
            margin(Pt(10.)),
            borderRadius(24.),
            flex(0.)
          ]),
        "boldText":
          style([
            fontFamily("open-sans-bold"),
            fontSize(Float(18.)),
            color(String("white")),
            backgroundColor(String("transparent")),
            marginBottom(Pt(20.))
          ]),
        "regularText":
          style([
            fontFamily("open-sans"),
            fontSize(Float(15.)),
            color(String("white")),
            backgroundColor(String("transparent")),
            marginBottom(Pt(20.))
          ])
      }
    )
  );

/* Make */
let make = (_, _children) => {
  ...component,
  render: (_self) =>
    <ScrollView style=styles##container>
      <View style=styles##contentContainer>
        <MapView initialRegion style=styles##mapview>
          <MapView.Marker
            title="Fremont Foundry"
            image=(Utils.switchPlatform(Packager.require("../../../static/pin.png"),
                Packager.require("../../../static/pin_small.png"))
            )
            coordinate
          />
        </MapView>
        <TouchableOpacity onPress=getDirections>
          <View style=styles##button> <Text style=styles##text value="GET DIRECTIONS" /> </View>
        </TouchableOpacity>
        <View style=styles##contentWrapper>
          <Text style=styles##boldText value="WHEN: AUGUST 10th - 11th 2017" />
          <Text style=styles##boldText value="WHERE: FREMONT FOUNDRY" />
          <Text
            style=styles##regularText
            value="SeattleJS's inaugural annual conference will be located at the beautiful and unique Fremont Foundry, located at 154 North 35th Street, Seattle, WA 98103."
          />
          <Image
            source=(`Required(Packager.require("../../../static/foundry.jpg")))
            style=styles##foundry
            resizeMode=`cover
          />
        </View>
      </View>
    </ScrollView>
};

/* JS Export */
let jsComponent = ReasonReact.wrapReasonForJs(~component, (_props) => make((), [||]));