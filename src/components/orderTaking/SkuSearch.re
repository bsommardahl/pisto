type state = {sku:string};

type action =
  | ChangeSku(string)
  | ResetSku
  | SearchForProductBySku
  | KeyDown(int);

let component = ReasonReact.reducerComponent("SkuSearch");

let make = (~allProducts: list(Product.t), ~productFound, _children) => {
  ...component,
  initialState: () => {sku:""},
  reducer: (action, state) =>
    switch (action) {
    | KeyDown(27) => ReasonReact.Update({sku:""})
    | KeyDown(13) =>
      ReasonReact.SideEffects((self => self.send(SearchForProductBySku)))
    | KeyDown(_) => ReasonReact.NoUpdate
    | ChangeSku(sku) => ReasonReact.Update({sku:sku})
    | SearchForProductBySku =>
      ReasonReact.UpdateWithSideEffects({sku:""}, 
        (
          _self => {
            let matches =
              allProducts |> List.filter((p: Product.t) => p.sku === state.sku);
            if (matches |> List.length > 0) {
              productFound(matches |. List.nth(0));              
            };
          }
        ),
      )
    },
  render: self => {
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    let refocusInput = ev => {
      let node = ev
      |> ReactEventRe.Focus.target
      |> ReactDOMRe.domElementToObj;
      node##focus();
    };
    <input
      autoFocus=Js.true_
      className="sku-search"
      value=self.state.sku
      onBlur=(ev => refocusInput(ev))
      onChange=(ev => self.send(ChangeSku(getVal(ev))))
      onKeyDown=(ev => self.send(KeyDown(ReactEventRe.Keyboard.which(ev))))
    />;
  },
};