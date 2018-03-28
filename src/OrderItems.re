open Util;

let component = ReasonReact.statelessComponent("OrderItems");

let make = (~order: OrderData.Order.orderVm, ~onRemoveItem, _children) => {
  ...component,
  render: _self => {
    let totals = order.orderItems |> OrderItemCalculation.getTotals;
    <div className="order-items">
      <h2> (s("Order Items")) </h2>
      <table>
        <tbody>
          (
            order.orderItems
            |> List.map((i: OrderData.Order.orderItem) =>
                 <tr>
                   <td>
                     <div
                       className="card small-card danger-card"
                       onClick=((_) => onRemoveItem(i))>
                       (s("Eliminar"))
                     </div>
                   </td>
                   <td> (s(i.name)) </td>
                   <td> (s(string_of_int(i.salePrice))) </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
        <tfoot>
          <tr className="divider">
            <th colSpan=2> (s("Sub-total")) </th>
            <td> (s(totals.subTotal |> string_of_int)) </td>
          </tr>
          <tr>
            <th colSpan=2> (s("Impuesto")) </th>
            <td> (s(totals.tax |> string_of_int)) </td>
          </tr>
          <tr>
            <th colSpan=2> (s("Total")) </th>
            <td> (s(totals.total |> string_of_int)) </td>
          </tr>
        </tfoot>
      </table>
    </div>;
  },
};