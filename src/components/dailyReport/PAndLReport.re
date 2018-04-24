open ReactUtils;

let component = ReasonReact.statelessComponent("PAndLReport");

let make =
    (
      ~startDate: Date.t,
      ~endDate: Date.t,
      ~orders: list(Order.t),
      ~expenses: list(Expense.t),
      ~key="",
      _children,
    ) => {
  ...component,
  render: _self => {
    let sales: list(Sale.t) =
      orders |> List.map(x => x |> Sale.listOfSales) |> List.concat;
    let taxRateGroups =
      sales |> Group.by((x: Sale.t) => x.taxRate |> string_of_int);
    <div className="report" key>
      <table className="table">
        <tr>
          <td> <h1> (s("Informe de Ganancia y Perdida")) </h1> </td>
          <td className="right-side-cell">
            (
              s(
                (startDate |> Date.toDisplayDate)
                ++ " - "
                ++ (endDate |> Date.toDisplayDate),
              )
            )
          </td>
        </tr>
      </table>
      <h2> (s("Ventas")) </h2>
      (
        taxRateGroups
        |> List.map((g: Group.group(Sale.t)) => {
             let key = g.key;
             let title = {j|Ventas con $key% ISV|j};
             <SalesReportSection title sales=g.value />;
           })
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
      <h2> (s("Descuentos")) </h2>
      <DiscountsReportSection orders />
      <h2> (s("Gastos")) </h2>
      <ExpenseReportSection expenses />
      <h2> (s("Totales")) </h2>
      <GrandTotalsSection sales expenses />
    </div>;
  },
};