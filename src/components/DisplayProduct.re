let component = ReasonReact.statelessComponent("DisplayProduct");

let make =
    (
      ~closed: bool,
      ~order: Order.orderVm,
      ~canRemoveItem=true,
      ~onRemoveItem=_i => (),
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="order-items">
      <table>
        <tbody>
          {
            order.orderItems
            |> List.map((i: OrderItem.t) => {
                 let totals =
                   OrderItemCalculation.getTotals(order.discounts, [i]);
                 <tr>
                   <td>
                     {
                       if (!closed && canRemoveItem) {
                         <Button
                           className="small-card danger-card"
                           onClick={_ => onRemoveItem(i)}
                           label="action.delete"
                           local=true
                         />;
                       } else {
                         ReasonReact.null;
                       }
                     }
                   </td>
                   <td>
                     {
                       if (!closed && canRemoveItem) {
                         <Button
                           className="small-card save-button-card"
                           onClick={_ => onRemoveItem(i)}
                           label="action.select"
                           local=true
                         />;
                       } else {
                         ReasonReact.null;
                       }
                     }
                   </td>
                   <td> {ReactUtils.s(i.name)} </td>
                   <td>
                     {ReactUtils.s(totals.subTotal |> Money.toDisplay)}
                   </td>
                 </tr>;
               })
            |> Array.of_list
            |> ReasonReact.array
          }
        </tbody>
      </table>
    </div>,
};
