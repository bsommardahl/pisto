open ReactUtils;

let component = ReasonReact.statelessComponent("ExpenseManagementList");

let make = (~expenses, ~onModify, ~onRemove, ~onNew, _children) => {
  ...component,
  render: _self => {
    let getSubTotalSum = (subTotals: list(Expense.SubTotal.t)) =>
      Belt.List.reduce(subTotals, 0, (acc, sub: Expense.SubTotal.t) =>
        acc + sub.amount
      );
    <div className="expense-management-list">
      <table className="table">
        <thead>
          <tr>
            <th />
            <th> (s("Vendor")) </th>
            <th> (s("Type")) </th>
            <th> (s("Descripcion")) </th>
            <th> (s("Date")) </th>
            <th> (s("Time")) </th>
            <th> (s("SubTotals")) </th>
            <th> (s("Tax")) </th>
            <th> (s("Total")) </th>
            <th />
          </tr>
        </thead>
        <tbody>
          (
            expenses
            |> List.map((e: Expense.t) =>
                 <tr key=e.id>
                   <td>
                     <button onClick=((_) => onModify(e))>
                       (s("Editar"))
                     </button>
                   </td>
                   <td> (s(e.vendor.name)) </td>
                   <td> (s(e.expenseType.name)) </td>
                   <td> (s(e.description)) </td>
                   <td> (s(e.date |> Date.toDisplayDate)) </td>
                   <td> (s(e.date |> Date.toShortTime)) </td>
                   <td>
                     (s(e.subTotals |> getSubTotalSum |> Money.toDisplay))
                   </td>
                   <td> (s(e.tax |> Money.toDisplay)) </td>
                   <td> (s(e.total |> Money.toDisplay)) </td>
                   <td>
                     <button onClick=((_) => onRemove(e))>
                       (s("Eliminar"))
                     </button>
                   </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
      </table>
      <div className="card" onClick=((_) => onNew())>
        (s("+ Nuevo Expense"))
      </div>
    </div>;
  },
};