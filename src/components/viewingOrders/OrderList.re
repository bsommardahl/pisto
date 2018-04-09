open Util;

let component = ReasonReact.statelessComponent("OrderList");

let headerRow = () =>
  <tr>
    <th />
    <th> (s("Cliente")) </th>
    <th> (s("Subtotal")) </th>
    <th> (s("Impuesto")) </th>
    <th> (s("Total")) </th>
    <th> (s("Fecha")) </th>
    <th> (s("Hora")) </th>
  </tr>;

let row = (o: Order.orderVm, onSelect) => {
  let totals = o.orderItems |> OrderItemCalculation.getTotals(o.discounts);
  let paidOn =
    switch (o.paid) {
    | Some(paid) => paid.on
    | None => 0.0
    };
  <tr>
    <td>
      <div className="card" onClick=((_) => onSelect(o))> (s("Ver")) </div>
    </td>
    <td> (s(o.customerName)) </td>
    <td> (s(totals.subTotal |> Money.toDisplay)) </td>
    <td> (s(totals.tax |> Money.toDisplay)) </td>
    <td> (s(totals.total |> Money.toDisplay)) </td>
    <td> (s(paidOn |> Date.toDisplayDate)) </td>
    <td> (s(paidOn |> Date.toDisplayTime)) </td>
  </tr>;
};

let make = (~orders: list(Order.orderVm), ~onSelect, _children) => {
  ...component,
  render: _self =>
    <table className="order-list">
      <thead> (headerRow()) </thead>
      <tbody>
        (
          orders
          |> List.map(o => row(o, onSelect))
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </tbody>
    </table>,
};