type state = {
  orderItems: list(OrderItem.t),
  quantity: int,
};

type action =
  | RemoveOrderItem(OrderItem.t)
  | ChangeQuantity(OrderItem.t, int);

let component = ReasonReact.reducerComponent("OrderItems");

let make =
    (
      ~closed: bool,
      ~orderItems: list(OrderItem.t),
      ~discounts: list(Discount.t),
      ~canDeselectDiscount=true,
      ~canRemoveItem=true,
      ~onChange=_ => (),
      ~deselectDiscount=_d => (),
      _children,
    ) => {
  ...component,
  initialState: () => {orderItems, quantity: 1},
  reducer: (action, state) =>
    switch (action) {
    | RemoveOrderItem(orderItem) =>
      Js.log("here");
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          orderItems:
            orderItems
            |> List.filter((i: OrderItem.t) => i.sku !== orderItem.sku),
        },
        (self => onChange(self.state.orderItems)),
      );
    | ChangeQuantity(orderItem, quantity) =>
      Js.log(quantity);
      ReasonReact.UpdateWithSideEffects(
        {
          orderItems:
            orderItems
            |> List.map((i: OrderItem.t) =>
                 if (i.sku === orderItem.sku) {
                   {...i, quantity};
                 } else {
                   i;
                 }
               ),
          quantity,
        },
        (self => onChange(self.state.orderItems)),
      );
    },
  render: self => {
    let totals =
      OrderItemCalculation.getTotals(discounts, self.state.orderItems);
    <div className="order-items">
      <h2> (ReactUtils.sloc("order.orderItems.header")) </h2>
      <table>
        <tbody>
          (
            orderItems
            |> List.map((i: OrderItem.t) => {
                 let totals = OrderItemCalculation.getTotals(discounts, [i]);
                 <tr>
                   <td>
                     (
                       if (! closed && canRemoveItem) {
                         <Button
                           className="small-card danger-card"
                           onClick=(_ => self.send(RemoveOrderItem(i)))
                           label="action.delete"
                           local=true
                         />;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td>
                     (
                       if (! closed && canRemoveItem) {
                         <QuantitySelector
                           onChange=(
                             quantity =>
                               self.send(ChangeQuantity(i, quantity))
                           )
                           value=i.quantity
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
            <td className="footTable">
              (ReactUtils.s(totals.subTotal |> Money.toDisplay))
            </td>
          </tr>
          (
            if (discounts |> List.length > 0) {
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
        discounts
        |> List.map((d: Discount.t) =>
             <button
               className="card small-card card-discount"
               disabled=(closed || canDeselectDiscount ? true : false)
               onClick=(_ev => deselectDiscount(d))>
               (ReactUtils.s(d.name))
             </button>
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};