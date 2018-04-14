let component = ReasonReact.statelessComponent("OrderItem");

let make = (~orderItem: OrderItem.t, _children) => {
  ...component,
  render: _self =>
    <div className="order-item">
      <div className="order-item-name"> (ReactUtils.s(orderItem.name)) </div>
      <div className="order-item-price">
        (ReactUtils.s(string_of_int(orderItem.salePrice)))
      </div>
    </div>,
};