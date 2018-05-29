open OrderHelper;
open BsReactstrap;

[@bs.module] external myModal : ReasonReact.reactClass = "./ModalDialog";

[@bs.deriving abstract]
type jsProps = {
  show:bool,
};

let make = (~show,children)=>
ReasonReact.wrapJsForReason(
  ~reactClass = myModal,
  ~props = {"show":show},
  children, 
); 

type userIntent =
  | Building
  | Returning;

type state = {userIntent,show:bool};

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

let make = (~order: Order.orderVm, ~onFinish, _children) => {
  ...component,
  initialState: () => {userIntent: Building, show:false},
  
  reducer: (action, state) =>
    switch (action) {
    | ChangeIntent(intent) => ReasonReact.Update({...state, userIntent: intent})
    | SaveAndExit =>
      ReasonReact.SideEffects((_self => saveOrder(order, onFinish)))
    | ReturnAndExit(cashier) =>
      ReasonReact.SideEffects(
        (_self => returnOrder(cashier, order, onFinish)),
      )
    | ShowDialog => 
      ReasonReact.Update({...state, show:true })
    | HideDialog => ReasonReact.Update({...state, show:false})
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
        onClick=((_) => self.send(SaveAndExit))
        label="action.save"
      />;
    let payButton =
      <Button
        local=true
        className="pay-button-card"
        disabled=disablePayButton
        onClick=((_) => self.send(SaveAndGoToPayScreen))
        label="action.pay"
      />;
    let deleteButton =
      <Button
        local=true
        className="remove-button-card"
        onClick=((_) => self.send(ShowDialog))
        label="action.delete"
      />;
    let returnButton =
      <Button
        local=true
        className="quiet-card"
        onClick=((_) => self.send(ChangeIntent(Returning)))
        label="action.return"
      />;
    let cancelButton =
      <Button
        onClick=((_) => onFinish(order))
        local=true
        label="action.cancel"
      />;
    <div className="order-actions">
      (
        switch (self.state.userIntent, order.paid, order.returned, order.id) {
        | (Returning, _, _, Some(_id)) =>
          <PinInput
            onFailure=(() => self.send(ChangeIntent(Building)))
            onSuccess=(cashier => self.send(ReturnAndExit(cashier)))
          />
        | (Building, None, None, Some(_id)) =>
          <span> saveButton payButton deleteButton </span>
        | (Building, None, None, None) => <span> saveButton payButton </span>
        | (Building, Some(_paid), None, Some(_id)) => returnButton
        | (_, _, _, _) => ReasonReact.nullElement
        }
      )
      cancelButton
    </div>;
  },
};