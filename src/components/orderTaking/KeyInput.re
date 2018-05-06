type state = {value: string};

type action =
  | Change(string)
  | Reset
  | KeyDown(int);

let component = ReasonReact.reducerComponent("KeyInput");

let make =
    (
      ~onCancel=() => (),
      ~onFinish,
      ~autoFocus=true,
      ~maintainFocus=false,
      ~className="",
      _children,
    ) => {
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
    | KeyDown(_) => ReasonReact.NoUpdate
    | Change(value) => ReasonReact.Update({value: value})
    | Reset => ReasonReact.Update({value: ""})
    },
    didMount: _ =>{
      Js.log("mounted. ");
      Js.log(maintainFocus);
      ReasonReact.NoUpdate;
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let refocusInput = ev =>
      if (maintainFocus) {
        let node =
          ev |> ReactEventRe.Focus.target |> ReactDOMRe.domElementToObj;
        node##focus();
      };
    <input
      autoFocus=(autoFocus ? Js.true_ : Js.false_)
      className=("key-input " ++ className)
      value=self.state.value
      onBlur=(ev => refocusInput(ev))
      onChange=(ev => self.send(Change(getVal(ev))))
      onKeyDown=(ev => self.send(KeyDown(ReactEventRe.Keyboard.which(ev))))
    />;
  },
};