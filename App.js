import React from "react";
import { View } from "react-native";
import { ApolloProvider } from "react-apollo";
import { Font } from "expo";
import ApolloClient from "apollo-boost";

// import Rehydrate from "./Rehydrate";
import { component as Root } from "./lib/js/re/root.bs.js";

console.ignoredYellowBox = [
  "Warning: View.propTypes has been deprecated and will be removed in a future version of ReactNative. Use ViewPropTypes instead."
];

const client = new ApolloClient({
  uri: "https://api.graph.cool/simple/v1/cj5cm0t4pnljk01087tv4g61a"
});

export default class App extends React.Component {
  state = {
    fontLoaded: false
  };

  async componentDidMount() {
    await Font.loadAsync({
      "open-sans": require("./static/OpenSans-Regular.ttf"),
      "open-sans-bold": require("./static/OpenSans-Bold.ttf"),
      montserrat: require("./static/Montserrat-Regular.ttf"),
      "montserrat-bold": require("./static/Montserrat-Bold.ttf")
    });

    this.setState({
      fontLoaded: true
    });
  }

  render() {
    return (
      <View style={{ flex: 1, backgroundColor: "rgb(54, 97, 115)" }}>
        <ApolloProvider client={client}>
          <Root />
        </ApolloProvider>
      </View>
    );
    return;
  }
}
