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
            <th> (s("expense.vendor" |> Lang.translate)) </th>
            <th> (s("expense.type" |> Lang.translate)) </th>
            <th> (s("expense.description" |> Lang.translate)) </th>
            <th> (s("expense.date" |> Lang.translate)) </th>
            <th> (s("expense.time" |> Lang.translate)) </th>
            <th> (s("expense.subTotals" |> Lang.translate)) </th>
            <th> (s("expense.tax" |> Lang.translate)) </th>
            <th> (s("expense.total" |> Lang.translate)) </th>
            <th />
          </tr>
        </thead>
        <tbody>
          (
            expenses
            |> List.map((e: Expense.t) =>
                 <tr key=e.id>
                   <td>
                     <Button
                       local=true
                       onClick=((_) => onModify(e))
                       label="action.edit"
                     />
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
                     <Button
                       local=true
                       onClick=((_) => onRemove(e))
                       label="action.delete"
                     />
                   </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
      </table>
      <Button local=true onClick=((_) => onNew()) label="action.create" />
    </div>;
  },
};