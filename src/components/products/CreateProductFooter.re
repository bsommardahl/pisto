type state = {
  newSku: string,
  newName: string,
  newTags: string,
  newPrice: string,
  newTaxMethod: string,
};

type action =
  | ChangeNewSku(string)
  | ChangeNewName(string)
  | ChangeNewPrice(string)
  | ChangeNewTaxMethod(string)
  | ChangeNewTags(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("CreateProductFooter");

let make = (~createProduct, _children) => {
  ...component,
  initialState: () => {
    newSku: "",
    newName: "",
    newTags: "",
    newPrice: "",
    newTaxMethod: "",
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeNewName(newVal) =>
      ReasonReact.Update({...state, newName: newVal})
    | ChangeNewSku(newVal) => ReasonReact.Update({...state, newSku: newVal})
    | ChangeNewPrice(newVal) =>
      ReasonReact.Update({...state, newPrice: newVal})
    | ChangeNewTaxMethod(newVal) =>
      ReasonReact.Update({...state, newTaxMethod: newVal})
    | ChangeNewTags(newVal) =>
      ReasonReact.Update({...state, newTags: newVal})
    | ClearInputs =>
      ReasonReact.Update({...state, newName: "", newPrice: "", newSku: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newProduct =
        Product.NewProduct.{
          sku: self.state.newSku,
          name: self.state.newName,
          tags: self.state.newTags |> Js.String.split(",") |> Array.to_list,
          suggestedPrice: self.state.newPrice |> int_of_string,
          taxCalculation: self.state.newTaxMethod |> Tax.Calculation.toMethod,
        };
      self.send(ClearInputs);
      createProduct(newProduct);
    };
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <tfoot>
      <tr>
        <td />
        <td>
          <input
            name=self.state.newSku
            onChange=(ev => self.send(ChangeNewSku(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.newName
            onChange=(ev => self.send(ChangeNewName(getVal(ev))))
          />
        </td>
        <td>
          <input
            _type="number"
            value=self.state.newPrice
            onChange=(ev => self.send(ChangeNewPrice(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.newTaxMethod
            onChange=(ev => self.send(ChangeNewTaxMethod(getVal(ev))))
          />
        </td>
        <td>
          <input
            value=self.state.newTags
            onChange=(ev => self.send(ChangeNewTags(getVal(ev))))
          />
        </td>
        <td>
          <Button
            local=true
            onClick=((_) => finishedEnteringData())
            label="action.create"
          />
        </td>
      </tr>
    </tfoot>;
  },
};