let component = ReasonReact.statelessComponent("ClosedOrderItems");

let make =
    (
      ~orderItems: list(OrderItem.t),
      ~discounts: list(Discount.t),
      ~deselectDiscount=_d => (),
      _children,
    ) => {
  ...component,
  render: _self => {
    let totals = OrderItemCalculation.getTotals(discounts, orderItems);
    <div className="order-items">
      <h2> (ReactUtils.sloc("order.orderItems.header")) </h2>
      <table>
        <tbody>
          (
            orderItems
            |> List.map((i: OrderItem.t) => {
                 let totals = OrderItemCalculation.getTotals(discounts, [i]);
                 <tr key=i.id>
                   <td className="quantity">
                     (ReactUtils.s(i.quantity |> string_of_int))
                   </td>
                   <td />
                   <td> (ReactUtils.s(i.name)) </td>
                   <td>
                     (
                       ReactUtils.s(
                         Js.Float.toFixedWithPrecision(
                           (i.suggestedPrice |> float_of_int) /. 100.,
                           ~digits=2,
                         ),
                       )
                     )
                   </td>
                   <td>
                     (ReactUtils.s(totals.subTotal |> Money.toDisplay))
                   </td>
                 </tr>;
               })
            |> Array.of_list
            |> ReasonReact.array
          )
        </tbody>
        <tfoot>
          <tr className="divider">
            <th colSpan=4> (ReactUtils.sloc("order.subTotal")) </th>
            <td className="footTable">
              (ReactUtils.s(totals.subTotal |> Money.toDisplay))
            </td>
          </tr>
          (
            if (discounts |> List.length > 0) {
              <tr>
                <th colSpan=4> (ReactUtils.sloc("order.discounts")) </th>
                <td> (ReactUtils.s(totals.discounts |> Money.toDisplay)) </td>
              </tr>;
            } else {
              <tr />;
            }
          )
          <tr>
            <th colSpan=4> (ReactUtils.sloc("order.tax")) </th>
            <td> (ReactUtils.s(totals.tax |> Money.toDisplay)) </td>
          </tr>
          <tr>
            <th colSpan=4> (ReactUtils.sloc("order.total")) </th>
            <td> (ReactUtils.s(totals.total |> Money.toDisplay)) </td>
          </tr>
        </tfoot>
      </table>
      (
        discounts
        |> List.map((d: Discount.t) =>
             <button
               className="card small-card card-discount"
               key=d.id
               onClick=(_ev => deselectDiscount(d))>
               (ReactUtils.s(d.name))
             </button>
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>;
  },
};