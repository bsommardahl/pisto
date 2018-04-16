open ReactUtils;

let component = ReasonReact.statelessComponent("ClosedOrderInfo");

let make = (~order: Order.orderVm, ~paidDateChanged, _children) => {
  ...component,
  render: _self =>
    <div className="paid-date">
      <table>
        <tbody>
          <tr> <td colSpan=2> <h2> (s("Creado")) </h2> </td> </tr>
          <tr>
            <th> (s("Fecha")) </th>
            <td> (s(order.createdOn |> Date.toDisplay)) </td>
          </tr>
        </tbody>
        (
          switch (order.paid) {
          | None => <tbody />
          | Some(paid) =>
            <tbody>
              <tr> <td colSpan=2> <h2> (s("Pagado")) </h2> </td> </tr>
              <tr>
                <th> (s("Fecha")) </th>
                <td>
                  <EditableDate date=paid.on onChange=paidDateChanged />
                </td>
              </tr>
              <tr> <th> (s("Por")) </th> <td> (s(paid.by)) </td> </tr>
            </tbody>
          }
        )
        (
          switch (order.returned) {
          | None => <tbody />
          | Some(returned) =>
            <tbody>
              <tr> <td colSpan=2> <h2> (s("Devuelto")) </h2> </td> </tr>
              <tr>
                <th> (s("Fecha")) </th>
                <td> (s(returned.on |> Date.toDisplay)) </td>
              </tr>
              <tr> <th> (s("Por")) </th> <td> (s(returned.by)) </td> </tr>
            </tbody>
          }
        )
      </table>
    </div>,
};