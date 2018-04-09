open Util;

type action =
  | EnableMod
  | DisableMod;

type state = {modifying: bool};

let component = ReasonReact.reducerComponent("EditableText");

let make = (~text: string, ~onChange, _children) => {
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
        <input value=text onChange=ontextChange />
        <button onClick=((_) => self.send(DisableMod))>
          (s("Cambiar"))
        </button>
      </div> :
      <div onClick=((_) => self.send(EnableMod))> (s(text)) </div>;
  },
};