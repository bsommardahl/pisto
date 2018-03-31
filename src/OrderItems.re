open Util;

let component = ReasonReact.statelessComponent("OrderItems");

let make =
    (
      ~closed: bool,
      ~order: OrderData.Order.orderVm,
      ~deselectDiscount,
      ~onRemoveItem,
      _children,
    ) => {
  ...component,
  render: _self => {
    let totals =
      order.orderItems |> OrderItemCalculation.getTotals(order.discounts);
    <div className="order-items">
      <h2> (s("Order Items")) </h2>
      <table>
        <tbody>
          (
            order.orderItems
            |> List.map((i: OrderData.Order.orderItem) =>
                 <tr>
                   <td>
                     (
                       if (! closed) {
                         <div
                           className="card small-card danger-card"
                           onClick=((_) => onRemoveItem(i))>
                           (s("Eliminar"))
                         </div>;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td> (s(i.name)) </td>
                   <td> (s(i.salePrice |> Money.toDisplay)) </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
        <tfoot>
          <tr className="divider">
            <th colSpan=2> (s("Sub-total")) </th>
            <td> (s(totals.subTotal |> Money.toDisplay)) </td>
          </tr>
          <tr>
            <th colSpan=2> (s("Impuesto")) </th>
            <td> (s(totals.tax |> Money.toDisplay)) </td>
          </tr>
          <tr>
            <th colSpan=2> (s("Total")) </th>
            <td> (s(totals.total |> Money.toDisplay)) </td>
          </tr>
        </tfoot>
      </table>
      (
        order.discounts
        |> List.map((d: Discount.t) =>
             <div
               className="card small-card card-discount"
               onClick=((_) => deselectDiscount(d))>
               (s(d.name))
             </div>
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};