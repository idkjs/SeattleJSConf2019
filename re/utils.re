open BsReactNative;
let getItemDate = (item: Item.t) => String.sub(item.start, 0, 10);
let str = ReasonReact.string;
let getUniqueDates = (schedule) => List.map(getItemDate, schedule) |> List.sort_uniq(compare);

let getScheduleForDate = (schedule, date) =>
  List.filter((item) => getItemDate(item) === date, schedule);

let getIndexFromSchedule = (schedule) =>
  switch (getUniqueDates(schedule)) {
  | [firstDate, ..._] => List.length(getScheduleForDate(schedule, firstDate)) + 2
  | [] => 2
  };

let switchPlatform = (arg1, arg2) => {
  switch(Platform.os()){
  | Platform.IOS(_) => arg1
  | Platform.Android => arg2
};};

/* let paddingTopStr = switch(Platform.os()){
  | Platform.IOS(_) => 20.
  | Platform.Android => float_of_int(StatBar.constants##currentHeight)
}; */