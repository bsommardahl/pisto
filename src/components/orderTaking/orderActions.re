[@bs.val] external alert : string => unit = "";
open OrderHelper;

type userIntent =
  | Building
  | Returning;

type state = {
  userIntent,
  showModal: bool,
};

type action =
  | ChangeIntent(userIntent)
  | SaveAndExit
  | SaveAndGoToPayScreen
  | ShowDialog
  | HideDialog
  | DeleteAndExit
  | ReturnAndExit(Cashier.t);

let component = ReasonReact.reducerComponent("OrderActions");

let stringOrDefault = (opt: option(string)) =>
  switch (opt) {
  | None => ""
  | Some(s) => s
  };

let make =
    (
      ~order: Order.orderVm,
      ~onShowProductModal=() => (),
      ~onFinish,
      _children,
    ) => {
  ...component,
  initialState: () => {userIntent: Building, showModal: false},
  reducer: (action, state) =>
    switch (action) {
    | ChangeIntent(intent) =>
      ReasonReact.Update({...state, userIntent: intent})
    | SaveAndExit =>
      ReasonReact.SideEffects((_self => saveOrder(order, onFinish)))
    | ReturnAndExit(cashier) =>
      ReasonReact.SideEffects(
        (_self => returnOrder(cashier, order, onFinish)),
      )
    | ShowDialog => ReasonReact.Update({...state, showModal: true})
    | HideDialog => ReasonReact.Update({...state, showModal: false})
    | DeleteAndExit =>
      ReasonReact.SideEffects((_self => removeOrder(order, onFinish)))
    | SaveAndGoToPayScreen =>
      ReasonReact.SideEffects(
        (
          _self =>
            saveOrder(order, o =>
              ReasonReact.Router.push(
                "/pay?orderId=" ++ stringOrDefault(o.id),
              )
            )
        ),
      )
    },
  render: self => {
    let items = order.orderItems |> Array.of_list;
    let disablePayButton = items |> Array.length === 0;
    let saveButton =
      <Button
        local=true
        className="save-button-card"
        onClick=(_ => self.send(SaveAndExit))
        label="action.save"
      />;
    let doneButton =
      <Button
        local=true
        className="save-button-card"
        onClick=(_ => self.send(SaveAndExit))
        label="action.done"
      />;
    let searchButton =
      <Button
        local=true
        className="pay-button-card"
        label="modal.SearchProduct"
        onClick=(_ => onShowProductModal())
      />;
    let payButton =
      <Button
        local=true
        className="pay-button-card"
        disabled=disablePayButton
        onClick=(_ => self.send(SaveAndGoToPayScreen))
        label="action.pay"
      />;
    let deleteButton =
      <Button
        local=true
        className="remove-button-card"
        onClick=(_ => self.send(ShowDialog))
        label="action.delete"
      />;
    let returnButton =
      <Button
        local=true
        className="quiet-card"
        onClick=(_ => self.send(ChangeIntent(Returning)))
        label="action.return"
      />;
    let cancelButton =
      <Button
        onClick=(_ => onFinish(order))
        local=true
        label="action.cancel"
      />;
    let pinInput =
      <PinInput
        onFailure=(
          () => {
            self.send(ChangeIntent(Building));
            alert("Please enter a valid pin.");
          }
        )
        onSuccess=(cashier => self.send(ReturnAndExit(cashier)))
      />;
    <div className="order-actions">
      (
        switch (self.state.userIntent, order.paid, order.returned, order.id) {
        | (Returning, _, _, Some(_id)) => pinInput
        | (Building, None, None, Some(_id)) =>
          <span> searchButton saveButton payButton deleteButton </span>
        | (Building, None, None, None) => <span> saveButton payButton </span>
        | (Building, Some(_paid), None, Some(_id)) =>
          <span> doneButton returnButton </span>
        | (_, _, _, _) => ReasonReact.nullElement
        }
      )
      cancelButton
      <DeleteModal
        contentLabel="modal.deleteOrderContent"
        label="modal.deleteOrder"
        isOpen=self.state.showModal
        onConfirm=(() => self.send(DeleteAndExit))
        onCancel=(() => self.send(HideDialog))
      />
    </div>;
  },
};