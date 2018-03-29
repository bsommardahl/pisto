open Util;

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
        (_self => onChange(state.date |> Date.toFloat)),
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
          disabled=(! self.state.valid |> Js.Boolean.to_js_boolean)
          onClick=((_) => self.send(DisableMod))>
          (s("Hecho"))
        </button>
        (
          self.state.valid ?
            <div className="valid"> (s("Valido")) </div> :
            <div className="invalid"> (s("No Valido")) </div>
        )
      </div> :
      <div onClick=((_) => self.send(EnableMod))>
        <p> (s("Fecha:")) (s(date |> Date.toDisplayDate)) </p>
        <p> (s("Hora:")) (s(date |> Date.toDisplayTime)) </p>
      </div>;
  },
};