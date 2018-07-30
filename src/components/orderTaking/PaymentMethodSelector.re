type state = {
  externalId: string,
  methods: list(PaymentMethod.t),
  selected: option(PaymentMethod.t),
};

type action =
  | Select(PaymentMethod.t)
  | PaymentMethodsLoaded(list(PaymentMethod.t))
  | ExternalIdChanged(string);

let component = ReasonReact.reducerComponent("PaymentMethodSelector");

let isValid = (method: PaymentMethod.t, externalId) =>
  switch (method.hasExternalId, externalId) {
  | (true, "") => false
  | (true, _) => true
  | (false, _) => true
  };

let validateMethod = (state, onValid, onInvalid) =>
  switch (state.selected) {
  | None => ()
  | Some(method) =>
    isValid(method, state.externalId) ?
      onValid(method, state.externalId) : onInvalid()
  };

let make =
    (
      ~onValid: (PaymentMethod.t, string) => unit,
      ~onInvalid: unit => unit,
      _children,
    ) => {
  ...component,
  didMount: self => {
    self.send(PaymentMethodsLoaded(PaymentMethod.tempAllPaymentMethods));
    ();
  },
  initialState: () => {externalId: "", methods: [], selected: None},
  reducer: (action, state) =>
    switch (action) {
    | ExternalIdChanged(id) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, externalId: id},
        (self => validateMethod(self.state, onValid, onInvalid)),
      )
    | PaymentMethodsLoaded(methods) =>
      ReasonReact.Update({...state, methods})
    | Select(method) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, selected: Some(method)},
        (self => validateMethod(self.state, onValid, onInvalid)),
      )
    },
  render: self =>
    <div className="select-payment-method">
      (
        self.state.methods
        |> List.map((m: PaymentMethod.t) => {
             let selected =
               switch (self.state.selected) {
               | None => false
               | Some(method) => method === m
               };
             <span>
               <Button
                 local=true
                 className=(
                   "payment-method"
                   ++ (selected ? " payment-method-selected" : "")
                 )
                 onClick=(_ => self.send(Select(m)))
                 label=m.name
               />
               (
                 selected && m.hasExternalId ?
                   <EditableText
                     placeholder=(m.name ++ ".externalId" |> Lang.translate)
                     text=self.state.externalId
                     mode=EditOnly
                     big=true
                     onChange=(text => self.send(ExternalIdChanged(text)))
                   /> :
                   <span />
               )
             </span>;
           })
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>,
};