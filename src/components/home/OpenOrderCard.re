let component = ReasonReact.statelessComponent("OpenOrderCard");

let make = (~order: Order.orderVm, ~onSelect, _children) => {
  ...component,
  render: _self => {
    let totals =
      OrderItemCalculation.getTotals(order.discounts, order.orderItems);
    <div className="open-order-card card" onClick=(_ => onSelect(order))>
      <div className="customer-name">
        (ReactUtils.s(order.customerName))
      </div>
      <div className="total">
        (ReactUtils.s(totals.total |> Money.toDisplay))
      </div>
      <div className="time">
        (ReactUtils.s(order.createdOn |> Date.toShortTime))
      </div>
    </div>;
  },
};