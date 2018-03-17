open Util;

let component = ReasonReact.statelessComponent("OrderItems");

let make = (~orderItems: list(OrderData.Order.orderItem), _children) => {
  ...component,
  render: _self =>
    <div id="order-items">
      <h2> (s("Order Items")) </h2>
      (
        orderItems
        |> List.map(i => <OrderItem orderItem=i />)
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>,
};