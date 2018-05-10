open ReactUtils;

type state = {currentValue: string};

type action =
  | UpdateCurrentValue(string)
  | FinishedEditing
  | StartEditing;

let component = ReasonReact.reducerComponent("DateInput");

let make =
    (
      ~title="",
      ~value: Date.t,
      ~onChange: Date.t => unit,
      ~key="",
      ~onValidate=(_: Date.t) => (),
      _children,
    ) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | UpdateCurrentValue(v) => ReasonReact.Update({currentValue: v})
    | FinishedEditing =>
      ReasonReact.UpdateWithSideEffects(
        {currentValue: state.currentValue},
        (
          self => {
            onChange(self.state.currentValue |> Date.fromISODate);
            onValidate(self.state.currentValue |> Date.fromISODate);
          }
        ),
      )
    | StartEditing => ReasonReact.Update({currentValue: state.currentValue})
    },
  initialState: () => {currentValue: value |> Date.toISODate},
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <span className="field date-input" key>
      <span className="field-title"> (s(title)) </span>
      <input
        className="date"
        _type="date"
        value=self.state.currentValue
        onChange=(ev => self.send(UpdateCurrentValue(getVal(ev))))
        onBlur=(_ev => self.send(FinishedEditing))
        onFocus=((_) => self.send(StartEditing))
      />
    </span>;
  },
};