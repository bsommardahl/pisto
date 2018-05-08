type action =
  | EnableMod
  | DisableMod
  | UpdateDate(string);

type state = {
  modifying: bool,
  date: string,
  valid: bool,
};

let component = ReasonReact.reducerComponent("EditableCustomerName");

let make = (~date: float, ~onChange, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    date: date |> Date.toDisplay,
    valid: true,
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | DisableMod =>
      ReasonReact.UpdateWithSideEffects(
        {...state, modifying: false, valid: true},
        (_self => onChange(state.date |> float_of_string)),
      )
    | UpdateDate(newDate) =>
      ReasonReact.Update({
        ...state,
        date: newDate,
        valid: newDate |> Date.isValid,
      })
    },
  render: self => {
    let onCustomerNameChange = ev => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(ev))##value;
      self.send(UpdateDate(value));
    };
    self.state.modifying ?
      <div>
        <input value=self.state.date onChange=onCustomerNameChange />
        <button
          disabled=(! self.state.valid)
          onClick=((_) => self.send(DisableMod))>
          (ReactUtils.s("Hecho"))
        </button>
        (
          self.state.valid ?
            <div className="valid"> (ReactUtils.s("Valido")) </div> :
            <div className="invalid"> (ReactUtils.s("No Valido")) </div>
        )
      </div> :
      <div onClick=((_) => self.send(EnableMod))>
        (ReactUtils.s(date |> Date.toDisplay))
      </div>;
  },
};