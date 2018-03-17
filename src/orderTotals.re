open Util;

let component = ReasonReact.statelessComponent("OrderTotals");

let make = (~orderItems: list(OrderData.Order.orderItem), _children) => {
  ...component,
  render: _self => {
    let subTotal = "L0.0";
    let tax = "L0.0";
    let listOfPrices =
      orderItems |> List.map((i: OrderData.Order.orderItem) => i.salePrice);
    let total =
      Belt.List.reduce(listOfPrices, 0.0, (acc, curr) => acc +. curr)
      |> string_of_float;
    <div id="order-totals">
      <h2> (s("Order Totals")) </h2>
      <table>
        <tr> <th> (s("Sub-total")) </th> <td> (s(subTotal)) </td> </tr>
        <tr> <th> (s("Impuesto")) </th> <td> (s(tax)) </td> </tr>
        <tr> <th> (s("Total")) </th> <td> (s(total)) </td> </tr>
      </table>
    </div>;
  },
};