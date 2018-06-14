open Js.Promise;

type intent =
  | Viewing
  | Deleting(Product.t)
  | Modifying(Product.t);

type state = {
  products: list(Product.t),
  intent,
};

type action =
  | LoadProducts
  | ProductsLoaded(list(Product.t))
  | RemoveProduct(Product.t)
  | ShowDialog(Product.t)
  | HideDialog
  | ModifyProduct(Product.t)
  | CreateProduct(Product.NewProduct.t)
  | ProductCreated(Product.t)
  | Change(intent);

let component = ReasonReact.reducerComponent("ProductManagement");

let make = _children => {
  ...component,
  initialState: () => {products: [], intent: Viewing},
  didMount: self => {
    self.send(LoadProducts);
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadProducts =>
      ReasonReact.SideEffects(
        (
          self =>
            ProductStore.getAll()
            |> Js.Promise.then_(products => {
                 self.send(ProductsLoaded(products));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | ProductsLoaded(products) => ReasonReact.Update({...state, products})
    | ShowDialog(prod) =>
      ReasonReact.Update({...state, intent: Deleting(prod)})
    | HideDialog => ReasonReact.Update({...state, intent: Viewing})
    | Change(intent) => ReasonReact.Update({...state, intent})
    | RemoveProduct(prod) =>
      ReasonReact.UpdateWithSideEffects(
        {
          intent: Viewing,
          products:
            state.products |> List.filter((d: Product.t) => d.id !== prod.id),
        },
        (
          _self =>
            ProductStore.remove(prod.id) |> then_(_ => resolve()) |> ignore
        ),
      )
    | ModifyProduct(product) =>
      ReasonReact.UpdateWithSideEffects(
        {
          intent: Viewing,
          products:
            state.products
            |> List.map((d: Product.t) => d.id === product.id ? product : d),
        },
        (
          _self =>
            ProductStore.update(product)
            |> Js.Promise.catch(err => {
                 Js.log(err);
                 Js.Promise.resolve(product);
               })
            |> ignore
        ),
      )
    | CreateProduct(prod) =>
      ReasonReact.SideEffects(
        (
          self =>
            ProductStore.add(prod)
            |> Js.Promise.then_((newProduct: Product.t) => {
                 self.send(ProductCreated(newProduct));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | ProductCreated(prod) =>
      ReasonReact.Update({
        ...state,
        products: List.concat([state.products, [prod]]),
      })
    },
  render: self => {
    let goBack = _ => ReasonReact.Router.push("/admin");
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (ReactUtils.sloc("admin.products.header"))
        </div>
      </div>
      (
        switch (self.state.intent) {
        | Deleting(prod) =>
          <DeleteModal
            contentLabel="modal.deleteProductContent"
            label="modal.deleteProduct"
            isOpen=true
            onConfirm=(() => self.send(RemoveProduct(prod)))
            onCancel=(() => self.send(HideDialog))
          />
        | Viewing =>
          <div className="product-management">
            <table className="table">
              <thead>
                <tr>
                  <th />
                  <th> (ReactUtils.sloc("product.sku")) </th>
                  <th> (ReactUtils.sloc("product.name")) </th>
                  <th> (ReactUtils.sloc("product.price")) </th>
                  <th> (ReactUtils.sloc("product.taxCalculation")) </th>
                  <th> (ReactUtils.sloc("product.tags")) </th>
                  <th />
                </tr>
              </thead>
              <tbody>
                (
                  self.state.products
                  |> List.map((prod: Product.t) =>
                       <tr key=prod.id>
                         <td>
                           <Button
                             local=true
                             disabled=false
                             onClick=(
                               _ => self.send(Change(Modifying(prod)))
                             )
                             label="action.edit"
                           />
                         </td>
                         <td> (ReactUtils.s(prod.sku)) </td>
                         <td> (ReactUtils.s(prod.name)) </td>
                         <td>
                           (
                             ReactUtils.s(
                               prod.suggestedPrice |> Money.toDisplay,
                             )
                           )
                         </td>
                         <td>
                           (
                             ReactUtils.s(
                               prod.taxCalculation
                               |> Tax.Calculation.toDelimitedString,
                             )
                           )
                         </td>
                         <td> (ReactUtils.s(prod.tags |> Tags.toCSV)) </td>
                         <td>
                           <Button
                             local=true
                             className="danger-card"
                             onClick=(_ => self.send(ShowDialog(prod)))
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
            <h3> (ReactUtils.sloc("action.create")) </h3>
            <ProductEdit
              products=self.state.products
              onSubmit=(
                ({values}) =>
                  self.send(
                    CreateProduct({
                      sku: values.sku,
                      name: values.name,
                      suggestedPrice: values.price |> Money.toT,
                      taxCalculation:
                        values.taxCalculation |> Tax.Calculation.toMethod,
                      tags: values.tags |> Tags.toList,
                      onHand: 0,
                      weight: 0,
                      startDate: None,
                      endDate: None,
                      department: "",
                      unit: "",
                      products: [],
                      location: "",
                    }),
                  )
              )
            />
          </div>
        | Modifying(product) =>
          <div>
            <h3> (ReactUtils.sloc("action.edit")) </h3>
            <ProductEdit
              products=self.state.products
              product=(Some(product))
              onSubmit=(
                ({values}) =>
                  self.send(
                    ModifyProduct({
                      id: product.id,
                      name: values.name,
                      sku: values.sku,
                      suggestedPrice: values.price |> Money.toT,
                      taxCalculation:
                        values.taxCalculation |> Tax.Calculation.toMethod,
                      tags: values.tags |> Tags.toList,
                      onHand: product.onHand,
                      startDate: product.startDate,
                      endDate: product.endDate,
                      department: product.department,
                      weight: product.weight,
                      unit: product.unit,
                      products: product.products,
                      location: product.location,
                    }),
                  )
              )
            />
          </div>
        }
      )
    </div>;
  },
};