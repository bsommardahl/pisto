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

let make = (~date: Date.t, ~onChange, _children) => {
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
        (_self => onChange(state.date |> Date.fromDisplay)),
      )
    | UpdateDate(newDate) =>
      ReasonReact.Update({
        ...state,
        date: newDate,
        valid: newDate |> Date.isValid,
      })
    },
  render: self => {
    let getVal = ev => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(ev))##value;
      value;
    };
    self.state.modifying ?
      <div>
        <input
          value=self.state.date
          onChange=(ev => self.send(UpdateDate(getVal(ev))))
        />
        <Button
          local=true
          disabled=(! self.state.valid)
          onClick=((_) => self.send(DisableMod))
          label="action.done"
        />
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