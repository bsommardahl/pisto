type state = string;

type action =
  | Update(string)
  | Verify(string);

let component = ReasonReact.reducerComponent("PinInput");

let make =
    (
      ~value,
      ~onSuccess: Cashier.t => unit,
      ~onFailure: unit => unit,
      _children,
    ) => {
  ...component,
  initialState: () => value,
  reducer: (action, _state) =>
    switch (action) {
    | Update(pin) =>
      ReasonReact.UpdateWithSideEffects(
        pin,
        (
          self =>
            if (pin |> Js.String.length === 4) {
              self.send(Verify(pin));
            }
        ),
      )
    | Verify(pin) =>
      ReasonReact.SideEffects(
        (
          _self =>
            CashierStore.getAll()
            |> Js.Promise.then_((cashiers: list(Cashier.t)) => {
                 switch (
                   cashiers |> List.find((c: Cashier.t) => c.pin === pin)
                 ) {
                 | exception Not_found => onFailure()
                 | cashier => onSuccess(cashier)
                 };
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div className="pin-entry">
      (ReactUtils.s("PIN:"))
      <input
        autoFocus=Js.true_
        className="big pin"
        value=self.state
        onChange=(ev => self.send(Update(getVal(ev))))
      />
    </div>;
  },
};