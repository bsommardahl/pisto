[@bs.val] external window: Dom.window = "";

[@bs.send]
external addEventListener: (Dom.window, string, 'a => unit) => unit = "";

[@bs.send]
external removeEventListener: (Dom.window, string, 'a => unit) => unit = "";

type state = {value: string};

type action =
  | Reset
  | KeyDown(int);

let component = ReasonReact.reducerComponent("KeyInput");

let stringOrDefault = (opt: option(string)) =>
  switch (opt) {
  | None => ""
  | Some(s) => s
  };

let make =
    (
      ~onCancel=() => (),
      ~acceptInput=true,
      ~onFinish,
      ~className="",
      _children,
    ) => {
  ...component,
  initialState: () => {value: ""},
  reducer: (action, state) =>
    switch (action) {
    | KeyDown(27) =>
      ReasonReact.UpdateWithSideEffects({value: ""}, (_ => onCancel()))
    | KeyDown(8) =>
      ReasonReact.Update({
        value: Js.String.slice(~from=0, ~to_=-1, state.value),
      })
    | KeyDown(20) => ReasonReact.Update({value: state.value})
    | KeyDown(112) => ReasonReact.Update({value: state.value})
    | KeyDown(113) => ReasonReact.Update({value: state.value})
    | KeyDown(114) => ReasonReact.Update({value: state.value})
    | KeyDown(115) => ReasonReact.Update({value: state.value})
    | KeyDown(116) => ReasonReact.Update({value: state.value})
    | KeyDown(117) => ReasonReact.Update({value: state.value})
    | KeyDown(118) => ReasonReact.Update({value: state.value})
    | KeyDown(119) => ReasonReact.Update({value: state.value})
    | KeyDown(120) => ReasonReact.Update({value: state.value})
    | KeyDown(121) => ReasonReact.Update({value: state.value})
    | KeyDown(122) => ReasonReact.Update({value: state.value})
    | KeyDown(123) => ReasonReact.Update({value: state.value})
    | KeyDown(17) => ReasonReact.Update({value: state.value})
    | KeyDown(38) => ReasonReact.Update({value: state.value})
    | KeyDown(39) => ReasonReact.Update({value: state.value})
    | KeyDown(37) => ReasonReact.Update({value: state.value})
    | KeyDown(40) => ReasonReact.Update({value: state.value})
    | KeyDown(45) => ReasonReact.Update({value: state.value})
    | KeyDown(46) => ReasonReact.Update({value: state.value})
    | KeyDown(18) => ReasonReact.Update({value: state.value})
    | KeyDown(191) => ReasonReact.Update({value: state.value})
    | KeyDown(192) => ReasonReact.Update({value: state.value})
    | KeyDown(188) => ReasonReact.Update({value: state.value})
    | KeyDown(190) => ReasonReact.Update({value: state.value})
    | KeyDown(16) => ReasonReact.Update({value: state.value})
    | KeyDown(35) => ReasonReact.Update({value: state.value})
    | KeyDown(186) => ReasonReact.Update({value: state.value})
    | KeyDown(189) => ReasonReact.Update({value: state.value})
    | KeyDown(187) => ReasonReact.Update({value: state.value})
    | KeyDown(219) => ReasonReact.Update({value: state.value})
    | KeyDown(220) => ReasonReact.Update({value: state.value})
    | KeyDown(221) => ReasonReact.Update({value: state.value})
    | KeyDown(222) => ReasonReact.Update({value: state.value})
    | KeyDown(13) =>
      ReasonReact.UpdateWithSideEffects(
        {value: ""},
        (_ => onFinish(state.value)),
      )
    | KeyDown(key) =>
      ReasonReact.Update({
        value:
          acceptInput ?
            state.value ++ (key |> Js.String.fromCharCode) : state.value,
      })
    | Reset => ReasonReact.Update({value: ""})
    },
  didMount: self => {
    let logKey = ev => self.send(KeyDown(ReactEvent.Keyboard.which(ev)));
    addEventListener(window, "keydown", logKey);
    self.onUnmount(() => removeEventListener(window, "keydown", logKey));
  },
  render: self =>
    <input
      readOnly=true
      className={"key-input " ++ className}
      value={self.state.value}
    />,
};