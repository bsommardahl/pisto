type mode =
  | TouchToEdit
  | EditOnly
  | ReadOnly;

type submitBehavior =
  | SubmitOnEnter
  | SubmitOnKey;

type action =
  | EnableMod
  | DisableMod
  | KeyDown(int)
  | ValueChanged(string);

type state = {
  modifying: bool,
  value: string,
  original: string,
};

let component = ReasonReact.reducerComponent("EditableText");

let make =
    (
      ~text: string,
      ~placeholder="",
      ~big=true,
      ~onChange,
      ~mode=TouchToEdit,
      ~autoFocus=true,
      ~required=false,
      _children,
    ) => {
  ...component,
  initialState: () => {
    modifying: mode === EditOnly,
    original: text,
    value: text,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod =>
      ReasonReact.Update({
        ...state,
        value: text,
        modifying: mode === TouchToEdit,
      })
    | DisableMod =>
      ReasonReact.Update({...state, modifying: mode === EditOnly})
    | ValueChanged(value) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, value},
        (self => onChange(self.state.value)),
      )
    | KeyDown(164) =>
      ReasonReact.Update({
        ...state,
        value: state.value ++ (164 |> Js.String.fromCharCode),
      })
    | KeyDown(165) =>
      ReasonReact.Update({
        ...state,
        value: state.value ++ (165 |> Js.String.fromCharCode),
      })
    | KeyDown(27) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, value: state.original},
        (
          self =>
            if (mode !== EditOnly) {
              self.send(DisableMod);
            }
        ),
      )
    | KeyDown(13) =>
      ReasonReact.SideEffects(
        (
          self => {
            onChange(self.state.value);
            if (mode !== EditOnly) {
              self.send(DisableMod);
            };
          }
        ),
      )
    | KeyDown(_) => ReasonReact.NoUpdate
    },
  render: self => {
    let getVal = ev => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(ev))##value;
      value;
    };
    self.state.modifying ?
      <div>
        <input
          value=self.state.value
          placeholder
          autoFocus
          onChange=(ev => self.send(ValueChanged(getVal(ev))))
          onKeyDown=(
            event => self.send(KeyDown(ReactEventRe.Keyboard.which(event)))
          )
          className=(big ? "big-text" : "")
        />
        <ValidationMessage
          hidden=(! required || required && self.state.value !== "")
        />
      </div> :
      <div onClick=(_ => self.send(EnableMod))> (ReactUtils.s(text)) </div>;
  },
};