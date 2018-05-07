[@bs.val] external window : Dom.window = "";

[@bs.send]
external addEventListener : (Dom.window, string, 'a => unit) => unit = "";

[@bs.send]
external removeEventListener : (Dom.window, string, 'a => unit) => unit = "";

type state = {value: string};

type action =
  | Reset
  | KeyDown(int);

let component = ReasonReact.reducerComponent("KeyInput");

let make = (~onCancel=() => (), ~onFinish, ~className="", _children) => {
  ...component,
  initialState: () => {value: ""},
  reducer: (action, state) =>
    switch (action) {
    | KeyDown(27) =>
      ReasonReact.UpdateWithSideEffects({value: ""}, ((_) => onCancel()))
    | KeyDown(13) =>
      ReasonReact.UpdateWithSideEffects(
        {value: ""},
        ((_) => onFinish(state.value)),
      )
    | KeyDown(key) =>
      ReasonReact.Update({
        value: state.value ++ (key |> Js.String.fromCharCode),
      })
    | Reset => ReasonReact.Update({value: ""})
    },
  subscriptions: self => {
    let logKey = ev => self.send(KeyDown(ReactEventRe.Keyboard.which(ev)));
    [
      Sub(
        () => addEventListener(window, "keydown", logKey),
        () => removeEventListener(window, "keydown", logKey),
      ),
    ];
  },
  render: self =>
    <input
      readOnly=true
      className=("key-input " ++ className)
      value=self.state.value
    />,
};