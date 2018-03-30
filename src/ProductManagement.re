open Util;

open Js.Promise;

type state = {products: list(Product.t)};

type action =
  | LoadProducts(list(Product.t))
  | NewProductCreated(Product.t)
  | ProductRemoved(Product.t)
  | ProductModified(Product.t);

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
  initialState: () => {products: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadProducts(productsLoaded) =>
      ReasonReact.Update({products: productsLoaded})
    | ProductRemoved(product) =>
      ReasonReact.Update({
        products:
          state.products |> List.filter((p: Product.t) => p.id !== product.id),
      })
    | ProductModified(product) =>
      ReasonReact.Update({
        products:
          state.products
          |> List.map((p: Product.t) => p.id == product.id ? product : p),
      })
    | NewProductCreated(newProduct) =>
      ReasonReact.Update({
        products: List.concat([state.products, [newProduct]]),
      })
    },
  render: self => {
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
      |> then_(rev => {
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
      </div>
    </div>;
  },
};