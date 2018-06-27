open Js.Promise;

open OrderHelper;

type doing =
  | GettingCashier
  | ChoosingPaymentMethod
  | Paying;

type state = {
  doing,
  order: Order.orderVm,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  createdOn: float,
  lastUpdated: option(float),
  returned: option(Return.t),
  paid: option(Paid.t),
  id:option(string),
  meta: option(Js.Json.t),
  removed: bool,
  customerName: string,
  method: option(PaymentMethod.t),
  externalId: string,
  cashier: option(Cashier.t),
};

type action =
  | LoadOrder(string)
  | OrderLoaded(Order.orderVm)
  | PayOrder
  | SelectPaymentMethod((PaymentMethod.t, string))
  | PaymentMethodInvalid
  | ExternalIdChanged(string)
  | CashierChanged(option(Cashier.t))
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
    | PaymentMethodInvalid =>
      ReasonReact.Update({...state, doing: ChoosingPaymentMethod})
    | SelectPaymentMethod((method, externalId)) =>
      ReasonReact.Update({
        ...state,
        method: Some(method),
        externalId,
        doing: GettingCashier,
      })
    | ExternalIdChanged(id) => ReasonReact.Update({...state, externalId: id})
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
    | PayOrder =>
      ReasonReact.SideEffects(
        (
          _self =>
            switch (state.method) {
            | None => ()
            | Some(method) =>
              payOrder(
                state.cashier, state.order, method, state.externalId, paidOrder =>
                onPay(state.cashier, paidOrder)
              )
            }
        ),
      )
    | CashierChanged(cashier) =>
      ReasonReact.Update({...state, cashier, doing: Paying})
    | OrderLoaded(order) => ReasonReact.Update({...state, 
    order,
    orderItems:order.orderItems,
    discounts:order.discounts,
    customerName:order.customerName,
    paid:order.paid,
    returned:order.returned,
    id:order.id,
    removed:order.removed,
    lastUpdated:order.lastUpdated,
    createdOn:order.createdOn,
    meta:order.meta,})
    | Cancel => ReasonReact.SideEffects((_ => onCancel(state.order)))
    },
  initialState: () => {
    order: buildNewOrder(""),
    orderItems: [],
    discounts: [],
    meta: None,
    removed: false,
    createdOn: 0.0,
    lastUpdated: None,
    id: None,
    paid: None,
    customerName: "",
    returned: None,
    method: None,
    externalId: "",
    cashier: None,
    doing: ChoosingPaymentMethod,
  },
  render: self => {
    let cashier =
      <h2>
        (ReactUtils.sloc("cashier"))
        (ReactUtils.s(": "))
        (
          ReactUtils.s(
            switch (self.state.cashier) {
            | None => "n/a"
            | Some(cashier) => cashier.name
            },
          )
        )
      </h2>;
    let paymentMethod =
      <h2>
        (ReactUtils.sloc("paymentMethod"))
        (ReactUtils.s(": "))
        (
          ReactUtils.sloc(
            switch (self.state.method) {
            | None => "none"
            | Some(m) => m.name
            },
          )
        )
      </h2>;
    <div className="order">
      <div className="order-header">
        <div className="header-menu">
          <Button
            local=true
            className="quiet-card"
            onClick=(_ => self.send(Cancel))
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
          orderItems=self.state.orderItems
          discounts=self.state.discounts
          canDeselectDiscount=false
          canRemoveItem=false
        />
        (
          switch (self.state.doing) {
          | ChoosingPaymentMethod =>
            <PaymentMethodSelector
              onValid=(
                (method, externalId) =>
                  self.send(SelectPaymentMethod((method, externalId)))
              )
              onInvalid=(_ => self.send(PaymentMethodInvalid))
            />
          | GettingCashier =>
            <div>
              paymentMethod
              <PinInput
                autoFocus=true
                onFailure=(() => self.send(CashierChanged(None)))
                onSuccess=(
                  cashier => self.send(CashierChanged(Some(cashier)))
                )
              />
            </div>
          | Paying =>
            <div>
              paymentMethod
              cashier
              <div>
                <Button
                  local=true
                  label="action.pay"
                  onClick=(_ => self.send(PayOrder))
                />
              </div>
            </div>
          }
        )
      </div>
    </div>;
  },
};