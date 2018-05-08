type state = {
  pin: string,
  cashiers: list(Cashier.t),
};

type action =
  | Update(string)
  | Verify(string)
  | LoadCashiers
  | CashiersLoaded(list(Cashier.t));

let component = ReasonReact.reducerComponent("PinInput");

let make =
    (
      ~onSuccess: Cashier.t => unit,
      ~onFailure: unit => unit,
      ~autoFocus=true,
      _children,
    ) => {
  ...component,
  didMount: self => {
    self.send(LoadCashiers);
    ReasonReact.NoUpdate;
  },
  initialState: () => {pin: "", cashiers: []},
  reducer: (action, state) =>
    switch (action) {
    | Update(pin) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, pin},
        (
          self =>
            if (pin |> Js.String.length === 4) {
              self.send(Verify(pin));
            }
        ),
      )
    | LoadCashiers =>
      ReasonReact.SideEffects(
        (
          self => {
            CashierStore.getAll()
            |> Js.Promise.then_((cashiers: list(Cashier.t)) => {
                 self.send(CashiersLoaded(cashiers));
                 Js.Promise.resolve();
               })
            |> ignore;
            ();
          }
        ),
      )
    | CashiersLoaded(cashiers) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, cashiers},
        (
          _self =>
            if (cashiers |> List.length === 0) {
              onSuccess({id: "none", name: "n/a", pin: ""});
            }
        ),
      )
    | Verify(pin) =>
      ReasonReact.SideEffects(
        (
          self =>
            switch (
              self.state.cashiers
              |> List.find((c: Cashier.t) => c.pin === pin)
            ) {
            | exception Not_found => onFailure()
            | cashier => onSuccess(cashier)
            }
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
        autoFocus
        className="big pin"
        value=self.state.pin
        onChange=(ev => self.send(Update(getVal(ev))))
      />
    </div>;
  },
};