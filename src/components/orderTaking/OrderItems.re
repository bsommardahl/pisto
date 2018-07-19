type state = {
  orderItems: list(OrderItem.t),
  selectedOrderItem: option(OrderItem.t),
  value: string,
  notes: list(OrderItemNote.t),
  showDialog: bool,
};

type action =
  | RemoveOrderItem(OrderItem.t)
  | ChangeQuantity(OrderItem.t, int)
  | DisplayNote(string)
  | AddNotesToOrderItem
  | RemoveNote(OrderItemNote.t)
  | ShowDialog(OrderItem.t)
  | HideDialog;
let str = ReasonReact.stringToElement;
let lastId = ref(0);
let newNote = (value: string) => {
  lastId := lastId^ + 1;
  {OrderItemNote.id: lastId^ |> string_of_int, value};
};

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
  initialState: () => {
    orderItems,
    showDialog: false,
    value: "",
    notes: [],
    selectedOrderItem: None,
  },
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
    | AddNotesToOrderItem =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          showDialog: false,
          orderItems:
            switch (state.selectedOrderItem) {
            | Some(orderItem) =>
              orderItems
              |> List.map((i: OrderItem.t) =>
                   if (i.id === orderItem.id) {
                     {...i, notes: state.notes};
                   } else {
                     i;
                   }
                 )
            | None => orderItems
            },
        },
        (self => onChange(self.state.orderItems)),
      )
    | DisplayNote(value) =>
      ReasonReact.Update({
        ...state,
        value,
        notes:
          value !== "" ?
            List.concat([state.notes, [newNote(value)]]) : state.notes,
      })
    | RemoveNote(note) =>
      ReasonReact.Update({
        ...state,
        notes:
          state.notes |> List.filter((n: OrderItemNote.t) => n.id !== note.id),
      })
    | ShowDialog(orderItem) =>
      ReasonReact.Update({
        ...state,
        showDialog: true,
        notes: orderItem.notes,
        selectedOrderItem: Some(orderItem),
      })
    | HideDialog => ReasonReact.Update({...state, showDialog: false})
    },
  render: self => {
    let totals = OrderItemCalculation.getTotals(discounts, orderItems);
    <div className="order-items">
      <h2> (ReactUtils.sloc("order.orderItems.header")) </h2>
      <OrderItemsNotes
        isOpen=self.state.showDialog
        removeNote=(note => self.send(RemoveNote(note)))
        addNote=(value => self.send(DisplayNote(value)))
        notes=self.state.notes
        value=self.state.value
        label="action.addNotes"
        onCancel=(_ => self.send(HideDialog))
        onAccept=(_ => self.send(AddNotesToOrderItem))
      />
      <table>
        (
          orderItems
          |> List.map((i: OrderItem.t) => {
               let totals = OrderItemCalculation.getTotals(discounts, [i]);
               <tbody key=i.id>
                 <tr>
                   <td>
                     <Button
                       className="smallItems-card danger-card"
                       onClick=(_ => self.send(RemoveOrderItem(i)))
                       label="actionSymbol.delete"
                       local=true
                     />
                   </td>
                   <td className="quantity">
                     <QuantitySelector
                       onChange=(
                         quantity => self.send(ChangeQuantity(i, quantity))
                       )
                       value=i.quantity
                     />
                   </td>
                   <td>
                     <Button
                       className="smallItems-card"
                       onClick=(_ => self.send(ShowDialog(i)))
                       label=">"
                     />
                   </td>
                   <td> (ReactUtils.s(i.name)) </td>
                   <td>
                    (
                      ReactUtils.s(
                        Js.Float.toFixedWithPrecision(
                          (i.suggestedPrice |> float_of_int) /. 100.,
                          2,
                        ),
                      )
                    )
                   </td>
                   <td>
                     (ReactUtils.s(totals.subTotal |> Money.toDisplay))
                   </td>
                 </tr>
                 (
                   i.notes |> List.length > 0 ?
                     <DisplayOrderItemNotes notes=i.notes /> :
                     ReasonReact.nullElement
                 )
               </tbody>;
             })
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
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