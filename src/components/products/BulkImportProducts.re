open Js.Promise;

type state = {bulkImport: string};

type action =
  | UpdateBulkImport(string)
  | ProcessBulkImport;

let component = ReasonReact.reducerComponent("BulkImportProducts");

let make = (~onNewProduct, _children) => {
  ...component,
  initialState: () => {bulkImport: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateBulkImport(newVal) => ReasonReact.Update({bulkImport: newVal})
    | ProcessBulkImport =>
      ReasonReact.SideEffects(
        (
          _self =>
            state.bulkImport
            |> Js.String.split("\n")
            |> Array.to_list
            |> List.map((line: string) => {
                 let arr = line |> Js.String.split("\t") |> Array.to_list;
                 let newProd: Product.NewProduct.t = {
                   sku: List.nth(arr, 0),
                   name: List.nth(arr, 1),
                   suggestedPrice: List.nth(arr, 2) |> Money.toT,
                   taxCalculation:
                     List.nth(arr, 3) |> Tax.Calculation.toMethod,
                   tags:
                     List.nth(arr, 4)
                     |> Js.String.split(",")
                     |> Array.to_list,
                 };
                 newProd;
               })
            |> List.iter(p => onNewProduct(p))
        ),
      )
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <div>
      <h3> (ReactUtils.sloc("admin.products.bulkImport.header")) </h3>
      <p>
        <i> (ReactUtils.sloc("admin.products.bulkImport.instructions")) </i>
      </p>
      <textarea
        className="bulk-import"
        value=self.state.bulkImport
        onChange=(ev => self.send(UpdateBulkImport(getVal(ev))))
      />
      <p>
        <Button
          local=true
          onClick=((_) => self.send(ProcessBulkImport))
          label="admin.products.bulkImport.importButton"
        />
      </p>
    </div>;
  },
};