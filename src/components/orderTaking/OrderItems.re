let component = ReasonReact.statelessComponent("OrderItems");

let make =
    (
      ~closed: bool,
      ~order: Order.orderVm,
      ~canDeselectDiscount=true,
      ~canRemoveItem=true,
      ~deselectDiscount=_d => (),
      ~onRemoveItem=_i => (),
      _children,
    ) => {
  ...component,
  render: _self => {
    let totals =
      OrderItemCalculation.getTotals(order.discounts, order.orderItems);
    <div className="order-items">
      <h2> (ReactUtils.sloc("order.orderItems.header")) </h2>
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
                       if (! closed && canRemoveItem) {
                         <Button
                           className="small-card danger-card"
                           onClick=((_) => onRemoveItem(i))
                           label="action.delete"
                           local=true
                         />;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td> (ReactUtils.s(i.name)) </td>
                   <td>
                     (ReactUtils.s(totals.subTotal |> Money.toDisplay))
                   </td>
                 </tr>;
               })
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
        <tfoot>
          <tr className="divider">
            <th colSpan=2> (ReactUtils.sloc("order.subTotal")) </th>
            <td> (ReactUtils.s(totals.subTotal |> Money.toDisplay)) </td>
          </tr>
          (
            if (order.discounts |> List.length > 0) {
              <tr>
                <th colSpan=2> (ReactUtils.sloc("order.discounts")) </th>
                <td> (ReactUtils.s(totals.discounts |> Money.toDisplay)) </td>
              </tr>;
            } else {
              <tr />;
            }
          )
          <tr>
            <th colSpan=2> (ReactUtils.sloc("order.tax")) </th>
            <td> (ReactUtils.s(totals.tax |> Money.toDisplay)) </td>
          </tr>
          <tr>
            <th colSpan=2> (ReactUtils.sloc("order.total")) </th>
            <td> (ReactUtils.s(totals.total |> Money.toDisplay)) </td>
          </tr>
        </tfoot>
      </table>
      (
        order.discounts
        |> List.map((d: Discount.t) =>
             <button
               className="card small-card card-discount"
               disabled=(closed || canDeselectDiscount ? true : false)
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