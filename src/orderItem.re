open Util;

let component = ReasonReact.statelessComponent("OrderItem");

let make = (~orderItem: OrderData.Order.orderItem, _children) => {
  ...component,
  render: _self =>
    <div className="order-item">
      <div className="order-item-name"> (s(orderItem.name)) </div>
      <div className="order-item-price">
        (s(string_of_float(orderItem.salePrice)))
      </div>
    </div>,
};