module ProductFormParams = {
  type state = {
    name: string,
    sku: string,
    price: string,
    taxCalculationMethod: string,
    taxRate: string,
    tags: string,
  };
  type fields = [
    | `name
    | `sku
    | `price
    | `taxCalculationMethod
    | `taxRate
    | `tags
  ];
  let lens = [
    (`name, s => s.name, (s, name) => {...s, name}),
    (`sku, s => s.sku, (s, sku) => {...s, sku}),
    (`price, s => s.price, (s, price) => {...s, price}),
    (
      `taxCalculationMethod,
      s => s.taxCalculationMethod,
      (s, taxCalculationMethod) => {...s, taxCalculationMethod},
    ),
    (`taxRate, s => s.taxRate, (s, taxRate) => {...s, taxRate}),
    (`tags, s => s.tags, (s, tags) => {...s, tags}),
  ];
};

let validationMessage = message =>
  switch (message) {
  | None => ReasonReact.nullElement
  | Some(msg) => <span className="invalid"> (ReactUtils.sloc(msg)) </span>
  };

module EditProductForm = ReForm.Create(ProductFormParams);

let defaultTaxCalculationMethod = "totalFirst";

let component = ReasonReact.statelessComponent("ProductEdit");

let make =
    (
      ~product: option(Product.t)=None,
      ~onCancel=() => (),
      ~onSubmit,
      ~products,
      _children,
    ) => {
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
        | None => {
            name: "",
            sku: "",
            price: "",
            taxCalculationMethod: defaultTaxCalculationMethod,
            taxRate: "",
            tags: "",
          }
        | Some(prod) =>
          let [|taxCalculationMethod, taxRate|] =
            Js.String.splitAtMost(
              "|",
              2,
              prod.taxCalculation |> Tax.Calculation.toDelimitedString,
            );
          {
            name: prod.name,
            sku: prod.sku,
            price: prod.suggestedPrice |> Money.toDisplay,
            taxCalculationMethod,
            taxRate,
            tags: prod.tags |> Tags.toCSV,
          };
        }
      )
      schema=[
        (`name, Required),
        (`sku, Custom(v => v.sku |> isUnique(product))),
        (`price, Required),
        (`taxCalculationMethod, Required),
        (`taxRate, Required),
        (`tags, Required),
      ]>
      ...(
           ({handleSubmit, handleChange, form, getErrorForField}) => {
             let field = (label, value, fieldType: ProductFormParams.fields) =>
               <div className="field-input productFields">
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
               <div className="field-input">
                 <label>
                   (ReactUtils.sloc("product.taxCalculationMethod"))
                   <select
                     value=form.values.taxCalculationMethod
                     onChange=(
                       ReForm.Helpers.handleDomFormChange(
                         handleChange(`taxCalculationMethod),
                       )
                     )>
                     <option value="totalFirst">
                       (ReactUtils.s("Total First"))
                     </option>
                     <option value="subTotalFirst">
                       (ReactUtils.s("Subtotal First"))
                     </option>
                     <option value="exempt">
                       (ReactUtils.s("Exempt"))
                     </option>
                   </select>
                 </label>
                 (validationMessage(getErrorForField(`taxCalculationMethod)))
               </div>
               (
                 if (form.values.taxCalculationMethod !== "exempt") {
                   <div className="field-input">
                     <label>
                       (ReactUtils.sloc("product.taxRate"))
                       <input
                         value=form.values.taxRate
                         onChange=(
                           ReForm.Helpers.handleDomFormChange(
                             handleChange(`taxRate),
                           )
                         )
                       />
                     </label>
                     (validationMessage(getErrorForField(`taxRate)))
                   </div>;
                 } else {
                   ReasonReact.nullElement;
                 }
               )
               (field("product.tags", form.values.tags, `tags))
               <div className="modal-footer">
                 <div className="spaceDivider" />
                 <Button
                   onClick=(_ => onCancel())
                   className="cancel-button-card"
                   label="action.cancelModal"
                   local=true
                 />
                 <Button
                   _type="submit"
                   className="pay-button-card"
                   label="action.done"
                   local=true
                 />
               </div>
             </form>;
           }
         )
    </EditProductForm>;
  },
};