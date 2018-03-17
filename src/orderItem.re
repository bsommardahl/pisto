open Util;

let component = ReasonReact.statelessComponent("OrderItem");

/* Here, the button should push to /NewOrder?customerName=john and the view changes */
let make = (~orderItem: OrderData.Order.orderItem, _children) => {
  ...component,
  render: _self =>
    <div className="order-item">
      <div className="order-item-name"> (s(orderItem.product.name)) </div>
      <div className="order-item-price">
        (s(string_of_float(orderItem.salePrice)))
      </div>
    </div>,
};