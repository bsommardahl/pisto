module ProductFormParams = {
  type state = {
    name: string,
    sku: string,
    price: string,
    taxCalculation: string,
    tags: string,
  };
  type fields = [ | `name | `sku | `price | `taxCalculation | `tags];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`sku, s => s.sku, (s, sku) => {...s, sku}),
    (`price, s => s.price, (s, price) => {...s, price}),
    (
      `taxCalculation,
      s => s.taxCalculation,
      (s, taxCalculation) => {...s, taxCalculation},
    ),
    (`tags, s => s.tags, (s, tags) => {...s, tags}),
  ];
};

let validationMessage = message =>
  switch (message) {
  | None => ReasonReact.nullElement
  | Some(msg) => <span className="invalid"> (ReactUtils.sloc(msg)) </span>
  };

module EditProductForm = ReForm.Create(ProductFormParams);

let component = ReasonReact.statelessComponent("ProductEdit");

let make =
    (~product: option(Product.t)=None, ~onSubmit, ~products, _children) => {
  ...component,
  render: _self => {
    let hasDuplicateSku = sku => {
      let duplicates =
        products |> List.filter((c: Product.t) => c.sku === sku);
      let isDuplicate = duplicates |> List.length > 0;
      isDuplicate ? Some("validation.duplicate") : None;
    };
    let isUnique = (original: option(Product.t), new_) =>
      switch (original) {
      | None => hasDuplicateSku(new_)
      | Some(prod) =>
        if (prod.sku === new_) {
          None;
        } else {
          hasDuplicateSku(new_);
        }
      };
    <EditProductForm
      onSubmit
      initialState=(
        switch (product) {
        | None => {name: "", sku: "", price: "", taxCalculation: "", tags: ""}
        | Some(prod) => {
            name: prod.name,
            sku: prod.sku,
            price: prod.suggestedPrice |> Money.toDisplay,
            taxCalculation:
              prod.taxCalculation |> Tax.Calculation.toDelimitedString,
            tags: prod.tags |> Tags.toCSV,
          }
        }
      )
      schema=[
        (`name, Required),
        (`sku, Custom(v => v.sku |> isUnique(product))),
        (`price, Required),
        (`taxCalculation, Required),
        (`tags, Required),
      ]>
      ...(
           ({handleSubmit, handleChange, form, getErrorForField}) => {
             let field = (label, value, fieldType: ProductFormParams.fields) =>
               <div className="field-input">
                 <label>
                   (ReactUtils.sloc(label))
                   <input
                     value
                     onChange=(
                       ReForm.Helpers.handleDomFormChange(
                         handleChange(fieldType),
                       )
                     )
                   />
                 </label>
                 (validationMessage(getErrorForField(fieldType)))
               </div>;
             <form
               onSubmit=(ReForm.Helpers.handleDomFormSubmit(handleSubmit))>
               (field("product.name", form.values.name, `name))
               (field("product.sku", form.values.sku, `sku))
               (field("product.price", form.values.price, `price))
               (
                 field(
                   "product.taxCalculation",
                   form.values.taxCalculation,
                   `taxCalculation,
                 )
               )
               (field("product.tags", form.values.tags, `tags))
               <Button _type="submit" label="action.done" local=true />
             </form>;
           }
         )
    </EditProductForm>;
  },
};