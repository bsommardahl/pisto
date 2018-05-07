open ReactUtils;

let component = ReasonReact.statelessComponent("SalesReportSection");

let make = (~title: string, ~sales: list(Sale.t), ~key="", _children) => {
  ...component,
  render: _self => {
    let groups: list(ProductGroup.t) = sales |> ProductGroup.fromSalesList;
    let grandSubTotal =
      groups |. Belt.List.reduce(0, (a, c) => a + c.subTotal);
    let grandTax = groups |. Belt.List.reduce(0, (a, c) => a + c.tax);
    let grandTotal = groups |. Belt.List.reduce(0, (a, c) => a + c.total);
    <div className="report-section" key>
      <h4> (s(title)) </h4>
      <table className="table">
        <thead>
          <tr className="table-header">
            <th className="text-cell">
              (sloc("daily.salesSection.product"))
            </th>
            <th className="number-cell">
              (sloc("daily.salesSection.price"))
            </th>
            <th className="number-cell">
              (sloc("daily.salesSection.quantity"))
            </th>
            <th className="number-cell">
              (sloc("daily.salesSection.subTotal"))
            </th>
            <th className="number-cell">
              (sloc("daily.salesSection.tax"))
            </th>
            <th className="number-cell">
              (sloc("daily.salesSection.total"))
            </th>
          </tr>
        </thead>
        <tbody>
          (
            groups
            |> List.map((group: ProductGroup.t) =>
                 <tr>
                   <td className="text-cell"> (s(group.productName)) </td>
                   <td className="number-cell">
                     (s(group.salePrice |> Money.toDisplay))
                   </td>
                   <td className="number-cell">
                     (s(group.quantity |> string_of_int))
                   </td>
                   <td className="number-cell">
                     (s(group.subTotal |> Money.toDisplay))
                   </td>
                   <td className="number-cell">
                     (s(group.tax |> Money.toDisplay))
                   </td>
                   <td className="number-cell">
                     (s(group.total |> Money.toDisplay))
                   </td>
                 </tr>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </tbody>
        <tfoot>
          <tr className="section-footer">
            <td />
            <td />
            <td />
            <td className="number-cell">
              (s(grandSubTotal |> Money.toDisplay))
            </td>
            <td className="number-cell">
              (s(grandTax |> Money.toDisplay))
            </td>
            <td className="number-cell">
              (s(grandTotal |> Money.toDisplay))
            </td>
          </tr>
        </tfoot>
      </table>
    </div>;
  },
};