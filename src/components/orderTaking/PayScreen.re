open Js.Promise;

open OrderHelper;

type state = {order: Order.orderVm};

type action =
  | LoadOrder(string)
  | OrderLoaded(Order.orderVm)
  | PayOrder(Cashier.t)
  | Cancel;

let component = ReasonReact.reducerComponent("PayScreen");

let make = (~orderId, ~onPay, ~onCancel, _children) => {
  ...component,
  didMount: self => {
    self.send(LoadOrder(orderId));
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadOrder(orderId) =>
      ReasonReact.SideEffects(
        (
          self =>
            getOrderVm(orderId)
            |> Js.Promise.then_(vm => {
                 self.send(OrderLoaded(vm));
                 Js.Promise.resolve(vm);
               })
            |> ignore
        ),
      )
    | PayOrder(cashier) =>
      ReasonReact.SideEffects(
        (
          _self =>
            payOrder(cashier, state.order, paidOrder =>
              onPay(cashier, paidOrder)
            )
        ),
      )
    | OrderLoaded(order) => ReasonReact.Update({order: order})
    | Cancel => ReasonReact.SideEffects(((_) => onCancel(state.order)))
    },
  initialState: () => {order: buildNewOrder("")},
  render: self =>
    <div className="order">
      <div className="order-header">
        <div className="header-menu">
          <Button
            local=true
            className="quiet-card"
            onClick=((_) => self.send(Cancel))
            label="action.cancel"
          />
        </div>
        <div className="customer-name">
          (ReactUtils.s(self.state.order.customerName))
        </div>
      </div>
      <div className="pay-for-order">
        <OrderItems
          closed=false
          order=self.state.order
          canDeselectDiscount=false
          canRemoveItem=false
        />
        <PinInput
          onFailure=(() => onCancel(self.state.order))
          onSuccess=(cashier => self.send(PayOrder(cashier)))
        />
      </div>
    </div>,
};