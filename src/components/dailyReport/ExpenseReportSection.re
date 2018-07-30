open ReactUtils;

let header =
  <tr className="table-header">
    <th className="text-cell"> (sloc("expense.date")) </th>
    <th className="text-cell"> (sloc("expense.vendor")) </th>
    <th className="text-cell"> (sloc("expense.description")) </th>
    <th className="number-cell"> (sloc("expense.subTotals")) </th>
    <th className="number-cell"> (sloc("expense.tax")) </th>
    <th className="number-cell"> (sloc("expense.total")) </th>
  </tr>;

let expenseRow = (e: Expense.denormalized) =>
  <tr>
    <td className="text-cell"> (s(e.date |> Date.toDisplayDate)) </td>
    <td className="text-cell"> (s(e.vendor.name)) </td>
    <td className="text-cell"> (s(e.description)) </td>
    <td className="number-cell"> (s(e.subTotal |> Money.toDisplay)) </td>
    <td className="number-cell"> (s(e.tax |> Money.toDisplay)) </td>
    <td className="number-cell"> (s(e.total |> Money.toDisplay)) </td>
  </tr>;

let expensesBody = (title: string, expenses: list(Expense.denormalized)) => {
  let grandSubTotal =
    expenses |. Belt.List.reduce(0, (a, c) => a + c.subTotal);
  let grandTax = expenses |. Belt.List.reduce(0, (a, c) => a + c.tax);
  let grandTotal = expenses |. Belt.List.reduce(0, (a, c) => a + c.total);
  <tbody>
    <tr className="section-header">
      <td colSpan=6> <h4> (s(title)) </h4> </td>
    </tr>
    header
    (
      expenses
      |> List.map((e: Expense.denormalized) => expenseRow(e))
      |> Array.of_list
      |> ReasonReact.array
    )
    <tr className="section-footer">
      <td />
      <td />
      <td />
      <td className="number-cell"> (s(grandSubTotal |> Money.toDisplay)) </td>
      <td className="number-cell"> (s(grandTax |> Money.toDisplay)) </td>
      <td className="number-cell"> (s(grandTotal |> Money.toDisplay)) </td>
    </tr>
  </tbody>;
};

let component = ReasonReact.statelessComponent("ExpenseReportSection");

let make = (~expenses: list(Expense.t), ~key="", _children) => {
  ...component,
  render: _self => {
    let denormalized = expenses |> Expense.denormalize;
    let groups =
      denormalized
      |> Group.by((x: Expense.denormalized) => {
           let pre = "daily.expensesSection.title.pre" |> Lang.translate;
           let post = "daily.expensesSection.title.post" |> Lang.translate;
           let title =
             x.expenseType.name
             ++ " "
             ++ pre
             ++ " "
             ++ (x.taxRate |> Percent.toDisplay)
             ++ " "
             ++ post;
           title;
         });
    <div className="report-section" key>
      <table className="table">
        (
          groups
          |> List.map((g: Group.group(Expense.denormalized)) =>
               expensesBody(g.key, g.value)
             )
          |> Array.of_list
          |> ReasonReact.array
        )
      </table>
    </div>;
  },
};