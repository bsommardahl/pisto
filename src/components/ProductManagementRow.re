open Util;

type state = {
  modifying: bool,
  modifiedProduct: Product.t,
  originalProduct: Product.t,
  taxCalculationMod: string,
  tagsMod: string,
  priceMod: string
};

type changeable =
  | Name
  | Sku;

type action =
  | EnableMod
  | CancelMod
  | SaveMod(Product.t)
  | Change(changeable, string)
  | ChangeTaxCalculation(string)
  | ChangePrice(string)
  | ChangeTags(string);

let component = ReasonReact.reducerComponent("ProductManagementRow");

let make = (~product, ~removeProduct, ~modifyProduct, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalProduct: product,
    modifiedProduct: product,
    taxCalculationMod: product.taxCalculation |> Tax.Calculation.toDelimitedString,
    tagsMod : product.tags |> Tags.toCSV,
    priceMod : product.suggestedPrice |> Money.toDisplay
  },
  reducer: (action, state) =>
    switch (action) {
    | EnableMod => ReasonReact.Update({...state, modifying: true})
    | CancelMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        modifiedProduct: state.originalProduct,
      })
    | SaveMod(prod) =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalProduct: prod,
        modifiedProduct: prod,
      })
    | ChangePrice(newVal) =>
      ReasonReact.Update({
        ...state,
        priceMod: newVal,
      })
    | Change(Name, newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedProduct: {
          ...state.modifiedProduct,
          name: newVal,
        },
      })
    | Change(Sku, newVal) =>
      ReasonReact.Update({
        ...state,
        modifiedProduct: {
          ...state.modifiedProduct,
          sku: newVal,
        },
      })
    | ChangeTaxCalculation(newVal) =>
      ReasonReact.Update({
        ...state,
        taxCalculationMod: newVal        
      })
    | ChangeTags(newVal) =>
        ReasonReact.Update({
          ...state,
          tagsMod:newVal
        })      
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let saveModification = (_) => {
      let modified = {...self.state.modifiedProduct, 
        taxCalculation: self.state.taxCalculationMod |> Tax.Calculation.toMethod,
        tags: self.state.tagsMod |> Tags.toList,
        suggestedPrice: self.state.priceMod |> Money.toT
      };
      modifyProduct(modified);
      self.send(SaveMod(modified));
    };
    switch (self.state.modifying) {
    | false =>
      <tr>
        <td>
          <button onClick=((_) => self.send(EnableMod))>
            (s("Editar"))
          </button>
        </td>
        <td> (s(self.state.originalProduct.sku)) </td>
        <td> (s(self.state.originalProduct.name)) </td>
        <td>
          (s(self.state.originalProduct.suggestedPrice |> Money.toDisplay))
        </td>
        <td>
          (
            s(
              self.state.originalProduct.taxCalculation
              |> Tax.Calculation.toDelimitedString,
            )
          )
        </td>
        <td> (s(self.state.originalProduct.tags |> Tags.toCSV)) </td>
        <td>
          <button onClick=((_) => removeProduct(self.state.originalProduct))>
            (s("Eliminar"))
          </button>
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <button onClick=((_) => self.send(CancelMod))>
            (s("Cancelar"))
          </button>
        </td>
        <td>
          <input
            value=self.state.modifiedProduct.sku
            onChange=(ev => self.send(Change(Sku, getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.modifiedProduct.name
            onChange=(ev => self.send(Change(Name, getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.priceMod
            onChange=(ev => self.send(ChangePrice(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=(
              self.state.taxCalculationMod
            )
            onChange=(ev => self.send(ChangeTaxCalculation(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=(self.state.tagsMod)
            onChange=(ev => self.send(ChangeTags(getVal(ev))))
          />
        </td>
        <td> <button onClick=saveModification> (s("Guardar")) </button> </td>
      </tr>
    };
  },
};