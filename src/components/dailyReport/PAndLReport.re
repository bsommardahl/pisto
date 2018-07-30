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
          <td> <h1> (sloc("daily.header")) </h1> </td>
          <td className="right-side-cell">
            (
              Config.App.get().language === "EN" ?
                s(
                  (startDate |> Date.toDisplayDateEN)
                  ++ " - "
                  ++ (endDate |> Date.toDisplayDateEN),
                ) :
                s(
                  (startDate |> Date.toDisplayDate)
                  ++ " - "
                  ++ (endDate |> Date.toDisplayDate),
                )
            )
          </td>
        </tr>
      </table>
      <h2> (sloc("daily.sales.header")) </h2>
      (
        taxRateGroups
        |> List.map((g: Group.group(Sale.t)) => {
             let key = g.key;
             let pre = "daily.salesSection.title.pre" |> Lang.translate;
             let post = "daily.salesSection.title.post" |> Lang.translate;
             let title =
               pre
               ++ " "
               ++ (key |> Percent.toT |> Percent.toDisplay)
               ++ " "
               ++ post;
             <SalesReportSection title sales=g.value />;
           })
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
      <h2> (sloc("daily.discounts.header")) </h2>
      <DiscountsReportSection orders />
      <h2> (sloc("daily.expenses.header")) </h2>
      <ExpenseReportSection expenses />
      <h2> (sloc("daily.totals.header")) </h2>
      <GrandTotalsSection sales expenses />
    </div>;
  },
};