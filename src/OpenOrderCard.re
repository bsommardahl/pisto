open Util;

let component = ReasonReact.statelessComponent("OpenOrderCard");

let make = (~order: OrderData.Order.orderVm, ~onSelect, _children) => {
  ...component,
  render: _self => {
    let totals = OrderItemCalculation.getTotals(order.orderItems);
    <div className="open-order-card card" onClick=((_) => onSelect(order))>
      <div className="customer-name"> (s(order.customerName)) </div>
      <div className="total"> (s(totals.total |> Money.toDisplay)) </div>
      <div className="time"> (s(order.createdOn |> Date.toShortTime)) </div>
    </div>;
  },
};