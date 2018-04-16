open ReactUtils;

type state = {currentValue: string};

type action =
  | UpdateCurrentValue(string)
  | FinishedEditing
  | StartEditing;

let component = ReasonReact.reducerComponent("MoneyInput");

let make =
    (~amount: int, ~onChange, ~key="", ~onValidate=(_: int) => (), _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | UpdateCurrentValue(v) => ReasonReact.Update({currentValue: v})
    | FinishedEditing =>
      ReasonReact.UpdateWithSideEffects(
        {currentValue: state.currentValue |> Money.toT |> Money.toDisplay},
        (
          self => {
            onChange(self.state.currentValue |> Money.toT);
            onValidate(self.state.currentValue |> Money.toT);
          }
        ),
      )
    | StartEditing =>
      ReasonReact.Update({
        currentValue:
          state.currentValue |> Util.cleanNonNumeric |> Util.stripLeadingZeros,
      })
    },
  initialState: () => {currentValue: amount |> Money.toDisplay},
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
        onFocus=((_) => self.send(StartEditing))
      />
    </div>;
  },
};