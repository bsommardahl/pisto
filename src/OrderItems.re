open Util;

let component = ReasonReact.statelessComponent("OrderItems");

let make = (~orderItems: list(OrderData.Order.orderItem), _children) => {
  ...component,
  render: _self =>{
    let subTotal = "L0.0";
    let tax = "L0.0";
    let listOfPrices =
      orderItems |> List.map((i: OrderData.Order.orderItem) => i.salePrice);
    let total =
      Belt.List.reduce(listOfPrices, 0.0, (acc, curr) => acc +. curr)
      |> string_of_float;
    
  (
    <div className="order-items">
      <h2> (s("Order Items")) </h2>
      <table>
        <tbody>
          (
            orderItems
            |> List.map((i:OrderData.Order.orderItem) =>
                 <tr>
                   <td> (s(i.name)) </td>
                   <td> (s(string_of_float(i.salePrice))) </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
          <tr><th>(s("Sub-total"))</th><td>(s("0.0"))</td></tr>

          <tr><th>(s("Impuesto"))</th><td>(s("0.0"))</td></tr>

          <tr><th>(s("Total"))</th><td>(s(total))</td></tr>
        </tbody>
      </table>
    </div>
  )
  }
};