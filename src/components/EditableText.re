type action =
  | EnableMod
  | DisableMod;

type state = {modifying: bool};

let component = ReasonReact.reducerComponent("EditableText");

let make = (~text: string, ~big=false, ~onChange, _children) => {
  ...component,
  initialState: () => {modifying: false},
  reducer: (action, _state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({modifying: true})
    | DisableMod => ReasonReact.Update({modifying: false})
    },
  render: self => {
    let ontextChange = ev => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(ev))##value;
      onChange(value);
    };
    self.state.modifying ?
      <div>
        <input
          value=text
          onChange=ontextChange
          className=(big ? "big-text" : "")
        />
        <Button
          local=true
          onClick=((_) => self.send(DisableMod))
          label="action.save"
        />
      </div> :
      <div onClick=((_) => self.send(EnableMod))>
        (ReactUtils.s(text))
      </div>;
  },
};