

let component = ReasonReact.statelessComponent("OrderItems");

let make =
    (
      ~closed: bool,
      ~order: Order.orderVm,
      ~deselectDiscount,
      ~onRemoveItem,
      _children,
    ) => {
  ...component,
  render: _self => {
    let totals =
      OrderItemCalculation.getTotals(order.discounts, order.orderItems);
    <div className="order-items">
      <h2> (ReactUtils.s("Order Items")) </h2>
      <table>
        <tbody>
          (
            order.orderItems
            |> List.map((i: OrderItem.t) => {
                 let totals =
                   OrderItemCalculation.getTotals(order.discounts, [i]);
                 <tr>
                   <td>
                     (
                       if (! closed) {
                         <div
                           className="card small-card danger-card"
                           onClick=((_) => onRemoveItem(i))>
                           (ReactUtils.s("Eliminar"))
                         </div>;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td> (ReactUtils.s(i.name)) </td>
                   <td> (ReactUtils.s(totals.subTotal |> Money.toDisplay)) </td>
                 </tr>;
               })
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
        <tfoot>
          <tr className="divider">
            <th colSpan=2> (ReactUtils.s("Sub-total")) </th>
            <td> (ReactUtils.s(totals.subTotal |> Money.toDisplay)) </td>
          </tr>
          (
            if (order.discounts |> List.length > 0) {
              <tr>
                <th colSpan=2> (ReactUtils.s("Descuento")) </th>
                <td> (ReactUtils.s(totals.discounts |> Money.toDisplay)) </td>
              </tr>;
            } else {
              <tr />;
            }
          )
          <tr>
            <th colSpan=2> (ReactUtils.s("Impuesto")) </th>
            <td> (ReactUtils.s(totals.tax |> Money.toDisplay)) </td>
          </tr>
          <tr>
            <th colSpan=2> (ReactUtils.s("Total")) </th>
            <td> (ReactUtils.s(totals.total |> Money.toDisplay)) </td>
          </tr>
        </tfoot>
      </table>
      (
        order.discounts
        |> List.map((d: Discount.t) =>
             <button
               className="card small-card card-discount"
               disabled=(closed ? Js.true_ : Js.false_)
               onClick=((_) => deselectDiscount(d))>
               (ReactUtils.s(d.name))
             </button>
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};