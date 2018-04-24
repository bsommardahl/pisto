open ReactUtils;

let header =
  <tr className="table-header">
    <th className="text-cell"> (s("Fecha")) </th>
    <th className="text-cell"> (s("Vendedor")) </th>
    <th className="text-cell"> (s("Descripcion")) </th>
    <th className="number-cell"> (s("Subtotal")) </th>
    <th className="number-cell"> (s("ISV")) </th>
    <th className="number-cell"> (s("Total")) </th>
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
      |> ReasonReact.arrayToElement
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
      |> Group.by((x: Expense.denormalized) =>
           x.expenseType.name
           ++ " con ISV "
           ++ (x.taxRate |> Percent.toDisplay)
         );
    <div className="report-section" key>
      <table className="table">
        (
          groups
          |> List.map((g: Group.group(Expense.denormalized)) =>
               expensesBody(g.key, g.value)
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </table>
    </div>;
  },
};