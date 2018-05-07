open Js.Promise;

type state = {order: Order.orderVm};

type action =
  | LoadOrder(string)
  | OrderLoaded(Order.orderVm)
  | PinEntered(string)
  | CancelPay;

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
            OrderStore.get(orderId)
            |> Js.Promise.then_(order => {
                 let vm = Order.vmFromExistingOrder(order);
                 self.send(OrderLoaded(vm));
                 Js.Promise.resolve(vm);
               })
            |> ignore
        ),
      )
    | OrderLoaded(order) => ReasonReact.Update({order: order})
    | CancelPay => ReasonReact.SideEffects(((_) => onCancel(state.order)))
    | Pay
    },
  initialState: () => {
    
      order: buildNewOrder(customerName),
    
  },
  render: self =>
    <div className="order">
    <PinInput
    onFailure=(() => self.send(ChangeIntent(Building)))
    onSuccess=(cashier => payOrder(cashier, order, onFinish))
  />
<OrderItems
          closed=false
          order=self.state.order
          canDeselectDiscount=false
          canRemoveItem=false
        />
      
    </div>,

};