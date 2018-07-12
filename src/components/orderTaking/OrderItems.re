type state = {
  orderItems: list(OrderItem.t),
  showDialog: bool,
};

type action =
  | RemoveOrderItem(OrderItem.t)
  | ChangeQuantity(OrderItem.t, int)
  | ShowDialog
  | HideDialog;

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
  initialState: () => {orderItems, showDialog: false},
  reducer: (action, state) =>
    switch (action) {
    | RemoveOrderItem(orderItem) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          orderItems:
            orderItems
            |> List.filter((i: OrderItem.t) => i.id !== orderItem.id),
        },
        (self => onChange(self.state.orderItems)),
      )
    | ChangeQuantity(orderItem, quantity) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          orderItems:
            orderItems
            |> List.map((i: OrderItem.t) =>
                 if (i.id === orderItem.id) {
                   {...i, quantity};
                 } else {
                   i;
                 }
               ),
        },
        (self => onChange(self.state.orderItems)),
      )
    | ShowDialog => ReasonReact.Update({...state, showDialog: true})
    | HideDialog => ReasonReact.Update({...state, showDialog: false})
    },
  render: self => {
    let totals = OrderItemCalculation.getTotals(discounts, orderItems);
    <div className="order-items">
      <OrderItemsNotes
        isOpen=self.state.showDialog
        label="action.addNotes"
        onCancel=(_ => self.send(HideDialog))
      />
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
                           className="smallItems-card danger-card"
                           onClick=(_ => self.send(RemoveOrderItem(i)))
                           label="actionSymbol.delete"
                           local=true
                         />;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td className="quantity">
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
                         ReactUtils.s(i.quantity |> string_of_int);
                       }
                     )
                   </td>
                   <td>
                     (
                       if (! closed && canRemoveItem) {
                         <Button
                           className="smallItems-card"
                           onClick=(_ => self.send(ShowDialog))
                           label=">"
                         />;
                       } else {
                         ReasonReact.nullElement;
                       }
                     )
                   </td>
                   <td> (ReactUtils.s(i.name)) </td>
                   <td>
                     (
                       ReactUtils.s(
                         mod_float(
                           (i.suggestedPrice |> float_of_int) /. 100.,
                           1.,
                         )
                         !== 0. ?
                           (i.suggestedPrice |> float_of_int)
                           /. 100.
                           |> string_of_float :
                           (
                             (i.suggestedPrice |> float_of_int)
                             /. 100.
                             |> string_of_float
                           )
                           ++ "00",
                       )
                     )
                   </td>
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
        |> ReasonReact.arrayToElement
      )
    </div>;
  },
};