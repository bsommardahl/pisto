open Util;

let component = ReasonReact.statelessComponent("OpenOrderCard");

let make = (~order: OrderData.Order.order, ~onSelect, _children) => {
  ...component,
  render: _self =>
    <div className="open-order-card" onClick=((_) => onSelect(order))>
      <div className="customer-name"> (s(order.customerName)) </div>
    </div>,
};