open Util;

type state = {
  modifying: bool,
  modifiedProduct: Product.t,
  originalProduct: Product.t,
};

type action =
  | EnableMod
  | CancelMod
  | SaveMod;

let component = ReasonReact.reducerComponent("ProductManagementRow");

let make = (~product, ~removeProduct, ~modifyProduct, _children) => {
  ...component,
  initialState: () => {
    modifying: false,
    originalProduct: product,
    modifiedProduct: product,
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
    | SaveMod =>
      ReasonReact.Update({
        ...state,
        modifying: false,
        originalProduct: state.modifiedProduct,
      })
    },
  render: self => {
    let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith(",");
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
        <td> (s(self.state.originalProduct.tags |> joinStrings)) </td>
        <td>
          <button onClick=((_) => removeProduct(self.state.originalProduct))>
            (s("Eliminar"))
          </button>
        </td>
      </tr>
    | true =>
      <tr>
        <td>
          <button onClick=((_) => self.send(SaveMod))>
            (s("Guardar"))
          </button>
        </td>
        <td> <input value=self.state.modifiedProduct.sku /> </td>
        <td> <input value=self.state.modifiedProduct.name /> </td>
        <td>
          <input
            value=(self.state.modifiedProduct.suggestedPrice |> string_of_int)
          />
        </td>
        <td>
          <input
            value=(
              self.state.modifiedProduct.taxCalculation
              |> Tax.Calculation.toDelimitedString
            )
          />
        </td>
        <td>
          <input value=(self.state.modifiedProduct.tags |> joinStrings) />
        </td>
        <td>
          <button onClick=((_) => self.send(CancelMod))>
            (s("Cancelar"))
          </button>
        </td>
      </tr>
    };
  },
};