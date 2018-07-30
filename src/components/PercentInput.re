type state = {currentValue: string};

type action =
  | UpdateCurrentValue(string)
  | FinishedEditing
  | StartEditing;

let component = ReasonReact.reducerComponent("PercentInput");

let make = (~percent: int, ~onChange, ~key="", ~onValidate=_ => (), _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | UpdateCurrentValue(v) =>
      ReasonReact.UpdateWithSideEffects(
        {currentValue: v},
        (_self => onValidate(v)),
      )
    | FinishedEditing =>
      ReasonReact.UpdateWithSideEffects(
        {
          currentValue: state.currentValue |> Percent.toT |> Percent.toDisplay,
        },
        (self => onChange(self.state.currentValue |> Percent.toT)),
      )
    | StartEditing =>
      ReasonReact.Update({
        currentValue:
          state.currentValue |> Util.cleanNonNumeric |> Util.stripLeadingZeros,
      })
    },
  initialState: () => {currentValue: percent |> Percent.toDisplay},
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="money-input" key>
      <input
        className="money"
        value=self.state.currentValue
        onChange=(ev => self.send(UpdateCurrentValue(getVal(ev))))
        onBlur=(_ev => self.send(FinishedEditing))
        onFocus=(_ => self.send(StartEditing))
      />
    </div>;
  },
};