open Util;

open Js.Promise;

type state = {
  products: list(Product.t),
  bulkImport: string,
};

type action =
  | LoadProducts(list(Product.t))
  | NewProductCreated(Product.t)
  | ProductRemoved(Product.t)
  | ProductModified(Product.t)
  | UpdateBulkImport(string);

let component = ReasonReact.reducerComponent("ProductManagement");

let make = _children => {
  ...component,
  didMount: self => {
    ProductStore.getAll()
    |> Js.Promise.then_(products => {
         self.send(LoadProducts(products));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {products: [], bulkImport: ""},
  reducer: (action, state) =>
    switch (action) {
    | LoadProducts(productsLoaded) =>
      ReasonReact.Update({...state, products: productsLoaded})
    | ProductRemoved(product) =>
      ReasonReact.Update({
        ...state,
        products:
          state.products |> List.filter((p: Product.t) => p.id !== product.id),
      })
    | ProductModified(product) =>
      ReasonReact.Update({
        ...state,
        products:
          state.products
          |> List.map((p: Product.t) => p.id === product.id ? product : p),
      })
    | NewProductCreated(newProduct) =>
      ReasonReact.Update({
        ...state,
        products: List.concat([state.products, [newProduct]]),
      })
    | UpdateBulkImport(newVal) =>
      ReasonReact.Update({...state, bulkImport: newVal})
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let removeProduct = (p: Product.t) => {
      ProductStore.remove(p.id)
      |> then_((_) => {
           self.send(ProductRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyProduct = (modifiedProduct: Product.t) =>
      ProductStore.update(modifiedProduct)
      |> then_((_) => {
           self.send(ProductModified(modifiedProduct));
           resolve();
         })
      |> ignore;
    let createProduct = (newProduct: Product.NewProduct.t) => {
      ProductStore.add(newProduct)
      |> Js.Promise.then_((newProduct: Product.t) => {
           self.send(NewProductCreated(newProduct));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    let importBulkProducts = blob =>
      blob
      |> Js.String.split("\n")
      |> Array.to_list
      |> List.map((line: string) => {
           let arr = line |> Js.String.split("\t") |> Array.to_list;
           let newProd: Product.NewProduct.t = {
             sku: List.nth(arr, 0),
             name: List.nth(arr, 1),
             suggestedPrice: List.nth(arr, 2) |> Money.toT,
             taxCalculation: List.nth(arr, 3) |> Tax.Calculation.toMethod,
             tags: List.nth(arr, 4) |> Js.String.split(",") |> Array.to_list,
           };
           newProd;
         })
      |> List.iter(p => createProduct(p));
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (s("Atras"))
          </div>
        </div>
        <div className="header-options"> (s("Gestion de Productos")) </div>
      </div>
      <div className="product-management">
        <table className="admin-table">
          <thead>
            <tr>
              <th />
              <th> (s("Sku")) </th>
              <th> (s("Nombre")) </th>
              <th> (s("Precio")) </th>
              <th> (s("Metodo de Impuesto")) </th>
              <th> (s("Tags/Categorias")) </th>
              <th />
            </tr>
          </thead>
          <tbody>
            (
              self.state.products
              |> List.map((p: Product.t) =>
                   <ProductManagementRow
                     product=p
                     removeProduct
                     modifyProduct
                   />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <CreateProductFooter createProduct />
        </table>
        <h3> (s("Bulk Import")) </h3>
        <textArea
          className="bulk-import"
          value=self.state.bulkImport
          onChange=(ev => self.send(UpdateBulkImport(getVal(ev))))
        />
        <p>
          <button onClick=((_) => importBulkProducts(self.state.bulkImport))>
            (s("Import All"))
          </button>
        </p>
      </div>
    </div>;
  },
};