let component = ReasonReact.statelessComponent("OrderItemRow");

let make =
    (
      ~orderItem: OrderItem.t,
      ~removeOrderItem=() => (),
      ~discounts: list(Discount.t),
      ~showDialog=() => (),
      ~changeQuantity=_quantity => (),
      _children,
    ) => {
  ...component,
  render: _self => {
    let totals = OrderItemCalculation.getTotals(discounts, [orderItem]);
    <tr>
      <td>
        <Button
          className="smallItems-card danger-card"
          onClick=(_ => removeOrderItem())
          iconClass="far fa-trash-alt"
          local=true
        />
      </td>
      <td className="quantity">
        <QuantitySelector
          onChange=(quantity => changeQuantity(quantity))
          value=orderItem.quantity
        />
      </td>
      <td>
        <Button
          className="smallItems-card"
          onClick=(_ => showDialog())
          iconClass="fas fa-edit"
        />
      </td>
      <td> (ReactUtils.s(orderItem.name)) </td>
      <td>
        (
          ReactUtils.s(
            Js.Float.toFixedWithPrecision(
              (orderItem.suggestedPrice |> float_of_int) /. 100.,
              2,
            ),
          )
        )
      </td>
      <td> (ReactUtils.s(totals.subTotal |> Money.toDisplay)) </td>
    </tr>;
  },
};